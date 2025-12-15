
#ifndef _ANT_H_
#define _ANT_H_

#include <vector>
#include <unordered_map>
#include <map>
#include <random>
#include <iostream>
#include <cmath>
#include <string>

#include "graph.h"
#include "node.h"
#include "cfig.h"

using namespace std;

struct Ant {
	// слева направо
	// параметр, контролирующий влияние количества феромонов на ребре
	// параметр, контролирующий влияние привлекательности ребра
	// скорость испарения феромона
	double alpha, beta, rho;
   	Ant(double a, double b, double r);
};

struct AntColony
{
	std::vector<Ant> ants;
    AntColony(const Cfig& config);
};

class ACO {

private:
   	Graph& graph; // сам граф (чтобы не создавать его внутри колонии)
   	const Cfig& config; // наша конфигурация
   	mt19937 rng; // генератор псевдослучайных чисел

   	// ассоциативный массив
   	// 2 ноды: количество феромонов на ребре
	map<pair<Node*, Node*>, double> pher;

	// инициализация феромонов по начальному значению из конфигурации
	void initPher();

	// получить значение феромона на ребре
	double getPher(Node* a, Node* b) const;

	// обновить значние феромона на ребре
	void updatePher(Node* a, Node* b, double val);

	// считаем путь
   	int computePathLength(const vector<Node*>& path) const;

	// получаем количество феромонов с пути
   	double getPathPhers(const vector<Node*>& path);
   	double getPhers();

	// считаем вероятность выбора пути муравьем
	double calcProb(Node* curr, Node* neigh, const Ant& ant);

	// выбор следующей ноды
   	Node* chooseNextNode(Node* current, const unordered_map<Node*, bool>& visited, const Ant& ant);

	// муравей строит путь
	vector<Node*> buildAntPath(vector<Node*>& nodes, Ant& ant);

	// обновляем феромоны на ребрах, которые прошел муравей
	void updatePhers(vector<Node*>& path, const double& len);

	void evaporate(AntColony& colony);

	// запуск одного муравья (с логированием)
	void runAnt(Ant& ant, vector<Node*>& nodes, int& bestLen, vector<Node*>& bestPath, int& iter, const int& antId, std::ofstream& outfile);
		
public:
	ACO(const Cfig& cfig, Graph& g);	
   	void run(); // запуск алгоритма
};

class FileNotFoundError {};

#endif
