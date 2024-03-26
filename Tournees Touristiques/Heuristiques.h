#include <iostream>
#include "Instance.h"
#include "Solution.h"
#ifndef Heuristiques_h
#define Heuristiques_h

using namespace std;

class Heuristiques
{
private:
	Instance* instance;
	Solution* solution;

public:
	Heuristiques();
	Heuristiques(Instance* instance_Param, Solution* solution_Param);

	Solution methode_Heuristique_V1();
	// Faire le destructeur

	float calcul_Distance_POI_Actuel_Et_POI_Courant(int num_Jour, vector<int>  vector_id_POI_Jour, int id_POI);
	void calcul_Distance_POI_Courant_Et_Plus_Proche_Hotel_Et_Id_PPH(float* distance_POI_Courant_Et_Plus_Proche_Hotel, int* id_Hotel_Plus_Proche_POI_Courant, int num_Jour, int nb_Jour, int nb_Hotel, int id_POI);
	float calcul_Distance_Parcouru_Jour(int num_Jour, vector<int> vector_id_POI_Jour);

	bool is_POI_Deja_Visite(int num_Jour, vector<int> vector_id_POI_Jour, int id_POI);
	bool is_Date_Depart_OK(int num_Jour, int id_POI, int id_Hotel_Plus_Proche_POI_Courant);
	bool is_Date_Arrive_POI_OK(int id_POI, float distance_Parcouru_Jour, float distance_POI_Actuel_Et_POI_Courant);

};

#endif