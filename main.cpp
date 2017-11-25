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

/**
	Contient un tableau 2D simulant la grille
*/
struct Grille
{
    int tab[NMAXLIGS][NMAXCOLS];
};

/**
	Grille permettant de repr�senter le parcours du robot
*/
typedef Grille GrilleVisite;

/**
	Contient 2 coordonnees x et y simulant une Position
*/ 
struct Position
{
    int x;
    int y;
};

/**
	Structure contenant les informations relatives � un parcours du robot
*/
struct RParcours
{
    int nbObj;		// nombre d'objets dans la grille
    int ptsTotal;	// nombre de points au total r�colt�s par le parcours du robot pour un joueur
    bool bonus;		// valeur du bonus si le robot passe par tous les miroirs
};

/**
	Contient le score et le nombre de joueurs
*/
struct Scores
{
    int score;
    int nbJoueur;
};

/**
	Contient tous les param�tres de jeu que l'utilisateur renseigne avant de jouer
*/
struct ParamsJeu{
    int nbLignes;	// nombre de lignes de la grille
    int nbCol;		// nombre de colonnes de la grille
    int nbJoueurs;	// nombre de joueurs pour la partie
    int nbObj;		// nombre d'objets dans la grille
    int nbMiroir;	// nombre de miroirs � disposition pour un tour
    int bobo; 		// valeur du bonus
};

//-------------------------------------- Modelisation du plateau --------------------------------
/**
	Initialise tous les �l�ments de la grille � 0
	@param[in] gr - grille � initialis�
	@param[in] pj - param�tres de jeu (nb lignes/colonnes)
*/
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
/**
	Indique si une position donn�e dans la grille est valide : x et y dans la grille et non en dehors
	@param[in] pos - position dont on v�rifie si les coordonn�es sont valides
	@param[in] gr - la grille dans laquelle on v�rifie les coordonn�es
	@param[in] pj - param�tres de jeu (nb lignes/colonnes)
	@return boolean - true si la position est valide, false sinon
*/
bool estValide(const Position &pos,const Grille &gr, const ParamsJeu &pj)
{
    return ( pos.x <= pj.nbCol && pos.y <= pj.nbLignes);
}

/**
	Inidique si une position donn�e est libre : c'est � dire si la valeur dans la grille est 0
	@param[in] pos - position dont on v�rifie si les coordonn�es dans la grille sont libres
	@param[in] gr - grille dans laquelle on v�rifie si la case est libre
	@return boolean - true si la case est libre
*/
bool estLibre(const Position &pos,const Grille &gr)
{
    return gr.tab[pos.y][pos.x] == 0;
}



//-------------------------------------- Initialisation de la grille --------------------------------
/**
	Met � jour la grille aux coordonn�es de la Position selon la valeur val
	@param[in] pos - position qui sert � changer une valeur dans la grille selon cette position
	@param[in,out] gr - grille pass�e en param�tre dont on change la valeur aux coordonn�es de la position selon la valeur val
	@param[in] val - valeur que prend la case de la grille
*/
void majGrille(const Position &pos, Grille &gr, int val)
{
    gr.tab[pos.y][pos.x] = val;
}

/**
	Renvoie un nombre al�atoire compris dans l'intervalle [min ; max[
	@param[in] min - valeur inf�rieure de l'intervalle
	@param[in] max - valeur sup�rieure de l'intervalle
	@return int - retourne un nombre entier al�atoire
*/
int aleatoire(int min, int max)
{
    return rand()%(max-min)+min;// intervalle [a;b[
}

/**
	Permet de poser un objet al�toire � un endroit al�atoire dans la grille
	@param[in,out] gr - grille dans laquelle on pose un objet
	@param[in] pos - coordonn�es auxquelles poser un objet, d�finies al�atoirement
	@param[in] pj - param�tres de  jeu (nb lignes/colonnes)
*/
void poserObjet(Grille &gr, Position &pos, const ParamsJeu &pj)
{
    pos.x = aleatoire(0,pj.nbCol-1); // genere une position pour l'objet aléatoirement
    pos.y = aleatoire(0,pj.nbLignes-1);
    int objet = aleatoire(1,VMAX+1); // genere un objet de valeur aleatoir
    cout << "la valeur de l'objet est : " << objet << endl;
    if  ( gr.tab[pos.y][pos.x] == 0 )
    {
        gr.tab[pos.y][pos.x] = objet;
    }else{
        poserObjet(gr, pos, pj); // pose un objet si les coordonnées n'étaient pas valides
    }
}

/**
	Permet de poser nObjets dans la grille. Appelle nObjets fois la fonctions poserObjet
	@param(in,out] gr - grille dans laquelle poser les objets
*/
void poserObjetsGrille( Grille &gr, int nObjets, const ParamsJeu &pj)
{
    Position pos;
    for (int i = 0; i < nObjets; i++)
    {
        poserObjet(gr, pos, pj);
    }


}



//-------------------------------------- Affichage de la grille --------------------------------
/**
	Indique le symbole d'une case donn�e aux coordonn�es x et y
	@param[in] y - coordonn�e y (ordonn�e)
	@param[in] x - coordonn�e x (abscisse)
	@param[in] gr - grille dont on v�rifie le symbole d'une case aux coordonn�es x et y
	@return char - retourne le symbole de la case [ '/' , '\' , '.' ]
*/
char symboleCase( int y, int x,  const Grille & gr)
{
    char c;
    switch (gr.tab[y][x])
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

/**
	Affiche la grille � l'�cran dans la console
	@param[in] gr - grille � afficher
	@param[in] pj - param�tres de jeu (nb lignes/colonnes)
*/
void afficherGrille(const Grille &gr, const ParamsJeu &pj)
{
    cout << ".  ";
    for ( int i = 0; i < pj.nbCol ;i++)
    {
        cout << i+1 << " " ;
    }
    cout << endl;
    for (int i = 0; i < pj.nbLignes; i++)
    {
        cout << i+1 << "| ";
        for (int j = 0; j < pj.nbCol; j++)
        {
            if (gr.tab[i][j] > 0)
            {
                cout << gr.tab[i][j] << " ";
            }
            else
            {
                cout << symboleCase(i,j,gr) << " ";
            }



        }
        cout << endl;

    }
    cout << endl;


}



//-------------------------------------- Placement des miroirs ------------------------------------
/**
	Indique si une position donn�e est jouable : c'est � dire si cette position dans la grille est libre et valide
	@param[in] gr - grille
	@param[in] pos - position (coordonn�es)
	@param[in] pj - param�tres de jeu (utilis� dans la fonction estValide)
	@return boolean - renvoie vrai si la case est jouable, faux sinon
*/
bool estJouable(const Grille &gr, const Position &pos, const ParamsJeu &pj)
{

    return (estLibre(pos,gr) && estValide(pos,gr, pj));
}

/**
	Permet de saisir la position d'un miroir � placer dans la grille
	@param[in] gr - grille dans laquelle on placera le miroir
	@param[out] pos - position que saisit l'utilisateur afin de placer un miroir
	@param[in] pj - param�tres de jeu (utilis� dans la fonction estJouable
	@return Position - retourne la position � laquelle on peut placer le miroir, si la case est jouable
*/
Position saisirMiroir(const Grille &gr, Position &pos, const ParamsJeu &pj) //
{
    cout << "Entrer x : " ;
    cin >> pos.x;
    pos.x--; // on décrémente afin que la coordonnée entrée par le joueur corresponde à la coordonnée dans le tableau
    cout << "Entrer y : " ;
    cin >> pos.y;
    pos.y--;

    if (estJouable(gr,pos,pj)){
        return pos;
    }else{
        cout << "Position invalide, reessayez." << endl;
        saisirMiroir( gr, pos, pj);
    }


}

/**
	Demande � l'utilisateur quel type de miroir il veut placer : '/' ou '\'
	@param[out] miroir - miroir que l'utilisateur d�finit
	@return int - retourne le miroir que l'utilisateur a saisi
*/
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

/**
	Permet de poser nbMiroir : param�tre que l'utilisateur renseigne avant de commencer la partie
	@param[in] pj - param�tres de jeu (nbMiroir)
	@param[in,out] gr - grille dans laquelle on pose un miroir
	@param[in,out] pos - position � laquelle l'utilisateur place le miroir
*/
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
/**
	Indique une position donn� dans la grille est un miroir
	@param[in] pos - coordonn�es
	@param[in] gr - grille
	@return boolean - true si la position dans la grille correspond � un miroir, false sinon
*/
bool estMiroir(const Position &pos,const Grille &gr)
{
    return ((gr.tab[pos.y][pos.x] == VMIROIR1) || (gr.tab[pos.y][pos.x] == VMIROIR2));
}

/**
	Permet d'effacer tous les miroirs de la grille pass�e en param�tres
	@param[in,out] gr - grille
	@param[in] pj - param�tres de jeu (nb lignes/colonnes)
*/
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
/**
	Affiche la grille de parcours du robot
	@param[in,out] grv - grille visite : grille de parcours du robot
	@param[in] pj - param�tres de jeu (nb lignes/colonnes)
*/
void afficherGrilleParcours(const GrilleVisite &grv, const ParamsJeu &pj)
{
    cout << ".  ";
    for ( int i = 0; i < pj.nbCol ;i++)
    {
        cout << i+1 << " " ;
    }
    cout << endl;
    for (int i = 0; i < pj.nbLignes; i++)
    {
        cout << i+1 << "| ";
        for (int j = 0; j < pj.nbCol; j++)
        {
            if ( grv.tab[i][j] == 1)
            {
                cout << "| ";
            }
            else if ( grv.tab[i][j] ==2 )
            {
                cout << "- ";
            }
            else{

                cout << symboleCase(i,j,grv) << " ";
            }


        }
        cout << endl;

    }
    cout << endl;
}

/**
	Permet de mettre � jour la grille de parcours du robot par les valeurs 1 ou 2 selon dirx et diry donn�s en param�tres
	@param[in] pos - position dans la grille de parcours
	@param[in,out] grv - grille de parcours du robot
	@param[in] dirx - direction x (abscisse) du robot. 1 si le robot se d�place vers la droite, 0 s'il ne se d�place pas selon l'axe des abscisses, -1 s'il se d�place vers la gauche
	@param[in] diry - direction y (ordonn�e) du robot. 1 si le robot se d�place vers le bas, 0 s'il ne se d�place pas selon l'axe des ordonn�es, -1 s'il se d�place vers le haut
*/
void majGrilleVisite(const Position &pos, GrilleVisite &grv, int dirx, int diry)
{
    if ( dirx == 0) {
        grv.tab[pos.y][pos.x] = EVERTICAL;
    }
    else if ( diry == 0)
    {
        grv.tab[pos.y][pos.x] = EHORIZ;
    }
}

/**
	Indique si une position dans la grille visite a d�j� �t� visit�
	@param[in] grv - grille de parcours du robot
	@param[in] pos - position
	@return boolean - true si la case est d�j� visit� (!=0), false sinon
*/
bool estDejaVisite(const GrilleVisite &grv, const Position &pos)
{
    return (grv.tab[pos.y][pos.x] != 0 ); // verifie si la case est différente de 0
}

/**
	Permet de permuter deux variables d'entier pass�es en param�tre
	@param[in,out] entier1 - 1er entier
	@param[in,out] entier2 - 2e entier
*/
void permuter2i(int &entier1, int &entier2)
{
    int inter = entier1;
    entier1 = entier2;
    entier2= inter;
}

/**
	Permet de changer la direction du parcours du robot dans la grille visite selon un miroir
	@param[in] grv - grille de parcours du robot
	@param[in] miroir - miroir selon lequel la direction changera
	@param[in] diry - direction y du robot
	@param[in] dirx - direction x du robot
*/
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
/**
	Indique si une position dans la grille est un objet ou non
	@param[in] gr - grille dans laquelle on v�rifie les coordonn�es
	@param[in] pos - position dans la grille
	@return boolean - true si la position dans la grille est un objet, false sinon
*/
bool estObjet(const Grille &gr, const Position&pos )
{

    return (gr.tab[pos.y][pos.x] != 0 && gr.tab[pos.y][pos.x] != VMIROIR1 && gr.tab[pos.y][pos.x] != VMIROIR2) ;
}

/**
	Indique si le robot termine son parcours par la derni�re case de la grille (en bas � droite)
	@param[in] gr - grille
	@param[in] pos - position
	@param[in] pj - param�tres de jeu (nb lignes/colonnes)
*/
bool estPosFinValide(const Grille &gr, const Position &pos, const ParamsJeu &pj)
{
    if ( gr.tab[pj.nbLignes][pj.nbCol] == VMIROIR2 ||  gr.tab[pj.nbLignes][pj.nbCol] == VMIROIR1)
    {
        return false;
    }
    return (pos.y == pj.nbLignes && pos.x == pj.nbCol - 1) || (pos.y == pj.nbLignes - 1 && pos.x == pj.nbCol);

}

/**
	Permet d'avancer la position du robot dans la grille visite
	@param[in,out] pos - position du robot, qui s'incr�mente ou se d�cr�mente
	@param[in] diry - direction y du robot
	@param[in] dirx - direction x du robot
*/
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

/**
	Permet de r�aliser le parcours du robot
	@param[in] gr - grille
	@param[in,out] grv - grille visite (parcours du robot)
	@param[in] rp - RParcours : renseigne les informations relative au joueur (nb objets, pts totaux, bonus)
	@param[in] nmiroirs - nombre de miroirs sur lesquels le robot est pass�
	@param[in] pj - param�tres de jeu (nb lignes/colonnes)
*/
void parcoursRobot(Grille &gr, GrilleVisite &grv,RParcours &rp, int nmiroirs, const ParamsJeu &pj)
{
    Position pos;
    pos.y = 0;
    pos.x = 0;
    int dirx = 1;
    int diry = 0;
    int cMiroir=0;
    rp.nbObj =0;
    rp.ptsTotal=0;
    rp.bonus=0;




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


    if (cMiroir == nmiroirs) rp.ptsTotal += pj.bobo;

    afficherGrilleParcours(grv,pj);

}





//------------------------------------ JOUEURS -----------------------------------------------------------
/**
	Initialise le score et le nombre de joueurs
	@param[in] pj - param�tres de jeu (nbJoueur)
	@param[in,out] score - score � initialiser
*/
void initialiserScores(const ParamsJeu &pj,Scores &score)
{
    score.score = 0;
    score.nbJoueur = pj.nbJoueurs;
}

/**
	Permet d'actualiser le score d'un joueur donn�
	@param[in,out] joueurs - tableau dans lequel sont renseign�s tous les joueurs de la partie
	@param[in] pj - param�tres de jeu
	@param[int,out] score - score � incr�menter
	@param[in] joueurCourant - num�ro du joueur dont il faut incr�menter le score
*/
void actualiserScore(int joueurs[], const ParamsJeu &pj, Scores &score, int joueurCourant)
{
    joueurs[joueurCourant] += score.score;
    score.score = 0;
}

/**
	Permet d'afficher le gagnant de la partie
	@param[in] joueurs - tableau des joueurs de la partie contenant les scores de chaque joueur
	@param[in] score - renseigne le nombre de joueurs
*/
void afficherGagnant(int joueurs[], Scores &score )
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
/**
	Permet � l'utilisateur de saisir les param�tres du jeu avant de d�buter la partie
	@param[in,out] pj - param�tres de jeu (nb lignes/colonnes, nb joueurs, nb objets, nb miroirs, valeur de bonus
*/
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

//void test_initialisation()
//{
//    Grille gr;
//    initialiserGrille(gr);
//    poserObjetsGrille( gr, 2);
//    afficherGrille(gr);
//    Position pos;
//    pos.x=0;
//    pos.y=0;
//    poserMiroirs(1,gr,pos);
//    afficherGrille(gr);
//}

//void test_miroirs()
//{
//    Grille gr;
//    initialiserGrille(gr);
//    poserObjetsGrille( gr, 2);
//    afficherGrille(gr);
//    Position pos;
//    pos.x=0;
//    pos.y=0;
//    poserMiroirs(3,gr,pos);
//    afficherGrille(gr);
//    effacerMiroirs(gr);
//    afficherGrille(gr);
//}

/*void test_parcours()
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

}*/

//
//void test_obj()
//{
//    Grille gr;
//    initialiserGrille(gr);
//    poserObjetsGrille( gr, 2);
//    afficherGrille(gr);
//    Position pos;
//    cout << "Entrer y : " << endl;
//    cin >> pos.y;
//    cout << "Entrer x : " << endl;
//    cin >> pos.x;
//    pos.x--;
//    pos.y--;
//    if( estObjet(gr,pos) )
//    {
//        cout << "c'est un obj";
//    }
//
//}


/**
	Permet d'initialiser un tableau de joueurs, avec un score �gale � 0
	@param[in,out] joueur - tableau de joueurs
	@param[in] pj - param�tres de jeu (nb lignes/colonnes)
*/
void initialiserJoueurs(int joueur[], ParamsJeu &pj)
{
    for (int i = 0; i < pj.nbJoueurs; i++)
    {
        joueur[i] = 0;
    }

}

/**
	Permet de jouer : appel toutes les fonctions n�cessaire au bon d�roulement de la partie
*/
void jeu()
{
    ParamsJeu pj;
    Grille gr;
    GrilleVisite grv;
    Position pos;

    Scores sc;


    // Initialisation du jeu et ses parametres
    saisirParams(pj);
    initialiserGrille(gr, pj);
    initialiserGrille(grv, pj);
    initialiserScores(pj,sc);
    poserObjetsGrille(gr,pj.nbObj,pj);
    int joueur[pj.nbJoueurs];
    initialiserJoueurs(joueur,pj);
   // afficherGrille(gr,pj);


    int objRestant = pj.nbObj;
    int joueurCourant = 0;

    for (int i = 0; i < pj.nbJoueurs; i++)
    {
        cout << joueur[i] << endl;
    }




    while ( objRestant != 0 || joueurCourant != pj.nbJoueurs-1 )
    {
        RParcours rp;
        cout << "Tour du joueur : " << joueurCourant + 1 << endl; // Affichage d'un message pour connaitre le joueur courant
        afficherGrille(gr,pj);
        poserMiroirs(pj,gr,pos); // on va demandé au joueur de placé ses miroirs
        parcoursRobot(gr,grv,rp,pj.nbMiroir,pj);
        objRestant -= rp.nbObj; // on enleve les objets ramassés
        sc.score = rp.ptsTotal;
        actualiserScore(joueur, pj, sc,joueurCourant);
        effacerMiroirs(gr, pj);

        if (joueurCourant == pj.nbJoueurs-1)
        {
            joueurCourant = 0;
        }
        else
        {
            joueurCourant++;
        }
        initialiserGrille(grv, pj);


    }
    afficherGagnant(joueur,sc);




}

int main() {
    srand(time(0));
    jeu();

//    test_obj();
//    test_parcours();
//    test_initialisation();
//    test_miroirs();

    //  std::cout << "Hello, World!" << std::endl;
    return 0;
}
