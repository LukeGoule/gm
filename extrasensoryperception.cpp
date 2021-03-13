#include "extrasensoryperception.h"

#include "aimbot.h"

#include "vector.h"
#include "vector2d.h"
#include "vmatrix.h"

#include "IVEngineClient.h"
#include "IEntityList.h"
#include "globals.h"

#include "C_BaseEntity.h"
#include "ICollideable.h"

#include "DXDraw.h"

bool ESP::WorldToScreen(Vector in, Vector& out)
{
    const VMatrix w2sm = g_pEngineClient->GetViewMatrix();
    float w = w2sm[3][0] * in.x + w2sm[3][1] * in.y + w2sm[3][2] * in.z + w2sm[3][3];

    if (w > 0.001f)
    {
        int width, height; g_pEngineClient->GetScreenSize(width, height);

        float fl1DBw = 1 / w;
        out.x = (width / 2) + (0.5f * ((w2sm[0][0] * in.x + w2sm[0][1] * in.y + w2sm[0][2] * in.z + w2sm[0][3]) * fl1DBw) * width + 0.5f);
        out.y = (height / 2) - (0.5f * ((w2sm[1][0] * in.x + w2sm[1][1] * in.y + w2sm[1][2] * in.z + w2sm[1][3]) * fl1DBw) * height + 0.5f);
        return true;
    }
    return false;
}

CFont* pESPFont;

void ESP::Setup() {
	pESPFont = new CFont(g_pD3DDevice9, 13, "Verdana");
}

BB2D ESP::DrawBounds(C_BaseEntity* pEnt) {
	const matrix3x4_t& trans = *pEnt->m_rgflCoordinateFrame();

	Vector min = *pEnt->m_vecMins();
	Vector max = *pEnt->m_vecMaxs();

	Vector points[] = { Vector(min.x, min.y, min.z),
						Vector(min.x, max.y, min.z),
						Vector(max.x, max.y, min.z),
						Vector(max.x, min.y, min.z),
						Vector(max.x, max.y, max.z),
						Vector(min.x, max.y, max.z),
						Vector(min.x, min.y, max.z),
						Vector(max.x, min.y, max.z) };

	Vector pointsTransformed[8];
	for (int i = 0; i < 8; i++)
	{
		Math::VectorTransform(points[i], trans, pointsTransformed[i]);
	}

	Vector pos = pEnt->GetAbsOrigin();
	Vector flb;
	Vector brt;
	Vector blb;
	Vector frt;
	Vector frb;
	Vector brb;
	Vector blt;
	Vector flt;


	if (!WorldToScreen(pointsTransformed[3], flb) || !WorldToScreen(pointsTransformed[5], brt)
		|| !WorldToScreen(pointsTransformed[0], blb) || !WorldToScreen(pointsTransformed[4], frt)
		|| !WorldToScreen(pointsTransformed[2], frb) || !WorldToScreen(pointsTransformed[1], brb)
		|| !WorldToScreen(pointsTransformed[6], blt) || !WorldToScreen(pointsTransformed[7], flt))
		return {};

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;		// left
	float top = flb.y;		// top
	float right = flb.x;	// right
	float bottom = flb.y;	// bottom

	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (top < arr[i].y)
			top = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (bottom > arr[i].y)
			bottom = arr[i].y;
	}

	auto DrawLine = [](Vector a, Vector b, DWORD clr) {
		g_pDrawHelper->Line(a.x, a.y, b.x, b.y, clr);
	};

	auto clr = 0xFFFFFFFF;

	DrawLine(Vector(left,	bottom, 0), Vector(left,	top,	0), clr);
	DrawLine(Vector(left,	top,	0), Vector(right,	top,	0), clr);
	DrawLine(Vector(right,	top,	0), Vector(right,	bottom, 0), clr);
	DrawLine(Vector(right,	bottom, 0), Vector(left,	bottom, 0), clr);

	return { left, top, right, bottom };
}

void ESP::DrawPlayer(BB2D drawResult, C_BaseEntity* pPlayer) {
	if (o.bESPDrawClassName)
		pESPFont->Render(drawResult.left, drawResult.bottom - 14, 0xFFFFFFFF, CFont::CFont_Flags::F_DROPSHADOW, "%s", pPlayer->GetClientClass()->pNetworkName);

	if (o.bESPDrawPlayerNames) {
		player_info_t info;
		g_pEngineClient->GetPlayerInfo(pPlayer->entindex(), &info);

		pESPFont->Render(drawResult.left, drawResult.bottom - (14*2), 0xFFFFFFFF, CFont::CFont_Flags::F_DROPSHADOW, "%s", info.name);
	}

	// Aimbot Target Indicator
	if (o.bAimMasterEnabled && o.pCurrentTarget && (C_BaseEntity*)o.pCurrentTarget == pPlayer) {
		pESPFont->Render(drawResult.left, drawResult.bottom - (14 * 2), 0xFFFF0000, CFont::CFont_Flags::F_DROPSHADOW, "TARGET");
	}
}

void ESP::DrawNPC(BB2D drawResult, C_BaseEntity* pNPC) {
	if (o.bESPDrawClassName)
		pESPFont->Render(drawResult.left, drawResult.bottom - 14, 0xFFFFFFFF, CFont::CFont_Flags::F_DROPSHADOW, "%s", pNPC->GetClientClass()->pNetworkName);

	// Aimbot Target Indicator
	if (o.bAimMasterEnabled && o.pCurrentTarget && (C_BaseEntity*)o.pCurrentTarget == pNPC) {
		pESPFont->Render(drawResult.left, drawResult.bottom - (14 * 2), 0xFFFF0000, CFont::CFont_Flags::F_DROPSHADOW, "TARGET");

		Vector vHeadPosition = Aimbot::Get().pTarget->vecTargetHead;
		Vector v2dHeadPosition;
		
		if (!WorldToScreen(vHeadPosition, v2dHeadPosition))
			return;

		g_pDrawHelper->Box(v2dHeadPosition.x - 1, v2dHeadPosition.y - 1, 2, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
}

void ESP::DrawWeapon(BB2D drawResult, C_BaseCombatWeapon* pWeapon) {
	if (o.bESPDrawClassName)
		pESPFont->Render(drawResult.left, drawResult.bottom - 14, 0xFFFFFFFF, CFont::CFont_Flags::F_DROPSHADOW, "%s", pWeapon->GetPrintName());
}

void ESP::DoDraw(int ClassType, C_BaseEntity* pEnt) {
	BB2D Bounds;

	switch (ClassType) {
	case 0: // Player

		if (!o.bESPShowPlayers) return;

		Bounds = this->DrawBounds(pEnt);
		this->DrawPlayer(Bounds, pEnt);

		break;

	case 1: // NPC

		if (!o.bESPShowNPCs) return;

		Bounds = this->DrawBounds(pEnt);
		this->DrawNPC(Bounds, pEnt);

		break;

	case 2: // Weapon

		if (!o.bESPShowPlayers) return;

		// weapons which are not being held, i.e dropped weapons, have no owner
		// we want to avoid drawing these otherwise the screen will be full of random lines from our own weapons
		// even when weapons aren't actually being rendered they still exist.
		//if (pEnt->m_hOwnerEntity()->IsValid()) 
		//	return;

		Bounds = this->DrawBounds(pEnt);
		this->DrawWeapon(Bounds, reinterpret_cast<C_BaseCombatWeapon*>(pEnt));

		break;
	}
}

void ESP::Render() {
    if (!g_pEngineClient || !g_pEngineClient->IsInGame())
        return;

    if (!g_pEntityList)
        return;

	if (!g_pLocalPlayer)
		return;

    // STUFF
	for (int i = 0; i < g_pEntityList->GetHighestEntityIndex(); i++) {
		C_BaseEntity* pEnt = reinterpret_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(i));

		if (!pEnt)
			continue;

		if (pEnt == g_pLocalPlayer) {
			g_pLocalPlayer = pEnt;
			continue;
		}

		if ((*pEnt->m_hOwnerEntity()).Get() == g_pLocalPlayer)
			continue;

		auto ClassName = std::string(pEnt->GetClientClass()->pNetworkName);
		auto ClassType = -1;

		if (ClassName.find("GMOD_Player") != std::string::npos)
			ClassType = ESPCLASSTYPE_PLAYER;

		if (ClassName.find("NPC") != std::string::npos)
			ClassType = ESPCLASSTYPE_NPC;

		if (ClassName.find("Weapon") != std::string::npos)
			ClassType = ESPCLASSTYPE_WEAPON;

		if (ClassType < 0)
			continue;

		this->DoDraw(ClassType, pEnt);
	}
}