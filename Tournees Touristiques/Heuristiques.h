#include <iostream>
#include <random>
#include "Instance.h"
#include "Solution.h"
#ifndef Heuristiques_h
#define Heuristiques_h

using namespace std;

class Heuristiques
{
private:
	Instance* instance;

	int nb_Jour;
	int nb_Hotel;
	int nb_POI;

	int id_Hotel_Dep;
	int id_Hotel_Arr;

	vector<int> vector_POI_Jour_Actuel;

	Solution* solution;

public:
	Heuristiques();
	Heuristiques(Instance* instance_Param);

	Solution heuristique_V1();
	Solution heuristique_V2();
	// Faire le destructeur

	void obtenir_Hotels(Solution* solution);

	float calcul_Distance_Hotel_Et_POI_Courant(int num_Jour, int id_POI);
	int obtenir_Hotel(int num_Jour, int id_POI, float distance_POI_Actuel_Et_POI_Courant);
	float calcul_Distance_Parcouru_Jour(int num_Jour, float date_Depart);

	float obtenir_Date_Depart(int id_POI, int num_Jour, float distance_POI_Actuel_Et_POI_Courant);

	bool is_POI_Deja_Visite(int num_Jour, int id_POI);
	bool is_Date_Depart_OK(int id_POI, float distance_Hotel_Et_POI_Courant);
	bool is_Date_Arrive_POI_OK(int num_Jour, int id_POI, float distance_Parcouru_Jour, float distance_POI_Actuel_Et_POI_Courant, float date_Depart);

	int obtenir_indice_Hotel_Plus_Proche_Hotel_Arrivee(vector<int> sequences_Hotel_Fin_Possible);

};

#endif