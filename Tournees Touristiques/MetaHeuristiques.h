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

	/* METAHEURISTIQUES : Constructeur par d�faut.
	 * Entr�e : Rien.
	 * N�cessite : Rien.
	 * Sortie : Rien.
	 * Entra�ne : Cr�e un objet de la classe MetaHeuristiques avec un pointeur d'instance null.
	*/
	MetaHeuristiques();

	/* METAHEURISTIQUES : Constructeur de confort.
	* Entr�e : instance_Param, Instance*, pointeur vers un objet de la classe instance.
	* N�cessite : Rien.
	* Sortie : Rien.
	* Entra�ne : Cr�e un objet de la classe MetaHeuristiques avec le pointeur d'instance voulu.
	*/
	MetaHeuristiques(Instance* instance_Param);

	/* ~METAHEURISTIQUES : Destructeur.
	* Entr�e : Rien.
	* N�cessite : Rien.
	* Sortie : Rien.
	* Entra�ne : D�salloue le pointeur de l'instance.
	*/
	~MetaHeuristiques();

	/* RECHERCHE_TABOU : R�alise une recherche tabou � partir de l'instance afin de chercher la meilleure solution possible.
	* Entr�e : Rien.
	* N�cessite : Rien.
	* Sortie : La meilleure solution que la recherche tabou � trouver.
	* Entra�ne : Rien.
	*/
	Solution recherche_Tabou();

	/* GENERER_VOISINAGE : Permets d'obtenir toutes les solutions voisines d'une solution.
	* Entr�e : - solution, Solution, solution � partir duquel on va obtenir le voisinage.
	*		   - solution_Voisinage, vector<Solution>*, pointeur vers le vector qui va contenir toutes les solutions voisines.
	*		   - mouvement_Voisinage, vector<vector<int>>*, pointeur vers le vector qui va contenir tous les mouvements utilis�s pour les solutions voisines.
	* N�cessite : Rien.
	* Sortie : Rien.
	* Entra�ne : Remplir solution_Voisinage et mouvement_Voisinage avec les voisinages de la solution ainsi que les mouvements associ�s.
	*/
	void generer_Voisinage(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage);

	/* GENERER_VOISINAGE_OPERATEUR_SWAP_HOTEL : Permets d'obtenir les solutions voisines d'une solution � partir de l'op�rateur "SWAP h�tel".
	* Entr�e : - solution, Solution, solution � partir duquel on va obtenir le voisinage.
	*		   - solution_Voisinage, vector<Solution>*, pointeur vers le vector o� l'on va ajouter toutes les solutions voisines.
	*		   - mouvement_Voisinage, vector<vector<int>>*, pointeur vers le vector o� l'on va ajouter tous les mouvements utilis�s pour les solutions voisines.
	* N�cessite : Rien.
	* Sortie : Rien.
	* Entra�ne : Remplir solution_Voisinage et mouvement_Voisinage avec les voisinages de la solution ainsi que les mouvements associ�s � partir de l'op�rateur SWAP h�tel.
	*/
	void generer_Voisinage_Operateur_SWAP_Hotel(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage);

	/* GENERER_VOISINAGE_OPERATEUR_AJOUTER_POI : Permets d'obtenir les solutions voisines d'une solution � partir de l'op�rateur "Ajouter POI".
	* Entr�e : - solution, Solution, solution � partir duquel on va obtenir le voisinage.
	*		   - solution_Voisinage, vector<Solution>*, pointeur vers le vector o� l'on va ajouter toutes les solutions voisines.
	*		   - mouvement_Voisinage, vector<vector<int>>*, pointeur vers le vector o� l'on va ajouter tous les mouvements utilis�s pour les solutions voisines.
	* N�cessite : Rien.
	* Sortie : Rien.
	* Entra�ne : Remplir solution_Voisinage et mouvement_Voisinage avec les voisinages de la solution ainsi que les mouvements associ�s � partir de l'op�rateur "Ajouter POI".
	*/
	void generer_Voisinage_Operateur_Ajouter_POI(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage);

	/* GENERER_VOISINAGE_OPERATEUR_SWAP_POI : Permets d'obtenir les solutions voisines d'une solution � partir de l'op�rateur "SWAP POI".
	* Entr�e : - solution, Solution, solution � partir duquel on va obtenir le voisinage.
	*		   - solution_Voisinage, vector<Solution>*, pointeur vers le vector o� l'on va ajouter toutes les solutions voisines.
	*		   - mouvement_Voisinage, vector<vector<int>>*, pointeur vers le vector o� l'on va ajouter tous les mouvements utilis�s pour les solutions voisines.
	* N�cessite : Rien.
	* Sortie : Rien.
	* Entra�ne : Remplir solution_Voisinage et mouvement_Voisinage avec les voisinages de la solution ainsi que les mouvements associ�s � partir de l'op�rateur "SWAP POI".
	*/
	void generer_Voisinage_Operateur_SWAP_POI(Solution solution, vector<Solution>* solution_Voisinage, vector<vector<int>>* mouvement_Voisinage);

	/* GET_INDICE_MEILLEURE_SOLUTION_VOISINAGE : Permets d'obtenir l'indice de la meilleure solution du voisage d'une solution qui n'est pas dans la liste tabou.
	* Entr�e : - solution_Voisinage, vector<Solution>, voisinage dans lequel on va chercher la meilleure solution n'�tant pas dans la liste tabou.
	*		   - mouvement_Voisinage, vector<vector<int>>, mouvement des solutions du voisinage pour v�rifier avec la liste tabou.
	*		   - liste_Tabou, vector<vector<int>>, liste tabou des mouvements
	* N�cessite : Rien.
	* Sortie : - L'indice de la meilleure solution du voisinage qui n'est pas dans la liste tabou.
	*		   - -1 si tout le voisinage est dans la liste tabou.
	* Entra�ne : Rien.
	*/
	int get_Indice_Meilleure_Solution_Voisinage(vector<Solution> solution_Voisinage, vector<vector<int>> mouvement_Voisinage, vector<vector<int>> liste_Tabou);

	/* IS_MOUVEMENT_DANS_LISTE_TABOU : Permets de savoir si un mouvement est dans la liste tabou ou non.
	* Entr�e : - mouvement, vector<int>, mouvement que l'on v�rifier s'il est dans la liste tabou.
	*		   - liste_Tabou, vector<vector<int>>, liste tabou des mouvements.
	* N�cessite : Rien.
	* Sortie : - true si le mouvement est dans la liste tabou.
	*	       - false sinon.
	* Entra�ne : Rien.
	*/
	bool is_Mouvement_Dans_Liste_Tabou(vector<int> mouvement, vector<vector<int>> liste_Tabou);

	/* REMPLACER_PLUS_ANCIEN_MOUVEMENT : Permets de supprimer le plus ancien mouvement de la liste tabou et d'ajouter un mouvement.
	* Entr�e : - liste_Tabou, vector<vector<int>>*, pointeur vers la liste tabou que l'on veut modifier.
	*		   - mouvement_Tabou, vector<int>, mouvement que l'on veut ajouter � la liste tabou.
	* N�cessite : Rien.
	* Sortie : Rien.
	* Entra�ne : Supprime le 1er �l�ment de la liste tabou et y ajoute le mouvement tabou.
	*/
	void remplacer_Plus_Ancien_Mouvement(vector<vector<int>>* liste_Tabou, vector<int> mouvement_Tabou);

	/* GENERER_SOLUTION_ALEATOIRE : G�n�re une solution al�atoire r�alisable avec un POI par jour.
	* Entr�e : Rien.
	* N�cessite : Rien.
	* Sortie : La solution al�atoire r�alisable avec un POI par jour.
	* Entra�ne : Rien.
	*/
	Solution generer_Solution_Aleatoire();

	/* GENERER_SOLUTION_RESET_JOUR_SCORE : G�n�re une solution r�alisable � partir d'une solution en rempla�ant les POI du jour avec le score minimum par un POI al�atoire.
	* Entr�e : solution, Solution, solution � partir de laquelle on va g�n�rer la solution.
	* N�cessite : Rien.
	* Sortie : La solution r�alisable g�n�rer.
	* Entra�ne : Rien.
	*/
	Solution generer_Solution_Reset_Jour_Score_Min(Solution solution);

	/* AJOUTER_POI_SOLUTION : Permets d'ajouter un POI � une solution et de modifier les param�tres de la solution en fonction de ce POI.
	* Entr�e : - solution, Solution, solution o� l'on ajouter le POI.
	*		   - num_Jour, int, num�ro du jour auquel on veut ajouter le POI.
	*		   - id_POI, int, id du POI que l'on veut ajouter.
	* N�cessite : Rien.
	* Sortie : Rien.
	* Entra�ne : Ajoute le POI au jour voulu et modifier les param�tres de la solution.
	*/
	void ajouter_POI_Solution(Solution* solution, int num_Jour, int id_POI);
};

#endif