#include "MetaHeuristiques.h"

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

	while (nb_Ite < 1000) {
		map<Solution, vector<int>> voisinage_Avec_Mouvement = generer_Voisinage();
		Solution* meilleur_Solution = get_Meilleur_Solution(voisinage_Avec_Mouvement, liste_Tabou);
		//liste_Tabou.push_back();

	}
	return *solution;
}

map<Solution, vector<int>> MetaHeuristiques::generer_Voisinage() {

}

Solution* MetaHeuristiques::get_Meilleur_Solution(map<Solution, vector<int>> voisinage, vector<vector<int>> liste_Tabou) {

}