/**
* Copyright (C) 2023 Elisha Riedlinger
*
* This software is  provided 'as-is', without any express  or implied  warranty. In no event will the
* authors be held liable for any damages arising from the use of this software.
* Permission  is granted  to anyone  to use  this software  for  any  purpose,  including  commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*   1. The origin of this software must not be misrepresented; you must not claim that you  wrote the
*      original  software. If you use this  software  in a product, an  acknowledgment in the product
*      documentation would be appreciated but is not required.
*   2. Altered source versions must  be plainly  marked as such, and  must not be  misrepresented  as
*      being the original software.
*   3. This notice may not be removed or altered from any source distribution.
*/

#include "dinput8.h"

HRESULT m_IDirectInputDevice8::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	if ((riid == WrapperID || riid == IID_IUnknown) && ppvObj)
	{
		AddRef();

		*ppvObj = this;

		return DI_OK;
	}

	HRESULT hr = ProxyInterface->QueryInterface(riid, ppvObj);

	if (SUCCEEDED(hr))
	{
		genericQueryInterface(riid, ppvObj);
	}

	return hr;
}

ULONG m_IDirectInputDevice8::AddRef()
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->AddRef();
}

ULONG m_IDirectInputDevice8::Release()
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	ULONG ref = ProxyInterface->Release();

	if (ref == 0)
	{
		delete this;
	}

	return ref;
}

HRESULT m_IDirectInputDevice8::GetCapabilities(LPDIDEVCAPS lpDIDevCaps)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->GetCapabilities(lpDIDevCaps);
}

template HRESULT m_IDirectInputDevice8::EnumObjectsT<IDirectInputDevice8A, LPDIENUMDEVICEOBJECTSCALLBACKA>(LPDIENUMDEVICEOBJECTSCALLBACKA, LPVOID, DWORD);
template HRESULT m_IDirectInputDevice8::EnumObjectsT<IDirectInputDevice8W, LPDIENUMDEVICEOBJECTSCALLBACKW>(LPDIENUMDEVICEOBJECTSCALLBACKW, LPVOID, DWORD);
template <class T, class V>
HRESULT m_IDirectInputDevice8::EnumObjectsT(V lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return GetProxyInterface<T>()->EnumObjects(lpCallback, pvRef, dwFlags);
}

HRESULT m_IDirectInputDevice8::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->GetProperty(rguidProp, pdiph);
}

HRESULT m_IDirectInputDevice8::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->SetProperty(rguidProp, pdiph);
}

HRESULT m_IDirectInputDevice8::Acquire()
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->Acquire();
}

HRESULT m_IDirectInputDevice8::Unacquire()
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->Unacquire();
}

HRESULT m_IDirectInputDevice8::GetDeviceState(DWORD cbData, LPVOID lpvData)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->GetDeviceState(cbData, lpvData);
}

HRESULT m_IDirectInputDevice8::FakeGetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
	DWORD dwItems = INFINITE;
	HRESULT hr = ProxyInterface->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), nullptr, &dwItems, DIGDD_PEEK);
	if (FAILED(hr))
	{
		if (pdwInOut)
		{
			*pdwInOut = 0;
		}
		return hr;
	}

	if (!pdwInOut || (rgdod && cbObjectData != sizeof(DIDEVICEOBJECTDATA) && cbObjectData != sizeof(DIDEVICEOBJECTDATA_DX3)))
	{
		return DIERR_INVALIDPARAM;
	}

	bool isPeek = ((dwFlags & DIGDD_PEEK) > 0);

	if (rgdod == nullptr && isPeek)
	{
		*pdwInOut = min(6, *pdwInOut);	// Just hard code to 6 as the current number of possible buffered events
		return DI_OK;
	}

	Lock();

	// Get latest ouse state from the DirectInput8 buffer
	{
		std::vector<DIDEVICEOBJECTDATA> dod;
		dod.resize(dwItems);
		hr = ProxyInterface->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), dod.data(), &dwItems, 0);
		if (SUCCEEDED(hr))
		{
			for (UINT x = 0; x < dwItems; x++)
			{
				if (dod[x].dwOfs == DIMOFS_X)
				{
					mouseStateDeviceData.lX += dod[x].dwData;
				}
				if (dod[x].dwOfs == DIMOFS_Y)
				{
					mouseStateDeviceData.lY += dod[x].dwData;
				}
				if (dod[x].dwOfs == DIMOFS_Z)
				{
					mouseStateDeviceData.lZ += dod[x].dwData;
				}
				if (dod[x].dwOfs == DIMOFS_BUTTON0)
				{
					mouseStateDeviceData.rgbButtons[0] = (BYTE)dod[x].dwData;
				}
				if (dod[x].dwOfs == DIMOFS_BUTTON1)
				{
					mouseStateDeviceData.rgbButtons[1] = (BYTE)dod[x].dwData;
				}
				if (dod[x].dwOfs == DIMOFS_BUTTON2)
				{
					mouseStateDeviceData.rgbButtons[2] = (BYTE)dod[x].dwData;
				}
			}
		}
	}

	DWORD dwOut = 0;

	// Determine timestamp:
	__int64 fTime = 0;
	GetSystemTimeAsFileTime((FILETIME*)&fTime);
	fTime = fTime / 1000;

	// Checking for overflow
	if (rgdod == nullptr && *pdwInOut == 0)
	{
		// Don't return overflow as just using mouse state
	}
	// Flush buffer
	else if (rgdod == nullptr && !isPeek)
	{
		// Don't flush buffer as just using mouse state
	}
	// Full device object data
	else if (rgdod)
	{
		memset(rgdod, 0, *pdwInOut * cbObjectData);

		LPDIDEVICEOBJECTDATA p_rgdod = rgdod;
		for (DWORD i = 0; i < *pdwInOut; i++)
		{
			// Sending DIMOFS_X
			if (mouseStateDeviceData.lX != 0)
			{
				p_rgdod->dwData = mouseStateDeviceData.lX;
				p_rgdod->dwOfs = DIMOFS_X;
				p_rgdod->dwSequence = dwSequence;
				p_rgdod->dwTimeStamp = (DWORD)fTime;
				if (cbObjectData == sizeof(DIDEVICEOBJECTDATA))
				{
					p_rgdod->uAppData = NULL;
				}

				dwSequence++;
				mouseStateDeviceData.lX = 0;
				dwOut++;
			}
			// Sending DIMOFS_Y
			else if (mouseStateDeviceData.lY != 0)
			{
				p_rgdod->dwData = mouseStateDeviceData.lY;
				p_rgdod->dwOfs = DIMOFS_Y;
				p_rgdod->dwSequence = dwSequence;
				p_rgdod->dwTimeStamp = (DWORD)fTime;
				if (cbObjectData == sizeof(DIDEVICEOBJECTDATA))
				{
					p_rgdod->uAppData = NULL;
				}

				dwSequence++;
				mouseStateDeviceData.lY = 0;
				dwOut++;
			}
			// Sending DIMOFS_Z
			else if (mouseStateDeviceData.lZ != 0)
			{
				p_rgdod->dwData = mouseStateDeviceData.lZ;
				p_rgdod->dwOfs = DIMOFS_Z;
				p_rgdod->dwSequence = dwSequence;
				p_rgdod->dwTimeStamp = (DWORD)fTime;
				if (cbObjectData == sizeof(DIDEVICEOBJECTDATA))
				{
					p_rgdod->uAppData = NULL;
				}

				dwSequence++;
				mouseStateDeviceData.lZ = 0;
				dwOut++;
			}
			// Sending DIMOFS_BUTTON0
			else if (mouseStateDeviceData.rgbButtons[0] != mouseStateDeviceDataGame.rgbButtons[0])
			{
				p_rgdod->dwData = mouseStateDeviceData.rgbButtons[0];
				p_rgdod->dwOfs = DIMOFS_BUTTON0;
				p_rgdod->dwSequence = dwSequence;
				p_rgdod->dwTimeStamp = (DWORD)fTime;
				if (cbObjectData == sizeof(DIDEVICEOBJECTDATA))
				{
					p_rgdod->uAppData = NULL;
				}

				dwSequence++;
				mouseStateDeviceDataGame.rgbButtons[0] = mouseStateDeviceData.rgbButtons[0];
				dwOut++;
			}
			// Sending DIMOFS_BUTTON1
			else if (mouseStateDeviceData.rgbButtons[1] != mouseStateDeviceDataGame.rgbButtons[1])
			{
				p_rgdod->dwData = mouseStateDeviceData.rgbButtons[1];
				p_rgdod->dwOfs = DIMOFS_BUTTON1;
				p_rgdod->dwSequence = dwSequence;
				p_rgdod->dwTimeStamp = (DWORD)fTime;
				if (cbObjectData == sizeof(DIDEVICEOBJECTDATA))
				{
					p_rgdod->uAppData = NULL;
				}

				dwSequence++;
				mouseStateDeviceDataGame.rgbButtons[1] = mouseStateDeviceData.rgbButtons[1];
				dwOut++;
			}
			// Sending DIMOFS_BUTTON2
			else if (mouseStateDeviceData.rgbButtons[2] != mouseStateDeviceDataGame.rgbButtons[2])
			{
				p_rgdod->dwData = mouseStateDeviceData.rgbButtons[2];
				p_rgdod->dwOfs = DIMOFS_BUTTON2;
				p_rgdod->dwSequence = dwSequence;
				p_rgdod->dwTimeStamp = (DWORD)fTime;
				if (cbObjectData == sizeof(DIDEVICEOBJECTDATA))
				{
					p_rgdod->uAppData = NULL;
				}

				dwSequence++;
				mouseStateDeviceDataGame.rgbButtons[2] = mouseStateDeviceData.rgbButtons[2];
				dwOut++;
			}
			// No more data to sent
			else
			{
				break;
			}

			p_rgdod = (LPDIDEVICEOBJECTDATA)((DWORD)p_rgdod + cbObjectData);
		}
	}

	Unlock();

	*pdwInOut = dwOut;

	return DI_OK;	// Always just return OK
}

HRESULT m_IDirectInputDevice8::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	if (Config.FilterNonActiveInput && pdwInOut)
	{
		// Check foreground window's process and don't copy device data if process is not active
		HWND hfgwnd = GetForegroundWindow();
		if (hfgwnd)
		{
			DWORD fgwndprocid = 0;
			GetWindowThreadProcessId(hfgwnd, &fgwndprocid);

			if (ProcessID != fgwndprocid)
			{
				// Foreground window belongs to another process, don't copy the device data
				*pdwInOut = 0;
			}
		}
	}

	if (Config.FixHighFrequencyMouse && IsMouse)
	{
		return FakeGetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
	}

	return ProxyInterface->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
}

HRESULT m_IDirectInputDevice8::SetDataFormat(LPCDIDATAFORMAT lpdf)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->SetDataFormat(lpdf);
}

HRESULT m_IDirectInputDevice8::SetEventNotification(HANDLE hEvent)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->SetEventNotification(hEvent);
}

HRESULT m_IDirectInputDevice8::SetCooperativeLevel(HWND hwnd, DWORD dwFlags)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->SetCooperativeLevel(hwnd, dwFlags);
}

template HRESULT m_IDirectInputDevice8::GetObjectInfoT<IDirectInputDevice8A, LPDIDEVICEOBJECTINSTANCEA>(LPDIDEVICEOBJECTINSTANCEA, DWORD, DWORD);
template HRESULT m_IDirectInputDevice8::GetObjectInfoT<IDirectInputDevice8W, LPDIDEVICEOBJECTINSTANCEW>(LPDIDEVICEOBJECTINSTANCEW, DWORD, DWORD);
template <class T, class V>
HRESULT m_IDirectInputDevice8::GetObjectInfoT(V pdidoi, DWORD dwObj, DWORD dwHow)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return GetProxyInterface<T>()->GetObjectInfo(pdidoi, dwObj, dwHow);
}

template HRESULT m_IDirectInputDevice8::GetDeviceInfoT<IDirectInputDevice8A, LPDIDEVICEINSTANCEA>(LPDIDEVICEINSTANCEA);
template HRESULT m_IDirectInputDevice8::GetDeviceInfoT<IDirectInputDevice8W, LPDIDEVICEINSTANCEW>(LPDIDEVICEINSTANCEW);
template <class T, class V>
HRESULT m_IDirectInputDevice8::GetDeviceInfoT(V pdidi)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return GetProxyInterface<T>()->GetDeviceInfo(pdidi);
}

HRESULT m_IDirectInputDevice8::RunControlPanel(HWND hwndOwner, DWORD dwFlags)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->RunControlPanel(hwndOwner, dwFlags);
}

HRESULT m_IDirectInputDevice8::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->Initialize(hinst, dwVersion, rguid);
}

HRESULT m_IDirectInputDevice8::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT * ppdeff, LPUNKNOWN punkOuter)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	HRESULT hr = ProxyInterface->CreateEffect(rguid, lpeff, ppdeff, punkOuter);

	if (SUCCEEDED(hr) && ppdeff)
	{
		*ppdeff = new m_IDirectInputEffect8(*ppdeff, IID_IDirectInputEffect);
	}

	return hr;
}

template HRESULT m_IDirectInputDevice8::EnumEffectsT<IDirectInputDevice8A, LPDIENUMEFFECTSCALLBACKA>(LPDIENUMEFFECTSCALLBACKA, LPVOID, DWORD);
template HRESULT m_IDirectInputDevice8::EnumEffectsT<IDirectInputDevice8W, LPDIENUMEFFECTSCALLBACKW>(LPDIENUMEFFECTSCALLBACKW, LPVOID, DWORD);
template <class T, class V>
HRESULT m_IDirectInputDevice8::EnumEffectsT(V lpCallback, LPVOID pvRef, DWORD dwEffType)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return GetProxyInterface<T>()->EnumEffects(lpCallback, pvRef, dwEffType);
}

template HRESULT m_IDirectInputDevice8::GetEffectInfoT<IDirectInputDevice8A, LPDIEFFECTINFOA>(LPDIEFFECTINFOA, REFGUID);
template HRESULT m_IDirectInputDevice8::GetEffectInfoT<IDirectInputDevice8W, LPDIEFFECTINFOW>(LPDIEFFECTINFOW, REFGUID);
template <class T, class V>
HRESULT m_IDirectInputDevice8::GetEffectInfoT(V pdei, REFGUID rguid)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return GetProxyInterface<T>()->GetEffectInfo(pdei, rguid);
}

HRESULT m_IDirectInputDevice8::GetForceFeedbackState(LPDWORD pdwOut)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->GetForceFeedbackState(pdwOut);
}

HRESULT m_IDirectInputDevice8::SendForceFeedbackCommand(DWORD dwFlags)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->SendForceFeedbackCommand(dwFlags);
}

HRESULT m_IDirectInputDevice8::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	if (!lpCallback)
	{
		return DIERR_INVALIDPARAM;
	}

	struct EnumEffect
	{
		LPVOID pvRef = nullptr;
		LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback = nullptr;

		static BOOL CALLBACK EnumEffectCallback(LPDIRECTINPUTEFFECT a, LPVOID pvRef)
		{
			EnumEffect *self = (EnumEffect*)pvRef;

			if (a)
			{
				a = ProxyAddressLookupTableDinput8.FindAddress<m_IDirectInputEffect8>(a, IID_IDirectInputEffect);
			}

			return self->lpCallback(a, self->pvRef);
		}
	} CallbackContext;
	CallbackContext.pvRef = pvRef;
	CallbackContext.lpCallback = lpCallback;

	return ProxyInterface->EnumCreatedEffectObjects(EnumEffect::EnumEffectCallback, &CallbackContext, fl);
}

HRESULT m_IDirectInputDevice8::Escape(LPDIEFFESCAPE pesc)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->Escape(pesc);
}

HRESULT m_IDirectInputDevice8::Poll()
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->Poll();
}

HRESULT m_IDirectInputDevice8::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return ProxyInterface->SendDeviceData(cbObjectData, rgdod, pdwInOut, fl);
}

template HRESULT m_IDirectInputDevice8::EnumEffectsInFileT<IDirectInputDevice8A, LPCSTR>(LPCSTR, LPDIENUMEFFECTSINFILECALLBACK, LPVOID, DWORD);
template HRESULT m_IDirectInputDevice8::EnumEffectsInFileT<IDirectInputDevice8W, LPCWSTR>(LPCWSTR, LPDIENUMEFFECTSINFILECALLBACK, LPVOID, DWORD);
template <class T, class V>
HRESULT m_IDirectInputDevice8::EnumEffectsInFileT(V lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return GetProxyInterface<T>()->EnumEffectsInFile(lpszFileName, pec, pvRef, dwFlags);
}

template HRESULT m_IDirectInputDevice8::WriteEffectToFileT<IDirectInputDevice8A, LPCSTR>(LPCSTR, DWORD, LPDIFILEEFFECT, DWORD);
template HRESULT m_IDirectInputDevice8::WriteEffectToFileT<IDirectInputDevice8W, LPCWSTR>(LPCWSTR, DWORD, LPDIFILEEFFECT, DWORD);
template <class T, class V>
HRESULT m_IDirectInputDevice8::WriteEffectToFileT(V lpszFileName, DWORD dwEntries, LPDIFILEEFFECT rgDiFileEft, DWORD dwFlags)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return GetProxyInterface<T>()->WriteEffectToFile(lpszFileName, dwEntries, rgDiFileEft, dwFlags);
}

template HRESULT m_IDirectInputDevice8::BuildActionMapT<IDirectInputDevice8A, LPDIACTIONFORMATA, LPCSTR>(LPDIACTIONFORMATA, LPCSTR, DWORD);
template HRESULT m_IDirectInputDevice8::BuildActionMapT<IDirectInputDevice8W, LPDIACTIONFORMATW, LPCWSTR>(LPDIACTIONFORMATW, LPCWSTR, DWORD);
template <class T, class V, class W>
HRESULT m_IDirectInputDevice8::BuildActionMapT(V lpdiaf, W lpszUserName, DWORD dwFlags)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return GetProxyInterface<T>()->BuildActionMap(lpdiaf, lpszUserName, dwFlags);
}

template HRESULT m_IDirectInputDevice8::SetActionMapT<IDirectInputDevice8A, LPDIACTIONFORMATA, LPCSTR>(LPDIACTIONFORMATA, LPCSTR, DWORD);
template HRESULT m_IDirectInputDevice8::SetActionMapT<IDirectInputDevice8W, LPDIACTIONFORMATW, LPCWSTR>(LPDIACTIONFORMATW, LPCWSTR, DWORD);
template <class T, class V, class W>
HRESULT m_IDirectInputDevice8::SetActionMapT(V lpdiActionFormat, W lptszUserName, DWORD dwFlags)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return GetProxyInterface<T>()->SetActionMap(lpdiActionFormat, lptszUserName, dwFlags);
}

template HRESULT m_IDirectInputDevice8::GetImageInfoT<IDirectInputDevice8A, LPDIDEVICEIMAGEINFOHEADERA>(LPDIDEVICEIMAGEINFOHEADERA);
template HRESULT m_IDirectInputDevice8::GetImageInfoT<IDirectInputDevice8W, LPDIDEVICEIMAGEINFOHEADERW>(LPDIDEVICEIMAGEINFOHEADERW);
template <class T, class V>
HRESULT m_IDirectInputDevice8::GetImageInfoT(V lpdiDevImageInfoHeader)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	return GetProxyInterface<T>()->GetImageInfo(lpdiDevImageInfoHeader);
}
