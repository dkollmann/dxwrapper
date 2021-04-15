#pragma once

#define INITGUID

#include <d3d9.h>

class m_IDirect3D9Ex;
class m_IDirect3DDevice9Ex;
class m_IDirect3DCubeTexture9;
class m_IDirect3DIndexBuffer9;
class m_IDirect3DPixelShader9;
class m_IDirect3DQuery9;
class m_IDirect3DStateBlock9;
class m_IDirect3DSurface9;
class m_IDirect3DSwapChain9Ex;
class m_IDirect3DTexture9;
class m_IDirect3DVertexBuffer9;
class m_IDirect3DVertexDeclaration9;
class m_IDirect3DVertexShader9;
class m_IDirect3DVolume9;
class m_IDirect3DVolumeTexture9;

#include "AddressLookupTable.h"
#include "Settings\Settings.h"
#include "Logging\Logging.h"

typedef int(WINAPI *D3DPERF_BeginEventProc)(D3DCOLOR, LPCWSTR);
typedef int(WINAPI *D3DPERF_EndEventProc)();
typedef DWORD(WINAPI *D3DPERF_GetStatusProc)();
typedef BOOL(WINAPI *D3DPERF_QueryRepeatFrameProc)();
typedef void(WINAPI *D3DPERF_SetMarkerProc)(D3DCOLOR, LPCWSTR);
typedef void(WINAPI *D3DPERF_SetOptionsProc)(DWORD);
typedef void(WINAPI *D3DPERF_SetRegionProc)(D3DCOLOR, LPCWSTR);
typedef IDirect3D9 *(WINAPI *Direct3DCreate9Proc)(UINT);
typedef HRESULT(WINAPI *Direct3DCreate9ExProc)(UINT, IDirect3D9Ex **);

DWORD UpdateBehaviorFlags(DWORD BehaviorFlags);
void UpdatePresentParameter(D3DPRESENT_PARAMETERS* pPresentationParameters, HWND hFocusWindow, bool SetWindow);
void UpdatePresentParameterForMultisample(D3DPRESENT_PARAMETERS* pPresentationParameters, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD MultiSampleQuality);

namespace D3d9Wrapper
{
	void WINAPI genericQueryInterface(REFIID riid, LPVOID *ppvObj, m_IDirect3DDevice9Ex* m_pDeviceEx);
}

extern HWND DeviceWindow;
extern LONG BufferWidth, BufferHeight;

// For AntiAliasing
extern bool DeviceMultiSampleFlag;
extern D3DMULTISAMPLE_TYPE DeviceMultiSampleType;
extern DWORD DeviceMultiSampleQuality;

#include "IDirect3D9Ex.h"
#include "IDirect3DDevice9Ex.h"
#include "IDirect3DCubeTexture9.h"
#include "IDirect3DIndexBuffer9.h"
#include "IDirect3DPixelShader9.h"
#include "IDirect3DQuery9.h"
#include "IDirect3DStateBlock9.h"
#include "IDirect3DSurface9.h"
#include "IDirect3DSwapChain9Ex.h"
#include "IDirect3DTexture9.h"
#include "IDirect3DVertexBuffer9.h"
#include "IDirect3DVertexDeclaration9.h"
#include "IDirect3DVertexShader9.h"
#include "IDirect3DVolume9.h"
#include "IDirect3DVolumeTexture9.h"
