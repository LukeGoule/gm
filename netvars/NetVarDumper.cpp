#include "NetVarDumper.h"

#include "../sdk.h"
#include "../ClientClass.h"
#include "../IBaseClientDLL.h"
#include "../globals.h"

#include <string>
#include <fstream>

#undef GetProp

std::stringstream fileBuffer;
std::unique_ptr<NetvarDumper> g_pNetvars;

/**
* NetvarTree - Constructor
NetvarTree
* Call populate_nodes on every RecvTable under client->GetAllClasses()
*/
NetvarDumper::NetvarDumper()
{
	const ClientClass* clientClass = g_pClientDLL->GetAllClasses();

	while (clientClass != nullptr)
	{
		fileBuffer << clientClass->pNetworkName << " {" << std::endl;

		const auto classInfo = std::make_shared<Node>(0);
		RecvTable* recvTable = clientClass->pRecvTable;

		this->PopulateNodes(recvTable, &classInfo->nodes);
		nodes.emplace(recvTable->GetName(), classInfo);

		clientClass = clientClass->pNext;

		fileBuffer << "}" << std::endl;
	}

	std::string fileLocation = this->getOutputFolder() + "netvars.hl2.dump.txt";
	std::ofstream outputFile(fileLocation);
	if (!outputFile.is_open()) {
		Utils::ConsolePrint("Failed to open %s\n", fileLocation.c_str());
		return;
	}
	else {
		Utils::ConsolePrint("Writing %s...\n", fileLocation.c_str());
	}

	outputFile << fileBuffer.str();
	outputFile.close();

	Utils::ConsolePrint("NetVar dumping complete!\n");
}

/**
* PopulateNodes - Populate a node map with brances
* @recvTable:	Table the map corresponds to
* @mapType:	Map pointer
*
* Add info for every prop in the recv table to the node map. If a prop is a
* datatable itself, initiate a recursive call to create more branches.
*/
size_t depth = 0;
void NetvarDumper::PopulateNodes(RecvTable* recvTable, MapType* mapType)
{
	for (size_t i = 0; i < depth; i++) { fileBuffer << "\t"; }
	fileBuffer << "\t\t" << recvTable->GetName() << " {" << std::endl;

	depth++;

	for (auto i = 0; i < recvTable->GetNumProps(); i++)
	{
		const RecvProp* prop = recvTable->GetProp(i);
		const auto propInfo = std::make_shared<Node>(prop->GetOffset());

		for (size_t i = 0; i < depth; i++) { fileBuffer << "\t"; }
		fileBuffer << "\t\t" << prop->GetName() << " @ 0x" << std::hex << prop->GetOffset() << std::endl;

		if (prop->GetType() == SendPropType::DPT_DataTable)
			this->PopulateNodes(prop->GetDataTable(), &propInfo->nodes);

		mapType->emplace(prop->GetName(), propInfo);
	}

	for (size_t i = 0; i < depth; i++) { fileBuffer << "\t"; }
	fileBuffer << "}" << std::endl;

	depth--;
}

std::string NetvarDumper::getOutputFolder()
{
	char* documentsPath = getenv("USERPROFILE");

	if (documentsPath) {
		std::string documentsPathStr(documentsPath);
		documentsPathStr += "\\Documents\\";
		return documentsPathStr;
	}

	return std::string(".\\"); // Default to current dir (hl2.exe containing.)
}