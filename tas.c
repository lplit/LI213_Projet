#include "tas.h"
#include <stdio.h>
#include <stdlib.h>


int pere(int i) 
{
  return i/2;
}

int hasPere(int i) 
{
  return i!=1;
}

void swap(Tas *t, int i, int j)
{
  Element * tmp = t->tab[i];
  t->tab[i] = t->tab[j];
  t->tab[j]=tmp;
}

void monter(Tas *t, int i) 
{
  if (!hasPere(i)) return; 
  int up = pere(i);
  if (t->tab[up]->dist > t->tab[i]->dist) {
    swap(t, i, up);
    t->indiceNoeud[t->tab[i]->num]=up;
    monter(t, up);
  }
}

int size(Tas * t) 
{
  return t->nbEl;
}

int isNoeud (Tas * t, int i) 
{
  return i<=size(t);
}

int isLeaf(Tas * t, int i)
{
  return !hasLeft(t, i);
}

int left (int i)
{
  return 2*i;
}

int right (int i) 
{
  return 2*i+1;
}

int hasRight(Tas * t, int i)
{
  return isNoeud(t, right(i));
}

int hasLeft(Tas *t, int i)
{
  return isNoeud(t, left(i));
}

int miniSon(Tas *t, int i) 
{
  if (!hasRight(t, i)) return left(i);
  else {
    int l = left(i); 
    int r = right(i);
    return (t->tab[l] < t->tab[r]) ? l:r;
  }
}

void descendre(Tas *t, int i)
{
  if (isLeaf(t, i)) return; 
  int son = miniSon(t,i);
  if (t->tab[i] > t->tab[son]) {
    swap(t, son, i); 
    descendre(t, son);
  }
}

void insertTas(Tas *t, Element * add) 
{
  if (t->nbEl>=t->tMax) {
    printf("Tas full\n");
    return;
  }
  t->nbEl++; 
  t->tab[t->nbEl]=add; 
  t->indiceNoeud[add->num]=t->nbEl;
  monter(t, t->nbEl);
}

Element * delMin(Tas *t) 
{
  if (t == NULL) perror("Tas NULL!\n");
  if (t->nbEl==0) {
    printf("Tas empty\n");
    perror("[delMin] Not possible\n");
  }
  Element * ret = t->tab[1];
  t->indiceNoeud[t->tab[0]->num]=-1;
  swap(t, t->nbEl, 1);
  t->nbEl--;
  descendre(t, 1);
  return ret;
}

void delEl(Tas *t, int i) 
{
  int iTas=t->indiceNoeud[i+1];
  swap(t, iTas, t->nbEl);
  t->nbEl--;
  monter(t, iTas);
  descendre(t, iTas);
}

Tas * newTas(int size) 
{
  Tas * t = (Tas *) malloc(sizeof(Tas));
  int i = 0;
  t->nbEl=0;
  t->tMax=size;
  t->tab = (Element **) malloc(sizeof(Element *)*size+1);
  for (i=0 ; i<=size ; i++) {
    t->tab[i]=(Element *) malloc(sizeof(Element));
  }
  t->indiceNoeud=(int *) malloc(sizeof(int)*size+1);
  return t;
}

Element * newElt(int i, double dist)
{
  Element * e = (Element *) malloc(sizeof(Element));
  e->num=i;
  e->dist=dist;
  return e;
}

void printElt(Element * e)
{
  printf("[printElt] ID: %D Dist: %.2f\n", e->num, e->dist);
}

void printTas(Tas * t) 
{
  int i=0;
  printf("[printTas] nbEl: %d tMax: %d\n", t->nbEl, t->tMax);
  for (i=0; i< t->tMax ; i++) printElt(t->tab[i]);
}

Element * findElt(Tas * t, int i) 
{
  int j = -1;
  j = t->indiceNoeud[i]; 
  if (j!=-1) return t->tab[j]; 
  else {
    perror("[findElt] Element not found!\n");
    exit(EXIT_FAILURE);
  }
}

