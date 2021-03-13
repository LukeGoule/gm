#pragma once

#include <vector>
#include <string>

#include "vector.h"
#include "Color.h"
#include "IViewRenderBeams.h"

// fully static class to control beams with ease.
class beams {
public:
	// List of beam sprites available to use
	static std::vector<std::string> beam_sprites;

	// Create a beam with a lot of parameters
	static BeamInfo_t GenerateBeamInfo(Color c, Vector pos1, Vector pos2, std::string sprite, float lifeSeconds, float startWidth, float endWidth, float brightness);

	// Create a beam with minimal parameters
	static BeamInfo_t GenerateBeamInfo(Color c, Vector pos1, Vector pos2);

	// Add the new beam to the internal draw list of cs
	static bool CreateBeam(BeamInfo_t beamInfo);
};