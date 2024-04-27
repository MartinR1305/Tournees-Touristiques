#include "Heuristiques.h"

/* EURISTIQUES : Constructeur par défaut.
 * Entrée : Rien.
 * Nécessite : Rien.
 * Sortie : Rien.
 * Entraîne : Crée un objet de la classe Heuristiques en l'initialisant avec des valeurs par défaut.
*/
Heuristiques::Heuristiques() {
	instance = new Instance();
	solution = new Solution();

	nb_Jour = 0;
	nb_Hotel = 0;
	nb_POI = 0;

	id_Hotel_Dep = 0;
	id_Hotel_Arr = 0;
}

/* HEURISTIQUES : Constructeur par défaut.
 * Entrée : Rien.
 * Nécessite : Rien.
 * Sortie : Rien.
 * Entraîne : Crée un objet de la classe Heuristiques avec le pointeur d'instance voulu et en l'initialisant les autres attrivuts avec des valeurs par défaut.
*/
Heuristiques::Heuristiques(Instance* instance_Param) {
	instance = instance_Param;
	solution = new Solution();

	nb_Jour = 0;
	nb_Hotel = 0;
	nb_POI = 0;

	id_Hotel_Dep = 0;
	id_Hotel_Arr = 0;
}

/* HEURISTIQUES_V2 : 2eme version de notre heuristique marchant pour toutes les instances.
 * Entrée : Rien.
 * Nécessite : Rien.
 * Sortie : Rien.
 * Entraîne : Renvoie une solution réalisable pour l'instance.
*/
Solution Heuristiques::heuristique_V2() {

	// Récupération des données de l'instance.
	id_Hotel_Dep = instance->get_Id_Hotel_depart();
	id_Hotel_Arr = instance->get_Id_Hotel_Arrivee();
	nb_Jour = instance->get_Nombre_Jour();
	nb_POI = instance->get_Nombre_POI();
	nb_Hotel = instance->get_Nombre_Hotel();

	// Booléen nous disant si l'on a bien un POI pour chaque jour de notre solution.
	bool is_POI_Chaque_Jour;

	// Tant que notre solution n'a pas au moins un POI par jour, on regènere une autre solution.
	do {

		// On (re)part d'une solution vide.
		solution->i_valeur_fonction_objectif = 0;
		solution->v_Date_Depart.clear();
		solution->v_Id_Hotel_Intermedaire.clear();
		solution->v_v_Sequence_Id_Par_Jour.clear();

		// On calcule notre vecteur hôtel afin de pouvoir atteindre l'hôtel d'arrivée.
		obtenir_Hotels_Intermediaire();

		// On parcours tous les jours.
		for (int num_Jour = 0; num_Jour < nb_Jour; num_Jour++) {

			vector_POI_Jour_Actuel.clear();
			float distance_Max_Jour_Actuel = instance->get_POI_Duree_Max_Voyage(num_Jour);

			// On parcours tous les POIs afin de choisir le 1er POI pour le jour actuel.
			for (int id_1er_POI = 0; id_1er_POI < nb_POI; id_1er_POI++) {

				// On regarde si le POI a déjà été visité.
				if (!is_POI_Deja_Visite(num_Jour, id_1er_POI)) {

					float distance_Hotel_POI_Courant = calcul_Distance_Hotel_Prec_POI(num_Jour, id_1er_POI);

					// On regarde si la date de départ par défaut est possible.
					if (is_Date_Depart_OK(id_1er_POI, distance_Hotel_POI_Courant)) {

						float date_Depart = obtenir_Date_Depart(id_1er_POI, distance_Hotel_POI_Courant);

						// On regarde si la d'arrivé au POI n'est pas supérieure à la date de fermeture du POI.
						if (is_Date_Arrive_POI_OK(id_1er_POI, 0, distance_Hotel_POI_Courant, date_Depart)) {

							float distance_Totale_Potentielle_Jour = calcul_Distance_Totale_Potentielle_Jour_1_POI(num_Jour, id_1er_POI, distance_Hotel_POI_Courant);

							// On regarde si la distance totale potentielle est bien inférieur ou égale à la distance max du jour.
							if (distance_Totale_Potentielle_Jour <= distance_Max_Jour_Actuel) {

								// On mets à jour la solution.
								solution->i_valeur_fonction_objectif += instance->get_POI_Score(id_1er_POI);
								solution->v_Date_Depart.push_back(date_Depart);

								// On ajoute le POI au vecteur.
								vector_POI_Jour_Actuel.push_back(id_1er_POI);

								// On reparcours tous les POIs afin d'en ajouter d'autres à la suite de notre vecteur.
								for (int id_POI_Suite = 0; id_POI_Suite < nb_POI; id_POI_Suite++) {

									// On regarde si le POI a déjà été visité.
									if (!is_POI_Deja_Visite(num_Jour, id_POI_Suite)) {

										float distance_POI_Actuel_POI_Courant = instance->get_distance_POI_POI(vector_POI_Jour_Actuel[vector_POI_Jour_Actuel.size() - 1], id_POI_Suite);
										float distance_Parcouru_Jour = calcul_Distance_Parcouru_Jour(num_Jour, date_Depart);

										// On regarde si la d'arrivé au POI n'est pas supérieure à la date de fermeture du POI.
										if (is_Date_Arrive_POI_OK(id_POI_Suite, distance_Parcouru_Jour, distance_POI_Actuel_POI_Courant, date_Depart)) {

											distance_Totale_Potentielle_Jour = calcul_Distance_Totale_Potentielle_Jour_N_POI(num_Jour, id_POI_Suite, date_Depart, distance_Parcouru_Jour, distance_POI_Actuel_POI_Courant);

											// On regarde si la distance totale potentielle est bien inférieur ou égale à la distance max du jour.
											if (distance_Totale_Potentielle_Jour <= distance_Max_Jour_Actuel) {

												// On mets à jour la F.O de la solution.
												solution->i_valeur_fonction_objectif += instance->get_POI_Score(id_POI_Suite);

												// On ajoute le POI au vecteur.
												vector_POI_Jour_Actuel.push_back(id_POI_Suite);
											}
										}
									}
								}
								// Dès que l'on a trouvé un vecteur on arrête pour le jour.
								break;
							}
						}
					}
				}
			}
			// On ajoute le vecteur à la solution.
			solution->v_v_Sequence_Id_Par_Jour.push_back(vector_POI_Jour_Actuel);
		}

		is_POI_Chaque_Jour = true;

		// On vérifie qu'il y a au moins un POI pour chaque jour.
		for (vector<int> vecteur_POI : solution->v_v_Sequence_Id_Par_Jour) {
			if (vecteur_POI.size() == 0) {
				is_POI_Chaque_Jour = false;
			}
		}

	} while (!is_POI_Chaque_Jour);

	return *solution;
}

/* OBTENIR_HOTELS_INTERMEDIARES : Permets de remplir le vecteur des hôtels intermédiaires de la solution avec des hôtels atteignables les uns les autres, y compris entre le dernier et l'hôtel d'arrivée.
 * Entrée : Rien.
 * Nécessite : Rien.
 * Sortie : Rien.
 * Entraîne : Remplie le vecteur des hôtels intermédiaires de la solution avec des hôtels atteignables les uns les autres, y compris entre le dernier et l'hôtel d'arrivée.
*/
void Heuristiques::obtenir_Hotels_Intermediaire() {

	vector<int> hotels;

	// Attribut permettant la génération d'entier.
	random_device rd;
	mt19937 gen(rd());

	// Tant que la distante entre le dernier hôtel de notre vecteur et l'hôtel d'arrivée est supérieur à la distance max du dernier jour, on regénère un autre vecteur d'hôtels.
	do {

		// On (re)part d'un vecteur d'hôtels vide.
		hotels.clear();

		// On parcours tous les jours sauf le dernier car on connait déjà l'hôtel d'arrivée.
		for (int num_Jour = 0; num_Jour < nb_Jour - 1; num_Jour++) {

			// Vecteur des hôtels atteignables à partir de l'hôtel précèdents.
			vector<int> id_Hotel_Potentiel;

			int id_Hotel_Choisi = -1;

			// On parcours tous les hôtels.
			for (int id_Hotel = 0; id_Hotel < nb_Hotel; id_Hotel++) {

				// Cas si c'est le 1er jour.
				if (num_Jour == 0) {

					// Si l'hôtel est atteignable on l'ajoute au vecteur des hôtels potentiels.
					if (instance->get_distance_Hotel_Hotel(id_Hotel_Dep, id_Hotel) < instance->get_POI_Duree_Max_Voyage(num_Jour)) {
						id_Hotel_Potentiel.push_back(id_Hotel);
					}
				}

				// Cas où ce n'est pas le 1er.
				else {

					// Si l'hôtel est atteignable on l'ajoute au vecteur des hôtels potentiels.
					if (instance->get_distance_Hotel_Hotel(hotels[num_Jour - 1], id_Hotel) < instance->get_POI_Duree_Max_Voyage(num_Jour)) {
						id_Hotel_Potentiel.push_back(id_Hotel);
					}
				}
			}

			// On choisit un hôtel au hasard.
			uniform_int_distribution<> dis_Hotel(0, id_Hotel_Potentiel.size() - 1);
			id_Hotel_Choisi = id_Hotel_Potentiel[dis_Hotel(gen)];

			// On l'ajoute à notre vecteur d'hôtel.
			hotels.push_back(id_Hotel_Choisi);
		}
	} while (instance->get_distance_Hotel_Hotel(hotels[nb_Jour - 2], id_Hotel_Arr) > instance->get_POI_Duree_Max_Voyage(nb_Jour - 2));

	// Dès que notre vecteur est correct, on remplace celui de notre solution par ce dernier.
	for (int num_Jour = 0; num_Jour < nb_Jour - 1; num_Jour++) {
		solution->v_Id_Hotel_Intermedaire.push_back(hotels[num_Jour]);
	}
}

/* IS_POI_DEJA_VISITE : Permets de savoir le POI a déjà été visité pendant la tournée.
 * Entrée : - num_Jour, int, numéro du jour auquel on est.
 *			- id_POI, int, id du POI que l'on veut vérifier la présence.
 * Nécessite : Rien.
 * Sortie : - True si le POI est présent.
 *			- False sinon.
 * Entraîne : Rien.
*/
bool Heuristiques::is_POI_Deja_Visite(int num_Jour, int id_POI) {

	// On regarde si le POI a déjà été visité aujourd'hui.
	if (vector_POI_Jour_Actuel.size() != 0) {
		for (int indice_POI_Jour_Act = 0; indice_POI_Jour_Act < vector_POI_Jour_Actuel.size(); indice_POI_Jour_Act++) {
			if (vector_POI_Jour_Actuel[indice_POI_Jour_Act] == id_POI) {
				return true;
			}
		}
	}

	// On regarde si le POI a déjà été visité dans les jours précédents.
	for (int num_Jour_Prec = 0; num_Jour_Prec < num_Jour; num_Jour_Prec++) {
		for (int indice_POI_Jour_Act = 0; indice_POI_Jour_Act < solution->v_v_Sequence_Id_Par_Jour[num_Jour_Prec].size(); indice_POI_Jour_Act++) {
			if (solution->v_v_Sequence_Id_Par_Jour[num_Jour_Prec][indice_POI_Jour_Act] == id_POI) {
				return true;
			}
		}
	}
	return false;
}

/* CALCUL_DISTANCE_HOTEL_PREC_POI : Permets de calculer la distance entre l'hôtel précèdent et le POI en paramètre.
 * Entrée : - num_Jour, int, numéro du jour auquel on est.
 *			- id_POI, int, id du POI que l'on veut calculer la distance avec l'hôtel.
 * Nécessite : Rien.
 * Sortie : float, la distance entre l'hôtel précèdent et le POI en paramètre.
 * Entraîne : Rien.
*/
float Heuristiques::calcul_Distance_Hotel_Prec_POI(int num_Jour, int id_POI) {

	// Cas si ce n'est pas le 1er jour.
	if (num_Jour != 0) {
		return instance->get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], id_POI);
	}

	// Cas si c'est le 1er jour.
	else {
		return instance->get_distance_Hotel_POI(instance->get_Id_Hotel_depart(), id_POI);
	}
}

/* IS_DATE_DEPART_OK : Permets de savoir si l'on peut avoir une date de départ positif avec ce POI.
 * Entrée : - id_POI, int, id du POI que l'on veut calculer la date de départ.
 *			- distance_Hotel_Et_POI_Courant, float, distance entre l'hôtel et le POI courant.
 * Nécessite : Rien.
 * Sortie : - True si l'on peut avoir une date de départ positif.
 *			- False sinon.
 * Entraîne : Rien.
*/
bool Heuristiques::is_Date_Depart_OK(int id_POI, float distance_POI_Actuel_Et_POI_Courant) {

	// Si l'on ne peut pas arriver à l'ouverture.
	if (instance->get_POI_Heure_ouverture(id_POI) - distance_POI_Actuel_Et_POI_Courant < 0) {

		// Si l'on ne peut pas arriver à la fermeture.
		if (instance->get_POI_Heure_fermeture(id_POI) - distance_POI_Actuel_Et_POI_Courant < 0) {
			return false;
		}
	}
	return true;
}

/* OBTENIR_DATE_DEPART : Permets d'obtenir la date de départ avec ce POI.
* Entrée : - id_POI, int, id du POI que l'on veut calculer la date de départ.
 *			- distance_Hotel_Et_POI_Courant, float, distance entre l'hôtel et le POI courant.
 * Nécessite : Rien.
 * Sortie : Date de départ avec ce POI
 * Entraîne : Rien.
*/
float Heuristiques::obtenir_Date_Depart(int id_POI, float distance_POI_Actuel_Et_POI_Courant) {

	float heure_Ouverture = instance->get_POI_Heure_ouverture(id_POI);
	float heure_Fermeture = instance->get_POI_Heure_fermeture(id_POI);

	if (heure_Ouverture - distance_POI_Actuel_Et_POI_Courant >= 0) {
		return heure_Ouverture - distance_POI_Actuel_Et_POI_Courant;
	}
	else {
		return heure_Fermeture - distance_POI_Actuel_Et_POI_Courant;
	}
}

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
bool Heuristiques::is_Date_Arrive_POI_OK(int id_POI, float distance_Parcouru_Jour, float distance_POI_Actuel_Et_POI_Courant, float date_Depart) {

	if (date_Depart + distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant > instance->get_POI_Heure_fermeture(id_POI)) {
		return false;
	}
	return true;
}

/* CALCUL_DISTANCE_TOTALE_POTENTIELLE_JOUR_1_POI : Permets d'obtenir la distance totale potentielle pour un jour avec 1 POI.
 * Entrée : - num_Jour, int, numéro du jour actuel.
			- id_POI, int, id du POI que l'on veut calculer la date de départ.
 *			- distance_Hotel_POI_Courant, float, distance entre l'hôtel et le POI courant.
 * Nécessite : Rien.
 * Sortie : Distance totale potentielle du jour.
 * Entraîne : Rien.
*/
float Heuristiques::calcul_Distance_Totale_Potentielle_Jour_1_POI(int num_Jour, int id_1er_POI, float distance_Hotel_POI_Courant) {
	float distance_POI_Courant_Hotel_Arrivee = 0;

	if (num_Jour == nb_Jour - 1) {
		distance_POI_Courant_Hotel_Arrivee = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), id_1er_POI);
	}

	else {
		distance_POI_Courant_Hotel_Arrivee = instance->get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour], id_1er_POI);
	}

	return distance_Hotel_POI_Courant + distance_POI_Courant_Hotel_Arrivee;
}

/* CALCUL_DISTANCE_PARCOURU_JOUR : Permets d'obtenir la distance parcouru actuellement dans la journée.
 * Entrée : - num_Jour, int, numéro du jour actuel.
 *			- date_Depart, float, date de départ du jour.
 * Nécessite : Rien.
 * Sortie : float, distance parcouru actuellement dans la journée.
 * Entraîne : Rien.
*/
float Heuristiques::calcul_Distance_Parcouru_Jour(int num_Jour, float date_Depart) {
	float heure_Actuelle = date_Depart;

	// Calcul distance parcouru dans la journée.
	if (vector_POI_Jour_Actuel.size() != 0) {

		// Distance entre l'hôtel et le 1er POI.
		if (num_Jour != 0) {
			heure_Actuelle += instance->get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], vector_POI_Jour_Actuel[0]);
		}
		else {
			heure_Actuelle += instance->get_distance_Hotel_POI(instance->get_Id_Hotel_depart(), vector_POI_Jour_Actuel[0]);
		}

		// Distance entre tout les POI.
		for (int id_POI_parcouru = 0; id_POI_parcouru < vector_POI_Jour_Actuel.size() - 1; id_POI_parcouru++) {
			heure_Actuelle += instance->get_distance_POI_POI(vector_POI_Jour_Actuel[id_POI_parcouru], vector_POI_Jour_Actuel[id_POI_parcouru + 1]);

			// Temps d'attente.
			if (heure_Actuelle < instance->get_POI_Heure_ouverture(vector_POI_Jour_Actuel[id_POI_parcouru + 1])) {
				heure_Actuelle = instance->get_POI_Heure_ouverture(vector_POI_Jour_Actuel[id_POI_parcouru + 1]);
			}
		}
	}
	return heure_Actuelle - date_Depart;
}

/* CALCUL_DISTANCE_TOTALE_POTENTIELLE_JOUR_N_POI : Permets d'obtenir la distance totale potentielle pour un jour avec n POIs.
 * Entrée : - num_Jour, int, numéro du jour actuel.
			- id_POI, int, id du POI que l'on veut calculer la date de départ.
 *			- distance_Hotel_POI_Courant, float, distance entre l'hôtel et le POI courant.
 * Nécessite : Rien.
 * Sortie : Distance totale potentielle du jour.
 * Entraîne : Rien.
*/
float Heuristiques::calcul_Distance_Totale_Potentielle_Jour_N_POI(int num_Jour, int id_POI_Suite, float date_Depart, float distance_Parcouru_Jour, float distance_POI_Actuel_POI_Courant) {

	float distance_POI_Courant_Hotel_Arrivee = 0;

	if (num_Jour == instance->get_Nombre_Jour() - 1) {
		distance_POI_Courant_Hotel_Arrivee = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), id_POI_Suite);
	}

	else {
		distance_POI_Courant_Hotel_Arrivee = instance->get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour], id_POI_Suite);
	}

	float tps_Attente = 0;

	// On calcul le temps d'attente avant ouverture à ce POI.
	if (date_Depart + distance_Parcouru_Jour + distance_POI_Actuel_POI_Courant < instance->get_POI_Heure_ouverture(id_POI_Suite)) {
		tps_Attente = instance->get_POI_Heure_ouverture(id_POI_Suite) - (date_Depart + distance_Parcouru_Jour + distance_POI_Actuel_POI_Courant);
	}

	return distance_Parcouru_Jour + distance_POI_Actuel_POI_Courant + tps_Attente + distance_POI_Courant_Hotel_Arrivee;
}

/* HEURISTIQUES_V1 : 1er version de notre heuristique marchant jusqu'à l'instance 16 mais ne compile plus dû au modification apportées aux méthodes pour le fonctionnement de la V2.
 * Entrée : Rien.
 * Nécessite : Rien.
 * Sortie : Rien.
 * Entraîne : Renvoie une solution réalisable pour l'instance ( ne marche pas pour les instances après 15 ).
*/
//Solution Heuristiques::heuristique_V1() {
	//int id_Hotel_Dep = instance->get_Id_Hotel_depart();
	//int id_Hotel_Arr = instance->get_Id_Hotel_Arrivee();

	//int nb_Jour = instance->get_Nombre_Jour();
	//int nb_POI = instance->get_Nombre_POI();
	//int nb_Hotel = instance->get_Nombre_Hotel();

	//Solution* solution = new Solution();

	//for (int id_Hotel = 0; id_Hotel < instance->get_Nombre_Hotel(); id_Hotel++) {
	//	cout << "Distance entre H" << id_Hotel << " et H-Arrivee : " << instance->get_distance_Hotel_Hotel(id_Hotel, instance->get_Id_Hotel_Arrivee()) << endl;
	//}

	//cout << endl;

	//for (int num_Jour = 0; num_Jour < nb_Jour; num_Jour++) {
	//	cout << "Num Jour : " << num_Jour << " | DMJ : " << instance->get_POI_Duree_Max_Voyage(num_Jour) << endl << endl << endl;

	//	float distance_Max_Jour_Actuel = instance->get_POI_Duree_Max_Voyage(num_Jour);

	//	vector<vector<int>> sequences_POI_Possible;
	//	vector<int> sequences_Hotel_Fin_Possible;
	//	vector<float> sequences_Date_Depart_Fin_Possible;
	//	vector<float> sequences_FO_Fin_Possible;

	//	// Vecteur qui va contenir tous les ID des POI du jour actuel.
	//	vector<int> vector_id_POI_Jour;

	//	// ID de l'hôtel de fin de la journée.
	//	int id_Hotel_Fin_Jour = -1;

	//	// On parcourt tous les POI pour voir si l'on peut commencer avec celui-ci.
	//	for (int id_1er_POI = 0; id_1er_POI < nb_POI; id_1er_POI++) {

	//		vector_id_POI_Jour.clear();

	//		// On regarde si le POI a déjà été visité.
	//		if (!is_POI_Deja_Visite(num_Jour, vector_id_POI_Jour, id_1er_POI, solution)) {

	//			// Distance entre le POI / Hôtel où l'on est et le POI courant que l'on lit.
	//			float distance_Hotel_Et_POI_Courant = calcul_Distance_Hotel_Et_POI_Courant(num_Jour, vector_id_POI_Jour, id_1er_POI, solution);

	//			// On regarde si la date de départ par défaut est possible.
	//			if (is_Date_Depart_OK(id_1er_POI, distance_Hotel_Et_POI_Courant, vector_id_POI_Jour)) {

	//				// On calcule la date de départ qui sera peut-être utile par la suite.
	//				float date_Depart = obtenir_Date_Depart(id_1er_POI, num_Jour, distance_Hotel_Et_POI_Courant, vector_id_POI_Jour, solution);

	//				cout << "ID POI : " << id_1er_POI << endl;

	//				int id_Hotel_Plus_Proche_POI_Courant = obtenir_Hotel(num_Jour, id_1er_POI, distance_Hotel_Et_POI_Courant);

	//				cout << "\tHotel : " << id_Hotel_Plus_Proche_POI_Courant << endl << endl;

	//				// On peut ajouter le POI si en le choisissant on peut aller à un hôtel par la suite.
	//				if (id_Hotel_Plus_Proche_POI_Courant != -1) {

	//					cout << "Prit" << endl << endl;

	//					id_Hotel_Fin_Jour = id_Hotel_Plus_Proche_POI_Courant;

	//					// On ajoute le POI.
	//					vector_id_POI_Jour.push_back(id_1er_POI);

	//					// On mets à jour la F.O.
	//					int score_Possible = instance->get_POI_Score(id_1er_POI);

	//					sequences_Date_Depart_Fin_Possible.push_back(date_Depart);

	//					//for (int id_POI_Suite = 0; id_POI_Suite < nb_POI; id_POI_Suite++) {

	//					//// On regarde si le POI a déjà été visité.
	//					//	if (!is_POI_Deja_Visite(num_Jour, vector_id_POI_Jour, id_POI_Suite, solution)) {

	//					//		// Distance entre le POI / Hôtel où l'on est et le POI courant que l'on lit.
	//					//		distance_POI_Actuel_Et_POI_Courant = calcul_Distance_POI_Actuel_Et_POI_Courant(num_Jour, vector_id_POI_Jour, id_POI_Suite, solution);

	//					//		// On calcul la distance parcouru actuellement du jour.
	//					//		distance_Parcouru_Jour = calcul_Distance_Parcouru_Jour(num_Jour, date_Depart, vector_id_POI_Jour, solution);

	//					//		// On regarde si la d'arrivé au POI n'est pas supérieure à la date de fermeture du POI.
	//					//		if (is_Date_Arrive_POI_OK(num_Jour, id_POI_Suite, distance_Parcouru_Jour, distance_POI_Actuel_Et_POI_Courant, date_Depart)) {

	//					//			tps_Attente = 0;

	//					//			// On calcul le temps d'attente avant ouverture à ce POI.
	//					//			if (date_Depart + distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant < instance->get_POI_Heure_ouverture(id_POI_Suite)) {
	//					//				tps_Attente = instance->get_POI_Heure_ouverture(id_POI_Suite) - (date_Depart + distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant);
	//					//			}

	//					//			id_Hotel_Plus_Proche_POI_Courant = obtenir_Hotel(num_Jour, vector_id_POI_Jour, id_POI_Suite, distance_POI_Actuel_Et_POI_Courant, tps_Attente, date_Depart, solution);

	//					//			// On peut ajouter le POI si en le choisissant on peut aller à un hôtel par la suite.
	//					//			if (id_Hotel_Plus_Proche_POI_Courant != -1) {

	//					//				id_Hotel_Fin_Jour = id_Hotel_Plus_Proche_POI_Courant;

	//					//				cout << "\tPOI : " << id_POI_Suite << endl,

	//					//					// On ajoute le POI.
	//					//					vector_id_POI_Jour.push_back(id_POI_Suite);

	//					//				// On mets à jour la F.O.
	//					//				score_Possible += instance->get_POI_Score(id_POI_Suite);
	//					//			}
	//					//		}
	//					//	}
	//					//}
	//					sequences_POI_Possible.push_back(vector_id_POI_Jour);
	//					sequences_Hotel_Fin_Possible.push_back(id_Hotel_Fin_Jour);
	//					sequences_FO_Fin_Possible.push_back(score_Possible);
	//				}
	//			}
	//		}
	//	}

	//	int indice_Hotel_Plus_Proche_Hotel_Arivee = obtenir_indice_Hotel_Plus_Proche_Hotel_Arrivee(sequences_Hotel_Fin_Possible);

	//	cout << "Indice hotel : " << indice_Hotel_Plus_Proche_Hotel_Arivee << endl;

	//	// On ajoute l'hôtel le plus proche sauf le dernier hôtel ( hôtel d'arrivé déjà connu ).
	//	if (num_Jour != nb_Jour - 1) {
	//		solution->v_Id_Hotel_Intermedaire.push_back(sequences_Hotel_Fin_Possible[indice_Hotel_Plus_Proche_Hotel_Arivee]);
	//	}

	//	solution->i_valeur_fonction_objectif += sequences_FO_Fin_Possible[indice_Hotel_Plus_Proche_Hotel_Arivee];
	//	solution->v_Date_Depart.push_back(sequences_Date_Depart_Fin_Possible[indice_Hotel_Plus_Proche_Hotel_Arivee]);

	//	// On ajoute le vector de POI.
	//	solution->v_v_Sequence_Id_Par_Jour.push_back(sequences_POI_Possible[indice_Hotel_Plus_Proche_Hotel_Arivee]);
	//}
	//return *solution;
// }


/* OBTENIR_HOTEL : Permets d'obtenir l'hôtel le plus de proche de l'hôtel d'arrivé atteignable par le POI.
	* Entrée : - num_Jour, int, numéro du jour actuel.
			- id_POI, int, id du POI que l'on veut calculer la date de départ.
	*			- distance_POI_Actuel_Et_POI_Courant, float, distance entre l'hôtel et le POI courant.
	* Nécessite : Rien.
	* Sortie : int, id de l'hôtel le plus de proche de l'hôtel d'arrivé atteignable par le POI.
	* Entraîne : Rien.
*/
//int Heuristiques::obtenir_Hotel(int num_Jour, int id_POI, float distance_Hotel_POI_Courant) {
//
//	float distance_Max_Jour_Actuel = instance->get_POI_Duree_Max_Voyage(num_Jour);
//
//	// Si c'est le dernier et que l'on doit obligatoirement rentrer à l'hôtel d'arrivé.
//	if (num_Jour == instance->get_Nombre_Jour() - 1) {
//		float distance_POI_Courant_Hotel_Arrivee = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), id_POI);
//		float distance_Totale_Jour = distance_Hotel_POI_Courant + distance_POI_Courant_Hotel_Arrivee;
//
//		//cout << "ID POI : " << id_POI << endl;
//		//cout << "\tDist. Prevu : " << distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel << " | Dist. Restante : " << (distance_Max_Jour_Actuel - distance_Parcouru_Jour) << endl;
//
//		if (distance_Totale_Jour <= distance_Max_Jour_Actuel) {
//			return instance->get_Id_Hotel_Arrivee();
//		}
//		else {
//			return -1;
//		}
//	}
//
//	// Si ce n'est pas le dernier jour.
//	else {
//		int id_Hotel_Selected = -1;
//
//		float distance_Hotel_Selected_Hotel_Arrive = 100000;
//
//		for (int id_Hotel = 0; id_Hotel < instance->get_Nombre_Hotel(); id_Hotel++) {
//
//			float distance_POI_Courant_Hotel_Courant = instance->get_distance_Hotel_POI(id_Hotel, id_POI);
//			float distance_Totale_Jour = distance_Hotel_POI_Courant + distance_POI_Courant_Hotel_Courant;
//
//			//cout << "ID H : " << id_Hotel << endl;
//			//cout << "DTJ : " << distance_Totale_Jour << " | DMJ : " << distance_Max_Jour_Actuel << " | Dist. avec Arrivee : " << instance->get_distance_Hotel_Hotel(id_Hotel, instance->get_Id_Hotel_Arrivee()) << endl;
//
//			if ((instance->get_distance_Hotel_Hotel(id_Hotel, instance->get_Id_Hotel_Arrivee()) < distance_Hotel_Selected_Hotel_Arrive) && (distance_Totale_Jour <= distance_Max_Jour_Actuel)) {
//				distance_Hotel_Selected_Hotel_Arrive = instance->get_distance_Hotel_Hotel(id_Hotel, instance->get_Id_Hotel_Arrivee());
//				id_Hotel_Selected = id_Hotel;
//			}
//		}
//		return id_Hotel_Selected;
//	}
//}

/* OBTENIR_INDICE_HOTEL_PLUS_PROCHE_HOTEL_ARRIVEE : Permets d'obtenir l'indice de l'hôtel le plus de proche de l'hôtel d'arrivé dans le vecteur des hôtels de fin possible.
	* Entrée : - sequences_Hotel_Fin_Possible, vector<int>, hôtel de fin possible.
	* Nécessite : Rien.
	* Sortie : int, indice de l'hôtel le plus de proche de l'hôtel d'arrivé dans le vecteur des hôtels de fin possible.
	* Entraîne : Rien.
*/
//int Heuristiques::obtenir_indice_Hotel_Plus_Proche_Hotel_Arrivee(vector<int> sequences_Hotel_Fin_Possible) {
//	int indice_Hotel = -1;
//	float distance_Min_Hotel_Hotel_Arrive = 1000000;
//
//	for (int indice = 0; indice < sequences_Hotel_Fin_Possible.size(); indice++) {
//
//		if (instance->get_distance_Hotel_Hotel(sequences_Hotel_Fin_Possible[indice], instance->get_Id_Hotel_Arrivee()) < distance_Min_Hotel_Hotel_Arrive) {
//			indice_Hotel = indice;
//			distance_Min_Hotel_Hotel_Arrive = instance->get_distance_Hotel_Hotel(sequences_Hotel_Fin_Possible[indice], instance->get_Id_Hotel_Arrivee());
//		}
//
//	}
//	return indice_Hotel;
//}