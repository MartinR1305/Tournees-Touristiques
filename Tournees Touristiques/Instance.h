#ifndef Instance_hpp
#define Instance_hpp

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

class Instance
{
private:
    string s_chemin;            //Chemin du fichier de donn�es
    int i_Nombre_Hotel;         //Nombre d'hot�ls num�rot� (id) de 0 � i_Nombre_Hotel-1
    int i_Nombre_POI;           //Nombre de POIs num�rot� (id) de 0 � i_Nombre_POI-1
    int i_Nombre_Jour;          //Nombre de jours num�rot� (id) de 0 � i_Nombre_Jour-1

    vector<float> v_f_Duree_Max_Voyage_Par_Jour; //Dur�e maxiamel d'une journ�e (heure arriv� - heure d�part pour chaque jour)

    class POI
    {
    public:
        float f_coord_X;        //Coordonn�e X
        float f_coord_Y;        //Coordonn�e Y
        int i_score;            //Score du POI
        float f_heure_ouverture;//Heure d'ouverture
        float f_heure_fermeture;//Heure de fermeture
    };
    vector<POI> v_POI;          //L'ensemble des POIs (Id = position dans le tableau)

    int i_Id_Hotel_Depart;      //Id de l'h�tel de d�part
    int i_Id_Hotel_Arrivee;     //Id de l'h�tel d'arriv�e
    class Hotel
    {
    public:
        float f_coord_X;        //Coordonn�e X
        float f_coord_Y;        //Coordonn�e Y
    };
    vector<Hotel> v_Hotel;      //L'ensemble des h�tels

    vector<vector<float>> v_v_f_distances; // Matrice sym�trique de distance (Hotel puis POI)

public:
    /* Constructeurs et destructeur  */
    Instance();
    virtual ~Instance();

    /* Instancie les donn�es (�crase les donn�es pr�c�dentes), retourne VRAI si chargement correct, FAUX sinon */
    bool chargement_Instance(string s_chemin) throw(string);

    /* LES Getters */
    string get_chemin(void);
    int get_Nombre_Hotel(void);
    int get_Nombre_POI(void);
    int get_Nombre_Jour(void);
    float get_POI_Duree_Max_Voyage(int i_Id_jour) throw(string);
    float get_POI_Coord_X(int i_Id_POI) throw(string);
    float get_POI_Coord_Y(int i_Id_POI) throw(string);
    int get_POI_Score(int i_Id_POI) throw(string);
    float get_POI_Heure_ouverture(int i_Id_POI) throw(string);
    float get_POI_Heure_fermeture(int i_Id_POI) throw(string);
    int get_Id_Hotel_depart(void);
    int get_Id_Hotel_Arrivee(void);
    float get_Hotel_Coord_X(int i_Id_Hotel) throw(string);
    float get_Hotel_Coord_Y(int i_Id_Hotel) throw(string);
    float get_distance_Hotel_POI(int i_Id_Hotel, int i_Id_POI) throw(string);
    float get_distance_POI_POI(int i_Id_POI1, int i_Id_POI2) throw(string);
    float get_distance_Hotel_Hotel(int i_Id_Hotel1, int i_Id_Hotel2) throw(string);
};


#endif
