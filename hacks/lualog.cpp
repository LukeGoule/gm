#include "lualog.h"

#include "../../helpers/utils.h"

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm> // for std::replace
#include <filesystem> // for std::filesystem::create_directories

void replaceSlashes(std::string& path) 
{
    // Replace all forward slashes with backslashes
    std::replace(path.begin(), path.end(), '/', '\\');
}

void createDirectories(const std::string& path) 
{
    // Create directories if they don't exist
    std::filesystem::create_directories(path);
}

std::string removeFileName(const std::string& path) {
    // Remove the file name from the path
    return std::filesystem::path(path).parent_path().string();
}


void LuaLog::saveFile(std::string fullPath, std::string fileData)
{
    std::string fileLocation = this->getOutputFolder() + fullPath;
    replaceSlashes(fileLocation);
    std::string filePath = removeFileName(fileLocation);

    Utils::ConsolePrint("Dumping: %s\n", fileLocation.c_str());
    
    createDirectories(filePath);

    std::ofstream outputFile(fileLocation);
    
    if (!outputFile.is_open()) 
    {
        Utils::ConsolePrint("Failed to open %s\n", fileLocation.c_str());
        return;
    }
    else 
    {
        Utils::ConsolePrint("Writing %s...\n", fileLocation.c_str());
    }

    outputFile << fileData;
    outputFile.close();
}

void LuaLog::onRunString(lua::ILuaInterface* pLuaInterface, const char* szFileName, const char* szFilePath, const char* szLuaString, bool bRun, bool bShowErrors)
{
    /*if (this->m_Stack.size() >= this->m_iMaxSize) 
    {
        this->m_Stack.pop_front();
    }*/

    Utils::ConsolePrint("RunString: %s/%s (%d bytes)\n", szFilePath, szFileName, strlen(szLuaString));

    const std::string label = std::string(szFilePath) + "/" + std::string(szFileName);

    const auto pScript = new RunstringLuaScript{
        pLuaInterface,
        std::string(szFileName),
        std::string(szFilePath),
        std::string(szLuaString),
        label,
        bRun,
        bShowErrors
    };

    this->m_scripts.push_back(pScript);

    if (label.find("RunString") == std::string::npos)
    {
        this->saveFile(std::string(szFileName), std::string(szLuaString));
    }
}

void LuaLog::onRunStringEx(lua::ILuaInterface* pLuaInterface, const char* szFileName, const char* szFilePath, const char* szLuaString, bool bRun, bool bShowErrors, bool bDontPushErrors, bool bNoReturns)
{
    /*if (this->m_Stack.size() >= this->m_iMaxSize)
    {
        this->m_Stack.pop_front();
    }*/

    Utils::ConsolePrint("RunStringEx: %s/%s (%d bytes)\n", szFilePath, szFileName, strlen(szLuaString));

    const std::string label = std::string(szFilePath) + "/" + std::string(szFileName);

    const auto pScript = new RunstringLuaScript{
        pLuaInterface,
        std::string(szFileName),
        std::string(szFilePath),
        std::string(szLuaString),
        label,
        bRun,
        bShowErrors
    };

    this->m_scripts.push_back(pScript);

    if (label.find("RunString(Ex)") == std::string::npos)
    {
        this->saveFile(std::string(szFileName), std::string(szLuaString));
    }
}

std::vector<const char*> LuaLog::listFileNames()
{
    std::vector<const char*> vec;

    for (const auto luaRun : this->m_scripts)
    {
        vec.push_back(luaRun->m_label.c_str());
    }

    return vec;
}

RunstringLuaScript* LuaLog::getItem(size_t iIndex)
{
    return this->m_scripts[iIndex];
}

std::string LuaLog::getOutputFolder()
{
    char* documentsPath = getenv("USERPROFILE");

    if (documentsPath) {
        std::string documentsPathStr(documentsPath);
        documentsPathStr += "\\Documents\\gmod\\";
        return documentsPathStr;
    }

    return std::string(".\\"); // Default to current dir (hl2.exe containing.)
}