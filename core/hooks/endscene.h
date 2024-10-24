#pragma once

#ifndef __ENDSCENE_H__
#define __ENDSCENE_H__

#pragma warning(push)
#pragma warning(disable:26495)
#include <d3d9.h>
#pragma warning(pop)

namespace Hooks 
{
	long __stdcall hkEndScene(IDirect3DDevice9* pDevice);
}

#endif