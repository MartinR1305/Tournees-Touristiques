#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
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

	void generer_Voisinage(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage);
	void generer_Voisinage_Operateur_SWAP_Hotel(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage);
	void generer_Voisinage_Operateur_Ajouter_POI(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage);
	void generer_Voisinage_Operateur_SWAP_POI(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage);

	int get_Indice_Meilleure_Solution(vector<Solution> solution_Voisinage, vector<vector<int>> mouvement_Voisinage, vector<vector<int>> liste_Tabou);

	void remplacer_Plus_Ancien_Mouvement(vector<vector<int>>* liste_Tabou, vector<int> mouvement_Tabou);

	Solution generer_Solution_Aleatoire();

	bool is_Mouvement_Dans_Liste_Tabou(vector<int> mouvement, vector<vector<int>> liste_Tabou);
	bool is_Solution_Prec_Egale(Solution solution_Precedente, vector<Solution> solution_Voisinage, int indice_Meilleure_Solution);

};

#endif