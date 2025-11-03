
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
#include "ic.h"

using namespace std;

struct Ant {
	// слева направо
	// параметр, контролирующий влияние количества феромонов на ребре
	// параметр, контролирующий влияние привлекательности ребра
	// скорость испарения феромона
	double alpha, beta, rho;
   	Ant(double a, double b, double r);
};

class AntColony {

private:
   	Graph& graph; // сам граф (чтобы не создавать его внутри колонии)
   	const Ic& ic; // наша конфигурация
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

	// инициализируем муравьёв (в одной пачке)
	void initAnts(std::vector<Ant>& ants);

	// считаем вероятность выбора пути муравьем
	double calcProb(Node* curr, Node* neigh, const Ant& ant);

	// выбор следующей ноды
   	Node* chooseNextNode(Node* current, const unordered_map<Node*, bool>& visited, const Ant& ant);

	// муравей строит путь
	vector<Node*> buildAntPath(vector<Node*>& nodes, Ant& ant);

	// обновляем феромоны
	void updatePhers(vector<Node*>& path, const double& len, Ant& ant);

	// запуск одного муравья (с логированием)
	void runAnt(Ant& ant, vector<Node*>& nodes, int& bestLen, vector<Node*>& bestPath, size_t& iter, const int& antId, std::ofstream& outfile);
	
public:
	AntColony(const Ic& aic, Graph& g);	
   	void run(); // запуск алгоритма
};

class FileReadError {};

#endif
