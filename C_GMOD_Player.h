#pragma once

#include "helpers/vector.h"
#include "helpers/qangle.h"
#include "C_BaseEntity.h"

class C_GMOD_Player : public C_BaseEntity
{
public:

	char pad_0x0000[0x2C]; //0x0000
	float flCurTime; //0x002C 
	char pad_0x0030[0x2D8]; //0x0030
	Vector angRotation; //0x0308 radians?
	char pad_0x0314[0x1C]; //0x0314
	float fUpDownAngle; //0x0330 
	char pad_0x0334[0x4]; //0x0334
	Vector vecABSPos; //0x0338 
	char pad_0x0344[0x1F8C]; //0x0344
	QAngle vecPunch1; //0x22D0 
	char pad_0x22DC[0x30]; //0x22DC
	QAngle vecPunchAngle; //0x230C 
	char pad_0x2318[0x428]; //0x2318

}; //Size=0x2740