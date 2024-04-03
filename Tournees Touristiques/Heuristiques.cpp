#include "Heuristiques.h"

// Constructeur par d�faut.
Heuristiques::Heuristiques() {
	instance = new Instance();
	solution = nullptr;
}

// Constructeur de confort.
Heuristiques::Heuristiques(Instance* instance_Param, Solution* solution_Param) {
	instance = instance_Param;
	solution = solution_Param;
}

Solution Heuristiques::methode_Heuristique_V1() {
	int id_Hotel_Dep = instance->get_Id_Hotel_depart();
	int id_Hotel_Arr = instance->get_Id_Hotel_Arrivee();

	int nb_Jour = instance->get_Nombre_Jour();
	int nb_POI = instance->get_Nombre_POI();
	int nb_Hotel = instance->get_Nombre_Hotel();

	for (int num_Jour = 0; num_Jour < nb_Jour; num_Jour++) {
		cout << "Num Jour : " << num_Jour << endl << endl << endl;

		float distance_Max_Jour_Actuel = instance->get_POI_Duree_Max_Voyage(num_Jour);

		// Vecteur qui va contenir tous les ID des POI du jour actuel.
		vector<int> vector_id_POI_Jour;

		// ID de l'h�tel de fin de la journ�e.
		int id_Hotel_Fin_Jour = 0;

		// On parcourt tous les POI.
		for (int id_POI = 0; id_POI < nb_POI; id_POI++) {

			// Distance entre le POI / H�tel o� l'on est et le POI courant que l'on lit.
			float distance_POI_Actuel_Et_POI_Courant = calcul_Distance_POI_Actuel_Et_POI_Courant(num_Jour, vector_id_POI_Jour, id_POI);

			float distance_POI_Courant_Et_Plus_Proche_Hotel = 0;
			int id_Hotel_Plus_Proche_POI_Courant = 0;

			// Distance entre le POI courant que l'on lit et son proche plus proche h�tel.
			calcul_Distance_POI_Courant_Et_Plus_Proche_Hotel_Et_Id_PPH(&distance_POI_Courant_Et_Plus_Proche_Hotel, &id_Hotel_Plus_Proche_POI_Courant, num_Jour, nb_Jour, nb_Hotel, id_POI);

			// On calcul la distance parcouru actuellement du jour.
			float distance_Parcouru_Jour = calcul_Distance_Parcouru_Jour(num_Jour, vector_id_POI_Jour);

			float tps_Attente = 0;

			// On calcul le temps d'attente avant ouverture � ce POI.
			if (distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant < instance->get_POI_Heure_ouverture(id_POI)) {
				tps_Attente = instance->get_POI_Heure_ouverture(id_POI) - distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant;
			}

			// Distance entre le POI / H�tel o� l'on est et le POI courant que l'on lit et son proche plus proche h�tel.
			float distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel = distance_POI_Actuel_Et_POI_Courant + tps_Attente + distance_POI_Courant_Et_Plus_Proche_Hotel;

			// On regarde si le POI a d�j� �t� visit�.
			bool is_POI_Present = is_POI_Deja_Visite(num_Jour, vector_id_POI_Jour, id_POI);

			// On regarde si la date de d�part par d�faut est possible.
			bool is_OK_Date_Depart = is_Date_Depart_OK(num_Jour, id_POI, id_Hotel_Plus_Proche_POI_Courant);

			// On regarde si la d'arriv� au POI n'est pas sup�rieure � la date de fermeture du POI.
			bool is_OK_Date_Arrive_POI = is_Date_Arrive_POI_OK(id_POI, distance_Parcouru_Jour, distance_POI_Actuel_Et_POI_Courant);

			bool is_OK_distance_Avec_Hotel_Arrive_Diminue = is_Distance_Avec_Hotel_Arrive_Diminue(num_Jour, vector_id_POI_Jour, id_POI);

			//cout << "ID POI : " << id_POI << " | Dt. POI Act - POI Cou - H : " << distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel << " | Dt. Rest : " << (distance_Max_Jour_Actuel - distance_Parcouru_Jour) << endl;
			//cout << "\tPOI Present : " << is_POI_Present << " | Date Dep. OK : " << is_OK_Date_Depart << " | Date Arr. OK : " << is_OK_Date_Arrive_POI << " Dist. Dim : " << is_OK_distance_Avec_Hotel_Arrive_Diminue << endl;

			// On peut ajouter le POI si en le choisissant on peut aller � un h�tel par la suite, qu'il n'a pas d�j� �t� visit�, que la date de d�part par d�faut est possible, que l'on peut l'atteindre avant sa fermeture et que la distance avec l'hotel d'arriv� diminue..
			if ((distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel <= distance_Max_Jour_Actuel - distance_Parcouru_Jour) && !is_POI_Present && is_OK_Date_Depart && is_OK_Date_Arrive_POI && is_OK_distance_Avec_Hotel_Arrive_Diminue) {

				// On ajoute le POI.
				vector_id_POI_Jour.push_back(id_POI);

				// On mets � jour la F.O.
				solution->i_valeur_fonction_objectif += instance->get_POI_Score(id_POI);

				// On mets � jour l'ID de l'h�tel le plus proche.
				id_Hotel_Fin_Jour = id_Hotel_Plus_Proche_POI_Courant;

				// On ajoute la date de d�part lorsque que l'on ajoute le premier POI.
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

float Heuristiques::calcul_Distance_POI_Actuel_Et_POI_Courant(int num_Jour, vector<int>  vector_id_POI_Jour, int id_POI) {
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

void Heuristiques::calcul_Distance_POI_Courant_Et_Plus_Proche_Hotel_Et_Id_PPH(float* distance_POI_Courant_Et_Plus_Proche_Hotel, int* id_Hotel_Plus_Proche_POI_Courant, int num_Jour, int nb_Jour, int nb_Hotel, int id_POI) {

	*distance_POI_Courant_Et_Plus_Proche_Hotel = instance->get_distance_Hotel_POI(0, id_POI);

	// Si c'est le dernier et que l'on doit obligatoirement rentrer � l'h�tel d'arriv�.
	if (num_Jour == nb_Jour - 1) {
		*distance_POI_Courant_Et_Plus_Proche_Hotel = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), id_POI);
		*id_Hotel_Plus_Proche_POI_Courant = instance->get_Id_Hotel_Arrivee();
	}
	else {
		// Calcul de distance_POI_Courant_Et_Plus_Proche_Hotel
		for (int id_Hotel = 1; id_Hotel < nb_Hotel; id_Hotel++) {
			if (instance->get_distance_Hotel_POI(id_Hotel, id_POI) < *distance_POI_Courant_Et_Plus_Proche_Hotel) {
				*distance_POI_Courant_Et_Plus_Proche_Hotel = instance->get_distance_Hotel_POI(id_Hotel, id_POI);
				*id_Hotel_Plus_Proche_POI_Courant = id_Hotel;
			}
		}
	}
}

float Heuristiques::calcul_Distance_Parcouru_Jour(int num_Jour, vector<int> vector_id_POI_Jour) {
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

bool Heuristiques::is_POI_Deja_Visite(int num_Jour, vector<int> vector_id_POI_Jour, int id_POI) {

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

bool Heuristiques::is_Date_Depart_OK(int num_Jour, int id_POI, int id_Hotel_Plus_Proche_POI_Courant) {
	if (num_Jour != 0) {
		//cout << "ID POI : " << id_POI << " | Date de d�part : " << (instance->get_POI_Heure_fermeture(id_POI) - instance->get_distance_Hotel_POI(id_Hotel_Plus_Proche_POI_Courant, id_POI)) << endl;
		if (instance->get_POI_Heure_fermeture(id_POI) - instance->get_distance_Hotel_POI(id_Hotel_Plus_Proche_POI_Courant, id_POI) < 0) {
			return false;
		}
	}
	else {
		//cout << "ID POI : " << id_POI << " | Date de d�part : " << (instance->get_POI_Heure_fermeture(id_POI) - instance->get_distance_Hotel_POI(id_Hotel_Plus_Proche_POI_Courant, id_POI)) << endl;

		if (instance->get_POI_Heure_fermeture(id_POI) - instance->get_distance_Hotel_POI(instance->get_Id_Hotel_depart(), id_POI) < 0) {
			return false;
		}
	}
	return true;
}

bool Heuristiques::is_Date_Arrive_POI_OK(int id_POI, float distance_Parcouru_Jour, float distance_POI_Actuel_Et_POI_Courant) {
	cout << "ID POI : " << id_POI << " | D. Par. J : " << distance_Parcouru_Jour << " | D. POI Act - POI Cour : " << distance_POI_Actuel_Et_POI_Courant << " | Hor. O : " << instance->get_POI_Heure_ouverture(id_POI) << " | Hor. F : " << instance->get_POI_Heure_fermeture(id_POI) << endl;
	
	// AJOUTER DATE DEBUT
	if (distance_Parcouru_Jour + distance_POI_Actuel_Et_POI_Courant > instance->get_POI_Heure_fermeture(id_POI)) {
		return false;
	}
	return true;
}

bool Heuristiques::is_Distance_Avec_Hotel_Arrive_Diminue(int num_Jour, vector<int> vector_id_POI_Jour, int id_POI) {
	float distance_POI_Actuel_Hotel_Arrive = 0;
	float distance_POI_Courant_Hotel_Arrive = 0;

	// Si l'on est sur un h�tel.
	if (vector_id_POI_Jour.size() == 0) {
		return true;
		//// Si c'est le 1er jour.
		//if (num_Jour == 0) {
		//	distance_POI_Actuel_Hotel_Arrive = instance->get_distance_Hotel_Hotel(instance->get_Id_Hotel_depart(), instance->get_Id_Hotel_Arrivee());
		//}

		//// Si ce n'est pas le 1er jour.
		//else {
		//	distance_POI_Actuel_Hotel_Arrive = instance->get_distance_Hotel_Hotel(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], instance->get_Id_Hotel_Arrivee());
		//}
	}

	// Si l'on est sur un POI.
	else {
		distance_POI_Actuel_Hotel_Arrive = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), vector_id_POI_Jour[vector_id_POI_Jour.size() - 1]);
	}

	distance_POI_Courant_Hotel_Arrive = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), id_POI);

	//cout << "ID : " << id_POI << " | Dist. Act-H : " << distance_POI_Actuel_Hotel_Arrive << " | Dist. Cour-H : " << distance_POI_Courant_Hotel_Arrive << endl;

	// On regarde si la distance avec l'h�tel a augment�e.
	if (distance_POI_Actuel_Hotel_Arrive > distance_POI_Courant_Hotel_Arrive) {
		return true;
	}

	return false;
}