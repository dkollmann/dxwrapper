#pragma once

m_IDirect3DExecuteBuffer* CreateDirect3DExecuteBuffer(IDirect3DExecuteBuffer* aOriginal, m_IDirect3DDeviceX** NewD3DDInterface);

class m_IDirect3DExecuteBuffer : public IDirect3DExecuteBuffer, public AddressLookupTableDdrawObject
{
private:
	IDirect3DExecuteBuffer *ProxyInterface = nullptr;
	REFIID WrapperID = IID_IDirect3DExecuteBuffer;
	ULONG RefCount = 1;

	// Convert Material
	m_IDirect3DDeviceX **D3DDeviceInterface = nullptr;

	// Interface initialization functions
	void InitInterface();
	void ReleaseInterface();

public:
	m_IDirect3DExecuteBuffer(IDirect3DExecuteBuffer *aOriginal) : ProxyInterface(aOriginal)
	{
		LOG_LIMIT(3, "Creating interface " << __FUNCTION__ << " (" << this << ")");

		InitInterface();

		ProxyAddressLookupTable.SaveAddress(this, (ProxyInterface) ? ProxyInterface : (void*)this);
	}
	m_IDirect3DExecuteBuffer(m_IDirect3DDeviceX **D3DDInterface) : D3DDeviceInterface(D3DDInterface)
	{
		LOG_LIMIT(3, "Creating interface " << __FUNCTION__ << " (" << this << ")");

		InitInterface();

		ProxyAddressLookupTable.SaveAddress(this, (ProxyInterface) ? ProxyInterface : (void*)this);
	}
	~m_IDirect3DExecuteBuffer()
	{
		LOG_LIMIT(3, __FUNCTION__ << " (" << this << ")" << " deleting interface!");

		ReleaseInterface();

		ProxyAddressLookupTable.DeleteAddress(this);
	}

	void SetProxy(IDirect3DExecuteBuffer* NewProxyInterface, m_IDirect3DDeviceX** NewD3DDInterface)
	{
		ProxyInterface = NewProxyInterface;
		D3DDeviceInterface = NewD3DDInterface;
		if (NewProxyInterface || NewD3DDInterface)
		{
			RefCount = 1;
			InitInterface();
			ProxyAddressLookupTable.SaveAddress(this, (ProxyInterface) ? ProxyInterface : (void*)this);
		}
		else
		{
			ReleaseInterface();
			ProxyAddressLookupTable.DeleteAddress(this);
		}
	}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	/*** IDirect3DExecuteBuffer methods ***/
	STDMETHOD(Initialize)(THIS_ LPDIRECT3DDEVICE, LPD3DEXECUTEBUFFERDESC);
	STDMETHOD(Lock)(THIS_ LPD3DEXECUTEBUFFERDESC);
	STDMETHOD(Unlock)(THIS);
	STDMETHOD(SetExecuteData)(THIS_ LPD3DEXECUTEDATA);
	STDMETHOD(GetExecuteData)(THIS_ LPD3DEXECUTEDATA);
	STDMETHOD(Validate)(THIS_ LPDWORD, LPD3DVALIDATECALLBACK, LPVOID, DWORD);
	STDMETHOD(Optimize)(THIS_ DWORD);
};
