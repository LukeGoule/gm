#pragma once

#include "singleton.h"
#include "vector.h"

#define ESPCLASSTYPE_PLAYER 0
#define ESPCLASSTYPE_NPC	1
#define ESPCLASSTYPE_WEAPON 2

class C_BaseEntity;
class C_BaseCombatWeapon;

struct BB2D {
	float left;
	float top;
	float right;
	float bottom;
};

class ESP : public Singleton<ESP> {

public:
	void Setup();

	BB2D DrawBounds(C_BaseEntity* pEnt);
	void DrawPlayer(BB2D drawResult, C_BaseEntity* pPlayer);
	void DrawNPC(BB2D drawResult, C_BaseEntity* pNPC);
	void DrawWeapon(BB2D drawResult, C_BaseCombatWeapon* pWeapon);
	void Render();
	void DoDraw(int ClassType, C_BaseEntity* pEnt);

	static bool WorldToScreen(Vector in, Vector& out);
};