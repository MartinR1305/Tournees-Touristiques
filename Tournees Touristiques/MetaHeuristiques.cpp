#include "MetaHeuristiques.h"

#define NOMBRE_ITERATION 1000
#define TAILLE_LISTE_TABOU 101

// Constructeur par d�faut.
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

	// Calcul de la solution initiale � partir de l'heuristique.
	*solution = heuristique->methode_Heuristique();

	vector<vector<int>> liste_Tabou;
	int nb_Ite = 0;

	while (nb_Ite < NOMBRE_ITERATION) {

		unordered_map<Solution, vector<int>> voisinage_Avec_Mouvement = generer_Voisinage(*solution);
		Solution* meilleure_Solution = get_Meilleure_Solution(voisinage_Avec_Mouvement, liste_Tabou);

		// On regarde si la meilleur solution n'�tait pas dans liste tabou.
		if (meilleure_Solution != nullptr) {

			// On cherche le mouvement tabou associ� � la meilleure solution.
			auto it = voisinage_Avec_Mouvement.find(*meilleure_Solution);
			vector<int> mouvement_Tabou = it->second;

			// On regarde si cette meilleur soluton  a une meilleur F.O que la solution.
			if (meilleure_Solution->i_valeur_fonction_objectif > solution->i_valeur_fonction_objectif) {

				// Si la liste tabou n'est pas pleine.
				if (liste_Tabou.size() < TAILLE_LISTE_TABOU) {
					liste_Tabou.push_back(mouvement_Tabou);
				}

				// Si la liste tabou est pleine.
				else {
					remplacer_Plus_Ancien_Mouvement(&liste_Tabou, &mouvement_Tabou);
				}

				// On prends donc la meilleur solution.
				solution = meilleure_Solution;
			}
		}
	}
	return *solution;
}

unordered_map<Solution, vector<int>> MetaHeuristiques::generer_Voisinage(Solution solution) {

	unordered_map<Solution, vector<int>> voisinage;

	// On parcours tous les jours.
	for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour() - 1 ; num_Jour++) {

		// On parcours tous les h�tels.
		for (int id_Hotel = 0; id_Hotel < instance->get_Nombre_Hotel(); id_Hotel++) {

			// On v�rifie que l'h�tel actuel n'est pas d�j� celui de la solution.
			if (id_Hotel != solution.v_Id_Hotel_Intermedaire[num_Jour]) {

				// On cr�e la solution � partir de la solution initiale.
				Solution solution_Temporaire = solution; // A v�rifier avec les pointeurs.

				// On switch l'h�tel.
				solution.v_Id_Hotel_Intermedaire[num_Jour] = id_Hotel;

				// On v�rifie que la solution est r�alisable.
				if (solution.Verification_Solution(instance)) {

					// Mouvement de la solution. Le premier �l�ment est un 0 afin de diff�rencier les mouvements "Swap Hotel" et "Swap POI".
					vector<int> mouvement = { 0 , solution.v_Id_Hotel_Intermedaire[num_Jour] , id_Hotel , num_Jour };

					// On ajoute la solution avec son mouvement associ�.
					voisinage.insert(pair<Solution, vector<int>>(solution_Temporaire, mouvement));
				}
			}
		}
	}

	// Ajouter un POI.


	// Switch un POI.

	return voisinage;
}

Solution* MetaHeuristiques::get_Meilleure_Solution(unordered_map<Solution, vector<int>> voisinage, vector<vector<int>> liste_Tabou) {

	int meilleur_FO = -1;
	Solution meilleure_Solution;
	vector<int> mouvement_Meilleure_Solution;

	// On parcours toutes les paires de la map.
	for (auto& paire_Voisinage : voisinage) {

		// On regarde si la F.O de la solution actuelle est meilleure ou pas.
		if (paire_Voisinage.first.i_valeur_fonction_objectif > meilleur_FO) {

			// On actualise nos variables.
			meilleur_FO = paire_Voisinage.first.i_valeur_fonction_objectif;
			meilleure_Solution = paire_Voisinage.first;
			mouvement_Meilleure_Solution = paire_Voisinage.second;
		}
	}

	// On regarde si le mouvement est dans la liste tabou ou pas.
	for (vector<int> mouvement : liste_Tabou) {
		if (mouvement == mouvement_Meilleure_Solution) {
			return nullptr;
		}
	}

	return &meilleure_Solution;
}

void MetaHeuristiques::remplacer_Plus_Ancien_Mouvement(vector<vector<int>>* liste_Tabou, vector<int>* mouvement_Tabou) {

	// On supprime le premier �l�ment de la liste tabou.
	liste_Tabou->erase(liste_Tabou->begin());

	// On ajoute le mouvement tabou.
	liste_Tabou->push_back(*mouvement_Tabou);
}