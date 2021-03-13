#pragma once

#include "Recv.h"
#include "IClientNetworkable.h"

typedef IClientNetworkable*   (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable*   (*CreateEventFn)();

enum EClassIds {
	lol = 0
};

class ClientClass
{
public:
	CreateClientClassFn      pCreateFn;
	CreateEventFn            pCreateEventFn;
	char*                    pNetworkName;
	RecvTable*               pRecvTable;
	ClientClass*             pNext;
	EClassIds                ClassID;
};