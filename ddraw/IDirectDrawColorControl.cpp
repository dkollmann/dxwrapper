/**
* Copyright (C) 2024 Elisha Riedlinger
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

#include "ddraw.h"

// Cached wrapper interface
namespace {
	m_IDirectDrawColorControl* WrapperInterfaceBackup = nullptr;
}

inline void SaveInterfaceAddress(m_IDirectDrawColorControl* Interface, m_IDirectDrawColorControl*& InterfaceBackup)
{
	if (Interface)
	{
		SetCriticalSection();
		Interface->SetProxy(nullptr, nullptr);
		if (InterfaceBackup)
		{
			InterfaceBackup->DeleteMe();
			InterfaceBackup = nullptr;
		}
		InterfaceBackup = Interface;
		ReleaseCriticalSection();
	}
}

m_IDirectDrawColorControl* CreateDirectDrawColorControl(IDirectDrawColorControl* aOriginal, m_IDirectDrawX* NewParent)
{
	SetCriticalSection();
	m_IDirectDrawColorControl* Interface = nullptr;
	if (WrapperInterfaceBackup)
	{
		Interface = WrapperInterfaceBackup;
		WrapperInterfaceBackup = nullptr;
		Interface->SetProxy(aOriginal, NewParent);
	}
	else
	{
		if (aOriginal)
		{
			Interface = new m_IDirectDrawColorControl(aOriginal);
		}
		else
		{
			Interface = new m_IDirectDrawColorControl(NewParent);
		}
	}
	ReleaseCriticalSection();
	return Interface;
}

HRESULT m_IDirectDrawColorControl::QueryInterface(REFIID riid, LPVOID FAR * ppvObj)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ") " << riid;

	if (!ProxyInterface && !ddrawParent)
	{
		if (ppvObj)
		{
			*ppvObj = nullptr;
		}
		return E_NOINTERFACE;
	}

	if (!ppvObj)
	{
		return E_POINTER;
	}
	*ppvObj = nullptr;

	if (riid == IID_GetRealInterface)
	{
		*ppvObj = ProxyInterface;
		return DD_OK;
	}
	if (riid == IID_GetInterfaceX)
	{
		*ppvObj = this;
		return DD_OK;
	}

	if (riid == IID_IDirectDrawColorControl || riid == IID_IUnknown)
	{
		AddRef();

		*ppvObj = this;

		return DD_OK;
	}

	return ProxyQueryInterface(ProxyInterface, riid, ppvObj, WrapperID);
}

ULONG m_IDirectDrawColorControl::AddRef()
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	if (!ProxyInterface && !ddrawParent)
	{
		return 0;
	}

	if (!ProxyInterface)
	{
		return InterlockedIncrement(&RefCount);
	}

	return ProxyInterface->AddRef();
}

ULONG m_IDirectDrawColorControl::Release()
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	if (!ProxyInterface && !ddrawParent)
	{
		return 0;
	}

	ULONG ref;

	if (!ProxyInterface)
	{
		ref = InterlockedDecrement(&RefCount);
	}
	else
	{
		ref = ProxyInterface->Release();
	}

	if (ref == 0)
	{
		SaveInterfaceAddress(this, WrapperInterfaceBackup);
	}

	return ref;
}

HRESULT m_IDirectDrawColorControl::GetColorControls(LPDDCOLORCONTROL lpColorControl)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	if (!ProxyInterface && !ddrawParent)
	{
		return DDERR_INVALIDOBJECT;
	}

	if (!ProxyInterface)
	{
		if (!lpColorControl)
		{
			return DDERR_INVALIDPARAMS;
		}

		*lpColorControl = ColorControl;

		return DD_OK;
	}

	return ProxyInterface->GetColorControls(lpColorControl);
}

HRESULT m_IDirectDrawColorControl::SetColorControls(LPDDCOLORCONTROL lpColorControl)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	if (!ProxyInterface && !ddrawParent)
	{
		return DDERR_INVALIDOBJECT;
	}

	if (!ProxyInterface)
	{
		if (!lpColorControl)
		{
			return DDERR_INVALIDPARAMS;
		}

		ColorControl = *lpColorControl;

		// Present new color setting
		if (ddrawParent)
		{
			ddrawParent->SetVsync();

			SetCriticalSection();

			m_IDirectDrawSurfaceX *lpDDSrcSurfaceX = ddrawParent->GetPrimarySurface();
			if (lpDDSrcSurfaceX)
			{
				lpDDSrcSurfaceX->PresentSurface(false);
			}

			ReleaseCriticalSection();
		}

		return DD_OK;
	}

	return ProxyInterface->SetColorControls(lpColorControl);
}

/************************/
/*** Helper functions ***/
/************************/

void m_IDirectDrawColorControl::InitInterface()
{
	ColorControl.dwSize = sizeof(DDCOLORCONTROL);
	ColorControl.dwFlags = DDCOLOR_BRIGHTNESS | DDCOLOR_CONTRAST | DDCOLOR_HUE | DDCOLOR_SATURATION | DDCOLOR_SHARPNESS | DDCOLOR_GAMMA | DDCOLOR_COLORENABLE;
	ColorControl.lBrightness = 750;
	ColorControl.lContrast = 10000;
	ColorControl.lHue = 0;
	ColorControl.lSaturation = 10000;
	ColorControl.lSharpness = 5;
	ColorControl.lGamma = 1;
	ColorControl.lColorEnable = 1;
	ColorControl.dwReserved1 = 0;
}

void m_IDirectDrawColorControl::ReleaseInterface()
{
	if (ddrawParent && !Config.Exiting)
	{
		ddrawParent->ClearColorInterface();
	}
}
