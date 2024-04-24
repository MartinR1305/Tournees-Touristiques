#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include "Instance.h"
#include "Solution.h"
#include "Heuristiques.h"

#ifndef MetaHeuristiques_h
#define MetaHeuristiques_h

using namespace std;

class MetaHeuristiques
{
private:
	Instance* instance;

public:

	/* METAHEURISTIQUES : Constructeur par défaut.
	 * Entrée : Rien.
	 * Nécessite : Rien.
	 * Sortie : Rien.
	 * Entraîne : Crée un objet de la classe MetaHeuristiques avec un pointeur d'instance null.
	*/
	MetaHeuristiques();

	/* METAHEURISTIQUES : Constructeur de confort.
	* Entrée : instance_Param, Instance*, pointeur vers un objet de la classe instance.
	* Nécessite : Rien.
	* Sortie : Rien.
	* Entraîne : Crée un objet de la classe MetaHeuristiques avec le pointeur d'instance voulu.
	*/
	MetaHeuristiques(Instance* instance_Param);

	/* ~METAHEURISTIQUES : Destructeur.
	* Entrée : Rien.
	* Nécessite : Rien.
	* Sortie : Rien.
	* Entraîne : Désalloue le pointeur de l'instance.
	*/
	~MetaHeuristiques();

	/* RECHERCHE_TABOU : Réalise une recherche tabou à partir de l'instance afin de chercher la meilleure solution possible.
	* Entrée : Rien.
	* Nécessite : Rien.
	* Sortie : La meilleure solution que la recherche tabou à trouver.
	* Entraîne : Rien.
	*/
	Solution recherche_Tabou();

	/* GENERER_VOISINAGE : Permets d'obtenir toutes les solutions voisines d'une solution.
	* Entrée : - solution, Solution, solution à partir duquel on va obtenir le voisinage.
	*		   - solution_Voisinage, vector<Solution>*, pointeur vers le vector qui va contenir toutes les solutions voisines.
	*		   - mouvement_Voisinage, vector<vector<int>>*, pointeur vers le vector qui va contenir tous les mouvements utilisés pour les solutions voisines.
	* Nécessite : Rien.
	* Sortie : Rien.
	* Entraîne : Remplir solution_Voisinage et mouvement_Voisinage avec les voisinages de la solution ainsi que les mouvements associés.
	*/
	void generer_Voisinage(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage);

	/* GENERER_VOISINAGE_OPERATEUR_SWAP_HOTEL : Permets d'obtenir les solutions voisines d'une solution à partir de l'opérateur "SWAP hôtel".
	* Entrée : - solution, Solution, solution à partir duquel on va obtenir le voisinage.
	*		   - solution_Voisinage, vector<Solution>*, pointeur vers le vector où l'on va ajouter toutes les solutions voisines.
	*		   - mouvement_Voisinage, vector<vector<int>>*, pointeur vers le vector où l'on va ajouter tous les mouvements utilisés pour les solutions voisines.
	* Nécessite : Rien.
	* Sortie : Rien.
	* Entraîne : Remplir solution_Voisinage et mouvement_Voisinage avec les voisinages de la solution ainsi que les mouvements associés à partir de l'opérateur SWAP hôtel.
	*/
	void generer_Voisinage_Operateur_SWAP_Hotel(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage);

	/* GENERER_VOISINAGE_OPERATEUR_AJOUTER_POI : Permets d'obtenir les solutions voisines d'une solution à partir de l'opérateur "Ajouter POI".
	* Entrée : - solution, Solution, solution à partir duquel on va obtenir le voisinage.
	*		   - solution_Voisinage, vector<Solution>*, pointeur vers le vector où l'on va ajouter toutes les solutions voisines.
	*		   - mouvement_Voisinage, vector<vector<int>>*, pointeur vers le vector où l'on va ajouter tous les mouvements utilisés pour les solutions voisines.
	* Nécessite : Rien.
	* Sortie : Rien.
	* Entraîne : Remplir solution_Voisinage et mouvement_Voisinage avec les voisinages de la solution ainsi que les mouvements associés à partir de l'opérateur "Ajouter POI".
	*/
	void generer_Voisinage_Operateur_Ajouter_POI(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage);

	/* GENERER_VOISINAGE_OPERATEUR_SWAP_POI : Permets d'obtenir les solutions voisines d'une solution à partir de l'opérateur "SWAP POI".
	* Entrée : - solution, Solution, solution à partir duquel on va obtenir le voisinage.
	*		   - solution_Voisinage, vector<Solution>*, pointeur vers le vector où l'on va ajouter toutes les solutions voisines.
	*		   - mouvement_Voisinage, vector<vector<int>>*, pointeur vers le vector où l'on va ajouter tous les mouvements utilisés pour les solutions voisines.
	* Nécessite : Rien.
	* Sortie : Rien.
	* Entraîne : Remplir solution_Voisinage et mouvement_Voisinage avec les voisinages de la solution ainsi que les mouvements associés à partir de l'opérateur "SWAP POI".
	*/
	void generer_Voisinage_Operateur_SWAP_POI(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage);

	/* GET_INDICE_MEILLEURE_SOLUTION_VOISINAGE : Permets d'obtenir l'indice de la meilleure solution du voisage d'une solution qui n'est pas dans la liste tabou.
	* Entrée : - solution_Voisinage, vector<Solution>, voisinage dans lequel on va chercher la meilleure solution n'étant pas dans la liste tabou.
	*		   - mouvement_Voisinage, vector<vector<int>>, mouvement des solutions du voisinage pour vérifier avec la liste tabou.
	*		   - liste_Tabou, vector<vector<int>>, liste tabou des mouvements
	* Nécessite : Rien.
	* Sortie : - L'indice de la meilleure solution du voisinage qui n'est pas dans la liste tabou.
	*		   - -1 si tout le voisinage est dans la liste tabou.
	* Entraîne : Rien.
	*/
	int get_Indice_Meilleure_Solution_Voisinage(vector<Solution> solution_Voisinage, vector<vector<int>> mouvement_Voisinage, vector<vector<int>> liste_Tabou);

	/* IS_MOUVEMENT_DANS_LISTE_TABOU : Permets de savoir si un mouvement est dans la liste tabou ou non.
	* Entrée : - mouvement, vector<int>, mouvement que l'on vérifier s'il est dans la liste tabou.
	*		   - liste_Tabou, vector<vector<int>>, liste tabou des mouvements.
	* Nécessite : Rien.
	* Sortie : - true si le mouvement est dans la liste tabou.
	*	       - false sinon.
	* Entraîne : Rien.
	*/
	bool is_Mouvement_Dans_Liste_Tabou(vector<int> mouvement, vector<vector<int>> liste_Tabou);

	/* REMPLACER_PLUS_ANCIEN_MOUVEMENT : Permets de supprimer le plus ancien mouvement de la liste tabou et d'ajouter un mouvement.
	* Entrée : - liste_Tabou, vector<vector<int>>*, pointeur vers la liste tabou que l'on veut modifier.
	*		   - mouvement_Tabou, vector<int>, mouvement que l'on veut ajouter à la liste tabou.
	* Nécessite : Rien.
	* Sortie : Rien.
	* Entraîne : Supprime le 1er élément de la liste tabou et y ajoute le mouvement tabou.
	*/
	void remplacer_Plus_Ancien_Mouvement(vector<vector<int>>* liste_Tabou, vector<int> mouvement_Tabou);

	/* GENERER_SOLUTION_ALEATOIRE : Génère une solution aléatoire réalisable avec un POI par jour.
	* Entrée : Rien.
	* Nécessite : Rien.
	* Sortie : La solution aléatoire réalisable avec un POI par jour.
	* Entraîne : Rien.
	*/
	Solution generer_Solution_Aleatoire();

	/* GENERER_SOLUTION_RESET_JOUR_SCORE : Génère une solution réalisable à partir d'une solution en remplaçant les POI du jour avec le score minimum par un POI aléatoire.
	* Entrée : solution, Solution, solution à partir de laquelle on va générer la solution.
	* Nécessite : Rien.
	* Sortie : La solution réalisable générer.
	* Entraîne : Rien.
	*/
	Solution generer_Solution_Reset_Jour_Score_Min(Solution solution);

	/* AJOUTER_POI_SOLUTION : Permets d'ajouter un POI à une solution et de modifier les paramètres de la solution en fonction de ce POI.
	* Entrée : - solution, Solution, solution où l'on ajouter le POI.
	*		   - num_Jour, int, numéro du jour auquel on veut ajouter le POI.
	*		   - id_POI, int, id du POI que l'on veut ajouter.
	* Nécessite : Rien.
	* Sortie : Rien.
	* Entraîne : Ajoute le POI au jour voulu et modifier les paramètres de la solution.
	*/
	void ajouter_POI_Solution(Solution* solution, int num_Jour, int id_POI);
};

#endif