#include "Heuristiques.h"

// Constructeur par défaut.
Heuristiques::Heuristiques() {
	instance = new Instance();
}

// Constructeur de confort.
Heuristiques::Heuristiques(Instance* instance_Param) {
	instance = instance_Param;
}

Solution Heuristiques::methode_Heuristique() {
	int id_Hotel_Dep = instance->get_Id_Hotel_depart();
	int id_Hotel_Arr = instance->get_Id_Hotel_Arrivee();

	int nb_Jour = instance->get_Nombre_Jour();
	int nb_POI = instance->get_Nombre_POI();
	int nb_Hotel = instance->get_Nombre_Hotel();

	Solution* solution = new Solution();

	for (int num_Jour = 0; num_Jour < nb_Jour; num_Jour++) {
		cout << "Num Jour : " << num_Jour << endl << endl << endl;

		float distance_Max_Jour_Actuel = instance->get_POI_Duree_Max_Voyage(num_Jour);

		// Vecteur qui va contenir tous les ID des POI du jour actuel.
		vector<int> vector_id_POI_Jour;

		// ID de l'hôtel de fin de la journée.
		int id_Hotel_Fin_Jour = 0;

		// On parcourt tous les POI.
		for (int id_POI = 0; id_POI < nb_POI; id_POI++) {

			// Distance entre le POI / Hôtel où l'on est et le POI courant que l'on lit.
			float distance_POI_Actuel_Et_POI_Courant = calcul_Distance_POI_Actuel_Et_POI_Courant(num_Jour, vector_id_POI_Jour, id_POI, solution);

			float distance_POI_Courant_Et_Plus_Proche_Hotel = 0;
			int id_Hotel_Plus_Proche_POI_Courant = 0;

			// Distance entre le POI courant que l'on lit et son proche plus proche hôtel.
			calcul_Distance_POI_Courant_Et_Plus_Proche_Hotel_Et_Id_PPH(&distance_POI_Courant_Et_Plus_Proche_Hotel, &id_Hotel_Plus_Proche_POI_Courant, num_Jour, nb_Jour, nb_Hotel, id_POI);

			// On calcul la distance parcouru actuellement du jour.
			float distance_Parcouru_Jour = calcul_Distance_Parcouru_Jour(num_Jour, vector_id_POI_Jour, solution);

			float tps_Attente = 0;

			// On calcul le temps d'attente avant ouverture à ce POI.
			if (distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant < instance->get_POI_Heure_ouverture(id_POI)) {
				tps_Attente = instance->get_POI_Heure_ouverture(id_POI) - distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant;
			}

			// Distance entre le POI / Hôtel où l'on est et le POI courant que l'on lit et son proche plus proche hôtel.
			float distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel = distance_POI_Actuel_Et_POI_Courant + tps_Attente + distance_POI_Courant_Et_Plus_Proche_Hotel;

			// On regarde si le POI a déjà été visité.
			bool is_POI_Present = is_POI_Deja_Visite(num_Jour, vector_id_POI_Jour, id_POI, solution);

			// On regarde si la date de départ par défaut est possible.
			bool is_OK_Date_Depart = is_Date_Depart_OK(num_Jour, id_POI, id_Hotel_Plus_Proche_POI_Courant);

			// On regarde si la d'arrivé au POI n'est pas supérieure à la date de fermeture du POI.
			bool is_OK_Date_Arrive_POI = is_Date_Arrive_POI_OK(num_Jour, id_POI, distance_Parcouru_Jour, distance_POI_Actuel_Et_POI_Courant, solution);

			bool is_OK_distance_Avec_Hotel_Arrive_Diminue = is_Distance_Avec_Hotel_Arrive_Diminue(nb_Jour, num_Jour, vector_id_POI_Jour, id_POI, solution);

			cout << "ID POI : " << id_POI << " | Dt. POI Act - POI Cou - H : " << distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel << " | Dt. Rest : " << (distance_Max_Jour_Actuel - distance_Parcouru_Jour) << endl;
			cout << "\tPOI Present : " << is_POI_Present << " | Date Dep. OK : " << is_OK_Date_Depart << " Dist. Dim : " << is_OK_distance_Avec_Hotel_Arrive_Diminue << endl;
			cout << endl;

			// On peut ajouter le POI si en le choisissant on peut aller à un hôtel par la suite, qu'il n'a pas déjà été visité, que l'on peut l'atteindre avant sa fermeture et que la distance avec l'hotel d'arrivé diminue..
			if ((distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel <= distance_Max_Jour_Actuel - distance_Parcouru_Jour) && !is_POI_Present && is_OK_Date_Arrive_POI) {

				cout << endl;
				cout << " ------------ " << endl;
				cout << "On peut le prendre --> " << id_POI << endl;
				cout << " ------------ " << endl;
				cout << endl;

				if (vector_id_POI_Jour.size() == 0) {
					if (is_OK_Date_Depart) {
						// On ajoute le POI.
						vector_id_POI_Jour.push_back(id_POI);

						// On mets à jour la F.O.
						solution->i_valeur_fonction_objectif += instance->get_POI_Score(id_POI);

						// On mets à jour l'ID de l'hôtel le plus proche.
						id_Hotel_Fin_Jour = id_Hotel_Plus_Proche_POI_Courant;
					}
				}
				else {
					// On ajoute le POI.
					vector_id_POI_Jour.push_back(id_POI);

					// On mets à jour la F.O.
					solution->i_valeur_fonction_objectif += instance->get_POI_Score(id_POI);

					// On mets à jour l'ID de l'hôtel le plus proche.
					id_Hotel_Fin_Jour = id_Hotel_Plus_Proche_POI_Courant;
				}

				// On ajoute la date de départ lorsque que l'on ajoute le premier POI.
				if (vector_id_POI_Jour.size() == 1) {

					// ID du 1er POI.
					int id_1er_POI = vector_id_POI_Jour[0];

					// Heure de fermeture du 1er POI.
					float heure_Fermeture_1er_POI = instance->get_POI_Heure_fermeture(id_1er_POI);

					// Si c'est pas le 1er jour.
					if (num_Jour != 0) {
						solution->v_Date_Depart.push_back(heure_Fermeture_1er_POI - instance->get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], id_1er_POI));

					}

					// Si c'est le 1er jour.
					else {
						solution->v_Date_Depart.push_back(heure_Fermeture_1er_POI - instance->get_distance_Hotel_POI(instance->get_Id_Hotel_depart(), id_1er_POI));
					}
				}
			}
		}

		// On ajoute l'hôtel le plus proche sauf le dernier hôtel ( hôtel d'arrivé déjà connu ).
		if (num_Jour != nb_Jour - 1) {
			solution->v_Id_Hotel_Intermedaire.push_back(id_Hotel_Fin_Jour);
		}

		// On ajoute le vector de POI.
		solution->v_v_Sequence_Id_Par_Jour.push_back(vector_id_POI_Jour);
		vector_id_POI_Jour.clear();
	}
	return *solution;
}

float Heuristiques::calcul_Distance_POI_Actuel_Et_POI_Courant(int num_Jour, vector<int>  vector_id_POI_Jour, int id_POI, Solution* solution) {
	float distance_POI_Actuel_Et_POI_Courant = 0;

	// Si l'on est sur le l'hôtel de départ de la journée.
	if (vector_id_POI_Jour.size() == 0) {
		if (num_Jour != 0) {
			distance_POI_Actuel_Et_POI_Courant = instance->get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], id_POI);
		}
		else {
			distance_POI_Actuel_Et_POI_Courant = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_depart(), id_POI);
		}
	}
	else {
		distance_POI_Actuel_Et_POI_Courant = instance->get_distance_POI_POI(vector_id_POI_Jour[vector_id_POI_Jour.size() - 1], id_POI);
	}

	return distance_POI_Actuel_Et_POI_Courant;
}

void Heuristiques::calcul_Distance_POI_Courant_Et_Plus_Proche_Hotel_Et_Id_PPH(float* distance_POI_Courant_Et_Hotel, int* id_Hotel_Plus_Proche_POI_Courant, int num_Jour, int nb_Jour, int nb_Hotel, int id_POI) {

	*distance_POI_Courant_Et_Hotel = instance->get_distance_Hotel_POI(0, id_POI);
	float distance_Hotel_Hotel_Arrive = 10000;

	// Si c'est le dernier et que l'on doit obligatoirement rentrer à l'hôtel d'arrivé.
	if (num_Jour == nb_Jour - 1) {
		*distance_POI_Courant_Et_Hotel = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), id_POI);
		*id_Hotel_Plus_Proche_POI_Courant = instance->get_Id_Hotel_Arrivee();
	}
	else {

		// Calcul de distance_POI_Courant_Et_Plus_Proche_Hotel
		for (int id_Hotel = 0; id_Hotel < nb_Hotel; id_Hotel++) {

			float nouv_Distance_Hotel_Hotel_Arrive = instance->get_distance_Hotel_Hotel(id_Hotel, instance->get_Id_Hotel_Arrivee());
			
			//cout << "Nouv Dist.H - H : " << nouv_Distance_Hotel_Hotel_Arrive << "Dist.H - H : " << distance_Hotel_Hotel_Arrive << endl;
			//cout << "Dist. Max J : " << instance->get_POI_Duree_Max_Voyage(num_Jour) << endl;

			if (nouv_Distance_Hotel_Hotel_Arrive < distance_Hotel_Hotel_Arrive && nouv_Distance_Hotel_Hotel_Arrive < instance->get_POI_Duree_Max_Voyage(num_Jour) && id_Hotel != instance->get_Id_Hotel_Arrivee()) {
				*distance_POI_Courant_Et_Hotel = instance->get_distance_Hotel_POI(id_Hotel, id_POI);
				*id_Hotel_Plus_Proche_POI_Courant = id_Hotel;
				distance_Hotel_Hotel_Arrive = nouv_Distance_Hotel_Hotel_Arrive;
			}
		}
	}
}

float Heuristiques::calcul_Distance_Parcouru_Jour(int num_Jour, vector<int> vector_id_POI_Jour, Solution* solution) {
	float distance_Parcouru_Jour = 0;

	// Calcul distance parcouru dans la journée.
	if (vector_id_POI_Jour.size() != 0) {
		// Distance entre l'hôtel et le 1er POI.
		if (num_Jour != 0) {
			distance_Parcouru_Jour += instance->get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], vector_id_POI_Jour[0]);
		}
		else {
			distance_Parcouru_Jour += instance->get_distance_Hotel_POI(instance->get_Id_Hotel_depart(), vector_id_POI_Jour[0]);
		}

		// Temps d'attente.
		if (distance_Parcouru_Jour < instance->get_POI_Heure_ouverture(vector_id_POI_Jour[0])) {
			distance_Parcouru_Jour = instance->get_POI_Heure_ouverture(vector_id_POI_Jour[0]);
		}

		// Distance entre tout les POI.
		for (int id_POI_parcouru = 0; id_POI_parcouru < vector_id_POI_Jour.size() - 1; id_POI_parcouru++) {
			distance_Parcouru_Jour = distance_Parcouru_Jour + instance->get_distance_POI_POI(vector_id_POI_Jour[id_POI_parcouru], vector_id_POI_Jour[id_POI_parcouru + 1]);

			// Temps d'attente.
			if (distance_Parcouru_Jour < instance->get_POI_Heure_ouverture(vector_id_POI_Jour[id_POI_parcouru + 1])) {
				distance_Parcouru_Jour = instance->get_POI_Heure_ouverture(vector_id_POI_Jour[id_POI_parcouru + 1]);
			}
		}
	}
	return distance_Parcouru_Jour;
}

bool Heuristiques::is_POI_Deja_Visite(int num_Jour, vector<int> vector_id_POI_Jour, int id_POI, Solution* solution) {

	// On regarde si le POI a déjà été visité aujourd'hui.
	for (int indice_POI_Jour_Act = 0; indice_POI_Jour_Act < vector_id_POI_Jour.size(); indice_POI_Jour_Act++) {
		if (vector_id_POI_Jour[indice_POI_Jour_Act] == id_POI) {
			return true;
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

bool Heuristiques::is_Date_Depart_OK(int num_Jour, int id_POI, int id_Hotel_Plus_Proche_POI_Courant) {
	if (num_Jour != 0) {
		if (instance->get_POI_Heure_fermeture(id_POI) - instance->get_distance_Hotel_POI(id_Hotel_Plus_Proche_POI_Courant, id_POI) < 0) {
			return false;
		}
	}
	else {
		if (instance->get_POI_Heure_fermeture(id_POI) - instance->get_distance_Hotel_POI(instance->get_Id_Hotel_depart(), id_POI) < 0) {
			return false;
		}
	}
	return true;
}

bool Heuristiques::is_Date_Arrive_POI_OK(int num_Jour, int id_POI, float distance_Parcouru_Jour, float distance_POI_Actuel_Et_POI_Courant, Solution* solution) {
	//cout << "ID POI : " << id_POI << " | D. Par. J : " << distance_Parcouru_Jour << " | D. POI Act - POI Cour : " << distance_POI_Actuel_Et_POI_Courant << " | Hor. O : " << instance->get_POI_Heure_ouverture(id_POI) << " | Hor. F : " << instance->get_POI_Heure_fermeture(id_POI) << endl;

	if (solution->v_Date_Depart.size() - num_Jour == 0) {
		if (distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant > instance->get_POI_Heure_fermeture(id_POI)) {
			return false;
		}
	}

	else if (solution->v_Date_Depart[num_Jour] + distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant > instance->get_POI_Heure_fermeture(id_POI)) {
		return false;
	}
	return true;
}

bool Heuristiques::is_Distance_Avec_Hotel_Arrive_Diminue(int nb_Jour, int num_Jour, vector<int> vector_id_POI_Jour, int id_POI, Solution* solution) {
	float distance_POI_Courant_Hotel_Arrive = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), id_POI);
	float distance_Max_Jours = 0;
	float distance_POI_Act_Hotel_Arrive = 0;

	//if (vector_id_POI_Jour.size() == 0) {
	//	if (num_Jour == 0) {
	//		distance_POI_Act_Hotel_Arrive = instance->get_distance_Hotel_Hotel(instance->get_Id_Hotel_depart(), instance->get_Id_Hotel_Arrivee());
	//	}
	//	else {
	//		distance_POI_Act_Hotel_Arrive = instance->get_distance_Hotel_Hotel(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], instance->get_Id_Hotel_Arrivee());
	//	}
	//}
	//else {
	//	distance_POI_Act_Hotel_Arrive = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), vector_id_POI_Jour[vector_id_POI_Jour.size() - 1]);
	//}

	//cout << "Distance Act-H.Arr : " << distance_POI_Act_Hotel_Arrive << endl;
	//cout << "Distance Cour-H.Arr : " << distance_POI_Courant_Hotel_Arrive << endl;

	//if (num_Jour != nb_Jour - 1) {
	//	if (distance_POI_Act_Hotel_Arrive - distance_POI_Courant_Hotel_Arrive > 0) {
	//		return true;
	//	}
	//	else if (distance_POI_Act_Hotel_Arrive - distance_POI_Courant_Hotel_Arrive > -2){
	//		return false;
	//	}
	//}
	//else {
	//	return true;
	//}
	return true;
}