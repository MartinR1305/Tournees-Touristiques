#include <iostream>
#include <map>
#include "Instance.h"
#include "Solution.h"
#include "Heuristiques.h"
#ifndef MetaHeuristiques_h
#define MetaHeuristiques_h

using namespace std;

class MetaHeuristiques
{
private:
	Instance* instance;

public:
	MetaHeuristiques();
	MetaHeuristiques(Instance* instance_Param);
	// Faire le destructeur ( promis on le fait )

	Solution methode_MetaHeuristiques();

	map<Solution, vector<int>> generer_Voisinage();
	Solution* get_Meilleur_Solution(map<Solution, vector<int>> voisinage, vector<vector<int>> liste_Tabou);

};

#endif