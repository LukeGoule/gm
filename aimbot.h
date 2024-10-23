#pragma once

#include "singleton.h"
#include "vector.h"
#include "qangle.h"

struct CUserCmd;

class C_BaseCombatWeapon;
class C_BaseEntity;

class Aimbot : public Singleton<Aimbot> {
public:

	void (*RandomSeed)(int);
	float (*RandomFloat)(float, float);

	struct TargetData_t {
		// Target's entity pointer.
		C_BaseEntity* pEntity = nullptr;

		// our head -> targets head
		QAngle angTargetHead; 
		
		// target's head for ESP W2S.
		Vector vecTargetHead; 
	};

	TargetData_t* pTarget = new TargetData_t; // current target's information, used in ESP.

	void			Setup();
	void			CreateMove(CUserCmd* pCmd);
	void			DoAimbot(CUserCmd* pCmd);
	void			CorrectMovement(QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove);
	void			RemoveSpread(CUserCmd* pCmd);
	void			RemoveKickDeviation(CUserCmd* pCmd);
	static Vector	GetBulletSpread(C_BaseCombatWeapon* pWeapo);
	QAngle			CalcAngle(Vector& src, Vector& dst);
	TargetData_t*	FindBestTarget();

	void			ForceSpread(Vector vForceSpread);
	void			SetUseForceSpread(bool b);

	bool bUseFASNoSpread = false;
};