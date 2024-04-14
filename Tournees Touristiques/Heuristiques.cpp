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
	cout << "H.Dep : " << instance->get_Id_Hotel_depart() << endl;
	//for (int hotel = 0; hotel < instance->get_Nombre_Hotel(); hotel++) {
	//	cout << "Distance entre hotel " << instance->get_Id_Hotel_Arrivee() << " et hotel " << hotel << " : " << instance->get_distance_Hotel_Hotel(hotel, instance->get_Id_Hotel_Arrivee()) << endl;
	//}

	for (int num_Jour = 0; num_Jour < nb_Jour; num_Jour++) {
		cout << "Num Jour : " << num_Jour << " | DMJ : " << instance->get_POI_Duree_Max_Voyage(num_Jour) << endl << endl << endl;

		float distance_Max_Jour_Actuel = instance->get_POI_Duree_Max_Voyage(num_Jour);

		// Vecteur qui va contenir tous les ID des POI du jour actuel.
		vector<int> vector_id_POI_Jour;

		// ID de l'hôtel de fin de la journée.
		int id_Hotel_Fin_Jour = -1;

		// On parcourt tous les POI.
		for (int id_POI = 0; id_POI < nb_POI; id_POI++) {

			// On regarde si le POI a déjà été visité.
			if (!is_POI_Deja_Visite(num_Jour, vector_id_POI_Jour, id_POI, solution)) {

				// Distance entre le POI / Hôtel où l'on est et le POI courant que l'on lit.
				float distance_POI_Actuel_Et_POI_Courant = calcul_Distance_POI_Actuel_Et_POI_Courant(num_Jour, vector_id_POI_Jour, id_POI, solution);

				// On regarde si la date de départ par défaut est possible.
				if (is_Date_Depart_OK(id_POI, distance_POI_Actuel_Et_POI_Courant, vector_id_POI_Jour)) {

					// On calcule la date de départ qui sera peut-être utile par la suite.
					float date_Depart = obtenir_Date_Depart(id_POI, num_Jour, distance_POI_Actuel_Et_POI_Courant, vector_id_POI_Jour, solution);


					// On calcul la distance parcouru actuellement du jour.
					float distance_Parcouru_Jour = calcul_Distance_Parcouru_Jour(num_Jour, date_Depart, vector_id_POI_Jour, solution);

					// On regarde si la d'arrivé au POI n'est pas supérieure à la date de fermeture du POI.
					if (is_Date_Arrive_POI_OK(num_Jour, id_POI, distance_Parcouru_Jour, distance_POI_Actuel_Et_POI_Courant, date_Depart)) {

						float tps_Attente = 0;

						// On calcul le temps d'attente avant ouverture à ce POI.
						if (date_Depart + distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant < instance->get_POI_Heure_ouverture(id_POI)) {
							tps_Attente = instance->get_POI_Heure_ouverture(id_POI) - (date_Depart + distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant);
						}

						int id_Hotel_Plus_Proche_POI_Courant = obtenir_Hotel(num_Jour, vector_id_POI_Jour, id_POI, distance_POI_Actuel_Et_POI_Courant, tps_Attente, date_Depart, solution);

						cout << "ID POI : " << id_POI << endl;
						cout << "\tID Hotel : " << id_Hotel_Plus_Proche_POI_Courant << endl;

						if (id_Hotel_Plus_Proche_POI_Courant != -1) {
							if (num_Jour == 0) {
								cout << "\tDist. H.act-H.arr : " << instance->get_distance_Hotel_Hotel(instance->get_Id_Hotel_depart(), instance->get_Id_Hotel_Arrivee()) << endl;
								cout << "\tDist. H.cour-H.arr : " << instance->get_distance_Hotel_Hotel(id_Hotel_Plus_Proche_POI_Courant, instance->get_Id_Hotel_Arrivee()) << endl;
							}
							else {
								cout << "\tDist. H.act-H.arr : " << instance->get_distance_Hotel_Hotel(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], instance->get_Id_Hotel_Arrivee()) << endl;
								cout << "\tDist. H.cour-H.arr : " << instance->get_distance_Hotel_Hotel(id_Hotel_Plus_Proche_POI_Courant, instance->get_Id_Hotel_Arrivee()) << endl;
							}
						}

						//cout << "\tDis. Parcour Act : " << distance_Parcouru_Jour << " | Dt.POI Act - POI Cou - H : " << distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel << " | Dt.Rest : " << (distance_Max_Jour_Actuel - distance_Parcouru_Jour) << endl;
						//cout << "\tHor-Ouv : " << instance->get_POI_Heure_ouverture(id_POI) << " | Hor-Ferm : " << instance->get_POI_Heure_fermeture(id_POI) << endl;
						//cout << "\tDist. Act-Cour : " << distance_POI_Actuel_Et_POI_Courant << " | Tps att : " << tps_Attente << " | Dist. Cour-H : " << distance_POI_Courant_Et_Plus_Proche_Hotel << endl;

						cout << endl;

						// On peut ajouter le POI si en le choisissant on peut aller à un hôtel par la suite, qu'il n'a pas déjà été visité, que l'on peut l'atteindre avant sa fermeture et que la distance avec l'hotel d'arrivé diminue..
						if (id_Hotel_Plus_Proche_POI_Courant != -1) {

							cout << endl;
							cout << " ------------ " << endl;
							cout << "On peut le prendre --> " << id_POI << endl;
							cout << " ------------ " << endl;
							cout << endl;

							cout << "Date Depart : " << date_Depart << endl;
							cout << "Dist. Parcouru Jour : " << distance_Parcouru_Jour << endl;
							cout << "Distance : Act-Cour : " << distance_POI_Actuel_Et_POI_Courant << endl;
							cout << "Temps d'attente : " << tps_Attente << endl;
							cout << "Heure Ouverture : " << instance->get_POI_Heure_ouverture(id_POI) << " | Heure Fermeture : " << instance->get_POI_Heure_fermeture(id_POI) << endl;
							cout << endl;

							// On ajoute le POI.
							vector_id_POI_Jour.push_back(id_POI);

							// On mets à jour la F.O.
							solution->i_valeur_fonction_objectif += instance->get_POI_Score(id_POI);

							// On mets à jour l'ID de l'hôtel le plus proche.
							id_Hotel_Fin_Jour = id_Hotel_Plus_Proche_POI_Courant;

							// On ajoute la date de départ lorsque que l'on ajoute le premier POI.
							if (vector_id_POI_Jour.size() == 1) {
								solution->v_Date_Depart.push_back(date_Depart);
							}
						}
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

	// Si l'on est sur le l'hôtel de départ de la journée.
	if (vector_id_POI_Jour.size() == 0) {
		if (num_Jour != 0) {
			return instance->get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], id_POI);
		}
		else {
			return instance->get_distance_Hotel_POI(instance->get_Id_Hotel_depart(), id_POI);
		}
	}
	else {
		return instance->get_distance_POI_POI(vector_id_POI_Jour[vector_id_POI_Jour.size() - 1], id_POI);
	}
}

int Heuristiques::obtenir_Hotel(int num_Jour, vector<int>  vector_id_POI_Jour, int id_POI, float distance_POI_Actuel_Et_POI_Courant, float tps_Attente, float date_Depart, Solution* solution) {

	// Si c'est le dernier et que l'on doit obligatoirement rentrer à l'hôtel d'arrivé.
	if (num_Jour == instance->get_Nombre_Jour() - 1) {
		float distance_POI_Courant_Hotel = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), id_POI);
		float distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel = distance_POI_Actuel_Et_POI_Courant + tps_Attente + distance_POI_Courant_Hotel;
		float distance_Parcouru_Jour = calcul_Distance_Parcouru_Jour(num_Jour, date_Depart, vector_id_POI_Jour, solution);
		float distance_Max_Jour_Actuel = instance->get_POI_Duree_Max_Voyage(num_Jour);

		cout << "ID POI : " << id_POI << endl;
		cout << "\tDist. Prevu : " << distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel << " | Dist. Restante : " << (distance_Max_Jour_Actuel - distance_Parcouru_Jour) << endl;

		if (distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel + distance_Parcouru_Jour <= distance_Max_Jour_Actuel) {
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

		float distance_Max_Jour_Actuel = instance->get_POI_Duree_Max_Voyage(num_Jour);
		float distance_Parcouru_Jour = calcul_Distance_Parcouru_Jour(num_Jour, date_Depart, vector_id_POI_Jour, solution);

		for (int id_Hotel = 0; id_Hotel < instance->get_Nombre_Hotel(); id_Hotel++) {

			float distance_POI_Courant_Hotel = instance->get_distance_Hotel_POI(id_Hotel, id_POI);
			float distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel = distance_POI_Actuel_Et_POI_Courant + tps_Attente + distance_POI_Courant_Hotel;

			cout << "ID POI : " << id_POI << " | ID Hotel : " << id_Hotel << endl;
			cout << "\tDist. H.Parcouru - Hotel.Arrivee : " << instance->get_distance_Hotel_Hotel(id_Hotel, instance->get_Id_Hotel_Arrivee()) << " | Dist. H.Selected - H.Arrivee : " << distance_Hotel_Selected_Hotel_Arrive << endl;
			cout << "\tDist. Prevu : " << distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel << " | Dist. Restante : " << (distance_Max_Jour_Actuel - distance_Parcouru_Jour) << endl;

			if ( ( instance->get_distance_Hotel_Hotel(id_Hotel, instance->get_Id_Hotel_Arrivee()) < distance_Hotel_Selected_Hotel_Arrive ) && ( distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel + distance_Parcouru_Jour <= distance_Max_Jour_Actuel ) ) {
				distance_Hotel_Selected_Hotel_Arrive = instance->get_distance_Hotel_Hotel(id_Hotel, instance->get_Id_Hotel_Arrivee());
				id_Hotel_Selected = id_Hotel;
			}
		}
		return id_Hotel_Selected;
	}
}

float Heuristiques::calcul_Distance_Parcouru_Jour(int num_Jour, float date_Depart, vector<int> vector_id_POI_Jour, Solution* solution) {
	float heure_Actuelle = date_Depart;

	// Calcul distance parcouru dans la journée.
	if (vector_id_POI_Jour.size() != 0) {

		// Distance entre l'hôtel et le 1er POI.
		if (num_Jour != 0) {
			heure_Actuelle += instance->get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], vector_id_POI_Jour[0]);
		}
		else {
			heure_Actuelle += instance->get_distance_Hotel_POI(instance->get_Id_Hotel_depart(), vector_id_POI_Jour[0]);
		}

		// Distance entre tout les POI.
		for (int id_POI_parcouru = 0; id_POI_parcouru < vector_id_POI_Jour.size() - 1; id_POI_parcouru++) {
			heure_Actuelle += instance->get_distance_POI_POI(vector_id_POI_Jour[id_POI_parcouru], vector_id_POI_Jour[id_POI_parcouru + 1]);

			// Temps d'attente.
			if (heure_Actuelle < instance->get_POI_Heure_ouverture(vector_id_POI_Jour[id_POI_parcouru + 1])) {
				heure_Actuelle = instance->get_POI_Heure_ouverture(vector_id_POI_Jour[id_POI_parcouru + 1]);
			}
		}
	}
	return heure_Actuelle - date_Depart;
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

bool Heuristiques::is_Date_Depart_OK(int id_POI, float distance_POI_Actuel_Et_POI_Courant, vector<int> vector_id_POI_Jour) {

	// On regarde si c'est le 1er POI de la journée.
	if (vector_id_POI_Jour.size() == 0) {

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

float Heuristiques::obtenir_Date_Depart(int id_POI, int num_Jour, float distance_POI_Actuel_Et_POI_Courant, vector<int> vector_id_POI_Jour, Solution* solution) {

	if (vector_id_POI_Jour.size() == 0) {
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
	//cout << "ID POI : " << id_POI << " | D. Par. J : " << distance_Parcouru_Jour << " | D. POI Act - POI Cour : " << distance_POI_Actuel_Et_POI_Courant << " | Hor. O : " << instance->get_POI_Heure_ouverture(id_POI) << " | Hor. F : " << instance->get_POI_Heure_fermeture(id_POI) << endl;

	if (date_Depart + distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant > instance->get_POI_Heure_fermeture(id_POI)) {
		return false;
	}
	return true;
}