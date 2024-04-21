#define CHEMIN_DOSSIER_DONNEES "../Format Etudiant Public/"
#define NOM_FICHIER_LISTE_FICHIER_DONNEES "data.txt"
#define NOM_FICHIER_LISTE_SORTIE "sortie.txt"

#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include "Instance.h"
#include "Solution.h"
#include "Heuristiques.h"
#include "MetaHeuristiques.h"

using namespace std;

int Resolution(Instance* instance);


int main(int argc, const char* argv[])
{
	try
	{
		string s_tmp;
		string s_chemin = CHEMIN_DOSSIER_DONNEES;
		s_chemin.append(NOM_FICHIER_LISTE_FICHIER_DONNEES);

		ifstream fichier(s_chemin.c_str(), std::ios::in); std::ofstream fichier_Sortie_Resume;
		s_chemin = CHEMIN_DOSSIER_DONNEES;
		s_chemin.append(NOM_FICHIER_LISTE_SORTIE);
		ofstream fichier_Sortie(s_chemin.c_str(), std::ios::out | std::ios::app);

		if (fichier)
		{
			if (fichier_Sortie)
			{
				fichier_Sortie << " Fichier données\t Tps de résolution \t Best solution" << endl;
				getline(fichier, s_tmp);
				while (s_tmp != "")
				{
					Instance* instance = new Instance();
					chrono::time_point<chrono::system_clock> chrono_start, chrono_end;
					chrono::duration<double> elapsed;
					int i_best_solution_score = 0;
					s_chemin = CHEMIN_DOSSIER_DONNEES;
					cout << " Résolution de " << s_tmp << endl;
					s_chemin.append(s_tmp);
					s_chemin.erase(remove(s_chemin.begin(), s_chemin.end(), '\r'), s_chemin.end());
					s_chemin.erase(remove(s_chemin.begin(), s_chemin.end(), '\n'), s_chemin.end());

					instance->chargement_Instance(s_chemin);
					chrono_start = chrono::system_clock::now();
					i_best_solution_score = Resolution(instance);
					cout << " Fin de résolution de " << s_tmp << endl;
					chrono_end = chrono::system_clock::now();
					elapsed = chrono_end - chrono_start;
					fichier_Sortie << s_chemin << "\t" << elapsed.count() << "\t" << i_best_solution_score << endl;
					s_tmp = "";
					getline(fichier, s_tmp);
					delete instance;
				}
				fichier_Sortie.close();
			}
			else
			{
				cout << " Erreur lecture des données : chemin vers la sortie non valide. " << endl;
			}
			fichier.close();
		}
		else
		{
			cout << " Erreur lecture des données : chemin listant l'ensemble des données non valide. " << endl;
		}
	}

	catch (string err)
	{
		cout << "Erreur fatale : " << endl;
		cout << err << endl;
	}
	return 0;
}

int Resolution(Instance* instance)
{
	int i_val_Retour_Fct_obj = 0;
	Solution* uneSolution = new Solution();
	vector<int> v_i_tmp;
	Heuristiques* heuristique = new Heuristiques(instance);
	*uneSolution = heuristique->methode_Heuristique();
	delete heuristique;

	/*INITIALISATION D'UN SOLUTION EN DUR
	v_i_tmp.clear();
	uneSolution->v_Id_Hotel_Intermedaire.push_back(2);
	uneSolution->v_Date_Depart.push_back(0.0);
	uneSolution->v_Date_Depart.push_back(0.0);
	v_i_tmp = { 0, 2, 5, 9, 14, 21, 28, 20, 27, 35, 42, 36, 29, 22, 30, 31 };
	uneSolution->v_v_Sequence_Id_Par_Jour.push_back(v_i_tmp);
	v_i_tmp = { 24, 32, 40, 33, 25, 19, 26, 34, 41, 47, 52, 56, 59, 61 };
	uneSolution->v_v_Sequence_Id_Par_Jour.push_back(v_i_tmp);
	uneSolution->i_valeur_fonction_objectif = 816;
	 */

	//cout << "Hotel : ";
	//for (int i = 0; i < uneSolution->v_Id_Hotel_Intermedaire.size(); i++)
	//	cout << "[ " << uneSolution->v_Id_Hotel_Intermedaire[i] << " ]";
	//cout << endl;

	//cout << "POI : ";
	//for (int i = 0; i < instance->get_Nombre_Jour(); i++) {
	//	cout << "{ ";
	//	for (int j = 0; j < uneSolution->v_v_Sequence_Id_Par_Jour[i].size(); j++) {
	//		cout << "[ " << uneSolution->v_v_Sequence_Id_Par_Jour[i][j] << " ]";
	//	}
	//	cout << " } ";
	//}
	//cout << endl;

	//cout << "F.O : " << uneSolution->i_valeur_fonction_objectif << endl << endl;

	uneSolution->Verification_Solution(instance);

	vector<Solution> solution_Voisinage;
	vector<vector<int>> mouvement_Voisinage;

	MetaHeuristiques* metaheuristique = new MetaHeuristiques(instance);

	// On génère le voisinage à partir de la solution.
	metaheuristique->generer_Voisinage(*uneSolution, &solution_Voisinage, &mouvement_Voisinage);

	for (int solutionV = 0; solutionV < solution_Voisinage.size(); solutionV++) {

		cout << "Voisinage num : " << solutionV << endl;

		//for (int i = 0; i < solution_Voisinage[solutionV].v_Id_Hotel_Intermedaire.size(); i++) {
		//	cout << "[ " << solution_Voisinage[solutionV].v_Id_Hotel_Intermedaire[i] << " ]";
		//}

		for (int i = 0; i < instance->get_Nombre_Jour(); i++) {
			cout << "{ ";
			for (int j = 0; j < solution_Voisinage[solutionV].v_v_Sequence_Id_Par_Jour[i].size(); j++) {
				cout << "[ " << solution_Voisinage[solutionV].v_v_Sequence_Id_Par_Jour[i][j] << " ]";
			}
			cout << " } ";
		}

		cout << "\tMouvement : ";

		for (int j = 0; j < mouvement_Voisinage[solutionV].size(); j++) {
			cout << mouvement_Voisinage[solutionV][j] << " ";
		}

		cout << "\tF.O :" << solution_Voisinage[solutionV].i_valeur_fonction_objectif;

		cout << endl;
	}

	vector<vector<int>> liste_Tabou;
	int id_Meilleure_Solution = metaheuristique->get_Indice_Meilleure_Solution(solution_Voisinage, mouvement_Voisinage, liste_Tabou);
	cout << "ID : Meilleure solution voisine : " << id_Meilleure_Solution << endl;

	i_val_Retour_Fct_obj = uneSolution->i_valeur_fonction_objectif;
	delete uneSolution;
	return i_val_Retour_Fct_obj;
}

