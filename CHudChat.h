// Generated using ReClass 2016

#pragma once

template<typename FuncType>
__forceinline static FuncType chudchatCallVFunction(void* ppClass, int index)
{
	int* pVTable = *(int**)ppClass;
	int dwAddress = pVTable[index];
	return (FuncType)(dwAddress);
}


class CHudChat
{
public:

	enum ChatFilters {
		CHAT_FILTER_NONE = 0,
		CHAT_FILTER_JOINLEAVE = 0x000001,
		CHAT_FILTER_NAMECHANGE = 0x000002,
		CHAT_FILTER_PUBLICCHAT = 0x000004,
		CHAT_FILTER_SERVERMSG = 0x000008,
		CHAT_FILTER_TEAMCHANGE = 0x000010,
		CHAT_FILTER_ACHIEVEMENT = 0x000020,
	};

	void ChatPrintf(int iPlayerIndex, int iFilter, const char* fmt, ...) {
		using fnChatPrintf = void(__cdecl*)(void* _this, int a2, int a3, int a4, int a5);

		chudchatCallVFunction<void(__cdecl*)(void*, int, int, const char*, ...)>(this, 26)(this, iPlayerIndex, iFilter, fmt);
	}

	__int32 m_bActive; //0x0004 
	__int32 m_iHiddenBits; //0x0008 
	char pad_0x000C[0x4]; //0x000C
	char* m_pElementName; //0x0010 
	unsigned char m_bNeedsRemove; //0x0014 
	unsigned char m_bIsParentedToClientDLLRootPanel; //0x0015 

}; //Size=0x0016

