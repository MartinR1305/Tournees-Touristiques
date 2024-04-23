#include "MetaHeuristiques.h"

#define NB_MINUTES 3

// Constructeur par défaut.
MetaHeuristiques::MetaHeuristiques() {
	instance = new Instance();
}

// Constructeur de confort.
MetaHeuristiques::MetaHeuristiques(Instance* instance_Param) {
	instance = instance_Param;
}

Solution MetaHeuristiques::recherche_Tabou() {

	Solution meilleure_Solution;
	Heuristiques* heuristique = new Heuristiques(instance);

	// Calcul de la solution initiale à partir de l'heuristique.
	meilleure_Solution = heuristique->methode_Heuristique();

	vector<vector<int>> liste_Tabou;
	vector<Solution> solution_Voisinage;

	int nb_Ite = 0;
	int indice_Meilleure_Solution = -1;
	int taille_Liste_Tabou = 5000;

	Solution solution_Actuel;
	solution_Actuel = meilleure_Solution;

	// Durée maximale (3 minutes)
	auto start = chrono::steady_clock::now();
	auto max_duration = chrono::minutes(NB_MINUTES);

	int ite_Pas_Divers_Trajet = 0;

	while (true) {

		// Calcul du temps écoulé
		auto current = chrono::steady_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(current - start);

		// Sortir de la boucle si 1 minute est écoulée
		if (duration >= max_duration) {
			break;
		}

		solution_Voisinage.clear();
		vector<vector<int>> mouvement_Voisinage;

		// On génère le voisinage à partir de la solution.
		generer_Voisinage(solution_Actuel, &solution_Voisinage, &mouvement_Voisinage);

		// On cherche la meilleure solution qui n'est pas dans la liste tabou.
		indice_Meilleure_Solution = get_Indice_Meilleure_Solution(solution_Voisinage, mouvement_Voisinage, liste_Tabou);

		// On regarde si il existe une meilleure solution dans le voisinage qui n'est pas dans la liste tabou.
		if (indice_Meilleure_Solution != -1) {


			cout << "V : " << solution_Voisinage[indice_Meilleure_Solution].i_valeur_fonction_objectif << " | M : " << meilleure_Solution.i_valeur_fonction_objectif << endl << endl;

			// Si la liste tabou n'est pas pleine.
			if (liste_Tabou.size() < taille_Liste_Tabou) {
				liste_Tabou.push_back(mouvement_Voisinage[indice_Meilleure_Solution]);
			}

			// Si la liste tabou est pleine.
			else {
				remplacer_Plus_Ancien_Mouvement(&liste_Tabou, mouvement_Voisinage[indice_Meilleure_Solution]);
			}

			// On regarde si cette meilleure soluton a une meilleur F.O que la solution.
			if (solution_Voisinage[indice_Meilleure_Solution].i_valeur_fonction_objectif > meilleure_Solution.i_valeur_fonction_objectif) {

				// On prends donc la meilleur solution.
				meilleure_Solution = solution_Voisinage[indice_Meilleure_Solution];
				solution_Actuel = meilleure_Solution;

				// Phase d'intensification.
				taille_Liste_Tabou = 1000;
				liste_Tabou.clear();
				ite_Pas_Divers_Trajet = 0;
			}

			// Cas où la meilleure solution du voisinage n'a pas une meilleure F.O que la solution.
			else {
				ite_Pas_Divers_Trajet++;

				if (ite_Pas_Divers_Trajet > 25) {

					// Phase de diversification moyenne.
					solution_Actuel = generer_Solution_Random_Trajet_Min(solution_Voisinage[indice_Meilleure_Solution]);
					ite_Pas_Divers_Trajet = 0;
					taille_Liste_Tabou = 7000;
				}

				else {
					solution_Actuel = solution_Voisinage[indice_Meilleure_Solution];
				}
			}
		}

		else {

			// Phase de diversification totale.
			solution_Actuel = generer_Solution_Aleatoire();
			ite_Pas_Divers_Trajet = 0;
			taille_Liste_Tabou = 10000;
		}

		nb_Ite++;
	}
	return meilleure_Solution;
}

Solution MetaHeuristiques::generer_Solution_Random_Trajet_Min(Solution solution) {

	int num_Jour_Score_Min = -1;
	int score_Min_Jour = 10000;

	// on récupère le num jour du score min.
	for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour(); num_Jour++) {

		int score_Jour = 0;

		for (int indice_POI = 0; indice_POI < solution.v_v_Sequence_Id_Par_Jour[num_Jour].size(); indice_POI++) {
			score_Jour += instance->get_POI_Score(solution.v_v_Sequence_Id_Par_Jour[num_Jour][indice_POI]);
		}

		if (score_Jour < score_Min_Jour) {
			score_Min_Jour = score_Jour;
			num_Jour_Score_Min = num_Jour;
		}
	}

	Solution solution_Modif;
	solution_Modif = solution;

	// On soustrait les scores des POI que l'on va enlever.
	for (int indice_POI = 0; indice_POI < solution_Modif.v_v_Sequence_Id_Par_Jour[num_Jour_Score_Min].size(); indice_POI++) {
		solution_Modif.i_valeur_fonction_objectif -= instance->get_POI_Score(solution_Modif.v_v_Sequence_Id_Par_Jour[num_Jour_Score_Min][indice_POI]);
	}

	// On ajoute un POI dans l'ordre croissant et on regarde si l'on obtient une solution faisable.
	for (int id_POI = 0; id_POI < instance->get_Nombre_POI(); id_POI++) {
		solution_Modif.v_Date_Depart.erase(solution_Modif.v_Date_Depart.begin() + num_Jour_Score_Min);
		solution_Modif.v_v_Sequence_Id_Par_Jour.erase(solution_Modif.v_v_Sequence_Id_Par_Jour.begin() + num_Jour_Score_Min);

		ajouter_POI_Solution(&solution_Modif, num_Jour_Score_Min, id_POI);

		if (!solution_Modif.Verification_Solution(instance));
			return solution_Modif;
		}
}

Solution MetaHeuristiques::generer_Solution_Aleatoire() {
	Solution solution_Aleatoire;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis_Hotel(0, instance->get_Nombre_Hotel() - 1);
	uniform_int_distribution<> dis_POI(0, instance->get_Nombre_POI() - 1);

	// Tant que l'on a pas de solution faisable on en recrée une.
	while (!solution_Aleatoire.Verification_Solution(instance)) {

		// On vide la solution.
		solution_Aleatoire.v_Id_Hotel_Intermedaire.clear();
		solution_Aleatoire.v_v_Sequence_Id_Par_Jour.clear();
		solution_Aleatoire.i_valeur_fonction_objectif = 0;
		solution_Aleatoire.v_Date_Depart.clear();

		// Ajout des l'hôtels aléatoire.
		for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour() - 1; num_Jour++) {
			solution_Aleatoire.v_Id_Hotel_Intermedaire.push_back(dis_Hotel(gen));
		}

		// Ajout des POIs aléatoire.
		for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour(); num_Jour++) {

			ajouter_POI_Solution(&solution_Aleatoire, num_Jour, dis_POI(gen));
		}
	}

	return solution_Aleatoire;
}

void MetaHeuristiques::ajouter_POI_Solution(Solution* solution_Aleatoire, int num_Jour, int id_POI) {

	// Ajout du POI aléatoirement.
	vector<int> vector_id_POI_Jour;
	vector_id_POI_Jour.push_back(id_POI);
	solution_Aleatoire->v_v_Sequence_Id_Par_Jour.push_back(vector_id_POI_Jour);

	float heure_Ouverture = instance->get_POI_Heure_ouverture(vector_id_POI_Jour[0]);
	float heure_Fermeture = instance->get_POI_Heure_fermeture(vector_id_POI_Jour[0]);

	float distance_POI_Actuel_Et_POI_Courant;

	if (num_Jour != 0) {
		distance_POI_Actuel_Et_POI_Courant = instance->get_distance_Hotel_POI(solution_Aleatoire->v_Id_Hotel_Intermedaire[num_Jour - 1], vector_id_POI_Jour[0]);
	}
	else {
		distance_POI_Actuel_Et_POI_Courant = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_depart(), vector_id_POI_Jour[0]);
	}

	// Ajout de la date de départ.
	if (heure_Ouverture - distance_POI_Actuel_Et_POI_Courant >= 0) {
		solution_Aleatoire->v_Date_Depart.push_back(heure_Ouverture - distance_POI_Actuel_Et_POI_Courant);
	}
	else {
		solution_Aleatoire->v_Date_Depart.push_back(heure_Fermeture - distance_POI_Actuel_Et_POI_Courant);
	}

	// MAJ de la F.O.
	solution_Aleatoire->i_valeur_fonction_objectif += instance->get_POI_Score(vector_id_POI_Jour[0]);
}

void MetaHeuristiques::remplacer_Plus_Ancien_Mouvement(vector<vector<int>>* liste_Tabou, vector<int> mouvement_Tabou) {

	// On supprime le premier élément de la liste tabou.
	liste_Tabou->erase(liste_Tabou->begin());

	// On ajoute le mouvement tabou.
	liste_Tabou->push_back(mouvement_Tabou);
}

void MetaHeuristiques::generer_Voisinage(Solution solution_Actuel, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage) {

	generer_Voisinage_Operateur_SWAP_Hotel(solution_Actuel, solution_Voisinage, mouvement_Voisinage);
	generer_Voisinage_Operateur_Ajouter_POI(solution_Actuel, solution_Voisinage, mouvement_Voisinage);
	generer_Voisinage_Operateur_SWAP_POI(solution_Actuel, solution_Voisinage, mouvement_Voisinage);
}

void MetaHeuristiques::generer_Voisinage_Operateur_SWAP_Hotel(Solution solution_Actuel, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage) {

	// On parcours tous les jours.
	for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour() - 1; num_Jour++) {

		// On parcours tous les hôtels.
		for (int id_Hotel = 0; id_Hotel < instance->get_Nombre_Hotel(); id_Hotel++) {

			// On vérifie que l'hôtel actuel n'est pas déjà celui de la solution.
			if (id_Hotel != solution_Actuel.v_Id_Hotel_Intermedaire[num_Jour]) {

				// On crée la solution à partir de la solution initiale.
				Solution solution_Temporaire = solution_Actuel;

				// On switch l'hôtel.
				solution_Temporaire.v_Id_Hotel_Intermedaire[num_Jour] = id_Hotel;

				// On vérifie que la solution est réalisable.
				if (solution_Temporaire.Verification_Solution(instance)) {

					// Mouvement de la solution. Le premier élément est un 0 afin de différencier les mouvements "Swap Hotel" et "Swap POI".
					vector<int> mouvement = { 0 , solution_Actuel.v_Id_Hotel_Intermedaire[num_Jour] , id_Hotel , num_Jour };
					mouvement_Voisinage->push_back(mouvement);

					// On ajotue la solution dans le voisinage.
					solution_Voisinage->push_back(solution_Temporaire);
				}
			}
		}
	}
}

void MetaHeuristiques::generer_Voisinage_Operateur_Ajouter_POI(Solution solution_Actuel, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage) {

	// On parcours tous les jours.
	for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour(); num_Jour++) {

		// On parcours tous les POI de l'instance.
		for (int id_POI_Instance = 0; id_POI_Instance < instance->get_Nombre_POI(); id_POI_Instance++) {

			// On crée la solution à partir de la solution initiale.
			Solution solution_Temporaire = solution_Actuel;

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

void MetaHeuristiques::generer_Voisinage_Operateur_SWAP_POI(Solution solution_Actuel, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage) {

	// On parcours tous les jours.
	for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour(); num_Jour++) {

		// On parcours tous les POI de la solution.
		for (int indice_POI_Solution = 0; indice_POI_Solution < solution_Actuel.v_v_Sequence_Id_Par_Jour[num_Jour].size(); indice_POI_Solution++) {

			// On parcours tous les POI de l'instance.
			for (int id_POI_Instance = 0; id_POI_Instance < instance->get_Nombre_POI(); id_POI_Instance++) {

				// On vérifie que le POI de l'instance n'est pas déjà celui de la solution.
				if (id_POI_Instance != solution_Actuel.v_v_Sequence_Id_Par_Jour[num_Jour][indice_POI_Solution]) {

					// On crée la solution à partir de la solution initiale.
					Solution solution_Temporaire = solution_Actuel;

					// On switch le POI.
					solution_Temporaire.v_v_Sequence_Id_Par_Jour[num_Jour][indice_POI_Solution] = id_POI_Instance;

					// On mets à jour la F.O.
					solution_Temporaire.i_valeur_fonction_objectif -= instance->get_POI_Score(solution_Actuel.v_v_Sequence_Id_Par_Jour[num_Jour][indice_POI_Solution]);
					solution_Temporaire.i_valeur_fonction_objectif += instance->get_POI_Score(id_POI_Instance);

					// Si l'on modifie le premier POI, il faut modifier la date de départ du jour.
					if (indice_POI_Solution == 0) {
						float heure_Ouverture = instance->get_POI_Heure_ouverture(id_POI_Instance);
						float heure_Fermeture = instance->get_POI_Heure_fermeture(id_POI_Instance);

						float distance_POI_Actuel_Et_POI_Courant;

						if (num_Jour != 0) {
							distance_POI_Actuel_Et_POI_Courant = instance->get_distance_Hotel_POI(solution_Actuel.v_Id_Hotel_Intermedaire[num_Jour - 1], id_POI_Instance);
						}
						else {
							distance_POI_Actuel_Et_POI_Courant = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_depart(), id_POI_Instance);
						}

						if (heure_Ouverture - distance_POI_Actuel_Et_POI_Courant >= 0) {
							solution_Temporaire.v_Date_Depart[num_Jour] = heure_Ouverture - distance_POI_Actuel_Et_POI_Courant;
						}
						else {
							solution_Temporaire.v_Date_Depart[num_Jour] = heure_Fermeture - distance_POI_Actuel_Et_POI_Courant;
						}
					}

					// On vérifie que la solution est réalisable.
					if (solution_Temporaire.Verification_Solution(instance)) {

						// Mouvement de la solution. Le premier élément est un 1 afin de différencier les mouvements "Swap Hotel" et "Swap POI".
						vector<int> mouvement = { 1 , solution_Actuel.v_v_Sequence_Id_Par_Jour[num_Jour][indice_POI_Solution] , id_POI_Instance , num_Jour };
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
		if (solution_Voisinage[indice_Solution_Voisine].i_valeur_fonction_objectif > meilleur_FO && !is_Mouvement_Dans_Liste_Tabou(mouvement_Voisinage[indice_Solution_Voisine], liste_Tabou)) {
			meilleur_FO = solution_Voisinage[indice_Solution_Voisine].i_valeur_fonction_objectif;
			indice_Meilleure_Solution = indice_Solution_Voisine;
		}
	}

	return indice_Meilleure_Solution;
}

bool MetaHeuristiques::is_Mouvement_Dans_Liste_Tabou(vector<int> mouvement, vector<vector<int>> liste_Tabou) {

	// On parcours tous les mouvements de la liste tabou.
	for (vector<int> mouvement_Tabou : liste_Tabou) {

		// On regarde si ce mouvement est le même que le mouvement en paramètre.
		if (mouvement == mouvement_Tabou) {
			return true;
		}
	}
	return false;
}