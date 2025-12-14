
#include <iostream> 
#include <fstream>
#include <string>

#include "cfig.h"
#include "./head/graph.h"
#include "./head/ant.h"

using namespace std;

void helper(const string& program);

int main()
{
	Cfig config("config.ini", Cfig::EQUAL, Cfig::HASH);
	Graph graph(config);
	AntColony aco(config, graph);
	aco.run();
    return 0;	
}
