#pragma once

#ifndef __RESET_H__
#define __RESET_H__

#pragma warning(push)
#pragma warning(disable:26495)
#include <d3d9.h>
#pragma warning(pop)

namespace Hooks 
{
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
}

#endif