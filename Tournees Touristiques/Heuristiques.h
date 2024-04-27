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

	/* EURISTIQUES : Constructeur par d�faut.
	 * Entr�e : Rien.
	 * N�cessite : Rien.
	 * Sortie : Rien.
	 * Entra�ne : Cr�e un objet de la classe Heuristiques en l'initialisant avec des valeurs par d�faut.
	*/
	Heuristiques();

	/* HEURISTIQUES : Constructeur par d�faut.
	 * Entr�e : Rien.
	 * N�cessite : Rien.
	 * Sortie : Rien.
	 * Entra�ne : Cr�e un objet de la classe Heuristiques avec le pointeur d'instance voulu et en l'initialisant les autres attrivuts avec des valeurs par d�faut.
	*/
	Heuristiques(Instance* instance_Param);

	/* HEURISTIQUES_V2 : 2eme version de notre heuristique marchant pour toutes les instances.
	 * Entr�e : Rien.
	 * N�cessite : Rien.
	 * Sortie : Rien.
	 * Entra�ne : Renvoie une solution r�alisable pour l'instance.
	*/
	Solution heuristique_V2();

	/* OBTENIR_HOTELS_INTERMEDIARES : Permets de remplir le vecteur des h�tels interm�diaires de la solution avec des h�tels atteignables les uns les autres, y compris entre le dernier et l'h�tel d'arriv�e.
	 * Entr�e : Rien.
	 * N�cessite : Rien.
	 * Sortie : Rien.
	 * Entra�ne : Remplie le vecteur des h�tels interm�diaires de la solution avec des h�tels atteignables les uns les autres, y compris entre le dernier et l'h�tel d'arriv�e.
	*/
	void obtenir_Hotels_Intermediaire();

	/* IS_POI_DEJA_VISITE : Permets de savoir le POI a d�j� �t� visit� pendant la tourn�e.
	 * Entr�e : - num_Jour, int, num�ro du jour auquel on est.
	 *			- id_POI, int, id du POI que l'on veut v�rifier la pr�sence.
	 * N�cessite : Rien.
	 * Sortie : - True si le POI est pr�sent.
	 *			- False sinon.
	 * Entra�ne : Rien.
	*/
	bool is_POI_Deja_Visite(int num_Jour, int id_POI);

	/* CALCUL_DISTANCE_HOTEL_PREC_POI : Permets de calculer la distance entre l'h�tel pr�c�dent et le POI en param�tre.
	 * Entr�e : - num_Jour, int, num�ro du jour auquel on est.
	 *			- id_POI, int, id du POI que l'on veut calculer la distance avec l'h�tel.
	 * N�cessite : Rien.
	 * Sortie : float, la distance entre l'h�tel pr�c�dent et le POI en param�tre.
	 * Entra�ne : Rien.
	*/
	float calcul_Distance_Hotel_Prec_POI(int num_Jour, int id_POI);

	/* IS_DATE_DEPART_OK : Permets de savoir si l'on peut avoir une date de d�part positif avec ce POI.
	 * Entr�e : - id_POI, int, id du POI que l'on veut calculer la date de d�part.
	 *			- distance_Hotel_Et_POI_Courant, float, distance entre l'h�tel et le POI courant.
	 * N�cessite : Rien.
	 * Sortie : - True si l'on peut avoir une date de d�part positif.
	 *			- False sinon.
	 * Entra�ne : Rien.
	*/
	bool is_Date_Depart_OK(int id_POI, float distance_Hotel_Et_POI_Courant);

	/* OBTENIR_DATE_DEPART : Permets d'obtenir la date de d�part avec ce POI.
	 * Entr�e : - id_POI, int, id du POI que l'on veut calculer la date de d�part.
	 *			- distance_Hotel_Et_POI_Courant, float, distance entre l'h�tel et le POI courant.
	 * N�cessite : Rien.
	 * Sortie : Date de d�part avec ce POI
	 * Entra�ne : Rien.
	*/
	float obtenir_Date_Depart(int id_POI, float distance_POI_Actuel_Et_POI_Courant);

	/* IS_DATE_ARRIVE_POI_OK : Permets de savoir si l'on arrive avant la fermeture du POI non.
	 * Entr�e : - id_POI, int, id du POI que l'on veut voir si la date d'arriv� est avant ou apr�s la fermeture.
	 *			- distance_Parcouru_Jour, float, distance parcouru actuellement dans la journ�e.
	 *			- distance_POI_Actuel_Et_POI_Courant, float, distance entre le POI / Hotel actuel et le POI courant.
	 *			- date_Depart, float, date de d�part du jour.
	 * N�cessite : Rien.
	 * Sortie : - True si l'on arrive au POI avant sa fermeture.
	 *			- False sinon.
	 * Entra�ne : Rien.
	*/
	bool is_Date_Arrive_POI_OK(int id_POI, float distance_Parcouru_Jour, float distance_POI_Actuel_Et_POI_Courant, float date_Depart);

	/* CALCUL_DISTANCE_TOTALE_POTENTIELLE_JOUR_1_POI : Permets d'obtenir la distance totale potentielle pour un jour avec 1 POI.
	 * Entr�e : - num_Jour, int, num�ro du jour actuel.
				- id_POI, int, id du POI que l'on veut calculer la date de d�part.
	 *			- distance_Hotel_POI_Courant, float, distance entre l'h�tel et le POI courant.
	 * N�cessite : Rien.
	 * Sortie : Distance totale potentielle du jour.
	 * Entra�ne : Rien.
	*/
	float calcul_Distance_Totale_Potentielle_Jour_1_POI(int num_Jour, int id_1er_POI, float distance_Hotel_POI_Courant);

	/* CALCUL_DISTANCE_PARCOURU_JOUR : Permets d'obtenir la distance parcouru actuellement dans la journ�e.
	 * Entr�e : - num_Jour, int, num�ro du jour actuel.
	 *			- date_Depart, float, date de d�part du jour.
	 * N�cessite : Rien.
	 * Sortie : float, distance parcouru actuellement dans la journ�e.
	 * Entra�ne : Rien.
	*/
	float calcul_Distance_Parcouru_Jour(int num_Jour, float date_Depart);

	/* CALCUL_DISTANCE_TOTALE_POTENTIELLE_JOUR_N_POI : Permets d'obtenir la distance totale potentielle pour un jour avec n POIs.
	 * Entr�e : - num_Jour, int, num�ro du jour actuel.
				- id_POI, int, id du POI que l'on veut calculer la date de d�part.
	 *			- distance_Hotel_POI_Courant, float, distance entre l'h�tel et le POI courant.
	 * N�cessite : Rien.
	 * Sortie : Distance totale potentielle du jour.
	 * Entra�ne : Rien.
	*/
	float calcul_Distance_Totale_Potentielle_Jour_N_POI(int num_Jour, int id_POI_Suite, float date_Depart, float distance_Parcouru_Jour, float distance_POI_Actuel_POI_Courant);

	/* HEURISTIQUES_V1 : 1er version de notre heuristique marchant jusqu'� l'instance 16 mais ne compile plus d� au modification apport�es aux m�thodes pour le fonctionnement de la V2.
	 * Entr�e : Rien.
	 * N�cessite : Rien.
	 * Sortie : Rien.
	 * Entra�ne : Renvoie une solution r�alisable pour l'instance ( ne marche pas pour les instances apr�s 15 ).
	*/
	// Solution heuristique_V1();

	/* OBTENIR_HOTEL : Permets d'obtenir l'h�tel le plus de proche de l'h�tel d'arriv� atteignable par le POI.
	 * Entr�e : - num_Jour, int, num�ro du jour actuel.
				- id_POI, int, id du POI que l'on veut calculer la date de d�part.
	 *			- distance_POI_Actuel_Et_POI_Courant, float, distance entre l'h�tel et le POI courant.
	 * N�cessite : Rien.
	 * Sortie : int, id de l'h�tel le plus de proche de l'h�tel d'arriv� atteignable par le POI.
	 * Entra�ne : Rien.
	*/
	// int obtenir_Hotel(int num_Jour, int id_POI, float distance_POI_Actuel_Et_POI_Courant); ( utilis� dans la V1 )

		/* OBTENIR_INDICE_HOTEL_PLUS_PROCHE_HOTEL_ARRIVEE : Permets d'obtenir l'indice de l'h�tel le plus de proche de l'h�tel d'arriv� dans le vecteur des h�tels de fin possible.
	* Entr�e : - sequences_Hotel_Fin_Possible, vector<int>, h�tel de fin possible.
	* N�cessite : Rien.
	* Sortie : int, indice de l'h�tel le plus de proche de l'h�tel d'arriv� dans le vecteur des h�tels de fin possible.
	* Entra�ne : Rien.
	*/
	// int obtenir_indice_Hotel_Plus_Proche_Hotel_Arrivee(vector<int> sequences_Hotel_Fin_Possible); (utilis� dans la V1)

};

#endif