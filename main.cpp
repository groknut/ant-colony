
#include <iostream> 
#include <fstream>
#include <string>

#include "./head/ic.h"
#include "./head/graph.h"
#include "./head/ant.h"

using namespace std;

void helper(const string& program);

int main(int argc, char** argv)
{

	if (argc != 2)
	{
		helper(argv[0]);
		return 0;
	}
		
	Ic ic(argv[1]);
	Graph graph(ic);

	AntColony aco(ic, graph);
	aco.run();
	
    return 0;	
}

void helper(const string& program)
{
	cout << "Example:" << std::endl;
	cout << program << "<file>: standard file reading (.ic format)" << std::endl;
}
