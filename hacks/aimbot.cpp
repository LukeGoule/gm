#include "aimbot.h"


#include "../hacks/esp.h"

#include "../helpers/md5.h"
#include "../helpers/vector2d.h"
#include "../helpers/obfs.h"
#include "../helpers/math.h"

#include "../sdk/CInput.h"
#include "../sdk/ClientClass.h"
#include "../sdk/IVEngineClient.h"
#include "../sdk/C_BaseEntity.h"
#include "../sdk/CGlobalVarsBase.h"

#include "../imgui/imgui.h"
#include "../gmod/luaShared.h"

#include "../core/globals.h"
#include "../core/options.h"

auto vecExcludedClassNames = std::vector<const char*>{
		"CWeaponPhysGun",
		"CWeaponPhysCannon",
		"CWeaponCrowbar",
		"CWeaponFrag",
		"CWeaponRPG"
};

auto vecExcludedPrintNames = std::vector<const char*>{
	"#GMOD_ToolGun",
	"#GMOD_Camera",
};

#pragma warning(push)
#pragma warning(disable: 6387)
void Aimbot::Setup() {
	*(void**)&this->RandomSeed = GetProcAddress(GetModuleHandleA(_("vstdlib.dll")), _("RandomSeed"));
	*(void**)&this->RandomFloat = GetProcAddress(GetModuleHandleA(_("vstdlib.dll")), _("RandomFloat"));
}
#pragma warning(pop)

void Aimbot::CreateMove(CUserCmd* pCmd) {

	if (!pCmd)
		return;

	if (!o.bAimMasterEnabled)
		return;

	if (!g_pEngineClient || !g_pEngineClient->IsInGame())
		return;

	if (!g_pLocalPlayer || !g_pLocalPlayer->is_alive())
		return;

	if (!g_pLocalPlayer->m_hActiveWeapon().Get())
		return;

	auto pWeapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pLocalPlayer->m_hActiveWeapon().Get());

	// don't use any excluded weapons.
	for (auto Excluded : vecExcludedClassNames) {
		if (!strcmp(pWeapon->GetClientClass()->pNetworkName, Excluded)) return;
	}

	for (auto Excluded : vecExcludedPrintNames) {
		if (!strcmp(pWeapon->GetPrintName(), Excluded)) return;
	}
	
	QAngle vOldAngles	= pCmd->view_angles;
	float fOldForward	= pCmd->fwd_move;
	float fOldSidemove	= pCmd->side_move;

	this->DoAimbot(pCmd);
	
	if (o.bAimRemoveSpread) {
		this->RemoveSpread(pCmd);
		this->RemoveKickDeviation(pCmd);
	}

	this->CorrectMovement(vOldAngles, pCmd, fOldForward, fOldSidemove);

	return;
}

void Aimbot::DoAimbot(CUserCmd* pCmd) {
	TargetData_t* pTarg = nullptr;

	if (!o.bAimAutofire) {
		if (!(pCmd->buttons & IN_ATTACK)) {
			return;
		}
	}

	pTarg = this->FindBestTarget();
	
	if (!pTarg) {
		return;
	}

	if (!pTarg->pEntity) {
		return;
	}

	if (o.bAimAutofire) {
		pCmd->buttons |= IN_ATTACK;
	}

	pCmd->view_angles = pTarg->angTargetHead;
}

void Aimbot::CorrectMovement(QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
{
	//side/forward move correction
	float deltaView = pCmd->view_angles.yaw - vOldAngles.yaw;
	float f1;
	float f2;

	if (vOldAngles.yaw < 0.f)
		f1 = 360.0f + vOldAngles.yaw;
	else
		f1 = vOldAngles.yaw;

	if (pCmd->view_angles.yaw < 0.0f)
		f2 = 360.0f + pCmd->view_angles.yaw;
	else
		f2 = pCmd->view_angles.yaw;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	pCmd->fwd_move	= cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	pCmd->side_move = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}

bool bForceSpread = false;
void Aimbot::SetUseForceSpread(bool b) {
	bForceSpread = b;
}

Vector forceSpreadVec = Vector(0, 0, 0);
void Aimbot::ForceSpread(Vector vForceSpread) {
	forceSpreadVec = vForceSpread;
}

void RandomSeed(float seed)
{
	auto glua = g_pLuaShared->GetLuaInterface(LUAINTERFACE_CLIENT);

	if (!glua)
		return;

	glua->PushSpecial(lua::SPECIAL_GLOB); // 1
	glua->GetField(-1, "math"); // 2
	glua->GetField(-1, "randomseed"); // 3
	glua->PushNumber(seed); // 4
	glua->Call(1, 0); // 4 - 2 = 2 + 0 = 2

	glua->Pop(2);
}

float Rand(float min, float max)
{
	auto glua = g_pLuaShared->GetLuaInterface(LUAINTERFACE_CLIENT);

	if (!glua)
		return 0.f;

	glua->PushSpecial(lua::SPECIAL_GLOB); // 1
	glua->GetField(-1, "math"); // 2
	glua->GetField(-1, "Rand"); // 3
	glua->PushNumber(min); // 4
	glua->PushNumber(max); // 5
	glua->Call(2, 1); // 5 - 3 = 2 + 1 = 3

	float randfloat = (float)glua->GetNumber(-1);
	glua->Pop(3);

	return randfloat;
}

float get_fas2_cone()
{
	auto glua = g_pLuaShared->GetLuaInterface(LUAINTERFACE_CLIENT);

	if (!glua)
		return 0.f;

	glua->PushSpecial(lua::SPECIAL_GLOB); // 1
	glua->GetField(-1, "LocalPlayer"); // 2
	glua->Call(0, 1); // 2 - 1 = 1 + 1 = 2

	glua->GetField(-1, "GetActiveWeapon"); // 3
	glua->Push(-2); // 4
	glua->Call(1, 1); // 4 - 2 = 2 + 1 = 3

	glua->GetField(-1, "CurCone"); // 4

	float cone = (float)glua->GetNumber(-1);

	glua->Pop(4);

	return cone;
}

void DoSpreadFAS(CUserCmd* cmd)
{
	if (!(cmd->buttons & IN_ATTACK) || !g_pLocalPlayer->m_hActiveWeapon().Get())
		return;

	float cone = get_fas2_cone();

	if (cone == 0.f)
		return;

	Utils::ConsolePrint("%f\n", g_pGlobalVars->m_fIntervalPerTick);
	RandomSeed(*g_pLocalPlayer->m_nTickBase() * g_pGlobalVars->m_fIntervalPerTick);

	float x = Rand(-cone, cone);
	float y = Rand(-cone, cone);
	
	auto pPunch = g_pLocalPlayer->m_vecPunchAngle();
	cmd->view_angles -= VEC2QANG((*pPunch)) * 0.5;

	QAngle nospread = cmd->view_angles + QAngle(-x, -y, 0);// *30;
	cmd->view_angles = nospread;
}

void Aimbot::RemoveSpread(CUserCmd* pCmd) {

	if (this->bUseFASNoSpread) {
		DoSpreadFAS(pCmd);

		return;
	}

	QAngle vIn = pCmd->view_angles;
	QAngle vOut;

	Vector vForward, vRight, vUp, vSpread;
	QAngle qWeapSpread;
	Vector vecSpread;

	RandomSeed((((MD5_PseudoRandom(pCmd->command_nr) & 0x7ffffff)) & 255));

	if (!bForceSpread) {
		vecSpread = this->GetBulletSpread((C_BaseCombatWeapon*)(g_pLocalPlayer->m_hActiveWeapon().Get()));
	}
	else {
		vecSpread = forceSpreadVec;
	}
	float flRandomX = RandomFloat(-0.5, 0.5) + RandomFloat(-0.5, 0.5);
	float flRandomY = RandomFloat(-0.5, 0.5) + RandomFloat(-0.5, 0.5);

	vSpread.x = flRandomX * vecSpread.x;
	vSpread.y = flRandomY * vecSpread.y;
	vSpread.z = 0.0f;

	Math::AngleVectors(pCmd->view_angles, vForward, vRight, vUp);

	Vector vDir = vForward + (vRight * vSpread.x) + (vUp * vSpread.y);
	Math::VectorAngles(vDir, qWeapSpread);
	Math::Normalize3(qWeapSpread);

	vOut = vIn - qWeapSpread;
	pCmd->view_angles = pCmd->view_angles + vOut;
}

void Aimbot::RemoveKickDeviation(CUserCmd* pCmd) {
	if (!bUseFASNoSpread) {
		auto pPunch = g_pLocalPlayer->m_vecPunchAngle();
		pCmd->view_angles -= VEC2QANG((*pPunch)) * 1.2;
	}
}

Vector Aimbot::GetBulletSpread(C_BaseCombatWeapon* pWeapon) {
	// stripped from IDA
	float* fvec = (float*)(*(int (**)(void))(*(DWORD*)pWeapon + 1300))();
	return Vector(fvec[0], fvec[1], fvec[2]);
}

QAngle Aimbot::CalcAngle(Vector& src, Vector& dst)
{
	QAngle vAngle;
	Vector delta((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));
	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

	vAngle.pitch = float(atanf(float(delta.z / hyp)) * 57.295779513082f);
	vAngle.yaw = float(atanf(float(delta.y / delta.x)) * 57.295779513082f);
	vAngle.roll = 0.0f;

	if (delta.x >= 0.0)
	{
		vAngle.yaw += 180.0f;
	}

	return vAngle;
}

Aimbot::TargetData_t* Aimbot::FindBestTarget() {
	this->pTarget->pEntity = 0;
	this->pTarget->angTargetHead = QAngle(0, 0, 0);
	this->pTarget->vecTargetHead = Vector(0, 0, 0);

	C_BaseEntity* pTempEntity = 0;
	float fCurrentMinDist = 99999999.f;

	// strangely, g_pEngineClient->getscreensize(x,y) is always 0? i check in IDA to see if the function was missing and it isn't. oh well.
	static ImGuiIO& IO = ImGui::GetIO();
	Vector2D vecScreenMiddle(IO.DisplaySize.x / 2.f, IO.DisplaySize.y / 2.f); // another point, the values given here when the game is minimized are totally wrong, i'll need to look into fixing that.

	for (size_t i = 0; i < g_pEntityList->GetHighestEntityIndex(); i++) {
		C_BaseEntity* pEnt = reinterpret_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(i));

		if (!pEnt)
			continue;

		if (pEnt->IsDormant())
			continue;

		if (pEnt == g_pLocalPlayer)
			continue;

		bool bIsPlayer	= false;
		bool bIsNPC		= false;

		auto classname = pEnt->GetClientClass()->pNetworkName;
		if (strstr(classname, "GMOD_Player")) {
			// ply
			bIsPlayer = true;
		}
		else if (strstr(classname, "NPC")) {
			// npc
			bIsNPC = true;
		}
		else {
			continue;
		}

		if (*pEnt->m_lifeState() != LIFE_ALIVE)
			continue;

		// target based on closest to our crosshair.

		Vector vecEntEyePos = pEnt->GetEyePos(); //pEnt->GetBonePos(8); // 8 = head on anything that looks like it has a head
		Vector _vec2dHeadPos;

		if (!ESP::WorldToScreen(vecEntEyePos, _vec2dHeadPos))
			continue;

		Vector2D vec2dHeadPos(_vec2dHeadPos.x, _vec2dHeadPos.y);

		float fDistance = sqrt(
			(vec2dHeadPos.x * vecScreenMiddle.x) +
			(vec2dHeadPos.y * vecScreenMiddle.y)
		);

		if (fDistance < fCurrentMinDist) {
			pTempEntity = pEnt;

			fCurrentMinDist = fDistance;
		}
	}
	
	if (pTempEntity) {
		pTarget->pEntity = pTempEntity;

		o.pCurrentTarget = (void*)pTarget->pEntity;
	}

	if (pTarget && pTarget->pEntity) {
		// We have a target

		Vector vecOurEyePos = g_pLocalPlayer->GetEyePos();
		Vector vecTheirEyePos = pTarget->pEntity->GetBonePos(o.iTargetBoneID); //pTarget->pEntity->GetEyePos();

		pTarget->angTargetHead = this->CalcAngle(vecOurEyePos, vecTheirEyePos);

		//if (!ESP::WorldToScreen(vecTheirEyePos, pTarget->vecTargetHead2D)) return nullptr;
		pTarget->vecTargetHead = vecTheirEyePos;

		return pTarget;
	}
	else {
		// No target, tell the aimbot::createmove to do nothing.

		o.pCurrentTarget = nullptr;

		return nullptr;
	}
}