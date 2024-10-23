#include "luabindings.h"
#include "hooks.h"

#include "../helpers/utils.h"
#include "../menu/GMMenu.h"
#include "../helpers/obfs.h"
#include "../hacks/aimbot.h"
#include "../C_BaseEntity.h"
#include "../helpers/md5.h"
#include "../CInput.h"

#define CL_LUA g_pLuaShared->GetLuaInterface(LUAINTERFACE_CLIENT)
#define LUA_START_FUNCTION auto LUA = CL_LUA;
#define LUA_END_FUNCTION(argc) return (argc);

LuaBinding_Func::LuaBinding_Func(const char* pszName, CFunc Function) {
	this->m_pszName = (char*)pszName;
	this->m_Function = Function;
}

LuaBindings::LuaBindings() {

}

LuaBindings::~LuaBindings() {

}

void LuaBindings::NewFunction(const char* pszName, CFunc Function) {
	if (!pszName) {
		Utils::ConsolePrint(_("[LUA] Attempt to create lua bound function with no name.\n"));
		return;
	}

	if (!Function) {
		Utils::ConsolePrint(_("[LUA] Attempt to set function %s with no C function.\n"), pszName);
	}

	this->LuaBoundFunctions.push_back(new LuaBinding_Func(pszName, Function));
}

void LuaBindings::SetupFunctions() {
	this->LuaBoundFunctions.clear(); // clear from previous executions.

	NewFunction(_("Loki_IsMenuOpen"), [](lua_State* L) {
		LUA_START_FUNCTION;
		{
			LUA->PushBool(GmMenu::Get().m_bMenuOpen);
		}
		LUA_END_FUNCTION(1);
	});

	NewFunction(_("Loki_ConsolePrint"), [](lua_State* L) {
		LUA_START_FUNCTION;
		{
			auto msg = CL_LUA->CheckString();

			Utils::ConsolePrint(msg);
		}
		LUA_END_FUNCTION(0);
	});

	// returns engine spread of localplayer's weapon
	NewFunction(_("Loki_GetEngineSpread"), [](lua_State* L) {
		LUA_START_FUNCTION;
		{
			if (!g_pLocalPlayer || !g_pLocalPlayer->m_hActiveWeapon().Get()) {
				auto nullvec = Vector(0, 0, 0);
				CL_LUA->PushVector(nullvec);

				LUA_END_FUNCTION(1);
			}

			auto pWeapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pLocalPlayer->m_hActiveWeapon().Get());
			auto vWeaponSpread = Aimbot::GetBulletSpread(pWeapon);
			CL_LUA->PushVector(vWeaponSpread);
		}
		LUA_END_FUNCTION(1);
	});

	NewFunction(_("Loki_RandomSeed"), [](lua_State* L) {
		LUA_START_FUNCTION;
		{
			auto pCmd = reinterpret_cast<CUserCmd*>(CL_LUA->GetUserdata()->data);

			Aimbot::Get().RandomSeed((((MD5_PseudoRandom(pCmd->command_nr) & 0x7ffffff)) & 255));
		}
		LUA_END_FUNCTION(0);
		});

	NewFunction(_("Loki_RandomFloat"), [](lua_State* L) {
		LUA_START_FUNCTION;
		{
			float min = CL_LUA->CheckNumber(-2);
			float max = CL_LUA->CheckNumber(-1);

			CL_LUA->PushNumber(Aimbot::Get().RandomFloat(min, max));
		}
		LUA_END_FUNCTION(1);
	});

	/*
	Math::AngleVectors(pCmd->view_angles, vForward, vRight, vUp);

	Vector vDir = vForward + (vRight * vSpread.x) + (vUp * vSpread.y);
	Math::VectorAngles(vDir, qWeapSpread);
	Math::Normalize3(qWeapSpread);
	*/

	NewFunction(_("AngleVectors"), [](lua_State* L) {
		LUA_START_FUNCTION;
		{
			QAngle angle = CL_LUA->GetAngle();
			Vector vForward, vRight, vUp;
			Math::AngleVectors(angle, vForward, vRight, vUp);

			CL_LUA->PushVector(vForward);
			CL_LUA->PushVector(vRight);
			CL_LUA->PushVector(vUp);

		}
		LUA_END_FUNCTION(3);
	});
	
	NewFunction(_("VectorAngles"), [](lua_State* L) {
		LUA_START_FUNCTION;
		{
			Vector vVec = CL_LUA->GetVector();
			QAngle qOutput;
			Math::VectorAngles(vVec, qOutput);

			CL_LUA->PushAngle(qOutput);
		}
		LUA_END_FUNCTION(1);
	});

	NewFunction(_("Loki_GetSilentAngle"), [](lua_State* L) {
		LUA_START_FUNCTION;
		{
			CL_LUA->PushAngle(Hooks::qCurrentAngle);
		}
		LUA_END_FUNCTION(1);
	});

	NewFunction(_("Loki_NospreadFAS"), [](lua_State* L) {
		LUA_START_FUNCTION;
		{
			CUserCmd* pCmd = reinterpret_cast<CUserCmd*>(CL_LUA->GetUserdata(-3));
			if (!pCmd) return 0;
			float x = (float)CL_LUA->GetNumber(-2);
			float y = (float)CL_LUA->GetNumber(-1);

			QAngle nospread = pCmd->view_angles + QAngle(-x, -y, 0) * 25;
			pCmd->view_angles = nospread;
		}
		LUA_END_FUNCTION(0);
	});

	NewFunction(_("Loki_SetUseForceSpread"), [](lua_State* L) {
		LUA_START_FUNCTION;
		{
			Aimbot::Get().SetUseForceSpread(CL_LUA->GetBool());
		}
		LUA_END_FUNCTION(1);
	});

	NewFunction(_("Loki_ForcedSpread"), [](lua_State* L) {
		LUA_START_FUNCTION;
		{
			Aimbot::Get().ForceSpread(CL_LUA->GetVector());
		}
		LUA_END_FUNCTION(1);
	});

	NewFunction(_("Loki_Nospread"), [](lua_State* L) {
		LUA_START_FUNCTION;
		{
			CUserCmd* pCmd = (CUserCmd*)CL_LUA->GetUserdata()->data;
			Aimbot::Get().RemoveSpread(pCmd);
		}
		LUA_END_FUNCTION(1);
	});
	
	NewFunction(_("Loki_Aimbot_UseFASSpreadValues"), [](lua_State* L) {
		LUA_START_FUNCTION;
		{
			Aimbot::Get().bUseFASNoSpread = CL_LUA->GetBool();
		}
		LUA_END_FUNCTION(1);
	});
}

void LuaBindings::CreateFunctions(lua::ILuaInterface* LUA) {
	for (auto lfunc : this->LuaBoundFunctions) {
		// Utils::ConsolePrint("[LUA] Pushed %s\n", lfunc->m_pszName); // removed -- too much console spam.
		LUA->PushSpecial(lua::SPECIAL_GLOB);	// Push the global table
		LUA->PushCFunction(lfunc->m_Function);	// Push our function
		LUA->SetField(-2, lfunc->m_pszName);	// Set MyFirstFunction in lua to our C++ function
		LUA->Pop();								// Pop the global table off the stack
	}
}

void LuaBindings::ExecString(lua::ILuaInterface* LUA, std::string str) {
	// load all Loki_ bindings
	this->SetupFunctions();
	this->CreateFunctions(LUA);
	
	// internally execute the string
	LUA->RunString("", "", str.c_str(), true, true);
}