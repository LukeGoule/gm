#include "NetvarManager.h"

#include "../ClientClass.h"
#include "../IBaseClientDLL.h"
#include "../globals.h"
#include "../helpers/obfs.h"

NetvarManager* g_pNetvarManager = nullptr;

NetvarManager::NetvarManager(void)
{
    m_tables.clear();


    ClientClass* clientClass = g_pClientDLL->GetAllClasses();

    if (!clientClass)
    {
        Utils::ConsolePrint(_("ClientClass was not found\n"));
        return;
    }


    while (clientClass)
    {
        RecvTable* recvTable = clientClass->pRecvTable;


        m_tables.push_back(recvTable);


        clientClass = clientClass->pNext;
    }

    this->m_bFinished = true;
}


// calls GetProp wrapper to get the absolute offset of the prop
int NetvarManager::GetOffset(const char* tableName, const char* propName)
{
    int offset = GetProp(tableName, propName);

    if (!offset)
    {
        Utils::ConsolePrint(_("Failed to find offset for prop: %s from table: %s\n"), propName, tableName);
        return 0;
    }


    return offset;
}


// calls GetProp wrapper to get prop and sets the proxy of the prop
bool NetvarManager::HookProp(const char* tableName, const char* propName, RecvVarProxyFn function)
{
    RecvProp* recvProp = 0;
    GetProp(tableName, propName, &recvProp);


    if (!recvProp)
    {
        Utils::ConsolePrint(_("Failed to hook prop: %s from table: %s\n"), propName, tableName);
        return false;
    }


    recvProp->ProxyFn = function;

    return true;
}


// wrapper so we can use recursion without too much performance loss
int NetvarManager::GetProp(const char* tableName, const char* propName, RecvProp** prop)
{
    RecvTable* recvTable = GetTable(tableName);

    if (!recvTable)
    {
        Utils::ConsolePrint(_("Failed to find table: %s\n"), tableName);
        return 0;
    }


    int offset = GetProp(recvTable, propName, prop);

    if (!offset)
    {
        Utils::ConsolePrint(_("Failed to find prop: %s from table: %s\n"), propName, tableName);
        return 0;
    }


    return offset;
}


// uses recursion to return a the relative offset to the given prop and sets the prop param
int NetvarManager::GetProp(RecvTable* recvTable, const char* propName, RecvProp** prop)
{
    int extraOffset = 0;

    for (int i = 0; i < recvTable->nProps; ++i)
    {
        RecvProp* recvProp = &recvTable->pProps[i];
        RecvTable* child = recvProp->pDataTable;

        if (child
            && (child->nProps > 0))
        {
            int tmp = GetProp(child, propName, prop);

            if (tmp)
            {
                extraOffset += (recvProp->Offset + tmp);
            }
        }


        if (_stricmp(recvProp->pVarName, propName))
        {
            continue;
        }


        if (prop)
        {
            *prop = recvProp;
        }

        return (recvProp->Offset + extraOffset);
    }

    return extraOffset;
}


RecvTable* NetvarManager::GetTable(const char* tableName)
{
    if (m_tables.empty())
    {
        Utils::ConsolePrint(_("Failed to find table: %s (m_tables is empty)\n"), tableName);
        return 0;
    }


    for (RecvTable* table : m_tables)
    {
        if (!table)
        {
            continue;
        }

        if (_stricmp(table->pNetTableName, tableName) == 0)
        {
            return table;
        }
    }

    return 0;
}