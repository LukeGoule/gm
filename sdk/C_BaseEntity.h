#pragma once

#include "../core/globals.h"
#include "../sdk.h"
#include "../helpers/vector.h"
#include "../gmod/luaShared.h"

#include "../helpers/obfs.h"
#include "../helpers/math.h"
#include "../helpers/vmatrix.h"

#include "IAppSystem.h"
#include "IEntityList.h"
#include "icliententity.h"
#include "CBaseHandle.h"

#include "../netvars/NetvarManager.h"

#include <string>
#include <functional>
#include <iostream>
#include <optional>

#define OFFSET(Class, Offset) g_pNetvarManager->GetOffset(_(Class), _(Offset))
#define NETVAR(Class, Offset, Type) inline Type* Offset() { static int _##Offset = OFFSET(#Class, #Offset); return GetFieldPointer<Type>(_##Offset);}

#define MAXSTUDIOBONES 128
#define BONE_CALCULATE_MASK			0x1F
#define BONE_PHYSICALLY_SIMULATED	0x01	// bone is physically simulated when physics are active
#define BONE_PHYSICS_PROCEDURAL		0x02	// procedural when physics is active
#define BONE_ALWAYS_PROCEDURAL		0x04	// bone is always procedurally animated
#define BONE_SCREEN_ALIGN_SPHERE	0x08	// bone aligns to the screen, not constrained in motion.
#define BONE_SCREEN_ALIGN_CYLINDER	0x10	// bone aligns to the screen, constrained by it's own axis.

#define BONE_USED_MASK				0x0007FF00
#define BONE_USED_BY_ANYTHING		0x0007FF00
#define BONE_USED_BY_HITBOX			0x00000100	// bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT		0x00000200	// bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK	0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0	0x00000400	// bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1	0x00000800	
#define BONE_USED_BY_VERTEX_LOD2	0x00001000  
#define BONE_USED_BY_VERTEX_LOD3	0x00002000
#define BONE_USED_BY_VERTEX_LOD4	0x00004000
#define BONE_USED_BY_VERTEX_LOD5	0x00008000
#define BONE_USED_BY_VERTEX_LOD6	0x00010000
#define BONE_USED_BY_VERTEX_LOD7	0x00020000
#define BONE_USED_BY_BONE_MERGE		0x00040000	// bone is available for bone merge to occur against it

// m_lifeState values
#define	LIFE_ALIVE				0 // alive
#define	LIFE_DYING				1 // playing death animation or still falling off of a ledge waiting to hit ground
#define	LIFE_DEAD				2 // dead. lying still.
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

struct model_t;
class C_BaseCombatWeapon;

template<class thisType, class T>
T __GetValue(thisType thisptr, const int offset)
{
	return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(thisptr) + offset);
}

class C_BaseEntity : public IClientEntity
{
protected:
	template<class T>
	T GetPointer(const int offset)
	{
		return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	// To get value from the pointer itself
	template<class T>
	T GetValue(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}

	template< class T >
	T* GetFieldPointer(int offset)
	{
		return (T*)((DWORD)this + offset);
	}
public:

	inline CBaseHandle m_hActiveWeapon() {
		static auto offset = g_pNetvarManager->GetOffset(_("DT_BaseCombatCharacter"), _("m_hActiveWeapon"));
		return CBaseHandle(GetValue<int>(offset));
	}

	inline const char* get_classname()
	{
		void* networkable = this + 8;
		using get_client_class_t = const char**(__thiscall*)(PVOID);
		return *(method<get_client_class_t>(2, networkable)(networkable) + 2);
	}

	inline model_t* get_model()
	{
		void *pRenderable = (void*)(this + 0x4);
		typedef model_t*(__thiscall* OriginalFn)(PVOID);
		return GetVFunc<OriginalFn>(pRenderable, 9)(pRenderable);
	}

	model_t* get_model2()
	{
		return *(model_t**)((DWORD)this + 0x6C);
	}

	inline Vector get_abs_origin()
	{
		typedef Vector(__thiscall* OriginalFn)(void*);
		return ((OriginalFn)GetFunction(this, 10))(this);
	}

	inline Vector get_abs_angles()
	{
		typedef Vector(__thiscall* OriginalFn)(void*);
		return ((OriginalFn)GetFunction(this, 11))(this);
	}

	inline bool is_dormant()
	{
		void* networkable = this + 8;
		using is_dormant_t = bool(__thiscall*)(PVOID);
		return method<is_dormant_t>(8, networkable)(networkable);
	}

	inline size_t get_index()
	{
		void* networkable = this + 8;
		using get_entity_index_t = size_t(__thiscall*)(PVOID);
		return method<get_entity_index_t>(9, networkable)(networkable);
	}

	inline bool is_npc()
	{
		return !!std::memcmp(this->get_classname(), _("npc"), 3);
	}

	inline bool is_player()
	{
		return !!std::strstr(this->get_classname(), _("player"));
	}
	
	inline const QAngle& get_angles()
	{
		auto offset = g_pNetvarManager->GetOffset(_("DT_HL2MPLocalPlayerExclusive"), _("m_angEyeAngles[0]"));
		return *reinterpret_cast<QAngle*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	inline const int32_t get_health()
	{
		static auto offset = g_pNetvarManager->GetOffset(_("DT_BaseEntity"), _("m_iHealth"));
		return *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	inline const int32_t get_max_health()
	{
		static auto offset = g_pNetvarManager->GetOffset(_("DT_BaseEntity"), _("m_iMaxHealth"));
		return *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	inline const Vector& get_pos()
	{
		static auto offset = g_pNetvarManager->GetOffset(_("DT_BaseEntity"), _("m_vecOrigin"));
		return *reinterpret_cast<Vector*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	inline const uint32_t get_team()
	{
		static auto offset = g_pNetvarManager->GetOffset(_("DT_BaseEntity"), _("m_iTeamNum"));
		return *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	inline uint32_t get_team_color()
	{
		auto glua = gm::SDK::Get().LuaShared()->GetLuaInterface(LUAINTERFACE_CLIENT);

		if (!glua)
			return {};

		glua->PushSpecial((int)lua::special::glob);
		glua->GetField(-1, _("team"));
		glua->GetField(-1, _("GetColor"));
		glua->PushNumber(this->get_team());
		glua->Call(1, 1);

		glua->PushString(_("r"));
		glua->GetTable(-2);
		auto r = glua->GetNumber(-1);
		glua->Pop();

		glua->PushString(_("g"));
		glua->GetTable(-2);
		auto g = glua->GetNumber(-1);
		glua->Pop();

		glua->PushString(_("b"));
		glua->GetTable(-2);
		auto b = glua->GetNumber(-1);
		glua->Pop(4);

		return 0xFF000000 + (static_cast<uint32_t>(r)) + (static_cast<uint32_t>(g) << 8) + (static_cast<uint32_t>(b) << 16);
	}

	inline std::optional<bool> is_admin()
	{
		auto glua = gm::SDK::Get().LuaShared()->GetLuaInterface(LUAINTERFACE_CLIENT);

		if (!glua)
			return {};

		glua->PushSpecial((int)lua::special::glob);
		glua->GetField(-1, _("Entity"));
		glua->PushNumber(this->get_index());
		glua->Call(1, 1);

		glua->GetField(-1, _("IsAdmin"));
		glua->Push(-2);
		glua->Call(1, 1);

		bool admin = glua->GetBool();
		glua->Pop(3);

		return admin;
	}

	inline const Vector& get_oob_min()
	{
		static auto offset = g_pNetvarManager->GetOffset(_("DT_BaseEntity"), _("m_vecMins"));
		return *reinterpret_cast<Vector*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	inline const Vector& get_oob_max()
	{
		static auto offset = g_pNetvarManager->GetOffset(_("DT_BaseEntity"), _("m_vecMaxs"));
		return *reinterpret_cast<Vector*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	inline std::string get_team_name()
	{
		auto glua = gm::SDK::Get().LuaShared()->GetLuaInterface(LUAINTERFACE_CLIENT);

		if (!glua)
			return {};

		glua->PushSpecial((int)lua::special::glob);
		glua->GetField(-1, _("team"));
		glua->GetField(-1, _("GetName"));
		glua->PushNumber(this->get_team());
		glua->Call(1, 1);

		std::string team = glua->GetString();
		glua->Pop(3);

		return team;
	}

	inline std::string get_name()
	{
		auto glua = gm::SDK::Get().LuaShared()->GetLuaInterface(LUAINTERFACE_CLIENT);

		if (!glua)
			return {};

		glua->PushSpecial((int)lua::special::glob);
		glua->GetField(-1, _("Entity"));
		glua->PushNumber(this->get_index());
		glua->Call(1, 1);

		glua->GetField(-1, _("Name"));
		glua->Push(-2);
		glua->Call(1, 1);

		std::string name = glua->GetString();
		glua->Pop(3);

		return name;
	}

	inline Vector GetEyePos()
	{
		Vector vecViewOffset = *reinterpret_cast<Vector*>((DWORD)this + 0xE8);
		return this->GetAbsOrigin() + vecViewOffset;
	}

	inline bool is_alive()
	{
		BYTE lifestate = *(BYTE*)((DWORD)this + 0x8F);
		return (lifestate == 0);
	}

	inline bool setup_bones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		void *pRenderable = (void*)(this + 0x4);
		typedef bool(__thiscall* OriginalFn)(PVOID, matrix3x4_t*, int, int, float);
		return ((OriginalFn)GetFunction(pRenderable, 16))(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}

	NETVAR(DT_BasePlayer, m_fFlags,			int);
	NETVAR(DT_GMOD_Player, m_bDrawViewmodel, bool);
	NETVAR(DT_BasePlayer, m_vecPunchAngle,	Vector);
	NETVAR(DT_BasePlayer, m_vecPunchAngleVel, Vector);
	NETVAR(DT_BasePlayer, m_iHealth,		int);
	NETVAR(DT_BasePlayer, m_vecViewOffset,  Vector);
	NETVAR(DT_BasePlayer, m_vecOrigin,		Vector);
	NETVAR(DT_BaseEntity, m_vecMins,		Vector);
	NETVAR(DT_BaseEntity, m_vecMaxs,		Vector);
	NETVAR(DT_BaseEntity, m_CollisionGroup, DWORD);
	NETVAR(DT_BaseEntity, m_hOwnerEntity,   CBaseHandle);
	NETVAR(DT_BasePlayer, m_nTickBase,		int);
	NETVAR(DT_LocalPlayerExclusive, m_hViewEntity,	CBaseHandle);

	BYTE* m_lifeState() {
		return (BYTE*)((DWORD)this + 0x8F);
	}

	Vector GetBonePos(int bone)
	{
		matrix3x4_t boneMatrix[MAXSTUDIOBONES];
		if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.0f)) {
			return boneMatrix[bone].at(3);
		}
		return Vector{};
	}


	inline QAngle *m_angEyeAngles() {
		static auto offy = 0x2BEC; //OFFSET(_("DT_BaseCombatCharacter"), _("m_angEyeAngles"));
		return (QAngle*)((uintptr_t)this + offy);
	}

	/*
	05855f394 = m_Collision
	05855F348 = m_rgflCoordinateFrame
	*/
	inline const matrix3x4_t* m_rgflCoordinateFrame()
	{
		static auto _m_rgflCoordinateFrame = OFFSET("DT_BaseEntity", "m_CollisionGroup") - (0x94 - 0x48);
		return (matrix3x4_t*)((uintptr_t)this + _m_rgflCoordinateFrame);
	}

};

class C_BaseCombatWeapon : public C_BaseEntity {
public:

	const char* GetPrintName() {
		using fnGetPrintName = const char* (__thiscall*)(C_BaseCombatWeapon* _this);
		return GetVFunc<fnGetPrintName>(this, 367)(this);
	}

};

float GetWeaponSpread(C_BaseCombatWeapon* pWeapon);
float GetWeaponCone(C_BaseCombatWeapon* pWeapon);
void UpdateAccuracyPenalty(C_BaseCombatWeapon* pWeapon);