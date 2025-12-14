
#include "../head/ant.h"

Ant::Ant(double a, double b, double r) : alpha(a), beta(b), rho(r) {}

ACO::ACO(const Cfig& cfig, Graph& g) : config(cfig), graph(g), rng(random_device{}())
{
	initPher();
}

void AntColony::initAnts(std::vector<double> params)
{
	for (int i = 0; i < nants; i++)
		ants.emplace_back(
			params[0], params[1], params[2]
		);
}

void ACO::initPher()
{
	double init_pher = config.get<double>("aco", "init", 1.0);	
	for (auto& node : graph.get_nodes())
		for (auto& other : graph.get_nodes())
			if (node != other && node->isNeigh(other))
				pher[make_pair(node, other)] = init_pher;	
}

double ACO::getPher(Node* a, Node* b) const
{
	auto it = pher.find(make_pair(a, b));
	if (it != pher.end())
		return it->second;
	return config("aco", "init").toDouble();
}

void ACO::updatePher(Node* a, Node* b, double val)
{
	pher[make_pair(a, b)] = val;
}

void ACO::updatePhers(vector<Node*>& path, const double& len)
{
	for (size_t i = 0; i + 1 < path.size(); i++)
	{
		Node* a = path[i];
		Node* b = path[i + 1];
		updatePher(a, b, getPher(a, b) + (config("aco", "Q").toDouble() / len));
	}
}

double ACO::getPathPhers(const vector<Node*>& path)
{
	double res = 0;
	for (size_t i = 0; i + 1 < path.size(); i++)
	{
		Node* a = path[i];
		Node* b = path[i + 1];
		res += getPher(a, b);
	}
	return res;
}

double ACO::getPhers()
{
	double res=0;
	for (auto& [edge, pheromone] : pher)
		res += pheromone;
	return res;
}

int ACO::computePathLength(const vector<Node*>& path) const {
   	int length = 0;
   	for (size_t i = 0; i + 1 < path.size(); i++) {
       	Node* a = path[i];
       	Node* b = path[i + 1];
       	length += a->getWeight(b);
   	}
   	return length;
}

double ACO::calcProb(Node* curr, Node* neigh, const Ant& ant)
{
	return pow(
		getPher(curr, neigh), ant.alpha
	) * pow(
		1.0 / curr->getWeight(neigh), ant.beta
	);
}

Node* ACO::chooseNextNode(Node* current, const unordered_map<Node*, bool>& visited, const Ant& ant)
{

	double total = 0.0;
	Node* select = nullptr;

	for (auto it = current->nb_begin(); it != current->nb_end(); it++)
		if (!visited.at(it->first))
		{
			total += calcProb(current, it->first, ant);
			select = it->first;
		}

	double pick = uniform_real_distribution<double>(0.0, total)(rng);

	double cum = 0.0;

	for (auto it = current->nb_begin(); it != current->nb_end(); it++)
	{
		if (visited.at(it->first))
			continue;
			
		cum += calcProb(current, it->first, ant);

		if (cum >= pick)
			return it->first;
	}

	return select;	
}

vector<Node*> ACO::buildAntPath(vector<Node*>& nodes, Ant& ant)
{
	unordered_map<Node*, bool> visited;
	std::vector<Node*> path;
	
	for (auto n : nodes)
		visited[n] = false;

	Node* start = nodes[rng() % nodes.size()];
	path.push_back(start);
	visited[start] = true;

	while (path.size() < nodes.size()) 
	{
		Node* next = chooseNextNode(path.back(), visited, ant);

		if (!next)
			break;
			
		path.push_back(next);
		visited[next] = true;
	}

	if (path.back()->isNeigh(start))                      
		path.push_back(start);

	return path;
}

void ACO::runAnt(Ant& ant, vector<Node*>& nodes, int& bestLen, vector<Node*>& bestPath, int& iter, const int& antId, std::ofstream& outfile)
{

	std::vector<Node*> path = buildAntPath(nodes, ant);

	double len = computePathLength(path);
	double currBestLen = bestLen;
	bool pathType = path.size() == nodes.size() + 1;

	if (len < bestLen && pathType)
	{
		bestLen = len;
		bestPath = path;
	}
	
	outfile << iter << "," << currBestLen << "," << antId << "," << len << ",";

	for (size_t i = 0; i < path.size(); i++)
	{
		outfile << path[i]->getName();
		if (i + 1 < path.size())
			outfile << "-";
	}
	
	outfile << "," << pathType;
	outfile << "," << getPhers() << "," << getPathPhers(bestPath);
	
	outfile << std::endl;

	updatePhers(path, len);
}

void ACO::evaporate()
{
	for (auto& [edge, pheromone] : pher)
		pheromone *= (1 - config("ant", "rho").toDouble());
}

void ACO::run() 
{
	set<Node*> nodeSet = graph.get_nodes();
	vector<Node*> nodes(nodeSet.begin(), nodeSet.end());
	int paraml = 1e6;
	int bestLen = paraml;
	vector<Node*> bestPath;

	std::ofstream output_file(config("output", "output_file"));

	if (!output_file.is_open())
		throw FileNotFoundError();

	double eps = config.get<double>("aco", "eps", -1.0);

	int n_iters = config.get<int>("aco", "n_iters", 0);

	int max_iters = config.get<int>("aco", "max_iters", 1000);

	output_file << "Iteration,CurrentBestLength,AntId,AntPathLength,AntPath,PathType,Phers,PhersOptimal" << std::endl;

	int antId = 1;

	int iters = config("aco", "iters").toInt();
    int p = config.get<int>("aco", "packs", 1);

    std::vector<double> params = {
		config("ant", "alpha").toDouble(),
		config("ant", "beta").toDouble(),
		config("ant", "rho").toDouble()    	
    };

    int nants = config("colony", "nants").toInt();

	if (!n_iters && eps < 0)
	{
		for (int i = 0; i < iters; i++)
		{
			for (int j = 0; j < p; j++)
			{
				AntColony colony(nants);
				colony.initAnts(params);
				for (Ant& ant : colony.ants)
					runAnt(ant, nodes, bestLen, bestPath, i, antId++, output_file);	
			}
			evaporate();
		}	
	}
	
	else
	{
		vector<Node*> lastBestPath;
		double lastBestPathLen;
			
		for (int i = 0; i < iters; i++)
		{
			for (int j = 0; j < p; j++)
			{
				AntColony colony(nants);
				colony.initAnts(params);
				for (Ant& ant : colony.ants)
					runAnt(ant, nodes, bestLen, bestPath, i, antId++, output_file);	
			}
			evaporate();
		}

		lastBestPath = bestPath;
		lastBestPathLen = bestLen;
		int sum_iter = iters;
		
		for (int i = 0; i < n_iters && sum_iter <= max_iters; i++, sum_iter++)
		{
        	for (int j = 0; j < p; j++)
			{
				AntColony colony(nants);
				colony.initAnts(params);
				for (Ant& ant : colony.ants)
					runAnt(ant, nodes, bestLen, bestPath, sum_iter, antId++, output_file);	
			}

			if (abs(lastBestPathLen - bestLen) > eps)
			{
				i = -1;
				lastBestPath = bestPath;
				lastBestPathLen = bestLen;	
			}
			evaporate();
		}		
	}

	output_file.close();

	if (bestLen == paraml)
	{
		cout << "inf" << std::endl;
		return;
	}

	cout << "Best path length: " << bestLen << std::endl << "Path: ";
	
	for (auto n : bestPath)
		cout << n->getName() << " ";
		
	std::cout << std::endl;
}
