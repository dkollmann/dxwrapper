#pragma once

#include <ddraw.h>

#include <DDrawCompat/v0.3.2/Common/CompatPtr.h>

namespace Direct3d
{
	void installHooks(CompatPtr<IDirectDraw> dd, CompatPtr<IDirectDraw7> dd7);
}
