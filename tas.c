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

Element * elMin(Tas *t) 
{
  if (t->nbEl==0) {
    printf("Tas vide\n");
    perror("[delMin] Not possible\n");
  }
  Element * ret = t->tab[1];
  t->indiceNoeud[t->tab[0]->num]=-1;
  swap(t, t->nbEl, 1);
  t->nbEl--;
  descendre(t, 1);
  return ret;
}
