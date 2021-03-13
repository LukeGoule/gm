#include "C_BaseEntity.h"

float GetWeaponSpread(C_BaseCombatWeapon* pWeapon)
{
	float flResult = 0.0f;
	/*
	__asm
	{
		MOV RAX, 1300;
		MOV RSI, pWeapon;
		MOV RDX, DWORD PTR DS : [RSI] ;
		MOV RAX, DWORD PTR DS : [RDX + RAX] ;
		MOV RCX, ESI;
		CALL RAX;
		FSTP flResult;
	}*/

	return flResult;
}

float GetWeaponCone(C_BaseCombatWeapon* pWeapon)
{
	float flResult = 0.0f;
	/*
	__asm
	{
		MOV RAX, 0x5BC;
		MOV RSI, pWeapon;
		MOV RDX, DWORD PTR DS : [RSI] ;
		MOV RAX, DWORD PTR DS : [RDX + RAX] ;
		MOV RCX, RSI;
		CALL RAX;
		FSTP flResult;
	}
	*/
	return flResult;
}

void UpdateAccuracyPenalty(C_BaseCombatWeapon* pWeapon)
{
	/*
	__asm
	{
		MOV EAX, 0x5C0
		MOV ESI, pWeapon
		MOV EDX, DWORD PTR DS : [ESI]
		MOV EAX, DWORD PTR DS : [EDX + EAX]
		MOV ECX, ESI
		CALL EAX
	}
	*/
}