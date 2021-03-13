#pragma once

// Generated using ReClass 2016

class CGlobalVarsBase;
class CSaveRestoreData;

class CGlobalVarsBase
{
public:
	CGlobalVarsBase(bool bIsClient);

	// This can be used to filter debug output or to catch the client or server in the act.
	bool IsClient() const;

	// for encoding m_flSimulationTime, m_flAnimTime
	int GetNetworkBase(int nTick, int nEntity);

	float					m_fRealtime; //0x0000 
	__int32					m_iFramecount; //0x0004 
	float					m_fAbsoluteFrametime; //0x0008 
	float					m_fCurtime; //0x000C 
	float					m_fFrametime; //0x0010 
	__int32					m_iMaxClients; //0x0014 
	__int32					m_iTickCount; //0x0018 
	float					m_fIntervalPerTick; //0x001C 
	float					m_fInterpolationAmount; //0x0020 
	__int32					m_iSimTicksThisFrame; //0x0024 
	__int32					m_iNetworkProtocol; //0x0028 
	CSaveRestoreData*		pSaveData; //0x002C
	bool					m_bClient;
	int						nTimestampNetworkingBase;
	int						nTimestampRandomizeWindow;

}; //Size=0x0040

class CSaveRestoreData
{
public:
	char pad_0x0000[0x4]; //0x0000

}; //Size=0x0004

inline int CGlobalVarsBase::GetNetworkBase(int nTick, int nEntity)
{
	int nEntityMod = nEntity % nTimestampRandomizeWindow;
	int nBaseTick = nTimestampNetworkingBase * (int)((nTick - nEntityMod) / nTimestampNetworkingBase);
	return nBaseTick;
}

inline CGlobalVarsBase::CGlobalVarsBase(bool bIsClient) :
	m_bClient(bIsClient),
	nTimestampNetworkingBase(100),
	nTimestampRandomizeWindow(32)
{
}

inline bool CGlobalVarsBase::IsClient() const
{
	return m_bClient;
}