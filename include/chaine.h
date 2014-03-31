#ifndef __CHAINE_H__
#define __CHAINE_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <dirent.h> 
#include <libgen.h>
#include <ctype.h>
#include "entree_sortie.h"
#define dbug printf("%s [%s] : %d\n", __FILE__, __FUNCTION__, __LINE__)

/* Points' linked list */
typedef struct cellPoint {
  double x,y;
  struct cellPoint *suiv; // Next point in the list
} CellPoint;

/* Linked list of lines of points */
typedef struct cellChaine {
  int numero; // Num
  CellPoint * points; // List of points of the chain
  struct cellChaine * suiv; // Next line of points
} CellChaine;

/* File struct */
typedef struct {
  int gamma; // Max fibres/cable
  int nbChaines; // Nb de chaines
  CellChaine * chaines; // Linked list of lines
} Chaines;


/** CellPoint Functions **/
/**
 * Creates a new point with (double x, double y) as coords.
 */
CellPoint *newPoint(double x, double y);


/**
 * Adds point, returns pointer.
 */
CellPoint * addPoint(double x, double y, CellPoint *p);

/**
 * Removes first element of p
 */
CellPoint* pop(CellPoint * p);

/**
 * Prints all the points in the list. 
 */
void printListPoints(CellPoint *p);

/**
 * Deletes list of points.
 */
CellPoint * deleteListPoints(CellPoint *p);


/** Cell Chaine Functions **/
/**
 * Creates a new CellChaine with numero n and points.
 */
CellChaine * createChain(int n, CellPoint * points);

/**
 * Adds new chain at the head of the lines
 */
CellChaine * addCellChaine(CellChaine * lines, int nb, CellPoint * points);

/**
 * Desallocates ch (pop)
 */
void deleteChain(CellChaine * ch);

/**
 * Desallocates whole list
 */
CellChaine * deleteListChains(CellChaine * c);


/** List of Chaine (FILE) functions **/
/**
 * Allocates new Chaines struct
 */
Chaines * createChainList(int gam, int nbCh);

/**
 * Desallocates the structure
 */
void deleteChainList(Chaines *ls);

/**
 * Prints all
 */
void printChaines (Chaines * ch);


/** I/O **/
/**
 * Creates and populates struct Chaines from file
 */
Chaines * lectureChaine(FILE * f);

/**
 * Total length of a line 
 */
double longueurChaine(CellChaine *l);

/**
 * Total distance between all the points in the struct
 */
double longueurTotale(Chaines *C);

/**
 * Calculates the number of points in list
 */
int nbPoints (CellPoint * points);

/**
 * Calculates total number of points
 */ 
int comptePointsTotal(Chaines *C);

/**
 * Saves ch to file (reproducing original formatting)
 */
void saveChaine (Chaines * ch);

/**
 * Removes 4 last characters from the string (file extensions for example)
 */
char *remove_extension(const char *filename);

/**
 * Creates GNUPlot configuration (used to invoke GNUPlot) and data (1 point per line) files for every file in ./Data/Chain
 */
void createConfigFiles();

/**
 * Renders all the plots using GNUPlot
 */
void generatePlots();

/**
 * Generates nbChaines chaines, with nbPointsChaines points par Chaine, with [xmax, ymax] max coords
 */
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax);

#endif
