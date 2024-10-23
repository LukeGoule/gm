#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Recv.h"
#include "utils.h"
#include "obfs.h"

class NetvarDumper
{
	struct Node;
	using MapType = std::unordered_map<std::string, std::shared_ptr<Node>>;

	struct Node
	{
		Node(int offset) : offset(offset) {}
		MapType nodes;
		int offset;
	};

	MapType nodes;

public:
	NetvarDumper();

private:
	void PopulateNodes(class RecvTable *recv_table, MapType *map);

	std::string getOutputFolder();
};
extern std::unique_ptr<NetvarDumper> g_pNetvars;