#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include "options.h"
#include "obfs.h"

#include <d3d9.h>

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
struct lua_State;
namespace lua {
	class CLuaShared;
}

// All defined within sdk.cpp
extern IDirect3DDevice9*	g_pD3DDevice9;
extern IBaseClientDLL*		g_pClientDLL;
extern IVEngineClient*		g_pEngineClient;
extern IEntityList*			g_pEntityList;
extern ISurface*			g_pSurface;
extern ICvar*				g_pCVar;
extern CInput*				g_pInput;
extern IClientMode*			g_pClientMode;
extern lua::CLuaShared*		g_pLuaShared;
extern lua_State*			g_pLuaState;
extern C_BaseEntity*		g_pLocalPlayer;
extern IViewRenderBeams*	g_pBeams;
extern IGameEventManager2*	g_pGameEvents;
extern IInputSystem*		g_pInputSystem;
extern CViewRender*			g_pViewRender;
extern CGlobalVarsBase*		g_pGlobalVars;

extern void SilentCrash();

#endif