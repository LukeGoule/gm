#pragma once

#include "math.h"
#include "EngineNetworking.h"
#include "BetterInterface.h"
#include "vmatrix.h"
#include "vector.h"

struct player_info_t
{
	char name[32];
	int  id;
	char guid[33];
	struct
	{
		uint32_t id;
		char name[32];
	} buddy;
	bool bot;
	bool hltv;
	uint32_t files[4];
	uint8_t  downloaded;
};

class IVEngineClient
{
public:
	inline size_t LocalPlayerID()
	{
		using get_local_player_t = size_t(__thiscall*)(void*);
		return method<get_local_player_t>(12, this)(this);
	}

	inline float TimeStamp(void)
	{
		typedef float(__thiscall* OriginalFn)(PVOID);
		return GetVFunc<OriginalFn>(this, 14)(this);
	}

	inline Channel* GetNetChannel()
	{
		using get_net_channel_t = Channel * (__thiscall*)(void*);
		return method<get_net_channel_t>(72, this)(this);
	}

	inline bool GetPlayerInfo(size_t index, player_info_t* info)
	{
		using get_player_info_t = bool(__thiscall*)(void*, size_t, player_info_t*);
		return method<get_player_info_t>(8, this)(this, index, info);
	}

	inline int GetPlayerForUserID(int ID) {
		return method<int(__thiscall*)(void*, int)>(9, this)(this, ID);
	}

	
	inline const VMatrix& GetViewMatrix()
	{
		using get_view_matrix_t = const VMatrix&(__thiscall*)(void*);
		return method<get_view_matrix_t>(36, this)(this);
	}

	inline size_t GetMaxClients()
	{
		using get_max_clients_t = size_t(__thiscall*)(void*);
		return method<get_max_clients_t>(21, this)(this);
	}

	// always 0?????
	inline void GetScreenSize(int& width, int& height)
	{
		using get_screen_size_t = void(__thiscall*)(void*, int&, int&);
		return method<get_screen_size_t>(5, this)(this, width, height);
	}

	inline void SetViewAngles(QAngle& Angles)
	{
		using  set_view_angles_t = void(__thiscall*)(void*, QAngle& Angles);
		return method<set_view_angles_t>(20, this)(this, Angles);
	}

	inline bool IsInGame()
	{
		using is_ingame_t = bool(__thiscall*)(void*);
		return method<is_ingame_t>(26, this)(this);
	}

	std::string GetCurrentMap(); // IVEngineClient.cpp
};

/*
typedef player_info_t player_info_s;

class SurfInfo;
class CAudioSource;
class con_nprint_s;
class IMaterialSystem;
class AudioState_t;
class CPhysCollide;
class IMaterial;
class color32_s;
class vmode_s;
class OcclusionParams_t;
class IAchievementMgr;
class CGamestatsData;
class KeyValues;
class IGMODDataTable;
class CGMODVariant;
class ButtonCode_t;
class model_t;

class IVEngineClient_GMOD {
public:
	// Auto reconstructed from vtable block @ 0x004C2288
	// from "engine.dylib", by ida_vtables.idc
	virtual void GetIntersectingSurfaces(model_t const*,Vector const&,float,bool,SurfInfo*,int) = 0;
	virtual void GetLightForPoint(Vector const&,bool) = 0;
	virtual void TraceLineMaterialAndLighting(Vector const&,Vector const&,Vector&,Vector&) = 0;
	virtual void ParseFile(char const*,char*,int) = 0;
	virtual void CopyLocalFile(char const*,char const*) = 0;
	virtual void GetScreenSize(int&,int&) = 0;
	virtual void ServerCmd(char const*,bool) = 0;
	virtual void ClientCmd(char const*) = 0;
	virtual void GetPlayerInfo(int,player_info_s*) = 0;
	virtual void GetPlayerForUserID(int) = 0;
	virtual void TextMessageGet(char const*) = 0;
	virtual void Con_IsVisible(void) = 0;
	virtual void GetLocalPlayer(void) = 0;
	virtual void LoadModel(char const*,bool) = 0;
	virtual void Time(void) = 0;
	virtual void GetLastTimeStamp(void) = 0;
	virtual void GetSentence(CAudioSource*) = 0;
	virtual void GetSentenceLength(CAudioSource*) = 0;
	virtual void IsStreaming(CAudioSource*)const = 0;
	virtual void GetViewAngles(QAngle&) = 0;
	virtual void SetViewAngles(QAngle&) = 0;
	virtual void GetMaxClients(void) = 0;
	virtual void Key_LookupBinding(char const*) = 0;
	virtual void Key_BindingForKey(ButtonCode_t) = 0;
	virtual void StartKeyTrapMode(void) = 0;
	virtual void CheckDoneKeyTrapping(ButtonCode_t&) = 0;
	virtual void IsInGame(void) = 0;
	virtual void IsConnected(void) = 0;
	virtual void IsDrawingLoadingImage(void) = 0;
	virtual void Con_NPrintf(int,char const*,...) = 0;
	virtual void Con_NXPrintf(con_nprint_s const*,char const*,...) = 0;
	virtual void IsBoxVisible(Vector const&,Vector const&) = 0;
	virtual void IsBoxInViewCluster(Vector const&,Vector const&) = 0;
	virtual void CullBox(Vector const&,Vector const&) = 0;
	virtual void Sound_ExtraUpdate(void) = 0;
	virtual void GetGameDirectory(void) = 0;
	virtual void WorldToScreenMatrix(void) = 0;
	virtual void WorldToViewMatrix(void) = 0;
	virtual void GameLumpVersion(int)const = 0;
	virtual void GameLumpSize(int)const = 0;
	virtual void LoadGameLump(int,void*,int) = 0;
	virtual void LevelLeafCount(void)const = 0;
	virtual void GetBSPTreeQuery(void) = 0;
	virtual void LinearToGamma(float*,float*) = 0;
	virtual void LightStyleValue(int) = 0;
	virtual void ComputeDynamicLighting(Vector const&,Vector const*,Vector&) = 0;
	virtual void GetAmbientLightColor(Vector&) = 0;
	virtual void GetDXSupportLevel(void) = 0;
	virtual void SupportsHDR(void) = 0;
	virtual void Mat_Stub(IMaterialSystem*) = 0;
	virtual void GetChapterName(char*,int) = 0;
	virtual void GetLevelName(void) = 0;
	virtual void GetLevelVersion(void) = 0;
	virtual void GetVoiceTweakAPI(void) = 0;
	virtual void EngineStats_BeginFrame(void) = 0;
	virtual void EngineStats_EndFrame(void) = 0;
	virtual void FireEvents(void) = 0;
	virtual void GetLeavesArea(int*,int) = 0;
	virtual void DoesBoxTouchAreaFrustum(Vector const&,Vector const&,int) = 0;
	virtual void SetAudioState(AudioState_t const&) = 0;
	virtual void SentenceGroupPick(int,char*,int) = 0;
	virtual void SentenceGroupPickSequential(int,char*,int,int,int) = 0;
	virtual void SentenceIndexFromName(char const*) = 0;
	virtual void SentenceNameFromIndex(int) = 0;
	virtual void SentenceGroupIndexFromName(char const*) = 0;
	virtual void SentenceGroupNameFromIndex(int) = 0;
	virtual void SentenceLength(int) = 0;
	virtual void ComputeLighting(Vector const&,Vector const*,bool,Vector&,Vector*) = 0;
	virtual void ActivateOccluder(int,bool) = 0;
	virtual void IsOccluded(Vector const&,Vector const&) = 0;
	virtual void SaveAllocMemory(unsigned long,unsigned long) = 0;
	virtual void SaveFreeMemory(void*) = 0;
	virtual void GetNetChannelInfo(void) = 0;
	virtual void DebugDrawPhysCollide(CPhysCollide const*,IMaterial*,matrix3x4_t&,color32_s const&) = 0;
	virtual void CheckPoint(char const*) = 0;
	virtual void DrawPortals(void) = 0;
	virtual void IsPlayingDemo(void) = 0;
	virtual void IsRecordingDemo(void) = 0;
	virtual void IsPlayingTimeDemo(void) = 0;
	virtual void GetDemoRecordingTick(void) = 0;
	virtual void GetDemoPlaybackTick(void) = 0;
	virtual void GetDemoPlaybackStartTick(void) = 0;
	virtual void GetDemoPlaybackTimeScale(void) = 0;
	virtual void GetDemoPlaybackTotalTicks(void) = 0;
	virtual void IsPaused(void) = 0;
	virtual void IsTakingScreenshot(void) = 0;
	virtual void IsHLTV(void) = 0;
	virtual void IsLevelMainMenuBackground(void) = 0;
	virtual void GetMainMenuBackgroundName(char*,int) = 0;
	virtual void GetVideoModes(int&,vmode_s*&) = 0;
	virtual void SetOcclusionParameters(OcclusionParams_t const&) = 0;
	virtual void GetUILanguage(char*,int) = 0;
	virtual void IsSkyboxVisibleFromPoint(Vector const&) = 0;
	virtual void GetMapEntitiesString(void) = 0;
	virtual void IsInEditMode(void) = 0;
	virtual void GetScreenAspectRatio(void) = 0;
	virtual void REMOVED_SteamRefreshLogin(char const*,bool) = 0;
	virtual void REMOVED_SteamProcessCall(bool&) = 0;
	virtual void GetEngineBuildNumber(void) = 0;
	virtual void GetProductVersionString(void) = 0;
	virtual void GrabPreColorCorrectedFrame(int,int,int,int) = 0;
	virtual void IsHammerRunning(void)const = 0;
	virtual void ExecuteClientCmd(char const*) = 0;
	virtual void MapHasHDRLighting(void) = 0;
	virtual void GetAppID(void) = 0;
	virtual void GetLightForPointFast(Vector const&,bool) = 0;
	virtual void ClientCmd_Unrestricted(char const*) = 0;
	virtual void SetRestrictServerCommands(bool) = 0;
	virtual void SetRestrictClientCommands(bool) = 0;
	virtual void SetOverlayBindProxy(int,void*) = 0;
	virtual void CopyFrameBufferToMaterial(char const*) = 0;
	virtual void ChangeTeam(char const*) = 0;
	virtual void ReadConfiguration(bool) = 0;
	virtual void SetAchievementMgr(IAchievementMgr*) = 0;
	virtual void GetAchievementMgr(void) = 0;
	virtual void MapLoadFailed(void) = 0;
	virtual void SetMapLoadFailed(bool) = 0;
	virtual void IsLowViolence(void) = 0;
	virtual void GetMostRecentSaveGame(void) = 0;
	virtual void SetMostRecentSaveGame(char const*) = 0;
	virtual void StartXboxExitingProcess(void) = 0;
	virtual void IsSaveInProgress(void) = 0;
	virtual void OnStorageDeviceAttached(void) = 0;
	virtual void OnStorageDeviceDetached(void) = 0;
	virtual void ResetDemoInterpolation(void) = 0;
	virtual void SetGamestatsData(CGamestatsData*) = 0;
	virtual void GetGamestatsData(void) = 0;
	virtual void GetMouseDelta(int&,int&,bool) = 0;
	virtual void ServerCmdKeyValues(KeyValues*) = 0;
	virtual void IsSkippingPlayback(void) = 0;
	virtual void IsLoadingDemo(void) = 0;
	virtual void IsPlayingDemoALocallyRecordedDemo(void) = 0;
	virtual void Key_LookupBindingExact(char const*) = 0;
	virtual void GMOD_SetTimeManipulator(float) = 0;
	virtual void GMOD_SendToServer(void*,unsigned int,bool) = 0;
	virtual void GMOD_PlaceDecalMaterial(IMaterial*,bool,int,IClientEntity*,Vector const&,Vector const&,color32_s const&,float,float) = 0;
	virtual void GMOD_GetSpew(char*,unsigned long) = 0;
	virtual void GMOD_SetViewEntity(unsigned int) = 0;
	virtual void GMOD_BrushMaterialOverride(IMaterial*) = 0;
	virtual void GMOD_R_RedownloadAllLightmaps(bool) = 0;
	virtual void GMOD_RawClientCmd_Unrestricted(char const*) = 0;
	virtual void GMOD_CreateDataTable(void (*)(void*,int,CGMODVariant const&)) = 0;
	virtual void GMOD_DestroyDataTable(IGMODDataTable*) = 0;
	virtual void GMOD_LoadModel(char const*) = 0;
	virtual void GMOD_DecalRemoveEntity(int) = 0;
	virtual void GMOD_TranslateAlias(char const*) = 0;
	virtual void GetProtocolVersion(void) = 0;
	virtual void IsWindowedMode(void) = 0;
	virtual void FlashWindow(void) = 0;
	virtual void GetClientVersion(void)const = 0;
	virtual void IsActiveApp(void) = 0;
	virtual void DisconnectInternal(void) = 0;
	virtual void IsInCommentaryMode(void) = 0;
};*/