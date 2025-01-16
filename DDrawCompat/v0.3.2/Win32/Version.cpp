#include <sstream>

#include <DDrawCompat/v0.3.2/Common/Hook.h>
#include <DDrawCompat/DDrawLog.h>
#include <DDrawCompat/v0.3.2/Common/Path.h>
#include <DDrawCompat/v0.3.2/Dll/Dll.h>
#include "Settings/Settings.h"

#include <DDrawCompat/v0.3.2/Win32/Version.h>

#pragma warning(disable : 4996)

namespace
{
	BOOL WINAPI Empty_GetFileVersionInfoA(LPCSTR, DWORD, DWORD, LPVOID) { return FALSE; }
	BOOL WINAPI Empty_GetFileVersionInfoW(LPCWSTR, DWORD, DWORD, LPVOID) { return FALSE; }
	BOOL WINAPI Empty_GetFileVersionInfoExA(DWORD, LPCSTR, DWORD, DWORD, LPVOID) { return FALSE; }
	BOOL WINAPI Empty_GetFileVersionInfoExW(DWORD, LPCWSTR, DWORD, DWORD, LPVOID) { return FALSE; }
	DWORD WINAPI Empty_GetFileVersionInfoSizeA(LPCSTR, LPDWORD) { return 0; }
	DWORD WINAPI Empty_GetFileVersionInfoSizeW(LPCWSTR, LPDWORD) { return 0; }
	DWORD WINAPI Empty_GetFileVersionInfoSizeExA(DWORD, LPCSTR, LPDWORD) { return 0; }
	DWORD WINAPI Empty_GetFileVersionInfoSizeExW(DWORD, LPCWSTR, LPDWORD) { return 0; }

	struct VersionInfo
	{
		DWORD version;
		DWORD build;
		DWORD platform;
	};
	std::vector<std::pair<std::string, VersionInfo>> VersionLie =
	{
		{ "95", {0xC3B60004, 0x3B6, 1} },
		{ "nt4", {0x5650004, 0x565, 2} },
		{ "98", {0xC0000A04, 0x40A08AE, 1} },
		{ "2000", {0x8930005, 0x893, 2} },
		{ "xp", {0xA280105, 0xA28, 2} }
	};

	VersionInfo vi = {};
	int sp = 0;

	DWORD getModuleFileName(HMODULE mod, char* filename, DWORD size)
	{
		return GetModuleFileNameA(mod, filename, size);
	}

	DWORD getModuleFileName(HMODULE mod, wchar_t* filename, DWORD size)
	{
		return GetModuleFileNameW(mod, filename, size);
	}

	HMODULE getModuleHandle(const char* moduleName)
	{
		return GetModuleHandleA(moduleName);
	}

	HMODULE getModuleHandle(const wchar_t* moduleName)
	{
		return GetModuleHandleW(moduleName);
	}

	template <typename Char>
	void fixVersionInfoFileName(const Char*& filename)
	{
		if (getModuleHandle(filename) == Dll::g_currentModule || getModuleHandle(filename) == Dll::g_localDDModule)
		{
			static Char path[MAX_PATH];
			if (0 != getModuleFileName(Dll::g_origDDrawModule, path, MAX_PATH))
			{
				filename = path;
			}
		}
	}

	template <auto func, typename Result, typename Char, typename... Params>
	Result WINAPI getFileVersionInfoFunc(const Char* filename, Params... params)
	{
		LOG_FUNC(Compat32::g_origFuncName<func>.c_str(), filename, params...);
		fixVersionInfoFileName(filename);
		return LOG_RESULT(CALL_ORIG_FUNC(func)(filename, params...));
	}

	template <auto func, typename Result, typename Char, typename... Params>
	Result WINAPI getFileVersionInfoFunc(DWORD flags, const Char* filename, Params... params)
	{
		LOG_FUNC(Compat32::g_origFuncName<func>.c_str(), filename, params...);
		fixVersionInfoFileName(filename);
		return LOG_RESULT(CALL_ORIG_FUNC(func)(flags, filename, params...));
	}

	DWORD WINAPI getVersion()
	{
		LOG_FUNC("GetVersion");
		//auto vi = Config32::winVersionLie.get();
		if (0 != vi.version)
		{
			return LOG_RESULT(vi.version);
		}
		return LOG_RESULT(CALL_ORIG_FUNC(GetVersion)());
	}

	template <typename OsVersionInfoEx, typename OsVersionInfo>
	BOOL getVersionInfo(OsVersionInfo* osVersionInfo, BOOL(WINAPI* origGetVersionInfo)(OsVersionInfo*),
		[[maybe_unused]] const char* funcName)
	{
		LOG_FUNC(funcName, osVersionInfo);
		BOOL result = origGetVersionInfo(osVersionInfo);
		//auto vi = Config32::winVersionLie.get();
		if (result && 0 != vi.version)
		{
			osVersionInfo->dwMajorVersion = vi.version & 0xFF;
			osVersionInfo->dwMinorVersion = (vi.version & 0xFF00) >> 8;
			osVersionInfo->dwBuildNumber = vi.build;
			osVersionInfo->dwPlatformId = vi.platform;

			//auto sp = Config32::winVersionLie.getParam();
			if (0 != sp)
			{
				typedef std::remove_reference_t<decltype(osVersionInfo->szCSDVersion[0])> Char;
				std::basic_ostringstream<Char> oss;
				oss << "Service Pack " << sp;
				memset(osVersionInfo->szCSDVersion, 0, sizeof(osVersionInfo->szCSDVersion));
				memcpy(osVersionInfo->szCSDVersion, oss.str().c_str(), oss.str().length() * sizeof(Char));

				if (osVersionInfo->dwOSVersionInfoSize >= sizeof(OsVersionInfoEx))
				{
					auto osVersionInfoEx = reinterpret_cast<OsVersionInfoEx*>(osVersionInfo);
					osVersionInfoEx->wServicePackMajor = static_cast<WORD>(sp);
					osVersionInfoEx->wServicePackMinor = 0;
				}
			}
		}
		return result;
	}

	BOOL WINAPI getVersionExA(LPOSVERSIONINFOA lpVersionInformation)
	{
		return getVersionInfo<OSVERSIONINFOEXA>(lpVersionInformation, CALL_ORIG_FUNC(GetVersionExA), "GetVersionExA");
	}

	BOOL WINAPI getVersionExW(LPOSVERSIONINFOW lpVersionInformation)
	{
		return getVersionInfo<OSVERSIONINFOEXW>(lpVersionInformation, CALL_ORIG_FUNC(GetVersionExW), "GetVersionExW");
	}

	template <auto origFunc>
	bool hookVersionInfoFunc(const char* moduleName, const char* funcName)
	{
		HMODULE mod = GetModuleHandleA(moduleName);
		if (mod)
		{
			FARPROC func = Compat32::getProcAddress(mod, funcName);
			if (func)
			{
				Compat32::hookFunction<origFunc>(moduleName, funcName, getFileVersionInfoFunc<origFunc>);
				return true;
			}
		}
		return false;
	}

	template <auto origFunc>
	void hookVersionInfoFunc(const char* funcName)
	{
		hookVersionInfoFunc<origFunc>("kernelbase", funcName) || hookVersionInfoFunc<origFunc>("version", funcName);
	}
}

namespace Compat32
{
	template<> decltype(&GetFileVersionInfoA) g_origFuncPtr<GetFileVersionInfoA> = nullptr;
	template<> decltype(&GetFileVersionInfoW) g_origFuncPtr<GetFileVersionInfoW> = nullptr;
	template<> decltype(&GetFileVersionInfoExA) g_origFuncPtr<GetFileVersionInfoExA> = nullptr;
	template<> decltype(&GetFileVersionInfoExW) g_origFuncPtr<GetFileVersionInfoExW> = nullptr;
	template<> decltype(&GetFileVersionInfoSizeA) g_origFuncPtr<GetFileVersionInfoSizeA> = nullptr;
	template<> decltype(&GetFileVersionInfoSizeW) g_origFuncPtr<GetFileVersionInfoSizeW> = nullptr;
	template<> decltype(&GetFileVersionInfoSizeExA) g_origFuncPtr<GetFileVersionInfoSizeExA> = nullptr;
	template<> decltype(&GetFileVersionInfoSizeExW) g_origFuncPtr<GetFileVersionInfoSizeExW> = nullptr;
}

#define HOOK_VERSION_INFO_FUNCTION(func) hookVersionInfoFunc<Empty_ ## func>(#func)

namespace Win32
{
	namespace Version
	{
		void installWinLieHooks()
		{
			auto it = std::find_if(VersionLie.begin(), VersionLie.end(), [](const auto& version) {
				return version.first == Config.WinVersionLie;
				});

			if (it != VersionLie.end())
			{
				vi = it->second;
				sp = (int)Config.WinVersionLieSP;

				Compat32::Log() << "Installing WinVersionLie hooks - os: " << it->first << " sp: " << sp;

				HOOK_FUNCTION(kernel32, GetVersion, getVersion);
				HOOK_FUNCTION(kernel32, GetVersionExA, getVersionExA);
				HOOK_FUNCTION(kernel32, GetVersionExW, getVersionExW);
			}
		}
		void installHooks()
		{
			HOOK_VERSION_INFO_FUNCTION(GetFileVersionInfoA);
			HOOK_VERSION_INFO_FUNCTION(GetFileVersionInfoW);
			HOOK_VERSION_INFO_FUNCTION(GetFileVersionInfoExA);
			HOOK_VERSION_INFO_FUNCTION(GetFileVersionInfoExW);
			HOOK_VERSION_INFO_FUNCTION(GetFileVersionInfoSizeA);
			HOOK_VERSION_INFO_FUNCTION(GetFileVersionInfoSizeW);
			HOOK_VERSION_INFO_FUNCTION(GetFileVersionInfoSizeExA);
			HOOK_VERSION_INFO_FUNCTION(GetFileVersionInfoSizeExW);
		}
	}
}
