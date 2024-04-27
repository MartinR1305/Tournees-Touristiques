#ifndef Solution_hpp
#define Solution_hpp

#include "Instance.h"
#include <stdio.h>
#include <vector>

using namespace std;

class Solution
{
public:// /!\ Normalement pour coder "proprepement" il faudrait mettre ces attributs en priv�e et passer par des fonctions setters/getters pour les modifier. Cependant, dans une m�thode de r�solution, les op�rations sur les solutions (ajout d'id POI, supression d'ID, etc.) sont tr�s tr�s nombreuses, passer par des setters/getters (qui en plus v�rifie certaines op�rations) est tr�s co�teux en temps de calcul. En optimisation, on �vite et manipule directement les solutions (autres m�thodes, passer par les inline mais �a reste lourd).
    vector<int> v_Id_Hotel_Intermedaire;            // Donne l'Id des h�tels entre deux jours (ne consid�re pas l'h�tel d�part et d'arriv�). La taille du tableau est �gale au nombre de jours - 1.
    vector<vector<int>> v_v_Sequence_Id_Par_Jour;   // Donne la s�quence d'Id des POIs visit�s par jour (v_v_Sequence_Id_Par_Jour[d] : s�quence de d'Id des POIs visit�s le jour d). La taille de la premi�re dimension du tableau est �gale au nombre de jours.
    vector<float> v_Date_Depart;                    // Donne la date/heure de d�part de l'h�tel de chaque jour. La taille du tableau est �gale au nombre de jours.
    int i_valeur_fonction_objectif;                 // Valeur de la fonction objectif (somme des scores des POIs visit�s

public:
    /* Constructeurs et destructeur  */
    Solution();
    virtual ~Solution();
    bool Verification_Solution(Instance* instance, bool affiche_Message_Erreur);
};

#endif
