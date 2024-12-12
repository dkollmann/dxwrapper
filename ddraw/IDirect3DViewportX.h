#pragma once

class m_IDirect3DViewportX : public IUnknown, public AddressLookupTableDdrawObject
{
private:
	IDirect3DViewport3 *ProxyInterface = nullptr;
	DWORD ProxyDirectXVersion;
	ULONG RefCount1 = 0;
	ULONG RefCount2 = 0;
	ULONG RefCount3 = 0;

	bool IsViewPortSet = false;
	D3DVIEWPORT vData = {};
	bool IsViewPort2Set = false;
	D3DVIEWPORT2 vData2 = {};

	// Light array
	std::vector<LPDIRECT3DLIGHT> AttachedLights;

	inline bool IsLightAttached(LPDIRECT3DLIGHT LightX)
	{
		auto it = std::find_if(AttachedLights.begin(), AttachedLights.end(),
			[=](auto pLight) -> bool { return pLight == LightX; });

		if (it != std::end(AttachedLights))
		{
			return true;
		}
		return false;
	}

	inline bool DeleteAttachedLight(LPDIRECT3DLIGHT LightX)
	{
		auto it = std::find_if(AttachedLights.begin(), AttachedLights.end(),
			[=](auto pLight) -> bool { return pLight == LightX; });

		if (it != std::end(AttachedLights))
		{
			AttachedLights.erase(it);
			return true;
		}
		return false;
	}

	struct MATERIALBACKGROUND {
		BOOL IsSet = FALSE;
		D3DMATERIALHANDLE hMat = NULL;
	} MaterialBackground;

	// Convert Viewport
	m_IDirect3DDeviceX **D3DDeviceInterface = nullptr;

	// Store version wrappers
	m_IDirect3DViewport *WrapperInterface = nullptr;
	m_IDirect3DViewport2 *WrapperInterface2 = nullptr;
	m_IDirect3DViewport3 *WrapperInterface3 = nullptr;

	// Wrapper interface functions
	inline REFIID GetWrapperType(DWORD DirectXVersion)
	{
		return (DirectXVersion == 1) ? IID_IDirect3DViewport :
			(DirectXVersion == 2) ? IID_IDirect3DViewport2 :
			(DirectXVersion == 3) ? IID_IDirect3DViewport3 : IID_IUnknown;
	}
	inline bool CheckWrapperType(REFIID IID)
	{
		return (IID == IID_IDirect3DViewport ||
			IID == IID_IDirect3DViewport2 ||
			IID == IID_IDirect3DViewport3) ? true : false;
	}
	inline IDirect3DViewport *GetProxyInterfaceV1() { return (IDirect3DViewport *)ProxyInterface; }
	inline IDirect3DViewport2 *GetProxyInterfaceV2() { return (IDirect3DViewport2 *)ProxyInterface; }
	inline IDirect3DViewport3 *GetProxyInterfaceV3() { return ProxyInterface; }

	// Interface initialization functions
	void InitInterface(DWORD DirectXVersion);
	void ReleaseInterface();

public:
	m_IDirect3DViewportX(IDirect3DViewport3 *aOriginal, DWORD DirectXVersion) : ProxyInterface(aOriginal)
	{
		ProxyDirectXVersion = GetGUIDVersion(ConvertREFIID(GetWrapperType(DirectXVersion)));

		if (ProxyDirectXVersion != DirectXVersion)
		{
			LOG_LIMIT(3, "Creating interface " << __FUNCTION__ << " (" << this << ")" << " converting interface from v" << DirectXVersion << " to v" << ProxyDirectXVersion);
		}
		else
		{
			LOG_LIMIT(3, "Creating interface " << __FUNCTION__ << " (" << this << ") v" << DirectXVersion);
		}

		InitInterface(DirectXVersion);
	}
	m_IDirect3DViewportX(m_IDirect3DDeviceX **D3DDInterface, DWORD DirectXVersion) : D3DDeviceInterface(D3DDInterface)
	{
		ProxyDirectXVersion = (!Config.Dd7to9) ? 3 : 9;

		if (ProxyDirectXVersion != DirectXVersion)
		{
			LOG_LIMIT(3, "Creating interface " << __FUNCTION__ << " (" << this << ")" << " converting interface from v" << DirectXVersion << " to v" << ProxyDirectXVersion);
		}
		else
		{
			LOG_LIMIT(3, "Creating interface " << __FUNCTION__ << " (" << this << ") v" << DirectXVersion);
		}

		InitInterface(DirectXVersion);
	}
	~m_IDirect3DViewportX()
	{
		LOG_LIMIT(3, __FUNCTION__ << " (" << this << ")" << " deleting interface!");

		ReleaseInterface();
	}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * ppvObj) { return QueryInterface(riid, ppvObj, 0); }
	STDMETHOD_(ULONG, AddRef) (THIS) { return AddRef(0); }
	STDMETHOD_(ULONG, Release) (THIS) { return Release(0); }

	/*** IDirect3DViewport methods ***/
	STDMETHOD(Initialize)(THIS_ LPDIRECT3D);
	STDMETHOD(GetViewport)(THIS_ LPD3DVIEWPORT);
	STDMETHOD(SetViewport)(THIS_ LPD3DVIEWPORT);
	STDMETHOD(TransformVertices)(THIS_ DWORD, LPD3DTRANSFORMDATA, DWORD, LPDWORD);
	STDMETHOD(LightElements)(THIS_ DWORD, LPD3DLIGHTDATA);
	STDMETHOD(SetBackground)(THIS_ D3DMATERIALHANDLE);
	STDMETHOD(GetBackground)(THIS_ LPD3DMATERIALHANDLE, LPBOOL);
	STDMETHOD(SetBackgroundDepth)(THIS_ LPDIRECTDRAWSURFACE);
	STDMETHOD(GetBackgroundDepth)(THIS_ LPDIRECTDRAWSURFACE*, LPBOOL);
	STDMETHOD(Clear)(THIS_ DWORD, LPD3DRECT, DWORD);
	STDMETHOD(AddLight)(THIS_ LPDIRECT3DLIGHT);
	STDMETHOD(DeleteLight)(THIS_ LPDIRECT3DLIGHT);
	STDMETHOD(NextLight)(THIS_ LPDIRECT3DLIGHT, LPDIRECT3DLIGHT*, DWORD);

	/*** IDirect3DViewport2 methods ***/
	STDMETHOD(GetViewport2)(THIS_ LPD3DVIEWPORT2);
	STDMETHOD(SetViewport2)(THIS_ LPD3DVIEWPORT2);

	/*** IDirect3DViewport3 methods ***/
	STDMETHOD(SetBackgroundDepth2)(THIS_ LPDIRECTDRAWSURFACE4);
	STDMETHOD(GetBackgroundDepth2)(THIS_ LPDIRECTDRAWSURFACE4*, LPBOOL);
	STDMETHOD(Clear2)(THIS_ DWORD, LPD3DRECT, DWORD, D3DCOLOR, D3DVALUE, DWORD);

	// Helper functions
	HRESULT QueryInterface(REFIID riid, LPVOID FAR * ppvObj, DWORD DirectXVersion);
	void *GetWrapperInterfaceX(DWORD DirectXVersion);
	void SetCurrentViewportActive(bool SetViewPortData, bool SetBackgroundData, bool SetLightData);
	ULONG AddRef(DWORD DirectXVersion);
	ULONG Release(DWORD DirectXVersion);
};
