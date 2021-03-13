#pragma once
#include <cstdint>
#include <vector>
#include <string>

#include "glua_types.h"
#include "math.h"
#include "lua_source/lua.hpp"

#define LUAINTERFACE_CLIENT		0
#define LUAINTERFACE_SERVER		1
#define LUAINTERFACE_MENU		2

class Color;
class CCommand;
class IThreadedCall;
class ILuaGameCallback;
class ILuaObject;

typedef int(__cdecl *_luaL_loadfile)	(lua_State *state, const char *filename);
typedef int(__cdecl *_lua_pcall)		(lua_State *state, int nargs, int nresults, int errfunc);
typedef int(*CFunc)						(lua_State* L);


namespace lua
{
	using number_t = double;

	enum class special
	{
		glob,
		env,
		reg
	};

	enum class type
	{
		client,
		server,
		menu
	};

	//
	// Use this to communicate between C and Lua
	//
	class ILuaBase
	{
	public:
		// You shouldn't need to use this struct
		// Instead, use the UserType functions
		struct UserData
		{
			void*         data;
			unsigned char type;
		};

		/*0*/virtual int         Top(void) = 0;
		/*1*/virtual void        Push(int iStackPos) = 0;
		/*2*/virtual void        Pop(int iAmt = 1) = 0;
		/*3*/virtual void        GetTable(int iStackPos) = 0;
		/*4*/virtual void        GetField(int iStackPos, const char* strName) = 0;
		/*5*/virtual void        SetField(int iStackPos, const char* strName) = 0;
		/*6*/virtual void        CreateTable() = 0;
		/*7*/virtual void        SetTable(int iStackPos) = 0;
		/*8*/virtual void        SetMetaTable(int iStackPos) = 0;
		/*9*/virtual bool        GetMetaTable(int i) = 0;
		/*10*/virtual void        Call(int iArgs, int iResults) = 0;
		/*11*/virtual int         PCall(int iArgs, int iResults, int iErrorFunc) = 0;
		/*12*/virtual int         Equal(int iA, int iB) = 0;
		/*13*/virtual int         RawEqual(int iA, int iB) = 0;
		/*14*/virtual void        Insert(int iStackPos) = 0;
		/*15*/virtual void        Remove(int iStackPos) = 0;
		/*16*/virtual int         Next(int iStackPos) = 0;

#ifdef GMOD_ALLOW_DEPRECATED
		// Deprecated: Use the UserType functions instead of this
		virtual void*       NewUserdata(unsigned int iSize) = 0;
#else
	protected:
		/*17*/virtual UserData*   NewUserdata(unsigned int iSize) = 0;
#endif

	public:
		/*18*/virtual void        ThrowError(const char* strError) = 0;
		/*19*/virtual void        CheckType(int iStackPos, int iType) = 0;
		/*20*/virtual void        ArgError(int iArgNum, const char* strMessage) = 0;
		/*21*/virtual void        RawGet(int iStackPos) = 0;
		/*22*/virtual void        RawSet(int iStackPos) = 0;
		/*23*/virtual const char* GetString(int iStackPos = -1, unsigned int* iOutLen = NULL) = 0;
		/*24*/virtual double      GetNumber(int iStackPos = -1) = 0;
		/*25*/virtual bool        GetBool(int iStackPos = -1) = 0;
		/*26*/virtual CFunc       GetCFunction(int iStackPos = -1) = 0;
#ifdef GMOD_ALLOW_DEPRECATED
		virtual void*       GetUserdata(int iStackPos = -1) = 0;
#else
		/*27*/virtual UserData*   GetUserdata(int iStackPos = -1) = 0;
#endif
		/*28*/virtual void        PushNil() = 0;
		/*29*/virtual void        PushString(const char* val, unsigned int iLen = 0) = 0;
		/*30*/virtual void        PushNumber(double val) = 0;
		/*31*/virtual void        PushBool(bool val) = 0;
		/*32*/virtual void        PushCFunction(CFunc val) = 0;
		/*33*/virtual void        PushCClosure(CFunc val, int iVars) = 0;
		/*34*/virtual void        PushUserdata(void*) = 0;
		/*35*/virtual int         ReferenceCreate() = 0;
		/*36*/virtual void        ReferenceFree(int i) = 0;
		/*37*/virtual void        ReferencePush(int i) = 0;
		/*38*/virtual void        PushSpecial(int iType) = 0;
		/*39*/virtual bool        IsType(int iStackPos, int iType) = 0;
		/*40*/virtual int         GetType(int iStackPos) = 0;
		/*41*/virtual const char* GetTypeName(int iType) = 0;
		/*42*/virtual void        CreateMetaTableType(const char* strName, int iType) = 0;

	public:
		/*43*/virtual const char* CheckString(int iStackPos = -1) = 0;
		/*44*/virtual double      CheckNumber(int iStackPos = -1) = 0;
		/*45*/virtual int         ObjLen(int iStackPos = -1) = 0;
		/*46*/virtual const QAngle& GetAngle(int iStackPos = -1) = 0;
		/*47*/virtual const Vector& GetVector(int iStackPos = -1) = 0;
		/*48*/virtual void        PushAngle(const QAngle& val) = 0;
		/*49*/virtual void        PushVector(const Vector& val) = 0;
		/*50*/virtual void        SetState(lua_State *L) = 0;
		/*51*/virtual int         CreateMetaTable(const char* strName) = 0;
		/*52*/virtual bool        PushMetaTable(int iType) = 0;
		/*53*/virtual void        PushUserType(void* data, int iType) = 0;
		/*54*/virtual void        SetUserType(int iStackPos, void* data) = 0;
		template <class T>
		T* GetUserType(int iStackPos, int iType)
		{
			UserData* ud = static_cast<UserData*>(GetUserdata(iStackPos));

			if (ud == NULL || ud->data == NULL || ud->type != iType)
				return NULL;

			return static_cast<T*>(ud->data);
		}
		template <typename T>
		T* NewUserType(int iType)
		{
			UserData* ud = static_cast<UserData*>(NewUserdata(sizeof(UserData) + sizeof(T)));
			if (ud == NULL)
				return NULL;

			T* data = reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(ud) + sizeof(UserData));
			ud->data = new(data) T;
			ud->type = static_cast<unsigned char>(iType);

			return data;
		}
		inline lua_State *GetState() const
		{
			return state;
		}

	private:
		lua_State * state;
	};

	class ILuaInterface : public ILuaBase 
	{
	public:
		/*55*/virtual bool			Init(ILuaGameCallback *, bool)					= 0;
		/*56*/virtual void			Shutdown()										= 0;
		/*57*/virtual void			Cycle()											= 0;
		/*58*/virtual ILuaObject	*Global()										= 0;
		/*59*/virtual ILuaObject	*GetObject(int index)							= 0;
		/*60*/virtual void			PushLuaObject(ILuaObject *obj)					= 0;
		/*61*/virtual void			PushLuaFunction(CFunc func)						= 0;
		/*62*/virtual void			LuaError(const char *err, int index)			= 0;
		/*63*/virtual void			TypeError(const char *name, int index)			= 0;
		/*64*/virtual void			CallInternal(int args, int rets)				= 0;
		/*65*/virtual void			CallInternalNoReturns(int args)					= 0;
		/*66*/virtual bool			CallInternalGetBool(int args)					= 0;
		/*67*/virtual const char	*CallInternalGetString(int args)				= 0;
		/*68*/virtual bool			CallInternalGet(int args, ILuaObject *obj)		= 0;
		/*69*/virtual void			NewGlobalTable(const char *name)				= 0;
		/*70*/virtual ILuaObject	*NewTemporaryObject()							= 0;
		/*71*/virtual bool			isUserData(int index)							= 0;
		/*72*/virtual ILuaObject	*GetMetaTableObject(const char *name, int type) = 0;
		/*73*/virtual ILuaObject	*GetMetaTableObject(int index)					= 0;
		/*74*/virtual ILuaObject	*GetReturn(int index)							= 0;
		/*75*/virtual bool			IsServer()										= 0;
		/*76*/virtual bool			IsClient()										= 0;
		/*77*/virtual bool			IsMenu()										= 0;
		/*78*/virtual void			DestroyObject(ILuaObject *obj)					= 0;
		/*79*/virtual ILuaObject	*CreateObject()									= 0;
		/*80*/virtual void			SetMember(ILuaObject *table, ILuaObject *key, ILuaObject *value) = 0;
		/*81*/virtual void			GetNewTable()									= 0;
		/*82*/virtual void			SetMember(ILuaObject *table, float key)			= 0;
		/*83*/virtual void			SetMember(ILuaObject *table, float key, ILuaObject *value) = 0;
		/*84*/virtual void			SetMember(ILuaObject *table, const char *key)	= 0;
		/*85*/virtual void			SetMember(ILuaObject *table, const char *key, ILuaObject *value) = 0;
		/*86*/virtual void			SetType(unsigned char)							= 0;
		/*87*/virtual void			PushLong(long num)								= 0;
		/*88*/virtual int			GetFlags(int index)								= 0;
		/*89*/virtual bool			FindOnObjectsMetaTable(int objIndex, int keyIndex) = 0;
		/*90*/virtual bool			FindObjectOnTable(int tableIndex, int keyIndex) = 0;
		/*91*/virtual void			SetMemberFast(ILuaObject *table, int keyIndex, int valueIndex) = 0;
		/*92*/virtual bool			RunString(const char *filename, const char *path, const char *stringToRun, bool run, bool showErrors) = 0;
		/*93*/virtual bool			IsEqual(ILuaObject *objA, ILuaObject *objB)		= 0;
		/*94*/virtual void			Error(const char *err)							= 0;
		/*95*/virtual const char	*GetStringOrError(int index)					= 0;
		/*96*/virtual bool			RunLuaModule(const char *name)					= 0;
		/*97*/virtual bool			FindAndRunScript(const char *filename, bool run, bool showErrors, const char *, bool) = 0;
		/*98*/virtual void			SetPathID(const char *pathID)					= 0;
		/*99*/virtual const char	*GetPathID()									= 0;
		/*100*/virtual void			ErrorNoHalt(const char *fmt, ...)				= 0;
		/*101*/virtual void			Msg(const char *fmt, ...)						= 0;
		/*102*/virtual void			PushPath(const char *path)						= 0;
		/*103*/virtual void			PopPath()										= 0;
		/*104*/virtual const char	*GetPath()										= 0;
		/*105*/virtual int			GetColor(int index)								= 0;
		/*106*/virtual void			PushColor(Color color)							= 0;
		/*107*/virtual int			GetStack(int level, lua_Debug *dbg)				= 0;
		/*109*/virtual int			GetInfo(const char *what, lua_Debug *dbg)		= 0;
		/*110*/virtual const char	*GetLocal(lua_Debug *dbg, int n)				= 0;
		/*111*/virtual const char	*GetUpvalue(int funcIndex, int n)				= 0;
		/*112*/virtual bool			RunStringEx(const char *filename, const char *path, const char *stringToRun, bool run, bool printErrors, bool dontPushErrors, bool noReturns) = 0;
		/*113*/virtual size_t		GetDataString(int index, const char **str)		= 0;
		/*114*/virtual void			ErrorFromLua(const char *fmt, ...)				= 0;
		/*115*/virtual void			*GetCurrentLocation()							= 0;
		/*116*/virtual void			MsgColour(const Color &col, const char *fmt, ...) = 0;
		/*117*/virtual void			GetCurrentFile(std::string &outStr)				= 0;
		/*118*/virtual void			CompileString(/*Bootil::Buffer &*/ void* dumper, const std::string &stringToCompile) = 0;
		/*119*/virtual bool			CallFunctionProtected(int, int, bool)			= 0;
		/*120*/virtual void			Require(const char *name)						= 0;
		/*121*/virtual const char	*GetActualTypeName(int type)					= 0;
		/*122*/virtual void			PreCreateTable(int arrelems, int nonarrelems)	= 0;
		/*123*/virtual void			PushPooledString(int index)						= 0;
		/*124*/virtual const char	*GetPooledString(int index)						= 0;
		/*125*/virtual void			*AddThreadedCall(IThreadedCall*)				= 0;
		/*126*/virtual void			AppendStackTrace(char *, unsigned long)			= 0;
		/*127*/virtual void			*CreateConVar(const char *, const char *, const char *, int) = 0;
		/*128*/virtual void			*CreateConCommand(const char *, const char *, int, void(*)(const CCommand &), int(*)(const char *, char(*)[128])) = 0;
	};

	// For use with ILuaBase::PushSpecial
	enum
	{
		SPECIAL_GLOB,       // Global table
		SPECIAL_ENV,        // Environment table
		SPECIAL_REG,        // Registry table
	};

	// Use these when calling ILuaBase::GetField or ILuaBase::SetField for example,
	// instead of pushing the specified table
	enum
	{
		INDEX_GLOBAL = -10002,  // Global table
		INDEX_ENVIRONMENT,      // Environment table
		INDEX_REGISTRY,         // Registry table
	};

	class LuaClientDatatableHook;
	class LuaFindResult;
	class CSteamAPIContext;
	class IGet;

	// re-reconstructed using gmod dylib.
	class CLuaShared
	{
	public:
		/*0*/virtual		        ~CLuaShared			()																		= 0;
		/*1*/virtual void			Init				(void* (*)(char const*, int*), bool, CSteamAPIContext*, IGet*)			= 0;
		/*2*/virtual void			Shutdown			(void)																	= 0;
		/*3*/virtual void			DumpStats			(void)																	= 0;
		/*4*/virtual void*			CreateLuaInterface	(unsigned char, bool)													= 0;
		/*5*/virtual void			CloseLuaInterface	(ILuaInterface*)														= 0;
		/*6*/virtual ILuaInterface*	GetLuaInterface		(int type)																= 0;
		/*7*/virtual void           LoadFile			(std::string const&, std::string const&, bool, bool)					= 0;
		/*8*/virtual void			GetCache			(std::string const&)													= 0;
		/*9*/virtual void			MountLua			(char const*)															= 0;
		/*10*/virtual void			MountLuaAdd			(char const*, char const*)												= 0;
		/*11*/virtual void			UnMountLua			(char const*)															= 0;
		/*12*/virtual void			SetFileContents		(char const*, char const*)												= 0;
		/*13*/virtual void			SetLuaFindHook		(LuaClientDatatableHook*)												= 0;
		/*14*/virtual void			FindScripts			(std::string const&, std::string const&, std::vector<LuaFindResult>&)	= 0;
		/*15*/virtual char*			InCallDebugString	()																		= 0;
		/*16*/virtual void			InvalidateCache		(std::string const&)													= 0;
		/*17*/virtual void			EmptyCache			(void)																	= 0;
	};

	
	inline int loadfile(lua_State *state, const char *filename) {
		static auto shared_module = GetModuleHandleA(_("lua_shared.dll"));
		static _luaL_loadfile luaL_loadfile = (_luaL_loadfile)GetProcAddress(shared_module, _("luaL_loadfile"));

		if (!luaL_loadfile) {
#ifdef _DEBUG
			printf(_("Failed to find function: luaL_loadfile.\n"));
#endif
			return -1;
		}

		return luaL_loadfile(state, filename);
	}

	inline int pcall(lua_State *state, int nargs, int nresults, int errfunc) {
		static auto shared_module = GetModuleHandleA(_("lua_shared.dll"));
		static _lua_pcall lua_pcall = (_lua_pcall)GetProcAddress(shared_module, _("lua_pcall"));

		if (!lua_pcall) {
#ifdef _DEBUG
			printf(_("Failed to find function: lua_pcall.\n"));
#endif
			return -1;
		}

		return lua_pcall(state, nargs, nresults, errfunc);
	}
}