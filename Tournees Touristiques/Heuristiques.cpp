#include "Heuristiques.h"

Solution Heuristiques::methode_Heuristique_V1(Instance instance) {
	int id_Hotel_Dep = instance.get_Id_Hotel_depart();
	int id_Hotel_Arr = instance.get_Id_Hotel_Arrivee();

	int nb_Jour = instance.get_Nombre_Jour();
	int nb_POI = instance.get_Nombre_POI();
	int nb_Hotel = instance.get_Nombre_Hotel();

	// On crée notre solution et on y ajoute le premier hôtel.
	Solution* solution = new Solution();
	solution->v_Id_Hotel_Intermedaire.push_back(id_Hotel_Dep);

	for (int num_Jour = 0; num_Jour < nb_Jour; num_Jour++) {
		vector<int> vector_id_POI_Jour;

		float distance_Max_Jour_Actuel = instance.get_POI_Duree_Max_Voyage(num_Jour);
		int nb_POI_Jour = 0;
		int id_Hotel_Fin_Jour = 0;

		for (int id_POI = 0; id_POI < nb_POI; id_POI++) {

			float distance_POI_Actuel_Et_POI_Courant = 0; // Distance entre le POI / Hôtel où l'on est et le POI courant que l'on lit.

			// Si l'on est sur le l'hôtel de départ.
			if (nb_POI_Jour == 0) {
				distance_POI_Actuel_Et_POI_Courant = instance.get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[0], id_POI);
			}
			else {
				distance_POI_Actuel_Et_POI_Courant = instance.get_distance_POI_POI(vector_id_POI_Jour[nb_POI_Jour - 1], id_POI);
			}

			float distance_POI_Courant_Et_Plus_Proche_Hotel = instance.get_distance_Hotel_POI(0, id_POI); // Distance entre le POI courant que l'on lit et son proche plus proche hôtel.
			int id_Hotel_Plus_Proche_POI_Courant = 0;

			// Calcul de distance_POI_Courant_Et_Plus_Proche_Hotel
			for (int id_Hotel = 1; id_Hotel < nb_Hotel; id_Hotel++) {
				if (instance.get_distance_Hotel_POI(id_Hotel, id_POI) < distance_POI_Courant_Et_Plus_Proche_Hotel) {
					distance_POI_Courant_Et_Plus_Proche_Hotel = instance.get_distance_Hotel_POI(id_Hotel, id_POI);
					id_Hotel_Plus_Proche_POI_Courant = id_Hotel;
				}
			}

			float distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel = distance_POI_Actuel_Et_POI_Courant + distance_POI_Courant_Et_Plus_Proche_Hotel; // Distance entre le POI / Hôtel où l'on est et le POI courant que l'on lit et son proche plus proche hôtel.
			float distance_Parcouru_Jour = 0;

			// Calcul distance parcouru dans la journée.
			if (vector_id_POI_Jour.size() != 0) {
				// Distance entre l'hôtel et le 1er POI.
				distance_Parcouru_Jour = distance_Parcouru_Jour + instance.get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour], vector_id_POI_Jour[0]);

				// Distance entre tout les POI.
				for (int id_POI_parcouru = 0; id_POI_parcouru < vector_id_POI_Jour.size() - 1; id_POI_parcouru++) {
					distance_Parcouru_Jour = distance_Parcouru_Jour + instance.get_distance_POI_POI(vector_id_POI_Jour[id_POI_parcouru], vector_id_POI_Jour[id_POI_parcouru + 1]);
				}
			}

			bool is_POI_Present = false;

			// On regarde si le POI a déjà été visité aujourd'hui.
			for (int indice_POI_Jour_Act = 0; indice_POI_Jour_Act < vector_id_POI_Jour.size(); indice_POI_Jour_Act++) {
				if (vector_id_POI_Jour[indice_POI_Jour_Act] == id_POI) {
					is_POI_Present = true;
				}
			}

			// On regarde si le POI a déjà été visité dans les jours précédents.
			for (int num_Jour_Prec = 0; num_Jour_Prec < num_Jour; num_Jour_Prec++ && !is_POI_Present) {
				for (int indice_POI_Jour_Act = 0; indice_POI_Jour_Act < solution->v_v_Sequence_Id_Par_Jour[num_Jour_Prec].size() ; indice_POI_Jour_Act++) {
					if (solution->v_v_Sequence_Id_Par_Jour[num_Jour_Prec][indice_POI_Jour_Act] == id_POI) {
						is_POI_Present = true;
					}
				}
			}

			// On peut ajouter le POI si en le choisissant on peut à un hôtel par la suite, et qu'il n'a pas déjà été visité.
			if ((distance_POI_Actuel_POI_Courant_Plus_Proche_Hotel <= distance_Max_Jour_Actuel - distance_Parcouru_Jour) && !is_POI_Present) {
				vector_id_POI_Jour.push_back(id_POI);
				solution->i_valeur_fonction_objectif += instance.get_POI_Score(id_POI);
				nb_POI_Jour++;
				id_Hotel_Fin_Jour = id_Hotel_Plus_Proche_POI_Courant;
			}
		}

		// On ajoute l'hôtel le plus proche.
		solution->v_Id_Hotel_Intermedaire.push_back(id_Hotel_Fin_Jour);

		// On ajoute le vector de POI.
		solution->v_v_Sequence_Id_Par_Jour.push_back(vector_id_POI_Jour);

		int id_1er_POI = solution->v_v_Sequence_Id_Par_Jour[num_Jour][0];
		float heure_Ouverture_1er_POI = instance.get_POI_Heure_ouverture(id_1er_POI);

		// On définit l'heure de départ.
		solution->v_Date_Depart.push_back(heure_Ouverture_1er_POI - instance.get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour], id_1er_POI));
	}
	return *solution;
}