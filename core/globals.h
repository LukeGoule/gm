#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include "options.h"
#include "../helpers/obfs.h"

class IBaseClientDLL;
class IVEngineClient;
class ISurface;
class ICvar;
class CInput;
class IClientMode;
class IEntityList;
class C_BaseEntity;
class IViewRenderBeams;
class IGameEventManager2;
class IInputSystem;
class CViewRender;
class CGlobalVarsBase;

struct IDirect3DDevice9;
struct lua_State;

namespace lua {
	class CLuaShared;
}

#endif