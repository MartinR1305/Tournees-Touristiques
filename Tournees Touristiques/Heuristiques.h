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

	/* EURISTIQUES : Constructeur par défaut.
	 * Entrée : Rien.
	 * Nécessite : Rien.
	 * Sortie : Rien.
	 * Entraîne : Crée un objet de la classe Heuristiques en l'initialisant avec des valeurs par défaut.
	*/
	Heuristiques();

	/* HEURISTIQUES : Constructeur par défaut.
	 * Entrée : Rien.
	 * Nécessite : Rien.
	 * Sortie : Rien.
	 * Entraîne : Crée un objet de la classe Heuristiques avec le pointeur d'instance voulu et en l'initialisant les autres attrivuts avec des valeurs par défaut.
	*/
	Heuristiques(Instance* instance_Param);

	/* HEURISTIQUES_V2 : 2eme version de notre heuristique marchant pour toutes les instances.
	 * Entrée : Rien.
	 * Nécessite : Rien.
	 * Sortie : Rien.
	 * Entraîne : Renvoie une solution réalisable pour l'instance.
	*/
	Solution heuristique_V2();

	/* OBTENIR_HOTELS_INTERMEDIARES : Permets de remplir le vecteur des hôtels intermédiaires de la solution avec des hôtels atteignables les uns les autres, y compris entre le dernier et l'hôtel d'arrivée.
	 * Entrée : Rien.
	 * Nécessite : Rien.
	 * Sortie : Rien.
	 * Entraîne : Remplie le vecteur des hôtels intermédiaires de la solution avec des hôtels atteignables les uns les autres, y compris entre le dernier et l'hôtel d'arrivée.
	*/
	void obtenir_Hotels_Intermediaire();

	/* IS_POI_DEJA_VISITE : Permets de savoir le POI a déjà été visité pendant la tournée.
	 * Entrée : - num_Jour, int, numéro du jour auquel on est.
	 *			- id_POI, int, id du POI que l'on veut vérifier la présence.
	 * Nécessite : Rien.
	 * Sortie : - True si le POI est présent.
	 *			- False sinon.
	 * Entraîne : Rien.
	*/
	bool is_POI_Deja_Visite(int num_Jour, int id_POI);

	/* CALCUL_DISTANCE_HOTEL_PREC_POI : Permets de calculer la distance entre l'hôtel précèdent et le POI en paramètre.
	 * Entrée : - num_Jour, int, numéro du jour auquel on est.
	 *			- id_POI, int, id du POI que l'on veut calculer la distance avec l'hôtel.
	 * Nécessite : Rien.
	 * Sortie : float, la distance entre l'hôtel précèdent et le POI en paramètre.
	 * Entraîne : Rien.
	*/
	float calcul_Distance_Hotel_Prec_POI(int num_Jour, int id_POI);

	/* IS_DATE_DEPART_OK : Permets de savoir si l'on peut avoir une date de départ positif avec ce POI.
	 * Entrée : - id_POI, int, id du POI que l'on veut calculer la date de départ.
	 *			- distance_Hotel_Et_POI_Courant, float, distance entre l'hôtel et le POI courant.
	 * Nécessite : Rien.
	 * Sortie : - True si l'on peut avoir une date de départ positif.
	 *			- False sinon.
	 * Entraîne : Rien.
	*/
	bool is_Date_Depart_OK(int id_POI, float distance_Hotel_Et_POI_Courant);

	/* OBTENIR_DATE_DEPART : Permets d'obtenir la date de départ avec ce POI.
	 * Entrée : - id_POI, int, id du POI que l'on veut calculer la date de départ.
	 *			- distance_Hotel_Et_POI_Courant, float, distance entre l'hôtel et le POI courant.
	 * Nécessite : Rien.
	 * Sortie : Date de départ avec ce POI
	 * Entraîne : Rien.
	*/
	float obtenir_Date_Depart(int id_POI, float distance_POI_Actuel_Et_POI_Courant);

	/* IS_DATE_ARRIVE_POI_OK : Permets de savoir si l'on arrive avant la fermeture du POI non.
	 * Entrée : - id_POI, int, id du POI que l'on veut voir si la date d'arrivé est avant ou après la fermeture.
	 *			- distance_Parcouru_Jour, float, distance parcouru actuellement dans la journée.
	 *			- distance_POI_Actuel_Et_POI_Courant, float, distance entre le POI / Hotel actuel et le POI courant.
	 *			- date_Depart, float, date de départ du jour.
	 * Nécessite : Rien.
	 * Sortie : - True si l'on arrive au POI avant sa fermeture.
	 *			- False sinon.
	 * Entraîne : Rien.
	*/
	bool is_Date_Arrive_POI_OK(int id_POI, float distance_Parcouru_Jour, float distance_POI_Actuel_Et_POI_Courant, float date_Depart);

	/* CALCUL_DISTANCE_TOTALE_POTENTIELLE_JOUR_1_POI : Permets d'obtenir la distance totale potentielle pour un jour avec 1 POI.
	 * Entrée : - num_Jour, int, numéro du jour actuel.
				- id_POI, int, id du POI que l'on veut calculer la date de départ.
	 *			- distance_Hotel_POI_Courant, float, distance entre l'hôtel et le POI courant.
	 * Nécessite : Rien.
	 * Sortie : Distance totale potentielle du jour.
	 * Entraîne : Rien.
	*/
	float calcul_Distance_Totale_Potentielle_Jour_1_POI(int num_Jour, int id_1er_POI, float distance_Hotel_POI_Courant);

	/* CALCUL_DISTANCE_PARCOURU_JOUR : Permets d'obtenir la distance parcouru actuellement dans la journée.
	 * Entrée : - num_Jour, int, numéro du jour actuel.
	 *			- date_Depart, float, date de départ du jour.
	 * Nécessite : Rien.
	 * Sortie : float, distance parcouru actuellement dans la journée.
	 * Entraîne : Rien.
	*/
	float calcul_Distance_Parcouru_Jour(int num_Jour, float date_Depart);

	/* CALCUL_DISTANCE_TOTALE_POTENTIELLE_JOUR_N_POI : Permets d'obtenir la distance totale potentielle pour un jour avec n POIs.
	 * Entrée : - num_Jour, int, numéro du jour actuel.
				- id_POI, int, id du POI que l'on veut calculer la date de départ.
	 *			- distance_Hotel_POI_Courant, float, distance entre l'hôtel et le POI courant.
	 * Nécessite : Rien.
	 * Sortie : Distance totale potentielle du jour.
	 * Entraîne : Rien.
	*/
	float calcul_Distance_Totale_Potentielle_Jour_N_POI(int num_Jour, int id_POI_Suite, float date_Depart, float distance_Parcouru_Jour, float distance_POI_Actuel_POI_Courant);

	/* HEURISTIQUES_V1 : 1er version de notre heuristique marchant jusqu'à l'instance 16 mais ne compile plus dû au modification apportées aux méthodes pour le fonctionnement de la V2.
	 * Entrée : Rien.
	 * Nécessite : Rien.
	 * Sortie : Rien.
	 * Entraîne : Renvoie une solution réalisable pour l'instance ( ne marche pas pour les instances après 15 ).
	*/
	// Solution heuristique_V1();

	/* OBTENIR_HOTEL : Permets d'obtenir l'hôtel le plus de proche de l'hôtel d'arrivé atteignable par le POI.
	 * Entrée : - num_Jour, int, numéro du jour actuel.
				- id_POI, int, id du POI que l'on veut calculer la date de départ.
	 *			- distance_POI_Actuel_Et_POI_Courant, float, distance entre l'hôtel et le POI courant.
	 * Nécessite : Rien.
	 * Sortie : int, id de l'hôtel le plus de proche de l'hôtel d'arrivé atteignable par le POI.
	 * Entraîne : Rien.
	*/
	// int obtenir_Hotel(int num_Jour, int id_POI, float distance_POI_Actuel_Et_POI_Courant); ( utilisé dans la V1 )

		/* OBTENIR_INDICE_HOTEL_PLUS_PROCHE_HOTEL_ARRIVEE : Permets d'obtenir l'indice de l'hôtel le plus de proche de l'hôtel d'arrivé dans le vecteur des hôtels de fin possible.
	* Entrée : - sequences_Hotel_Fin_Possible, vector<int>, hôtel de fin possible.
	* Nécessite : Rien.
	* Sortie : int, indice de l'hôtel le plus de proche de l'hôtel d'arrivé dans le vecteur des hôtels de fin possible.
	* Entraîne : Rien.
	*/
	// int obtenir_indice_Hotel_Plus_Proche_Hotel_Arrivee(vector<int> sequences_Hotel_Fin_Possible); (utilisé dans la V1)

};

#endif