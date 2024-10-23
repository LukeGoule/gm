#include "beams.h"

#include "../helpers/vector.h"
#include "../helpers/color.h"
#include "../helpers/obfs.h"

#include "../sdk.h"
#include "../IVEngineClient.h"
#include "../globals.h"

std::vector<std::string> beams::beam_sprites = {
	"materials/sprites/physbeam.vmt",
	"sprites/purplelaser1.vmt",
	"sprites/blueglow1.vmt",
	"sprites/bubble.vmt",
	"sprites/glow01.vmt",
	"sprites/radio.vmt",
	"sprites/white.vmt",
};

BeamInfo_t beams::GenerateBeamInfo(
	Color c,
	Vector pos1,
	Vector pos2,
	std::string sprite,
	float lifeSeconds,
	float startWidth,
	float endWidth,
	float brightness) {
	BeamInfo_t beamInfo;

	beamInfo.m_nType = TE_BEAMPOINTS;
	beamInfo.m_pszModelName = sprite.c_str();
	beamInfo.m_nModelIndex = -1;
	beamInfo.m_flHaloScale = 0.0f;
	beamInfo.m_flLife = lifeSeconds;
	beamInfo.m_flWidth = startWidth;
	beamInfo.m_flEndWidth = endWidth;
	beamInfo.m_flFadeLength = 0.0f;
	beamInfo.m_flAmplitude = 2.0f;
	beamInfo.m_flBrightness = brightness;
	beamInfo.m_flSpeed = 0.2f;
	beamInfo.m_nStartFrame = 0;
	beamInfo.m_flFrameRate = 0.f;
	beamInfo.m_flRed = 1.f / float(c.r());
	beamInfo.m_flGreen = 1.f / float(c.g());
	beamInfo.m_flBlue = 1.f / float(c.b());
	beamInfo.m_nSegments = 2;
	beamInfo.m_bRenderable = true;
	beamInfo.m_nFlags = 0;
	beamInfo.m_vecStart = pos1;
	beamInfo.m_vecEnd = pos2;
	beamInfo.m_nFlags = FBEAM_SOLID;

	return beamInfo;
}

BeamInfo_t beams::GenerateBeamInfo(Color c, Vector pos1, Vector pos2) {
	return GenerateBeamInfo(c, pos1, pos2, beam_sprites[0], 1.f, 2.f, 2.f, 255.f);
}

bool beams::CreateBeam(BeamInfo_t beamInfo) {
	if (!g_pBeams) {
		Utils::ConsolePrint(_("[beams] SDK::Beams() == nullptr\n"));
		return false;
	}

	if (!g_pEngineClient->IsInGame()) {
		Utils::ConsolePrint(_("[beams] SDK::Engine()->IsInGame() == false\n"));
		return false;
	}

	auto newBeam = g_pBeams->CreateBeamPoints(beamInfo);
	if (newBeam) {
		g_pBeams->DrawBeam(newBeam);
		return true;
	}
	else {
		Utils::ConsolePrint(_("[beams] newBeam == nullptr!\n"));
		return false;
	}
}