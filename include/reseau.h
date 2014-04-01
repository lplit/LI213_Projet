#ifndef __RESEAU_H__ 
#define __RESEAU_H__

#include "chaine.h"

/* Liste chainee de noeuds */
typedef struct cellNoeud { 
struct noeud *cour; 
struct cellNoeud *suiv;
} CellNoeud;

/* Liste chainee de commodites */
typedef struct cellCommodite { 
struct noeud *extrA , *extrB; 
struct cellCommodite *suiv;
} CellCommodite;

/* Noeud du reseau */
typedef struct noeud { 
int num;
double x, y;
CellNoeud *voisins; 
} Noeud;

/* Un reseau */
typedef struct { 
int nbNoeuds;
int gamma;
CellNoeud *noeuds; 
CellCommodite *commodites;
} Reseau;

/* Tab de hachage */
typedef struct hachage{
  CellNoeud **tab;
  int size;
} Hachage;

 /* Arbre quaternaire contenant les noeuds du reseau */
 typedef struct arbreQuat{ 
  double xc, yc; /* Coordonnees du centre de la cellule */
  double coteX; /* Longueur de la cellule */
  double coteY; /* Hauteur de la cellule */
  Noeud* noeud; /* Pointeur vers le noeud du reseau */
  struct arbreQuat *so; /* Sous-arbre sud-ouest, pour x < xc et y < yc */ 
  struct arbreQuat *se; /* Sous-arbre sud-est, pour x >= xc et y < yc */
  struct arbreQuat *no; /* Sous-arbre nord-ouest, pour x < xc et y >= yc */ 
  struct arbreQuat *ne; /* Sous-arbre nord-est, pour x >= xc et y >= yc */
} ArbreQuat;

/**
 * Allocates structure memory
 */
Reseau * createReseau();

/**
 * Allocates and affects new structure
 */
Noeud * createNoeud (int num, double x, double y) ;

/**
 * Creates new "Commodite" with left and right as values, adds 'list' as suiv. 
 */
CellCommodite * addCellCommodite(Noeud * left, Noeud * right, CellCommodite * list);

/**
 * Adds 'add_this' to the list 'cells' 
 */
CellNoeud * addCellNoeud (Noeud * add_this, CellNoeud * cells);

/**
 * Finds Noeud number 'id' in 'list', adds Noeud 'add' to it.
 */
CellNoeud * addNeighbour(CellNoeud * list, int id, Noeud * add);

/**
 * Returns Noeud number 'id' from 'list' 
 */
Noeud * findNoeudID (int id, CellNoeud * list);

/**
 * Finds and returns Noeud by its coorodonates
 */
Noeud * findNoeudCoords(double x, double y, CellNoeud * list);

/**
 * Allocate and populate Reseau struct from FILE f.
 */
Reseau * lectureReseau(FILE * f);

/**
 * Prints the Noeud 'n' and all its neighbours. This is internal to print_all, output won't be any good alone.
 */
void print_NoeudAndNei (Noeud * n);
  
/**
 * Prints all Noeuds and their respective neighbours.
 */
void print_all (Reseau * rs); 

/**
 * Calculates the number of cables in the network
 */
int compteCable(Reseau * r);

/**
 * Creates and saves configuration file.
 */
void saveReseau(Reseau * r);

/**
 * Generates GNUPlot of Reseau r as 'name'.png
 */
void reseauToPlot(Reseau * r, char* name);

/* HASH */

Hachage * createHashTab(int size);
/**
 * Does compare, similar to strcmp, returns negative if [x1,y1] < [x2,y2], 0 if equal, else 1.
 */
int compare(double x1, double y1, double x2, double y2);

/**
 * Calcs hashing key
 */
double key (double x,double y);

/**
 * Calcs hash position
 */
int hash (double k,int taille);

/**
 * Finds hash for (x,y) in 'h', returns it, if not exist, create, add to 'r', return created instance.
 */
Noeud * findAddHash(Reseau * r, Hachage * h, double x, double y);

/**
 * Hashes the C down, returns it.
 */
Reseau * recreateReseau(Chaines *C);

/**
 * Finds and saves all the variables in Chaine C
 */
void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax);

/**
 * Recreates Reseau directly from file, using lectureChaine()
 */
Reseau * hashFromFile(FILE * f);

/**
 * Hash creates Reseau for every file in ./Data/Network and saves execution time
 */
void hashAllFiles();

/**
 * Allocates new quadtree
 */
ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY);

/**
 * Returns 1 for SW, 2 for SE, 3 for NW, 4 for NE
 */
int whichQ (Noeud * n, ArbreQuat * parent);
#endif
