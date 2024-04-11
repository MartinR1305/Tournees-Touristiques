#include "Heuristiques.h"

// Constructeur par d�faut.
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

		// ID de l'h�tel de fin de la journ�e.
		int id_Hotel_Fin_Jour = -1;

		// On parcourt tous les POI.
		for (int id_POI = 0; id_POI < nb_POI; id_POI++) {

			// On regarde si le POI a d�j� �t� visit�.
			if (!is_POI_Deja_Visite(num_Jour, vector_id_POI_Jour, id_POI, solution)) {

				// Distance entre le POI / H�tel o� l'on est et le POI courant que l'on lit.
				float distance_POI_Actuel_Et_POI_Courant = calcul_Distance_POI_Actuel_Et_POI_Courant(num_Jour, vector_id_POI_Jour, id_POI, solution);

				// On regarde si la date de d�part par d�faut est possible.
				if (is_Date_Depart_OK(id_POI, distance_POI_Actuel_Et_POI_Courant, vector_id_POI_Jour)) {

					// On calcule la date de d�part qui sera peut-�tre utile par la suite.
					float date_Depart = obtenir_Date_Depart(id_POI, num_Jour, distance_POI_Actuel_Et_POI_Courant, vector_id_POI_Jour, solution);

					// On calcul la distance parcouru actuellement du jour.
					float distance_Parcouru_Jour = calcul_Distance_Parcouru_Jour(num_Jour, vector_id_POI_Jour, solution);

					// On regarde si la d'arriv� au POI n'est pas sup�rieure � la date de fermeture du POI.
					if (is_Date_Arrive_POI_OK(num_Jour, id_POI, distance_Parcouru_Jour, distance_POI_Actuel_Et_POI_Courant, date_Depart)) {

						float tps_Attente = 0;

						// On calcul le temps d'attente avant ouverture � ce POI.
						if (distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant < instance->get_POI_Heure_ouverture(id_POI)) {
							tps_Attente = instance->get_POI_Heure_ouverture(id_POI) - (distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant);
						}

						float distance_POI_Courant_Et_Plus_Proche_Hotel = 0;
						int id_Hotel_Plus_Proche_POI_Courant = -1;

						// Distance entre le POI courant que l'on lit et son proche plus proche h�tel.
						calcul_Distance_POI_Courant_Et_Plus_Proche_Hotel_Et_Id_PPH(&distance_POI_Courant_Et_Plus_Proche_Hotel, &id_Hotel_Plus_Proche_POI_Courant, num_Jour, nb_Jour, nb_Hotel, id_POI, solution);

						// Distance entre le POI / H�tel o� l'on est et le POI courant que l'on lit et son proche plus proche h�tel.
						float distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel = distance_POI_Actuel_Et_POI_Courant + tps_Attente + distance_POI_Courant_Et_Plus_Proche_Hotel;

						cout << "ID POI : " << id_POI << endl;
						cout << "\tDis. Parcour Act : " << distance_Parcouru_Jour << " | Dt.POI Act - POI Cou - H : " << distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel << " | Dt.Rest : " << (distance_Max_Jour_Actuel - distance_Parcouru_Jour) << endl;
						cout << "\tHor-Ouv : " << instance->get_POI_Heure_ouverture(id_POI) << " | Hor-Ferm : " << instance->get_POI_Heure_fermeture(id_POI) << endl;
						cout << "\tDist. Act-Cour : " << distance_POI_Actuel_Et_POI_Courant << " | Tps att : " << tps_Attente << " | Dist. Cour-H : " << distance_POI_Courant_Et_Plus_Proche_Hotel << endl;
						if (num_Jour == 0) {
							cout << "\tDist. H.act-H.cour : " << instance->get_distance_Hotel_Hotel(instance->get_Id_Hotel_depart(), id_Hotel_Plus_Proche_POI_Courant);
							cout << "\tDist. H.act-H.arr : " << instance->get_distance_Hotel_Hotel(instance->get_Id_Hotel_depart(), instance->get_Id_Hotel_Arrivee());
						}
						else {
							cout << "\tDDist. H.act-H.cour : " << instance->get_distance_Hotel_Hotel(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], id_Hotel_Plus_Proche_POI_Courant);
							cout << "\tDist. H.act-H.arr : " << instance->get_distance_Hotel_Hotel(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], instance->get_Id_Hotel_Arrivee());
						}
						cout << endl;

						// On peut ajouter le POI si en le choisissant on peut aller � un h�tel par la suite, qu'il n'a pas d�j� �t� visit�, que l'on peut l'atteindre avant sa fermeture et que la distance avec l'hotel d'arriv� diminue..
						if ((distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel <= distance_Max_Jour_Actuel - distance_Parcouru_Jour)) {

							cout << endl;
							cout << " ------------ " << endl;
							cout << "On peut le prendre --> " << id_POI << endl;
							cout << " ------------ " << endl;
							cout << endl;

							// On ajoute le POI.
							vector_id_POI_Jour.push_back(id_POI);

							// On mets � jour la F.O.
							solution->i_valeur_fonction_objectif += instance->get_POI_Score(id_POI);

							// On mets � jour l'ID de l'h�tel le plus proche.
							id_Hotel_Fin_Jour = id_Hotel_Plus_Proche_POI_Courant;

							// On ajoute la date de d�part lorsque que l'on ajoute le premier POI.
							if (vector_id_POI_Jour.size() == 1) {
								solution->v_Date_Depart.push_back(date_Depart);
							}
						}
					}
				}
			}
		}

		// On ajoute l'h�tel le plus proche sauf le dernier h�tel ( h�tel d'arriv� d�j� connu ).
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

	// Si l'on est sur le l'h�tel de d�part de la journ�e.
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

void Heuristiques::calcul_Distance_POI_Courant_Et_Plus_Proche_Hotel_Et_Id_PPH(float* distance_POI_Courant_Et_Hotel, int* id_Hotel_Plus_Proche_POI_Courant, int num_Jour, int nb_Jour, int nb_Hotel, int id_POI, Solution* solution) {

	*distance_POI_Courant_Et_Hotel = instance->get_distance_Hotel_POI(0, id_POI);

	// Si c'est le dernier et que l'on doit obligatoirement rentrer � l'h�tel d'arriv�.
	if (num_Jour == nb_Jour - 1) {
		*distance_POI_Courant_Et_Hotel = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), id_POI);
		*id_Hotel_Plus_Proche_POI_Courant = instance->get_Id_Hotel_Arrivee();
	}
	else {
		int id_Hotel_Prec = 0;

		if (num_Jour == 0) {
			id_Hotel_Prec = instance->get_Id_Hotel_depart();
		}
		else {
			id_Hotel_Prec = solution->v_Id_Hotel_Intermedaire[num_Jour - 1];
		}

		float distance_Hotel_Prec_Hotel_Arrive = instance->get_distance_Hotel_Hotel(id_Hotel_Prec, instance->get_Id_Hotel_Arrivee());

		for (int id_Hotel = 0; id_Hotel < nb_Hotel; id_Hotel++) {

			float distance_Hotel_Hotel_Arrive = instance->get_distance_Hotel_Hotel(id_Hotel, instance->get_Id_Hotel_Arrivee());

			float distance_Hotel_Hotel_Prec = instance->get_distance_Hotel_Hotel(id_Hotel_Prec, id_Hotel);

			bool is_Hotel_Deja_Utilise = false;
			if (id_Hotel == instance->get_Id_Hotel_Arrivee() || id_Hotel == instance->get_Id_Hotel_depart()) {
				is_Hotel_Deja_Utilise = true;
			}

			auto it = find(solution->v_Id_Hotel_Intermedaire.begin(), solution->v_Id_Hotel_Intermedaire.end(), id_Hotel);

			if (it != solution->v_Id_Hotel_Intermedaire.end()) {
				is_Hotel_Deja_Utilise = true;
			}

			//cout << "D. H-H.arv : " << distance_Hotel_Hotel_Arrive << " | D. H.prec-H.arv : " << distance_Hotel_Prec_Hotel_Arrive << " | D. H-H.prec : " << distance_Hotel_Hotel_Prec + 5 << " | DMJ : " << instance->get_POI_Duree_Max_Voyage(num_Jour) << endl;

			if (distance_Hotel_Hotel_Arrive < distance_Hotel_Prec_Hotel_Arrive && distance_Hotel_Hotel_Prec + 5 < instance->get_POI_Duree_Max_Voyage(num_Jour)) {
				*distance_POI_Courant_Et_Hotel = instance->get_distance_Hotel_POI(id_Hotel, id_POI);
				*id_Hotel_Plus_Proche_POI_Courant = id_Hotel;
				break;
			}
		}
	}
}

float Heuristiques::calcul_Distance_Parcouru_Jour(int num_Jour, vector<int> vector_id_POI_Jour, Solution* solution) {
	float distance_Parcouru_Jour = 0;

	// Calcul distance parcouru dans la journ�e.
	if (vector_id_POI_Jour.size() != 0) {
		// Distance entre l'h�tel et le 1er POI.
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

	// On regarde si le POI a d�j� �t� visit� aujourd'hui.
	for (int indice_POI_Jour_Act = 0; indice_POI_Jour_Act < vector_id_POI_Jour.size(); indice_POI_Jour_Act++) {
		if (vector_id_POI_Jour[indice_POI_Jour_Act] == id_POI) {
			return true;
		}
	}

	// On regarde si le POI a d�j� �t� visit� dans les jours pr�c�dents.
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

	// On regarde si c'est le 1er POI de la journ�e.
	if (vector_id_POI_Jour.size() == 0) {

		// On regarde si l'on peut arriver � l'ouverture.
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