#include "lua.h"

#include "../../helpers/utils.h"

void LuaLog::onRunString(lua::ILuaInterface* pLuaInterface, const char* szFileName, const char* szFilePath, const char* szLuaString, bool bRun, bool bShowErrors)
{
    if (this->m_Stack.size() >= this->m_iMaxSize) 
    {
        this->m_Stack.pop_front();
    }

    Utils::ConsolePrint("RunString: %s/%s (%d bytes)\n", szFilePath, szFileName, strlen(szLuaString));

    this->m_Stack.push_back(new LuaLog::LogItem{
        pLuaInterface,
        std::string(szFileName),
        std::string(szFilePath),
        std::string(szLuaString),
        bRun,
        bShowErrors
    });
}

void LuaLog::onRunStringEx(lua::ILuaInterface* pLuaInterface, const char* szFileName, const char* szFilePath, const char* szLuaString, bool bRun, bool bShowErrors, bool bDontPushErrors, bool bNoReturns)
{
    if (this->m_Stack.size() >= this->m_iMaxSize)
    {
        this->m_Stack.pop_front();
    }

    Utils::ConsolePrint("RunStringEx: %s/%s (%d bytes)\n", szFilePath, szFileName, strlen(szLuaString));

    this->m_Stack.push_back(new LuaLog::LogItem{
        pLuaInterface,
        std::string(szFileName),
        std::string(szFilePath),
        std::string(szLuaString),
        bRun,
        bShowErrors
    });
}

void LuaLog::onCompileString(lua::ILuaInterface* pLuaInterface, void* dumper, const std::string* stringToCompile)
{
    if (this->m_Stack.size() >= this->m_iMaxSize)
    {
        this->m_Stack.pop_front();
    }

    Utils::ConsolePrint("CompileString: %s (%d bytes)\n", stringToCompile->c_str(), stringToCompile->size());

    this->m_Stack.push_back(new LuaLog::LogItem{
        pLuaInterface,
        std::string("CompileString"),
        std::string(""),
        std::string(stringToCompile->c_str()),
        false,
        false
    });
}

std::vector<const char*> LuaLog::listFileNames()
{
    std::vector<const char*> vec;

    for (const auto luaRun : this->m_Stack)
    {
        vec.push_back((luaRun->m_szFilePath + "/" + luaRun->m_szFileName).c_str());
    }

    return vec;
}

LuaLog::LogItem* LuaLog::getItem(size_t iIndex)
{
    return this->m_Stack[iIndex];
}