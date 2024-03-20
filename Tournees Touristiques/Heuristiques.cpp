#include "Heuristiques.h"

Solution Heuristiques::methode_Heuristique_V1(Instance instance) {
	int id_Hotel_Dep = instance.get_Id_Hotel_depart();
	int id_Hotel_Arr = instance.get_Id_Hotel_Arrivee();

	int nb_Jour = instance.get_Nombre_Jour();
	int nb_POI = instance.get_Nombre_POI();
	int nb_Hotel = instance.get_Nombre_Hotel();

	bool on_Peut_Ajouter_un_POI = true;

	// On crée notre solution et on y ajoute le premier hôtel.
	Solution *solution = new Solution();
	solution->v_Id_Hotel_Intermedaire.push_back(id_Hotel_Dep);

	for (int num_Jour = 0; num_Jour < nb_Jour; num_Jour++) {

		on_Peut_Ajouter_un_POI = true;
		float duree_Jour_Actuel = instance.get_POI_Duree_Max_Voyage(num_Jour);

		while (on_Peut_Ajouter_un_POI) {
			for (int id_POI = 0; id_POI < nb_POI; id_POI++) {
				// Si on peut l'ajouter.


				// Si on ne peut pas l'ajouter.
			}
		}

		// On ajoute l'hôtel le plus proche.
	}


	for (int POI = 0; POI < nb_POI; POI++) {
		float duree_Max_Jour = instance.get_POI_Duree_Max_Voyage(0);
	}

}