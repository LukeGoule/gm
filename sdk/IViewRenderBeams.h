#pragma once

#include "../helpers/vector.h"

class C_BaseEntity;
class ITraceFilter;

enum
{
	TE_BEAMPOINTS = 0x00,		// beam effect between two points
	TE_SPRITE = 0x01,	// additive sprite, plays 1 cycle
	TE_BEAMDISK = 0x02,	// disk that expands to max radius over lifetime
	TE_BEAMCYLINDER = 0x03,		// cylinder that expands to max radius over lifetime
	TE_BEAMFOLLOW = 0x04,		// create a line of decaying beam segments until entity stops moving
	TE_BEAMRING = 0x05,		// connect a beam ring to two entities
	TE_BEAMSPLINE = 0x06,
	TE_BEAMRINGPOINT = 0x07,
	TE_BEAMLASER = 0x08,		// Fades according to viewpoint
	TE_BEAMTESLA = 0x09,
};
enum
{
	FBEAM_STARTENTITY = 0x00000001,
	FBEAM_ENDENTITY = 0x00000002,
	FBEAM_FADEIN = 0x00000004,
	FBEAM_FADEOUT = 0x00000008,
	FBEAM_SINENOISE = 0x00000010,
	FBEAM_SOLID = 0x00000020,
	FBEAM_SHADEIN = 0x00000040,
	FBEAM_SHADEOUT = 0x00000080,
	FBEAM_ONLYNOISEONCE = 0x00000100,		// Only calculate our noise once
	FBEAM_NOTILE = 0x00000200,
	FBEAM_USE_HITBOXES = 0x00000400,		// Attachment indices represent hitbox indices instead when this is set.
	FBEAM_STARTVISIBLE = 0x00000800,		// Has this client actually seen this beam's start entity yet?
	FBEAM_ENDVISIBLE = 0x00001000,		// Has this client actually seen this beam's end entity yet?
	FBEAM_ISACTIVE = 0x00002000,
	FBEAM_FOREVER = 0x00004000,
	FBEAM_HALOBEAM = 0x00008000,		// When drawing a beam with a halo, don't ignore the segments and endwidth
	FBEAM_REVERSED = 0x00010000,
	NUM_BEAM_FLAGS = 17	// KEEP THIS UPDATED!
};

extern void SetBeamCreationAllowed(bool state);
extern bool BeamCreationAllowed(void);

//-----------------------------------------------------------------------------
// beam flags
//-----------------------------------------------------------------------------


class C_Beam;
class Beam_t;

//-----------------------------------------------------------------------------
// Purpose: Popcorn trail for Beam Follow rendering...
//-----------------------------------------------------------------------------

struct BeamTrail_t
{
	// NOTE:  Don't add user defined fields except after these four fields.
	BeamTrail_t* next;
	float			die;
	Vector			org;
	Vector			vel;
};

struct RenderableInstance_t
{
	unsigned char m_nAlpha;
};

//-----------------------------------------------------------------------------
// Data type for beams.
//-----------------------------------------------------------------------------
struct BeamInfo_t
{
	int	m_nType;
	C_BaseEntity* m_pStartEnt = nullptr;
	int	m_nStartAttachment = 0;
	C_BaseEntity* m_pEndEnt = nullptr;
	int m_nEndAttachment = 0;
	Vector m_vecStart;
	Vector m_vecEnd;
	int m_nModelIndex = 0;
	const char* m_pszModelName = nullptr;
	int m_nHaloIndex = 0;
	const char* m_pszHaloName = nullptr;
	float m_flHaloScale = 0.f;
	float m_flLife = 0.f;
	float m_flWidth = 0.f;
	float m_flEndWidth = 0.f;
	float m_flFadeLength = 0.f;
	float m_flAmplitude = 0.f;
	float m_flBrightness = 0.f;
	float m_flSpeed = 0.f;
	int m_nStartFrame = 0;
	float m_flFrameRate = 0.f;
	float m_flRed = 0.f;
	float m_flGreen = 0.f;
	float m_flBlue = 0.f;
	bool m_bRenderable = true;
	int	m_nSegments = 0;
	int	m_nFlags = 0;

	// Rings
	Vector m_vecCenter;
	float m_flStartRadius = 0.f;
	float m_flEndRadius = 0.f;

	BeamInfo_t()
	{
		m_nType = TE_BEAMPOINTS;
		m_nSegments = -1;
		m_pszModelName = NULL;
		m_pszHaloName = NULL;
		m_nModelIndex = -1;
		m_nHaloIndex = -1;
		m_bRenderable = true;
		m_nFlags = 0;
	}
};

//-----------------------------------------------------------------------------
// Purpose: Declare client .dll beam entity interface
//-----------------------------------------------------------------------------

class IViewRenderBeams
{
public:
	// Construction
public:
	IViewRenderBeams(void);
	virtual ~IViewRenderBeams(void) = 0;

	// Implement IViewRenderBeams
public:
	virtual	void		InitBeams(void) = 0;
	virtual	void		ShutdownBeams(void) = 0;
	virtual	void		ClearBeams(void) = 0;

	// Updates the state of the temp ent beams
	virtual void		UpdateTempEntBeams() = 0;
	virtual void		DrawBeam(Beam_t* pbeam) = 0;
	virtual void		DrawBeam(C_Beam* pbeam, ITraceFilter* pEntityBeamTraceFilter = NULL) = 0;

	virtual	void		KillDeadBeams(C_BaseEntity* pDeadEntity) = 0;

	virtual	void		CreateBeamEnts(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int type = -1) = 0;
	virtual Beam_t* CreateBeamEnts(BeamInfo_t& beamInfo) = 0;

	virtual	void		CreateBeamEntPoint(int	nStartEntity, const Vector* pStart, int nEndEntity, const Vector* pEnd,
		int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b) = 0;
	virtual Beam_t* CreateBeamEntPoint(BeamInfo_t& beamInfo) = 0;

	virtual	void		CreateBeamPoints(Vector& start, Vector& end, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b) = 0;
	virtual	Beam_t* CreateBeamPoints(BeamInfo_t& beamInfo) = 0;

	virtual	void		CreateBeamRing(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags) = 0;
	virtual Beam_t* CreateBeamRing(BeamInfo_t& beamInfo) = 0;

	virtual void		CreateBeamRingPoint(const Vector& center, float start_radius, float end_radius, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags) = 0;
	virtual Beam_t* CreateBeamRingPoint(BeamInfo_t& beamInfo) = 0;

	virtual	void		CreateBeamCirclePoints(int type, Vector& start, Vector& end,
		int modelIndex, int haloIndex, float haloScale, float life, float width,
		float endWidth, float fadeLength, float amplitude, float brightness, float speed,
		int startFrame, float framerate, float r, float g, float b) = 0;
	virtual Beam_t* CreateBeamCirclePoints(BeamInfo_t& beamInfo) = 0;

	virtual	void		CreateBeamFollow(int startEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float r, float g, float b,
		float brightness) = 0;
	virtual Beam_t* CreateBeamFollow(BeamInfo_t& beamInfo) = 0;
};

extern IViewRenderBeams* beams;
