#include "Heuristiques.h"

// Constructeur par défaut.
Heuristiques::Heuristiques() {
	instance = new Instance();
	solution = new Solution();
}

// Constructeur de confort.
Heuristiques::Heuristiques(Instance* instance_Param) {
	instance = instance_Param;
	solution = new Solution();
}

Solution Heuristiques::heuristique_V2() {
	id_Hotel_Dep = instance->get_Id_Hotel_depart();
	id_Hotel_Arr = instance->get_Id_Hotel_Arrivee();

	nb_Jour = instance->get_Nombre_Jour();
	nb_POI = instance->get_Nombre_POI();
	nb_Hotel = instance->get_Nombre_Hotel();

	do {
		solution->i_valeur_fonction_objectif = 0;
		solution->v_Date_Depart.clear();
		solution->v_Id_Hotel_Intermedaire.clear();
		solution->v_v_Sequence_Id_Par_Jour.clear();

		obtenir_Hotels(solution);

		for (int num_Jour = 0; num_Jour < nb_Jour; num_Jour++) {

			float distance_Max_Jour_Actuel = instance->get_POI_Duree_Max_Voyage(num_Jour);
			vector_POI_Jour_Actuel.clear();

			for (int id_1er_POI = 0; id_1er_POI < nb_POI; id_1er_POI++) {

				// On regarde si le POI a déjà été visité.
				if (!is_POI_Deja_Visite(num_Jour, id_1er_POI)) {

					// Distance entre le POI / Hôtel où l'on est et le POI courant que l'on lit.
					float distance_Hotel_POI_Courant = calcul_Distance_Hotel_Et_POI_Courant(num_Jour, id_1er_POI);

					// On regarde si la date de départ par défaut est possible.
					if (is_Date_Depart_OK(id_1er_POI, distance_Hotel_POI_Courant)) {

						float date_Depart = obtenir_Date_Depart(id_1er_POI, num_Jour, distance_Hotel_POI_Courant);

						if(is_Date_Arrive_POI_OK(num_Jour, id_1er_POI, 0, distance_Hotel_POI_Courant, date_Depart)) {
							float distance_POI_Courant_Hotel_Arrivee = 0;

							if (num_Jour == instance->get_Nombre_Jour() - 1) {
								distance_POI_Courant_Hotel_Arrivee = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), id_1er_POI);
							}

							else {
								distance_POI_Courant_Hotel_Arrivee = instance->get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour], id_1er_POI);
							}

							float distance_Totale_Jour = distance_Hotel_POI_Courant + distance_POI_Courant_Hotel_Arrivee;

							if (distance_Totale_Jour <= distance_Max_Jour_Actuel) {

								solution->i_valeur_fonction_objectif += instance->get_POI_Score(id_1er_POI);
								solution->v_Date_Depart.push_back(date_Depart);
								vector_POI_Jour_Actuel.push_back(id_1er_POI);
								break;
							}
						}
					}
				}
			}
			solution->v_v_Sequence_Id_Par_Jour.push_back(vector_POI_Jour_Actuel);
		}
	} while (solution->v_v_Sequence_Id_Par_Jour[nb_Jour - 1].size() == 0);
	return *solution;
}

void Heuristiques::obtenir_Hotels(Solution* solution) {

	vector<int> hotels;

	random_device rd;
	mt19937 gen(rd());

	do {
		hotels.clear();
		for (int num_Jour = 0; num_Jour < nb_Jour - 1; num_Jour++) {

			vector<int> id_Hotel_Potentiel;
			int id_Hotel_Choisi = -1;

			for (int id_Hotel = 0; id_Hotel < nb_Hotel; id_Hotel++) {

				if (num_Jour == 0) {

					if (instance->get_distance_Hotel_Hotel(id_Hotel_Dep, id_Hotel) < instance->get_POI_Duree_Max_Voyage(num_Jour)) {
						id_Hotel_Potentiel.push_back(id_Hotel);
					}
				}

				else {
					if (instance->get_distance_Hotel_Hotel(hotels[num_Jour - 1], id_Hotel) < instance->get_POI_Duree_Max_Voyage(num_Jour)) {
						id_Hotel_Potentiel.push_back(id_Hotel);
					}
				}
			}

			uniform_int_distribution<> dis_Hotel(0, id_Hotel_Potentiel.size() - 1);

			id_Hotel_Choisi = id_Hotel_Potentiel[dis_Hotel(gen)];
			hotels.push_back(id_Hotel_Choisi);
		}
	} while (instance->get_distance_Hotel_Hotel(hotels[nb_Jour - 2], id_Hotel_Arr) > instance->get_POI_Duree_Max_Voyage(nb_Jour - 2));

	for (int num_Jour = 0; num_Jour < nb_Jour - 1; num_Jour++) {
		solution->v_Id_Hotel_Intermedaire.push_back(hotels[num_Jour]);
	}
}

Solution Heuristiques::heuristique_V1() {
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
	return *solution;
}

int Heuristiques::obtenir_Hotel(int num_Jour, int id_POI, float distance_Hotel_POI_Courant) {

	float distance_Max_Jour_Actuel = instance->get_POI_Duree_Max_Voyage(num_Jour);

	// Si c'est le dernier et que l'on doit obligatoirement rentrer à l'hôtel d'arrivé.
	if (num_Jour == instance->get_Nombre_Jour() - 1) {
		float distance_POI_Courant_Hotel_Arrivee = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), id_POI);
		float distance_Totale_Jour = distance_Hotel_POI_Courant + distance_POI_Courant_Hotel_Arrivee;

		//cout << "ID POI : " << id_POI << endl;
		//cout << "\tDist. Prevu : " << distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel << " | Dist. Restante : " << (distance_Max_Jour_Actuel - distance_Parcouru_Jour) << endl;

		if (distance_Totale_Jour <= distance_Max_Jour_Actuel) {
			return instance->get_Id_Hotel_Arrivee();
		}
		else {
			return -1;
		}
	}

	// Si ce n'est pas le dernier jour.
	else {
		int id_Hotel_Selected = -1;

		float distance_Hotel_Selected_Hotel_Arrive = 100000;

		for (int id_Hotel = 0; id_Hotel < instance->get_Nombre_Hotel(); id_Hotel++) {

			float distance_POI_Courant_Hotel_Courant = instance->get_distance_Hotel_POI(id_Hotel, id_POI);
			float distance_Totale_Jour = distance_Hotel_POI_Courant + distance_POI_Courant_Hotel_Courant;

			//cout << "ID H : " << id_Hotel << endl;
			//cout << "DTJ : " << distance_Totale_Jour << " | DMJ : " << distance_Max_Jour_Actuel << " | Dist. avec Arrivee : " << instance->get_distance_Hotel_Hotel(id_Hotel, instance->get_Id_Hotel_Arrivee()) << endl;

			if ((instance->get_distance_Hotel_Hotel(id_Hotel, instance->get_Id_Hotel_Arrivee()) < distance_Hotel_Selected_Hotel_Arrive) && (distance_Totale_Jour <= distance_Max_Jour_Actuel)) {
				distance_Hotel_Selected_Hotel_Arrive = instance->get_distance_Hotel_Hotel(id_Hotel, instance->get_Id_Hotel_Arrivee());
				id_Hotel_Selected = id_Hotel;
			}
		}
		return id_Hotel_Selected;
	}
}

int Heuristiques::obtenir_indice_Hotel_Plus_Proche_Hotel_Arrivee(vector<int> sequences_Hotel_Fin_Possible) {
	int indice_Hotel = -1;
	float distance_Min_Hotel_Hotel_Arrive = 1000000;

	for (int indice = 0; indice < sequences_Hotel_Fin_Possible.size(); indice++) {

		if (instance->get_distance_Hotel_Hotel(sequences_Hotel_Fin_Possible[indice], instance->get_Id_Hotel_Arrivee()) < distance_Min_Hotel_Hotel_Arrive) {
			indice_Hotel = indice;
			distance_Min_Hotel_Hotel_Arrive = instance->get_distance_Hotel_Hotel(sequences_Hotel_Fin_Possible[indice], instance->get_Id_Hotel_Arrivee());
		}

	}
	return indice_Hotel;
}

float Heuristiques::calcul_Distance_Hotel_Et_POI_Courant(int num_Jour, int id_POI) {

	// Si l'on est sur le l'hôtel de départ de la journée.
	if (vector_POI_Jour_Actuel.size() == 0) {
		if (num_Jour != 0) {
			return instance->get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], id_POI);
		}
		else {
			return instance->get_distance_Hotel_POI(instance->get_Id_Hotel_depart(), id_POI);
		}
	}
	//else {
	//	return instance->get_distance_POI_POI(vector_id_POI_Jour[vector_id_POI_Jour.size() - 1], id_POI);
	//}
}

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

bool Heuristiques::is_Date_Depart_OK(int id_POI, float distance_POI_Actuel_Et_POI_Courant) {

	// On regarde si c'est le 1er POI de la journée.
	if (vector_POI_Jour_Actuel.size() == 0) {

		// On regarde si l'on peut arriver à l'ouverture.
		if (instance->get_POI_Heure_ouverture(id_POI) - distance_POI_Actuel_Et_POI_Courant < 0) {
			// On regarde si l'on peut arriver juste avant la fermeture.
			if (instance->get_POI_Heure_fermeture(id_POI) - distance_POI_Actuel_Et_POI_Courant < 0) {
				return false;
			}
		}
	}
	else {
		return true;
	}
}

float Heuristiques::obtenir_Date_Depart(int id_POI, int num_Jour, float distance_POI_Actuel_Et_POI_Courant) {

	if (vector_POI_Jour_Actuel.size() == 0) {
		float heure_Ouverture = instance->get_POI_Heure_ouverture(id_POI);
		float heure_Fermeture = instance->get_POI_Heure_fermeture(id_POI);

		if (heure_Ouverture - distance_POI_Actuel_Et_POI_Courant >= 0) {
			return heure_Ouverture - distance_POI_Actuel_Et_POI_Courant;
		}
		else {
			return heure_Fermeture - distance_POI_Actuel_Et_POI_Courant;
		}
	}
	else {
		return solution->v_Date_Depart[num_Jour];
	}
}

bool Heuristiques::is_Date_Arrive_POI_OK(int num_Jour, int id_POI, float distance_Parcouru_Jour, float distance_POI_Actuel_Et_POI_Courant, float date_Depart) {

	if (date_Depart + distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant > instance->get_POI_Heure_fermeture(id_POI)) {
		return false;
	}
	return true;
}