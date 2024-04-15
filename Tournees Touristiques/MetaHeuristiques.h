#include <iostream>
#include <unordered_map>
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

	unordered_map<Solution, vector<int>> generer_Voisinage(Solution solution);
	Solution* get_Meilleure_Solution(unordered_map<Solution, vector<int>> voisinage, vector<vector<int>> liste_Tabou);

	void remplacer_Plus_Ancien_Mouvement(vector<vector<int>>* liste_Tabou, vector<int>* mouvement_Tabou);

};

#endif