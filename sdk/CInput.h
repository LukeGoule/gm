#pragma once

#include "Interface.h"
#include "../helpers/math.h"

#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)
#define IN_SPEED		(1 << 17)
#define IN_WALK			(1 << 18)
#define IN_ZOOM			(1 << 19)
#define IN_WEAPON1		(1 << 20)
#define IN_WEAPON2		(1 << 21)
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)
#define IN_GRENADE2		(1 << 24)
#define IN_LOOKSPIN		(1 << 25)

#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<10)	// In water

#include "../helpers/crc.h"
#include "../helpers/qangle.h"

constexpr auto multiplayer_backup = 90;

class bf_read;
class bf_write;

struct CUserCmd
{
	std::int32_t command_nr;
	std::int32_t tick_count;
	QAngle        view_angles;
	float        fwd_move;
	float        side_move;
	float        up_move;
	std::int32_t buttons;
	std::uint8_t impulse;
	std::size_t  weapon;
	std::int32_t weapon_type;
	std::int32_t random_seed;
	std::int16_t mouse_dx;
	std::int16_t mouse_dy;
	bool         has_been_predicted;

private:
	std::uint8_t __pad[0x108]; // WARNING: Removing this will lead to dinosaurs with
							   // laser guns spawning behind you

public:
	CRC32_t get_checksum() const
	{
		CRC32_t crc{};

		NEWCRC32_Init(&crc);
		NEWCRC32_ProcessBuffer(&crc, &command_nr, sizeof(command_nr));
		NEWCRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
		NEWCRC32_ProcessBuffer(&crc, &view_angles, sizeof(view_angles));
		NEWCRC32_ProcessBuffer(&crc, &fwd_move, sizeof(fwd_move));
		NEWCRC32_ProcessBuffer(&crc, &side_move, sizeof(side_move));
		NEWCRC32_ProcessBuffer(&crc, &up_move, sizeof(up_move));
		NEWCRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		NEWCRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		NEWCRC32_ProcessBuffer(&crc, &weapon, sizeof(weapon));
		NEWCRC32_ProcessBuffer(&crc, &weapon_type, sizeof(weapon_type));
		NEWCRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
		NEWCRC32_ProcessBuffer(&crc, &mouse_dx, sizeof(mouse_dx));
		NEWCRC32_ProcessBuffer(&crc, &mouse_dy, sizeof(mouse_dy));

		NEWCRC32_ProcessBuffer(
			&crc, __pad + 5, sizeof(bool)); // I haven't checked out what this is

		NEWCRC32_Final(&crc);

		return crc;
	}
};

class CVerifiedUserCmd
{
public:
	CUserCmd  m_cmd;
	CRC32_t   m_crc;
};

class CInput
{
	char __pad[0x31 * sizeof(uint32_t)];

public:
	inline CUserCmd* GetUserCmd(int sequence_number)
	{
		using OriginalFn = CUserCmd * (__thiscall *)(void *, int);
		return ((OriginalFn)GetFunction(this, 8))(this, sequence_number);
	}

	inline CVerifiedUserCmd* GetVerifiedCmd(int sequence_number)
	{
		return &this->verified_cmds[sequence_number % multiplayer_backup];
	}

	
	CUserCmd*         cmds;
	CVerifiedUserCmd* verified_cmds;
};