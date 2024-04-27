#include "MetaHeuristiques.h"

#define NB_MINUTES 1

/* METAHEURISTIQUES : Constructeur par d�faut.
 *
 * Entr�e : Rien.
 * N�cessite : Rien.
 * Sortie : Rien.
	 * Entra�ne : Cr�e un objet de la classe MetaHeuristiques en l'initialisant avec des valeurs par d�faut.
*/
MetaHeuristiques::MetaHeuristiques() {
	instance = new Instance();
	solution = new Solution();
}

/* METAHEURISTIQUES : Constructeur de confort.
*
* Entr�e : instance_Param, pointeur vers un objet de la classe instance.
* N�cessite : Rien.
* Sortie : Rien.
* Entra�ne : Cr�e un objet de la classe MetaHeuristiques avec le pointeur d'instance voulu.
*/
MetaHeuristiques::MetaHeuristiques(Instance* instance_Param) {
	instance = instance_Param;
	solution = new Solution();
}

/* ~METAHEURISTIQUES : Destructeur.
*
* Entr�e : Rien.
* N�cessite : Rien.
* Sortie : Rien.
* Entra�ne : D�salloue le pointeur de l'instance.
*/
MetaHeuristiques::~MetaHeuristiques() {
	delete instance;
}

/* RECHERCHE_TABOU : R�alise une recherche tabou � partir de l'instance afin de chercher la meilleure solution possible.
* Entr�e : Rien.
* N�cessite : Rien.
* Sortie : La meilleure solution que la recherche tabou � trouver.
* Entra�ne : Rien.
*/
Solution MetaHeuristiques::recherche_Tabou() {

	Heuristiques* heuristique = new Heuristiques(instance);

	// Calcul de la solution initiale � partir de l'heuristique.
	*solution = heuristique->heuristique_V2();

	vector<vector<int>> liste_Tabou;
	vector<Solution> solution_Voisinage;

	int ite = 0;
	int indice_Meilleure_Solution = -1;
	int taille_Liste_Tabou = 5000;

	Solution solution_Actuel;
	solution_Actuel = *solution;

	// Dur�e maximale (3 minutes)
	auto start = chrono::steady_clock::now();
	auto max_duration = chrono::minutes(NB_MINUTES);

	cout << "Recherche Tabou en cours ... " << endl;

	while (true) {

		// Calcul du temps �coul�
		auto current = chrono::steady_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(current - start);

		// Sortir de la boucle si 1 minute est �coul�e
		if (duration >= max_duration) {
			break;
		}

		solution_Voisinage.clear();
		vector<vector<int>> mouvement_Voisinage;

		// On g�n�re le voisinage � partir de la solution.
		generer_Voisinage(solution_Actuel, &solution_Voisinage, &mouvement_Voisinage);

		// On cherche la meilleure solution qui n'est pas dans la liste tabou.
		indice_Meilleure_Solution = get_Indice_Meilleure_Solution_Voisinage(solution_Voisinage, mouvement_Voisinage, liste_Tabou);

		// On regarde si il existe une meilleure solution dans le voisinage qui n'est pas dans la liste tabou.
		if (indice_Meilleure_Solution != -1) {

			// Si la liste tabou n'est pas pleine.
			if (liste_Tabou.size() < taille_Liste_Tabou) {
				liste_Tabou.push_back(mouvement_Voisinage[indice_Meilleure_Solution]);
			}

			// Si la liste tabou est pleine.
			else {
				remplacer_Plus_Ancien_Mouvement(&liste_Tabou, mouvement_Voisinage[indice_Meilleure_Solution]);
			}

			// On regarde si cette meilleure soluton a une meilleur F.O que la solution ou une F.O proche.
			if (solution_Voisinage[indice_Meilleure_Solution].i_valeur_fonction_objectif > solution->i_valeur_fonction_objectif) {

				// On prends donc la meilleur solution.
				*solution = solution_Voisinage[indice_Meilleure_Solution];
				solution_Actuel = *solution;

				// Phase d'intensification.
				taille_Liste_Tabou = 500;
				liste_Tabou.clear();
				ite = 0;
			}

			// Cas o� la meilleure solution du voisinage n'a pas une meilleure F.O que la solution.
			else {
				if (ite > 500) {
					// Phase de diversification totale.
					solution_Actuel = generer_Solution_Aleatoire();
					taille_Liste_Tabou = 10000;
					ite = 0;
				}

				else if (ite > 200) {

					// Phase de diversification moyenne.
					solution_Actuel = generer_Solution_Reset_Jour_Score_Min(solution_Voisinage[indice_Meilleure_Solution]);
					ite++;

				}

				else {
					solution_Actuel = solution_Voisinage[indice_Meilleure_Solution];
					ite++;
				}
			}
		}

		else {
			// Phase de diversification totale.
			solution_Actuel = generer_Solution_Aleatoire();
			ite = 0;
			taille_Liste_Tabou = 10000;
		}
	}
	return *solution;
}


/* GENERER_VOISINAGE : Permets d'obtenir toutes les solutions voisines d'une solution.
* Entr�e : - solution, Solution, solution � partir duquel on va obtenir le voisinage.
*		   - solution_Voisinage, vector<Solution>*, pointeur vers le vector qui va contenir toutes les solutions voisines.
*		   - mouvement_Voisinage, vector<vector<int>>*, pointeur vers le vector qui va contenir tous les mouvements utilis�s pour les solutions voisines.
* N�cessite : Rien.
* Sortie : Rien.
* Entra�ne : Remplir solution_Voisinage et mouvement_Voisinage avec les voisinage de la solution ainsi que les mouvements associ�s.
*/
void MetaHeuristiques::generer_Voisinage(Solution solution_Actuel, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage) {

	generer_Voisinage_Operateur_SWAP_Hotel(solution_Actuel, solution_Voisinage, mouvement_Voisinage);
	generer_Voisinage_Operateur_Ajouter_POI(solution_Actuel, solution_Voisinage, mouvement_Voisinage);
	generer_Voisinage_Operateur_SWAP_POI(solution_Actuel, solution_Voisinage, mouvement_Voisinage);
}

/* GENERER_VOISINAGE_OPERATEUR_SWAP_HOTEL : Permets d'obtenir les solutions voisines d'une solution � partir de l'op�rateur "SWAP h�tel".
* Entr�e : - solution, Solution, solution � partir duquel on va obtenir le voisinage.
*		   - solution_Voisinage, vector<Solution>*, pointeur vers le vector o� l'on va ajouter toutes les solutions voisines.
*		   - mouvement_Voisinage, vector<vector<int>>*, pointeur vers le vector o� l'on va ajouter tous les mouvements utilis�s pour les solutions voisines.
* N�cessite : Rien.
* Sortie : Rien.
* Entra�ne : Remplir solution_Voisinage et mouvement_Voisinage avec les voisinages de la solution ainsi que les mouvements associ�s � partir de l'op�rateur "SWAP h�tel".
*/
void MetaHeuristiques::generer_Voisinage_Operateur_SWAP_Hotel(Solution solution_Actuel, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage) {

	// On parcours tous les jours.
	for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour() - 1; num_Jour++) {

		// On parcours tous les h�tels.
		for (int id_Hotel = 0; id_Hotel < instance->get_Nombre_Hotel(); id_Hotel++) {

			// On v�rifie que l'h�tel actuel n'est pas d�j� celui de la solution.
			if (id_Hotel != solution_Actuel.v_Id_Hotel_Intermedaire[num_Jour]) {

				// On cr�e la solution � partir de la solution initiale.
				Solution solution_Temporaire = solution_Actuel;

				// On switch l'h�tel.
				solution_Temporaire.v_Id_Hotel_Intermedaire[num_Jour] = id_Hotel;

				// On v�rifie que la solution est r�alisable.
				if (solution_Temporaire.Verification_Solution(instance, false)) {

					// Mouvement de la solution. Le premier �l�ment est un 0 afin de diff�rencier les mouvements "Swap Hotel" et "Swap POI".
					vector<int> mouvement = { 0 , solution_Actuel.v_Id_Hotel_Intermedaire[num_Jour] , id_Hotel , num_Jour };
					mouvement_Voisinage->push_back(mouvement);

					// On ajotue la solution dans le voisinage.
					solution_Voisinage->push_back(solution_Temporaire);
				}
			}
		}
	}
}

/* GENERER_VOISINAGE_OPERATEUR_AJOUTER_POI : Permets d'obtenir les solutions voisines d'une solution � partir de l'op�rateur "Ajouter POI".
* Entr�e : - solution, Solution, solution � partir duquel on va obtenir le voisinage.
*		   - solution_Voisinage, vector<Solution>*, pointeur vers le vector o� l'on va ajouter toutes les solutions voisines.
*		   - mouvement_Voisinage, vector<vector<int>>*, pointeur vers le vector o� l'on va ajouter tous les mouvements utilis�s pour les solutions voisines.
* N�cessite : Rien.
* Sortie : Rien.
* Entra�ne : Remplir solution_Voisinage et mouvement_Voisinage avec les voisinages de la solution ainsi que les mouvements associ�s � partir de l'op�rateur "Ajouter POI".
*/
void MetaHeuristiques::generer_Voisinage_Operateur_Ajouter_POI(Solution solution_Actuel, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage) {

	// On parcours tous les jours.
	for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour(); num_Jour++) {

		// On parcours tous les POI de l'instance.
		for (int id_POI_Instance = 0; id_POI_Instance < instance->get_Nombre_POI(); id_POI_Instance++) {

			// Pour chaque position o� l'on peut ajouter le POI.
			for (int position = 0; position < solution_Actuel.v_v_Sequence_Id_Par_Jour.size(); position++) {
				// On cr�e la solution � partir de la solution initiale.
				Solution solution_Temporaire = solution_Actuel;

				// On switch le POI.
				solution_Temporaire.v_v_Sequence_Id_Par_Jour[num_Jour].push_back(id_POI_Instance);

				// On mets � jour la F.O.
				solution_Temporaire.i_valeur_fonction_objectif += instance->get_POI_Score(id_POI_Instance);

				// On doit changer la date de d�part si l'on a ajout� le POI en 1ere position.
				if (position == 0) {
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

				// On v�rifie que la solution est r�alisable.
				if (solution_Temporaire.Verification_Solution(instance, false)) {

					// Mouvement de la solution, ce mouvement ne sera cepedant jamasi dans la liste tabou par logique.
					vector<int> mouvement = { position, id_POI_Instance, num_Jour };
					mouvement_Voisinage->push_back(mouvement);

					// On ajotue la solution dans le voisinage.
					solution_Voisinage->push_back(solution_Temporaire);
				}
			}
		}
	}
}

/* GENERER_VOISINAGE_OPERATEUR_SWAP_POI : Permets d'obtenir les solutions voisines d'une solution � partir de l'op�rateur "SWAP POI".
* Entr�e : - solution, Solution, solution � partir duquel on va obtenir le voisinage.
*		   - solution_Voisinage, vector<Solution>*, pointeur vers le vector o� l'on va ajouter toutes les solutions voisines.
*		   - mouvement_Voisinage, vector<vector<int>>*, pointeur vers le vector o� l'on va ajouter tous les mouvements utilis�s pour les solutions voisines.
* N�cessite : Rien.
* Sortie : Rien.
* Entra�ne : Remplir solution_Voisinage et mouvement_Voisinage avec les voisinages de la solution ainsi que les mouvements associ�s � partir de l'op�rateur "SWAP POI".
*/
void MetaHeuristiques::generer_Voisinage_Operateur_SWAP_POI(Solution solution_Actuel, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage) {

	// On parcours tous les jours.
	for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour(); num_Jour++) {

		// On parcours tous les POI de la solution.
		for (int indice_POI_Solution = 0; indice_POI_Solution < solution_Actuel.v_v_Sequence_Id_Par_Jour[num_Jour].size(); indice_POI_Solution++) {

			// On parcours tous les POI de l'instance.
			for (int id_POI_Instance = 0; id_POI_Instance < instance->get_Nombre_POI(); id_POI_Instance++) {

				// On v�rifie que le POI de l'instance n'est pas d�j� celui de la solution.
				if (id_POI_Instance != solution_Actuel.v_v_Sequence_Id_Par_Jour[num_Jour][indice_POI_Solution]) {

					// On cr�e la solution � partir de la solution initiale.
					Solution solution_Temporaire = solution_Actuel;

					// On switch le POI.
					solution_Temporaire.v_v_Sequence_Id_Par_Jour[num_Jour][indice_POI_Solution] = id_POI_Instance;

					// On mets � jour la F.O.
					solution_Temporaire.i_valeur_fonction_objectif -= instance->get_POI_Score(solution_Actuel.v_v_Sequence_Id_Par_Jour[num_Jour][indice_POI_Solution]);
					solution_Temporaire.i_valeur_fonction_objectif += instance->get_POI_Score(id_POI_Instance);

					// Si l'on modifie le premier POI, il faut modifier la date de d�part du jour.
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

					// On v�rifie que la solution est r�alisable.
					if (solution_Temporaire.Verification_Solution(instance, false)) {

						// Mouvement de la solution. Le premier �l�ment est un 1 afin de diff�rencier les mouvements "Swap Hotel" et "Swap POI".
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

/* GET_INDICE_MEILLEURE_SOLUTION_VOISINAGE : Permets d'obtenir l'indice de la meilleure solution du voisage d'une solution qui n'est pas dans la liste tabou.
* Entr�e : - solution_Voisinage, vector<Solution>, voisinage dans lequel on va chercher la meilleure solution n'�tant pas dans la liste tabou.
*		   - mouvement_Voisinage, vector<vector<int>>, mouvement des solutions du voisinage pour v�rifier avec la liste tabou.
*		   - liste_Tabou, vector<vector<int>>, liste tabou des mouvements
* N�cessite : Rien.
* Sortie : - L'indice de la meilleure solution du voisinage qui n'est pas dans la liste tabou.
*		   - -1 si tout le voisinage est dans la liste tabou.
* Entra�ne : Rien.
*/
int MetaHeuristiques::get_Indice_Meilleure_Solution_Voisinage(vector<Solution> solution_Voisinage, vector<vector<int>> mouvement_Voisinage, vector<vector<int>> liste_Tabou) {

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

/* IS_MOUVEMENT_DANS_LISTE_TABOU : Permets de savoir si un mouvement est dans la liste tabou ou non.
* Entr�e : - mouvement, vector<int>, mouvement que l'on v�rifier s'il est dans la liste tabou.
*		   - liste_Tabou, vector<vector<int>>, liste tabou des mouvements.
* N�cessite : Rien.
* Sortie : - true si le mouvement est dans la liste tabou.
*	       - false sinon.
* Entra�ne : Rien.
*/
bool MetaHeuristiques::is_Mouvement_Dans_Liste_Tabou(vector<int> mouvement, vector<vector<int>> liste_Tabou) {

	// On parcours tous les mouvements de la liste tabou.
	for (vector<int> mouvement_Tabou : liste_Tabou) {

		// On regarde si ce mouvement est le m�me que le mouvement en param�tre.
		if (mouvement == mouvement_Tabou) {
			return true;
		}
	}
	return false;
}

/* REMPLACER_PLUS_ANCIEN_MOUVEMENT : Permets de supprimer le plus ancien mouvement de la liste tabou et d'ajouter un mouvement.
* Entr�e : - liste_Tabou, vector<vector<int>>*, pointeur vers la liste tabou que l'on veut modifier.
*		   - mouvement_Tabou, vector<int>, mouvement que l'on veut ajouter � la liste tabou.
* N�cessite : Rien.
* Sortie : Rien.
* Entra�ne : Supprime le 1er �l�ment de la liste tabou et y ajoute le mouvement tabou.
*/
void MetaHeuristiques::remplacer_Plus_Ancien_Mouvement(vector<vector<int>>* liste_Tabou, vector<int> mouvement_Tabou) {

	// On supprime le premier �l�ment de la liste tabou.
	liste_Tabou->erase(liste_Tabou->begin());

	// On ajoute le mouvement tabou.
	liste_Tabou->push_back(mouvement_Tabou);
}

/* GENERER_SOLUTION_ALEATOIRE : G�n�re une solution al�atoire r�alisable avec un POI par jour.
* Entr�e : Rien.
* N�cessite : Rien.
* Sortie : La solution al�atoire r�alisable avec un POI par jour.
* Entra�ne : Rien.
*/
Solution MetaHeuristiques::generer_Solution_Aleatoire() {
	Solution solution_Aleatoire;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis_Hotel(0, instance->get_Nombre_Hotel() - 1);
	uniform_int_distribution<> dis_POI(0, instance->get_Nombre_POI() - 1);

	// Tant que l'on a pas de solution faisable on en recr�e une.
	while (!solution_Aleatoire.Verification_Solution(instance, false)) {

		// On vide la solution.
		solution_Aleatoire.v_Id_Hotel_Intermedaire.clear();
		solution_Aleatoire.v_v_Sequence_Id_Par_Jour.clear();
		solution_Aleatoire.i_valeur_fonction_objectif = 0;
		solution_Aleatoire.v_Date_Depart.clear();

		// Ajout des l'h�tels al�atoire.
		for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour() - 1; num_Jour++) {
			solution_Aleatoire.v_Id_Hotel_Intermedaire.push_back(dis_Hotel(gen));
		}

		// Ajout des POIs al�atoire.
		for (int num_Jour = 0; num_Jour < instance->get_Nombre_Jour(); num_Jour++) {

			ajouter_POI_Solution(&solution_Aleatoire, num_Jour, dis_POI(gen));
		}
	}

	return solution_Aleatoire;
}

/* GENERER_SOLUTION_RESET_JOUR_SCORE : G�n�re une solution r�alisable � partir d'une solution en rempla�ant les POI du jour avec le score minimum par un POI al�atoire.
* Entr�e : solution, Solution, solution � partir de laquelle on va g�n�rer la solution.
* N�cessite : Rien.
* Sortie : La solution r�alisable g�n�rer.
* Entra�ne : Rien.
*/
Solution MetaHeuristiques::generer_Solution_Reset_Jour_Score_Min(Solution solution) {

	int num_Jour_Score_Min = -1;
	int score_Min_Jour = 10000;

	// on r�cup�re le num jour du score min.
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

		if (!solution_Modif.Verification_Solution(instance, false));
			return solution_Modif;
		}
}

/* AJOUTER_POI_SOLUTION : Permets d'ajouter un POI � une solution et de modifier les param�tres de la solution en fonction de ce POI.
* Entr�e : - solution, Solution, solution o� l'on ajouter le POI.
*		   - num_Jour, int, num�ro du jour auquel on veut ajouter le POI.
*		   - id_POI, int, id du POI que l'on veut ajouter.
* N�cessite : Rien.
* Sortie : Rien.
* Entra�ne : Ajoute le POI au jour voulu et modifier les param�tres de la solution.
*/
void MetaHeuristiques::ajouter_POI_Solution(Solution* solution, int num_Jour, int id_POI) {

	// Ajout du POI al�atoirement.
	vector<int> vector_id_POI_Jour;
	vector_id_POI_Jour.push_back(id_POI);
	solution->v_v_Sequence_Id_Par_Jour.push_back(vector_id_POI_Jour);

	float heure_Ouverture = instance->get_POI_Heure_ouverture(vector_id_POI_Jour[0]);
	float heure_Fermeture = instance->get_POI_Heure_fermeture(vector_id_POI_Jour[0]);

	float distance_POI_Actuel_Et_POI_Courant;

	if (num_Jour != 0) {
		distance_POI_Actuel_Et_POI_Courant = instance->get_distance_Hotel_POI(solution->v_Id_Hotel_Intermedaire[num_Jour - 1], vector_id_POI_Jour[0]);
	}
	else {
		distance_POI_Actuel_Et_POI_Courant = instance->get_distance_Hotel_POI(instance->get_Id_Hotel_depart(), vector_id_POI_Jour[0]);
	}

	// Ajout de la date de d�part.
	if (heure_Ouverture - distance_POI_Actuel_Et_POI_Courant >= 0) {
		solution->v_Date_Depart.push_back(heure_Ouverture - distance_POI_Actuel_Et_POI_Courant);
	}
	else {
		solution->v_Date_Depart.push_back(heure_Fermeture - distance_POI_Actuel_Et_POI_Courant);
	}

	// MAJ de la F.O.
	solution->i_valeur_fonction_objectif += instance->get_POI_Score(vector_id_POI_Jour[0]);
}