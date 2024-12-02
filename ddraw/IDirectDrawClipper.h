#pragma once

m_IDirectDrawClipper* CreateDirectDrawClipper(IDirectDrawClipper* aOriginal, m_IDirectDrawX* NewParent, DWORD dwFlags);

class m_IDirectDrawClipper : public IDirectDrawClipper, public AddressLookupTableDdrawObject
{
private:
	IDirectDrawClipper *ProxyInterface = nullptr;
	REFIID WrapperID = IID_IDirectDrawClipper;
	ULONG RefCount = 1;
	DWORD clipperCaps = 0;						// Clipper flags
	HWND cliphWnd = nullptr;
	std::vector<BYTE> ClipList;
	bool IsClipListSet = false;
	bool IsClipListChangedFlag = false;

	// Convert to Direct3D9
	m_IDirectDrawX* ddrawParent = nullptr;

	// Interface initialization functions
	void InitInterface(DWORD dwFlags);
	void ReleaseInterface();

public:
	m_IDirectDrawClipper(IDirectDrawClipper *aOriginal) : ProxyInterface(aOriginal)
	{
		LOG_LIMIT(3, "Creating interface " << __FUNCTION__ << " (" << this << ")");

		InitInterface(0);

		ProxyAddressLookupTable.SaveAddress(this, (ProxyInterface) ? ProxyInterface : (void*)this);
	}
	m_IDirectDrawClipper(m_IDirectDrawX* Interface, DWORD dwFlags) : ddrawParent(Interface)
	{
		LOG_LIMIT(3, "Creating interface " << __FUNCTION__ << " (" << this << ")");

		InitInterface(dwFlags);

		ProxyAddressLookupTable.SaveAddress(this, (ProxyInterface) ? ProxyInterface : (void*)this);
	}
	~m_IDirectDrawClipper()
	{
		LOG_LIMIT(3, __FUNCTION__ << " (" << this << ")" << " deleting interface!");

		ReleaseInterface();

		ProxyAddressLookupTable.DeleteAddress(this);
	}

	void SetProxy(IDirectDrawClipper* NewProxyInterface, m_IDirectDrawX* NewParent, DWORD dwFlags)
	{
		ProxyInterface = NewProxyInterface;
		ddrawParent = NewParent;
		if (NewProxyInterface || NewParent)
		{
			RefCount = 1;
			InitInterface(dwFlags);
			ProxyAddressLookupTable.SaveAddress(this, (ProxyInterface) ? ProxyInterface : (void*)this);
		}
		else
		{
			ReleaseInterface();
			ProxyAddressLookupTable.DeleteAddress(this);
		}
	}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * ppvObj);
	STDMETHOD_(ULONG, AddRef) (THIS);
	STDMETHOD_(ULONG, Release) (THIS);

	/*** IDirectDrawClipper methods ***/
	STDMETHOD(GetClipList)(THIS_ LPRECT, LPRGNDATA, LPDWORD);
	STDMETHOD(GetHWnd)(THIS_ HWND FAR *);
	STDMETHOD(Initialize)(THIS_ LPDIRECTDRAW, DWORD);
	STDMETHOD(IsClipListChanged)(THIS_ BOOL FAR *);
	STDMETHOD(SetClipList)(THIS_ LPRGNDATA, DWORD);
	STDMETHOD(SetHWnd)(THIS_ DWORD, HWND);

	// Functions handling the ddraw parent interface
	void SetDdrawParent(m_IDirectDrawX* ddraw) { ddrawParent = ddraw; }
	void ClearDdraw() { ddrawParent = nullptr; }
};
