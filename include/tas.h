#ifndef TAS
#define TAS 

typedef struct _elt { 
  int num; 
  double dist; 
} Element; 

typedef struct _tas { 
  Element ** tab;
  int nbEl; 
  int tMax; 
  int *indiceNoeud; 
  int *places; 
} Tas; 

// Returns the pere of i
int pere(int i);

// If pere exists returns 1
int hasPere(int i); 

// Swaps i and j in t.
void swap(Tas *t, int i, int j);

// Brings it up
void monter(Tas *t, int i);

// Size
int size(Tas * t);

int isNoeud (Tas * t, int i);
int isLeaf(Tas * t, int i);
int left (int i);
int right (int i);
int hasRight(Tas * t, int i);
int hasLeft(Tas *t, int i);
int miniSon(Tas *t, int i);
void descendre(Tas *t, int i);
void insertTas(Tas *t, Element * add);
Element * delMin(Tas *t);
void delEl(Tas *t, int i); 
Tas * newTas(int size); 
Element * newElt(int i, double dist); 
void printElt(Element * e); 
void printTas(Tas *t); 
Element * findElt(Tas * t, int i); 

#endif
