#include <iostream>
#include <cstdlib>
#include <ctime>
#include <time.h>

using namespace std;


const int VMAX = 9; // valeur maximal des objets
const int NMAXLIGS = 8;
const int NMAXCOLS = 8;
const int LIBRE = 0;
const int VMIROIR1 = -1;
const int VMIROIR2 = -2;
const int EVERTICAL = 1;
const int EHORIZ = 2;

const int BOBO = 9;
const int NJOUEURS = 3;


struct Grille
{
    int tab[NMAXLIGS][NMAXCOLS];
};

typedef Grille GrilleVisite;

struct Position
{
    int x;
    int y;
};

struct RParcours
{
    int nbObj;
    int ptsTotal;
    bool bonus;
};


struct Scores
{
    int score;
    int nbJoueur;
};

struct ParamsJeu{
    int nbLignes;
    int nbCol;
    int nbJoueurs;
    int nbObj;
    int nbMiroir;
    int bobo; // valeur du bonus
};

//-------------------------------------- Modelisation du plateau --------------------------------
void initialiserGrille(Grille &gr,const ParamsJeu &pj)
{
    for (int i = 0; i < pj.nbLignes; i++)
    {
        for (int j = 0; j < pj.nbCol; j++)
        {
            gr.tab[i][j] = 0;
        }
    }
}



//-------------------------------------- Modelisation d'une position --------------------------------
bool estValide(const Position &pos,const Grille &gr, const ParamsJeu &pj)
{
    return ( pos.x <= pj.nbCol && pos.y <= pj.nbLignes);
}


bool estLibre(const Position &pos,const Grille &gr)
{
    return gr.tab[pos.y][pos.x] == 0;
}



//-------------------------------------- Initialisation de la grille --------------------------------
void majGrille(const Position &pos, Grille &gr, int val)
{
    gr.tab[pos.y][pos.x] = val;
}

int aleatoire(int min, int max)
{
    return rand()%(max-min)+min;// intervalle [a;b[
}

void poserObjet(Grille &gr, Position &pos, const ParamsJeu &pj)
{
    pos.x = aleatoire(0,pj.nbCol-1); // genere une position pour l'objet aléatoirement
    pos.y = aleatoire(0,pj.nbLignes-1);
    int objet = aleatoire(1,pj.nbObj+1); // genere un objet de valeur aleatoir
    cout << "la valeur de l'objet est : " << objet << endl;
    if  ( gr.tab[pos.y][pos.x] == 0 )
    {
        gr.tab[pos.y][pos.x] = objet;
    }else{
        poserObjet(gr, pos, pj); // pose un objet si les coordonnées n'étaient pas valides
    }
}

void poserObjetsGrille( Grille &gr, int nObjets, const ParamsJeu &pj)
{
    Position pos;
    for (int i = 0; i < nObjets; i++)
    {
        poserObjet(gr, pos, pj);
    }


}



//-------------------------------------- Affichage de la grille --------------------------------
char symboleCase( const Position &pos, const Grille & gr)
{
    char c;
    switch (gr.tab[pos.y][pos.x])
    {
        case VMIROIR1:
            c = '/';
            break;
        case VMIROIR2:
            c = '\\';
            break;
        case LIBRE:
            c = '.';
            break;
        default:
            break;
    }
    return c;
}

void afficherGrille(const Grille &gr, const ParamsJeu &pj)
{
    for (int i = 0; i < pj.nbLignes; i++)
    {
        for (int j = 0; j < pj.nbCol; j++)
        {
            cout << gr.tab[i][j] << " ";

        }
        cout << endl;

    }
    cout << endl;
}



//-------------------------------------- Placement des miroirs --------------------------------
bool estJouable(const Grille &gr, const Position &pos, const ParamsJeu &pj)
{

    return (estLibre(pos,gr) && estValide(pos,gr, pj));
}

Position saisirMiroir(const Grille &gr, Position &pos, const ParamsJeu &pj) //
{
    cout << "Entrer x : " ;
    cin >> pos.x;
    pos.x--; // on décrémente afin que la coordonnée entrée par le joueur corresponde à la coordonnée dans le tableau
    cout << "Entrer y : " ;
    cin >> pos.y;
    pos.y--;

    if (estValide(pos, gr, pj) && estLibre(pos, gr)){
        return pos;
    }else{
        cout << "Position invalide, rééssayez." << endl;
        saisirMiroir( gr, pos, pj);
    }


}

int saisieTypeMiroir(int &miroir)
{
    cout << "Saisir un type de miroir : " << endl << "-1 = /" << endl << "-2 = \\" << endl;
    cin >> miroir;
    if ( static_cast<int>(miroir) != VMIROIR1 && static_cast<int>(miroir) != VMIROIR2) // on verifie si le type saisie est un miroir
    {
        cout << "Type de miroir invalide, ecrire -1 ou -2 " << endl;
        saisieTypeMiroir(miroir);
    }
    return miroir;
}


void poserMiroirs(const ParamsJeu &pj, Grille &gr, Position &pos)
{
    int m;
    for (int i = 0; i < pj.nbMiroir; i++)
    {
        cout << "Entrer la position du miroir " << i+1 << endl;
        saisirMiroir(gr, pos, pj);
        m = saisieTypeMiroir(m);
        majGrille(pos, gr, m);
    }
}



//-------------------------------------- Effacement des miroirs --------------------------------
bool estMiroir(const Position &pos,const Grille &gr)
{
    return ((gr.tab[pos.y][pos.x] == VMIROIR1) || (gr.tab[pos.y][pos.x] == VMIROIR2));
}


void effacerMiroirs(Grille &gr, const ParamsJeu &pj)
{
    Position pos;

    for ( int i = 0; i < pj.nbLignes; i++)
    {
        for ( int j = 0; j < pj.nbCol; j++)
        {
            pos.y = i;
            pos.x = j;
            if ( estMiroir(pos,gr) )
            {
                gr.tab[pos.y][pos.x] = 0;
            }
        }
    }
}




//-------------------------------------- Modelisation du parcours --------------------------------
void afficherGrilleParcours(const GrilleVisite &grv, const ParamsJeu &pj)
{
    for (int i = 0; i < pj.nbLignes; i++)
    {
        for (int j = 0; j < pj.nbCol; j++)
        {
            cout << grv.tab[i][j] << " ";

        }
        cout << endl;

    }
    cout << endl;
}


void majGrilleVisite(const Position &pos, GrilleVisite &grv, int dirx, int diry)  //
{
    if ( dirx == 0) {
        grv.tab[pos.y][pos.x] = EVERTICAL;
    }
    else if ( diry == 0)
    {
        grv.tab[pos.y][pos.x] = EHORIZ;
    }
}


//fait a la maison
bool estDejaVisite(const GrilleVisite &grv, const Position &pos)
{
    return (grv.tab[pos.y][pos.x] != 0 ); // verifie si la case est différente de 0
}


void permuter2i(int &entier1, int &entier2)
{
    int inter = entier1;
    entier1 = entier2;
    entier2= inter;
}


void changerDir(GrilleVisite &grv, int miroir, int &diry,  int &dirx)
{
    if (miroir == VMIROIR1)
    {
        dirx = -dirx;
        diry = -diry;
        permuter2i( diry,dirx);

    }
    else
    {
        permuter2i(diry,dirx);
    }
}





//-------------------------------------- Parcours du robot --------------------------------
bool estObjet(const Grille &gr, const Position&pos )
{

    return (gr.tab[pos.y][pos.x] != 0 && gr.tab[pos.y][pos.x] != VMIROIR1 && gr.tab[pos.y][pos.x] != VMIROIR2) ;
}


bool estPosFinValide(const Grille &gr, const Position &pos, const ParamsJeu &pj)
{
    if ( gr.tab[pj.nbLignes][pj.nbCol] == VMIROIR2 ||  gr.tab[pj.nbLignes][pj.nbCol] == VMIROIR1)
    {
        return false;
    }
    return (pos.y == pj.nbLignes && pos.x == pj.nbCol - 1) || (pos.y == pj.nbLignes - 1 && pos.x == pj.nbCol);

}


void avancerPos(Position &pos, int diry, int dirx)
{
    switch (dirx)
    {
        case 1: // avance de 1 horizontalement
            pos.x++;
            break;
        case -1:
            pos.x--;
            break;
        default:
            break;
    }
    switch (diry)
    {
        case 1: // avance de 1 verticalement
            pos.y++;
            break;
        case -1:
            pos.y--;
            break;
        default:
            break;
    }
}

void parcoursRobot(Grille &gr, GrilleVisite &grv,RParcours &rp, int nmiroirs, const ParamsJeu &pj)
{
    Position pos;
    pos.y = 0;
    pos.x = 0;
    int dirx = 1;
    int diry = 0;
    int cMiroir=0;


    cout << endl << "--------------------------------------------------" << endl;
    afficherGrilleParcours(grv,pj);


    while(pos.y < pj.nbLignes && pos.y >= 0 && pos.x < pj.nbCol && pos.x >= 0 )
    {

        if ( estObjet(gr,pos) )
        {
            rp.nbObj++;
            rp.ptsTotal += gr.tab[pos.y][pos.x];
            majGrille(pos,gr,0);
        }


        if ( estMiroir(pos, gr))
        {
            int miroir= gr.tab[pos.y][pos.x];
            changerDir(grv,miroir,diry,dirx);
            cMiroir++;
            grv.tab[pos.y][pos.x] = gr.tab[pos.y][pos.x]; // on place le miroir dans grv
        }
        else
        {
            majGrilleVisite(pos,grv,dirx,diry); // sinon on ecrit le type de deplacement
        }


        avancerPos(pos, diry, dirx);
        // vérifier si la position suivante est un miroir --> agir en conséquence
        // si c'est un objet, mettre à jour RParcours
        // actualiser la grille
    }


    cout << endl << "--------------------------------------------------" << endl;
    afficherGrilleParcours(grv,pj);
    afficherGrille(gr,pj);
}





//------------------------------------ JOUEURS -----------------------------------------------------------
void initialiserScores(const ParamsJeu &pj,Scores &score)
{
    score.score = 0;
    score.nbJoueur = pj.nbJoueurs;
}

void actualiserScore(int &joueurs[], const ParamsJeu &pj,Scores &score)
{
    joueurs[pj.nbJoueurs-1] += score.score;
    score.score = 0;
}


void afficherGagnant(int &joueurs[], Scores &score )
{
    int max = joueurs[0];
    int numJoueur = 1;

    for (int i = 1; i < score.nbJoueur; i++ )
    {
        if ( joueurs[i] > max)
        {
            max = joueurs[i];
            numJoueur = i+1;
        }
    }
    cout << "Le joueur " << numJoueur << " a gagné" << endl;
}



//------------------------------------------Parametres du jeu ----------------------------------------------------
void saisirParams(ParamsJeu &pj)
{
    nbligne:
    cout << "Entrer le nombres de ligne : " ;
    cin >> pj.nbLignes;
    if ( pj.nbLignes > NMAXLIGS || pj.nbLignes < 2)
    {
        cout << "Invalide, saisir une taille comprise entre 2 et 8" << endl;
        goto nbligne;
    }

    nbCol:
    cout << "Entrer le nombres de colone : " ;
    cin >> pj.nbCol;
    if ( pj.nbLignes > NMAXCOLS || pj.nbLignes < 2)
    {
        cout << "Invalide, saisir une taille comprise entre 2 et 8" << endl;
        goto nbCol;
    }

    nbJoueur:
    cout << "Entrer le nombres de joueurs : " ;
    cin >> pj.nbJoueurs;
    if ( pj.nbJoueurs > NJOUEURS || pj.nbJoueurs < 2)
    {
        cout << "Invalide, saisir un nombre compris entre 2 et 3" << endl;
        goto nbJoueur;
    }

    nbObj:
    cout << "Entrer le nombres d'objet à placer : " ;
    cin >> pj.nbObj;
    if ( pj.nbObj > pj.nbCol*pj.nbLignes || pj.nbObj <= 0)
    {
        cout << "Invalide, entrer un nombre valide d'objets" << endl;
        goto nbObj;
    }

    nbMiroir:
    cout << "Entrer un nombre de miroir : " ;
    cin >> pj.nbMiroir;
    if ( pj.nbMiroir > pj.nbCol*pj.nbLignes -pj.nbObj || pj.nbMiroir <= 0)
    {
        cout << "Invalide, entrer un nombre valide de miroirs"  << endl;
        goto nbMiroir;
    }

    vBonus:
    cout << "Entrer la valeur du bonus : " ;
    cin >> pj.bobo;
    if ( pj.bobo < 1)
    {
        cout << "Invalide, entrer un bonus supérieur ou égal à 1"  << endl;
        goto vBonus;
    }

}







/*
 * METTRE PJ PARTOUT ET NE PAS OUBLIER DE LE DECLARER
 *
 */

void test_initialisation()
{
    Grille gr;
    initialiserGrille(gr);
    poserObjetsGrille( gr, 2);
    afficherGrille(gr);
    Position pos;
    pos.x=0;
    pos.y=0;
    poserMiroirs(1,gr,pos);
    afficherGrille(gr);
}

void test_miroirs()
{
    Grille gr;
    initialiserGrille(gr);
    poserObjetsGrille( gr, 2);
    afficherGrille(gr);
    Position pos;
    pos.x=0;
    pos.y=0;
    poserMiroirs(3,gr,pos);
    afficherGrille(gr);
    effacerMiroirs(gr);
    afficherGrille(gr);
}

void test_parcours()
{
    GrilleVisite grv;
    initialiserGrille(grv);
    afficherGrilleParcours(grv);
    Position pos;
    pos.x=0;
   pos.y=0;
//    majGrilleVisite(pos, grv,0,1);
//
//    pos.x=3;
//    pos.y=4;
//    majGrilleVisite(pos, grv,1,0);
//    afficherGrilleParcours(grv);
//    pos.x=2;
//    pos.y=3;
    if ( estDejaVisite(grv,pos))
    {
        cout << "deja visite" << endl;
    }

//------------- TEST----------------

//    int dirx = 0;
//    int diry = -1;
//    changerDir(grv,VMIROIR2,diry, dirx);
//
//    cout << "y = " << diry << " et x = " << dirx << endl;
//    changerDir(grv,VMIROIR1,diry, dirx);
//    cout << "y = " << diry << " et x = " << dirx << endl;
//---------------------------------

    RParcours rp;
    Grille gr;
    initialiserGrille(gr);
    poserMiroirs(2,gr,pos);
    parcoursRobot(gr,grv,rp,2);

}


void test_obj()
{
    Grille gr;
    initialiserGrille(gr);
    poserObjetsGrille( gr, 2);
    afficherGrille(gr);
    Position pos;
    cout << "Entrer y : " << endl;
    cin >> pos.y;
    cout << "Entrer x : " << endl;
    cin >> pos.x;
    pos.x--;
    pos.y--;
    if( estObjet(gr,pos) )
    {
        cout << "c'est un obj";
    }

}



int main() {
    srand(time(0));


//    test_obj();
    test_parcours();
//    test_initialisation();
//    test_miroirs();

    //  std::cout << "Hello, World!" << std::endl;
    return 0;
}