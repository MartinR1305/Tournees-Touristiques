#include "MetaHeuristiques.h"

#define NOMBRE_ITERATION 1000
#define TAILLE_LISTE_TABOU 101

// Constructeur par défaut.
MetaHeuristiques::MetaHeuristiques() {
	instance = new Instance();
}

// Constructeur de confort.
MetaHeuristiques::MetaHeuristiques(Instance* instance_Param) {
	instance = instance_Param;
}

Solution MetaHeuristiques::methode_MetaHeuristiques() {

	Solution* solution = new Solution();
	Heuristiques* heuristique = new Heuristiques(instance);

	// Calcul de la solution initiale à partir de l'heuristique.
	*solution = heuristique->methode_Heuristique();

	vector<vector<int>> liste_Tabou;
	int nb_Ite = 0;

	while (nb_Ite < NOMBRE_ITERATION) {

		vector<Solution> solution_Voisinage;
		vector<vector<int>> mouvement_Voisinage;

		// On génère le voisinage à partir de la solution.
		generer_Voisinage(*solution, &solution_Voisinage, &mouvement_Voisinage);

		// On cherche la meilleure solution qui n'est pas dans la liste tabou.
		int indice_Meilleure_Solution = get_Indice_Meilleure_Solution(solution_Voisinage, mouvement_Voisinage, liste_Tabou);

		//// On regarde si la meilleur solution n'était pas dans liste tabou.
		//if (meilleure_Solution != nullptr) {

		//	// On cherche le mouvement tabou associé à la meilleure solution.
		//	auto it = voisinage_Avec_Mouvement.find(*meilleure_Solution);
		//	vector<int> mouvement_Tabou = it->second;

		//	// On regarde si cette meilleur soluton  a une meilleur F.O que la solution.
		//	if (meilleure_Solution->i_valeur_fonction_objectif > solution->i_valeur_fonction_objectif) {

		//		// Si la liste tabou n'est pas pleine.
		//		if (liste_Tabou.size() < TAILLE_LISTE_TABOU) {
		//			liste_Tabou.push_back(mouvement_Tabou);
		//		}

		//		// Si la liste tabou est pleine.
		//		else {
		//			remplacer_Plus_Ancien_Mouvement(&liste_Tabou, &mouvement_Tabou);
		//		}

		//		// On prends donc la meilleur solution.
		//		solution = meilleure_Solution;
		//	}
		//}
	}
	return *solution;
}

void MetaHeuristiques::generer_Voisinage(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage) {

	generer_Voisinage_Operateur_SWAP_Hotel(solution, solution_Voisinage, mouvement_Voisinage);
	generer_Voisinage_Operateur_Ajouter_POI(solution, solution_Voisinage, mouvement_Voisinage);
	generer_Voisinage_Operateur_SWAP_POI(solution, solution_Voisinage, mouvement_Voisinage);
}

void MetaHeuristiques::generer_Voisinage_Operateur_SWAP_Hotel(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage) {

	// On parcours tous les jours.
	for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour() - 1; num_Jour++) {

		// On parcours tous les hôtels.
		for (int id_Hotel = 0; id_Hotel < instance->get_Nombre_Hotel(); id_Hotel++) {

			// On vérifie que l'hôtel actuel n'est pas déjà celui de la solution.
			if (id_Hotel != solution.v_Id_Hotel_Intermedaire[num_Jour]) {

				// On crée la solution à partir de la solution initiale.
				Solution solution_Temporaire = solution;

				// On switch l'hôtel.
				solution_Temporaire.v_Id_Hotel_Intermedaire[num_Jour] = id_Hotel;

				// On vérifie que la solution est réalisable.
				if (solution_Temporaire.Verification_Solution(instance)) {

					// Mouvement de la solution. Le premier élément est un 0 afin de différencier les mouvements "Swap Hotel" et "Swap POI".
					vector<int> mouvement = { 0 , solution.v_Id_Hotel_Intermedaire[num_Jour] , id_Hotel , num_Jour };
					mouvement_Voisinage->push_back(mouvement);

					// On ajotue la solution dans le voisinage.
					solution_Voisinage->push_back(solution_Temporaire);
				}
			}
		}
	}
}

void MetaHeuristiques::generer_Voisinage_Operateur_Ajouter_POI(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage) {

	// On parcours tous les jours.
	for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour(); num_Jour++) {

		// On parcours tous les POI de l'instance.
		for (int id_POI_Instance = 0; id_POI_Instance < instance->get_Nombre_POI(); id_POI_Instance++) {

			// On crée la solution à partir de la solution initiale.
			Solution solution_Temporaire = solution;

			// On switch le POI.
			solution_Temporaire.v_v_Sequence_Id_Par_Jour[num_Jour].push_back(id_POI_Instance);

			// On mets à jour la F.O.
			solution_Temporaire.i_valeur_fonction_objectif += instance->get_POI_Score(id_POI_Instance);

			// On vérifie que la solution est réalisable.
			if (solution_Temporaire.Verification_Solution(instance)) {

				// Mouvement de la solution, ce mouvement ne sera cepedant jamasi dans la liste tabou par logique.
				vector<int> mouvement = { id_POI_Instance, num_Jour };
				mouvement_Voisinage->push_back(mouvement);

				// On ajotue la solution dans le voisinage.
				solution_Voisinage->push_back(solution_Temporaire);
			}
		}
	}
}

void MetaHeuristiques::generer_Voisinage_Operateur_SWAP_POI(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage) {

	// On parcours tous les jours.
	for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour(); num_Jour++) {

		// On parcours tous les POI de la solution.
		for (int indice_POI_Solution = 0; indice_POI_Solution < solution.v_v_Sequence_Id_Par_Jour[num_Jour].size(); indice_POI_Solution++) {

			// On parcours tous les POI de l'instance.
			for (int id_POI_Instance = 0; id_POI_Instance < instance->get_Nombre_POI(); id_POI_Instance++) {

				// On vérifie que le POI de l'instance n'est pas déjà celui de la solution.
				if (id_POI_Instance != solution.v_v_Sequence_Id_Par_Jour[num_Jour][indice_POI_Solution]) {

					// On crée la solution à partir de la solution initiale.
					Solution solution_Temporaire = solution;

					// On switch le POI.
					solution_Temporaire.v_v_Sequence_Id_Par_Jour[num_Jour][indice_POI_Solution] = id_POI_Instance;

					// On mets à jour la F.O.
					solution_Temporaire.i_valeur_fonction_objectif -= instance->get_POI_Score(solution.v_v_Sequence_Id_Par_Jour[num_Jour][indice_POI_Solution]);
					solution_Temporaire.i_valeur_fonction_objectif += instance->get_POI_Score(id_POI_Instance);

					// On vérifie que la solution est réalisable.
					if (solution_Temporaire.Verification_Solution(instance)) {

						// Mouvement de la solution. Le premier élément est un 1 afin de différencier les mouvements "Swap Hotel" et "Swap POI".
						vector<int> mouvement = { 1 , solution.v_v_Sequence_Id_Par_Jour[num_Jour][indice_POI_Solution] , id_POI_Instance , num_Jour };
						mouvement_Voisinage->push_back(mouvement);

						// On ajotue la solution dans le voisinage.
						solution_Voisinage->push_back(solution_Temporaire);
					}
				}
			}
		}
	}
}

int MetaHeuristiques::get_Indice_Meilleure_Solution(vector<Solution> solution_Voisinage, vector<vector<int>> mouvement_Voisinage, vector<vector<int>> liste_Tabou) {

	int meilleur_FO = -1;
	int indice_Meilleure_Solution = -1;

	// On parcours toutes les paires de la map.
	for (int indice_Solution_Voisine = 0; indice_Solution_Voisine < solution_Voisinage.size(); indice_Solution_Voisine++) {

		// On cherche la solution avec la meilleure F.O.
		if (solution_Voisinage[indice_Solution_Voisine].i_valeur_fonction_objectif > meilleur_FO) {
			meilleur_FO = solution_Voisinage[indice_Solution_Voisine].i_valeur_fonction_objectif;
			indice_Meilleure_Solution = indice_Solution_Voisine;
		}
	}

	// On regarde si le mouvement est dans la liste tabou ou pas.
	for (vector<int> mouvement : liste_Tabou) {
		if (mouvement == mouvement_Voisinage[indice_Meilleure_Solution]) {
			indice_Meilleure_Solution -1;
			break;
		}
	}

	return indice_Meilleure_Solution;
}

void MetaHeuristiques::remplacer_Plus_Ancien_Mouvement(vector<vector<int>>* liste_Tabou, vector<int>* mouvement_Tabou) {

	// On supprime le premier élément de la liste tabou.
	liste_Tabou->erase(liste_Tabou->begin());

	// On ajoute le mouvement tabou.
	liste_Tabou->push_back(*mouvement_Tabou);
}