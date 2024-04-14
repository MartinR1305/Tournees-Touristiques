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

public:
	Heuristiques();
	Heuristiques(Instance* instance_Param);

	Solution methode_Heuristique();
	// Faire le destructeur

	float calcul_Distance_POI_Actuel_Et_POI_Courant(int num_Jour, vector<int>  vector_id_POI_Jour, int id_POI, Solution* solution);
	int obtenir_Hotel(int num_Jour, vector<int>  vector_id_POI_Jour, int id_POI, float distance_POI_Actuel_Et_POI_Courant, float tps_Attente, float date_Depart, Solution* solution);
	float calcul_Distance_Parcouru_Jour(int num_Jour, float date_Depart, vector<int> vector_id_POI_Jour, Solution* solution);

	float obtenir_Date_Depart(int id_POI, int num_Jour, float distance_POI_Actuel_Et_POI_Courant, vector<int> vector_id_POI_Jour, Solution* solution);

	bool is_POI_Deja_Visite(int num_Jour, vector<int> vector_id_POI_Jour, int id_POI, Solution* solution);
	bool is_Date_Depart_OK(int id_POI, float distance_POI_Actuel_Et_POI_Courant, vector<int> vector_id_POI_Jour);
	bool is_Date_Arrive_POI_OK(int num_Jour, int id_POI, float distance_Parcouru_Jour, float distance_POI_Actuel_Et_POI_Courant, float date_Depart);

};

#endif