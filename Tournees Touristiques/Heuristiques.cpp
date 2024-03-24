#include "Heuristiques.h"

// Constructeur par d�faut.
Heuristiques::Heuristiques() {
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
		vector<int> vector_id_POI_Jour;

		float distance_Max_Jour_Actuel = instance->get_POI_Duree_Max_Voyage(num_Jour);
		int id_Hotel_Fin_Jour = 0;

		for (int id_POI = 0; id_POI < nb_POI; id_POI++) {

			// Distance entre le POI / H�tel o� l'on est et le POI courant que l'on lit.
			float distance_POI_Actuel_Et_POI_Courant = calcul_Distance_POI_Actuel_Et_POI_Courant(num_Jour, vector_id_POI_Jour, id_POI); 

			// Distance entre le POI courant que l'on lit et son proche plus proche h�tel.
			float distance_POI_Courant_Et_Plus_Proche_Hotel = 0;
			int id_Hotel_Plus_Proche_POI_Courant = 0;
			calcul_Distance_POI_Courant_Et_Plus_Proche_Hotel_Et_Id_PPH(&distance_POI_Courant_Et_Plus_Proche_Hotel, &id_Hotel_Plus_Proche_POI_Courant, num_Jour, nb_Jour, nb_Hotel, id_POI);

			// Distance entre le POI / H�tel o� l'on est et le POI courant que l'on lit et son proche plus proche h�tel.
			float distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel = distance_POI_Actuel_Et_POI_Courant + distance_POI_Courant_Et_Plus_Proche_Hotel; 
			
			// On calcul la distance parcouru du jour.
			float distance_Parcouru_Jour = calcul_Distance_Parcouru_Jour(num_Jour, vector_id_POI_Jour);

			// On regarde si le POI a d�j� �t� visit�.
			bool is_POI_Present = is_POI_Deja_Visite(num_Jour, vector_id_POI_Jour, id_POI);

			// On regarde si la date de d�part par d�faut est possible.
			bool is_OK_Date_Depart = is_Data_Depart_OK(num_Jour, id_POI, id_Hotel_Plus_Proche_POI_Courant);

			// On peut ajouter le POI si en le choisissant on peut � un h�tel par la suite, qu'il n'a pas d�j� �t� visit� et que la  date de d�part par d�faut est possible.
			if ((distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel <= distance_Max_Jour_Actuel - distance_Parcouru_Jour) && !is_POI_Present && is_OK_Date_Depart) {
				// On ajoute le POI.
				vector_id_POI_Jour.push_back(id_POI);

				// On mets � jour la F.O.
				solution->i_valeur_fonction_objectif += instance->get_POI_Score(id_POI);

				// On mets � jour l'ID de l'h�tel le plus proche.
				id_Hotel_Fin_Jour = id_Hotel_Plus_Proche_POI_Courant;
			}
		}
		cout << "id hotel fin jour : " << id_Hotel_Fin_Jour << endl;
		// On ajoute l'h�tel le plus proche sauf le dernier h�tel ( h�tel d'arriv� d�j� connu ).
		if (num_Jour != nb_Jour - 1) {
			solution->v_Id_Hotel_Intermedaire.push_back(id_Hotel_Fin_Jour);
		}

		// On ajoute le vector de POI.
		solution->v_v_Sequence_Id_Par_Jour.push_back(vector_id_POI_Jour);

		int id_1er_POI = solution->v_v_Sequence_Id_Par_Jour[num_Jour][0];
		float heure_Ouverture_1er_POI = instance->get_POI_Heure_ouverture(id_1er_POI);

		// On d�finit l'heure de d�part.
		if (num_Jour != 0) {
			solution->v_Date_Depart.push_back(heure_Ouverture_1er_POI - instance->get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], id_1er_POI));
		}
		else {
			solution->v_Date_Depart.push_back(heure_Ouverture_1er_POI - instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), id_1er_POI));
		}
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

float Heuristiques::calcul_Distance_Parcouru_Jour(int num_Jour,vector<int> vector_id_POI_Jour) {
	float distance_Parcouru_Jour = 0;

	// Calcul distance parcouru dans la journ�e.
	if (vector_id_POI_Jour.size() != 0) {
		// Distance entre l'h�tel et le 1er POI.
		if (num_Jour != 0) {
			distance_Parcouru_Jour = distance_Parcouru_Jour + instance->get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], vector_id_POI_Jour[0]);
		}
		else {
			distance_Parcouru_Jour = distance_Parcouru_Jour + instance->get_distance_Hotel_POI(instance->get_Id_Hotel_depart(), vector_id_POI_Jour[0]);
		}

		// Distance entre tout les POI.
		for (int id_POI_parcouru = 0; id_POI_parcouru < vector_id_POI_Jour.size() - 1; id_POI_parcouru++) {
			distance_Parcouru_Jour = distance_Parcouru_Jour + instance->get_distance_POI_POI(vector_id_POI_Jour[id_POI_parcouru], vector_id_POI_Jour[id_POI_parcouru + 1]);
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
	for (int num_Jour_Prec = 0; num_Jour_Prec < num_Jour; num_Jour_Prec++ ) {
		for (int indice_POI_Jour_Act = 0; indice_POI_Jour_Act < solution->v_v_Sequence_Id_Par_Jour[num_Jour_Prec].size(); indice_POI_Jour_Act++) {
			if (solution->v_v_Sequence_Id_Par_Jour[num_Jour_Prec][indice_POI_Jour_Act] == id_POI) {
				return true;
			}
		}
	}
	return false;
}

bool Heuristiques::is_Data_Depart_OK(int num_Jour, int id_POI, int id_Hotel_Plus_Proche_POI_Courant) {
	if (num_Jour != 0) {
		if (instance->get_POI_Heure_ouverture(id_POI) - instance->get_distance_Hotel_POI(id_Hotel_Plus_Proche_POI_Courant, id_POI) < 0) {
			return false;
		}
	}
	else {
		if (instance->get_POI_Heure_ouverture(id_POI) - instance->get_distance_Hotel_POI(instance->get_Id_Hotel_Arrivee(), id_POI) < 0) {
			return false;
		}
	}
	return true;
}
