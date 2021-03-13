#pragma once

#ifndef GARRYSMOD_LUA_TYPES_H
#define GARRYSMOD_LUA_TYPES_H

#ifdef ENTITY
#undef ENTITY
#endif

#ifdef VECTOR
#undef VECTOR
#endif

#include "obfs.h"

namespace GarrysMod
{
	namespace Lua
	{
		namespace Type
		{
			enum
			{
				// Deprecated: Use NONE instead of INVALID
				INVALID = -1,

				// Lua Types
				NONE = -1,
				NIL,
				BOOL,
				LIGHTUSERDATA,
				NUMBER,
				STRING,
				TABLE,
				FUNCTION,
				USERDATA,
				THREAD,

				// GMod Types
				ENTITY,
				VECTOR,
				ANGLE,
				PHYSOBJ,
				SAVE,
				RESTORE,
				DAMAGEINFO,
				EFFECTDATA,
				MOVEDATA,
				RECIPIENTFILTER,
				USERCMD,
				SCRIPTEDVEHICLE,
				MATERIAL,
				PANEL,
				PARTICLE,
				PARTICLEEMITTER,
				TEXTURE,
				USERMSG,
				CONVAR,
				IMESH,
				MATRIX,
				SOUND,
				PIXELVISHANDLE,
				DLIGHT,
				VIDEO,
				FILE,
				LOCOMOTION,
				PATH,
				NAVAREA,
				SOUNDHANDLE,
				NAVLADDER,
				PARTICLESYSTEM,
				PROJECTEDTEXTURE,

				COUNT
			};

			// You should use ILuaBase::GetTypeName instead of directly accessing this array
			static const char* Name[] =
			{
				_("nil"),
				_("bool"),
				_("lightuserdata"),
				_("number"),
				_("string"),
				_("table"),
				_("function"),
				_("userdata"),
				_("thread"),
				_("entity"),
				_("vector"),
				_("angle"),
				_("physobj"),
				_("save"),
				_("restore"),
				_("damageinfo"),
				_("effectdata"),
				_("movedata"),
				_("recipientfilter"),
				_("usercmd"),
				_("vehicle"),
				_("material"),
				_("panel"),
				_("particle"),
				_("particleemitter"),
				_("texture"),
				_("usermsg"),
				_("convar"),
				_("mesh"),
				_("matrix"),
				_("sound"),
				_("pixelvishandle"),
				_("dlight"),
				_("video"),
				_("file"),
				_("locomotion"),
				_("path"),
				_("navarea"),
				_("soundhandle"),
				_("navladder"),
				_("particlesystem"),
				_("projectedtexture"),

				0
			};
		}
	}
}

#endif