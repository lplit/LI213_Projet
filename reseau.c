#include "reseau.h"
Reseau * createReseau() //**OK
{
  Reseau * rs = (Reseau*) malloc(sizeof(Reseau));
  if (rs != NULL) {
    rs->nbNoeuds=-1;
    rs->gamma=-1;
    rs->noeuds=NULL;
    rs->commodites=NULL;
  } else { 
    printf("[createReseau] Error allocating, output probably NULL or invalid\n");
  }
  return rs;
}

Noeud * createNoeud (int num, double x, double y)  //**OK
{
  Noeud * nd = (Noeud *) malloc(sizeof(Noeud)); 
  nd->num=num;
  nd->x=x;
  nd->y=y;
  nd->voisins=NULL;
  return nd;
}

CellCommodite * addCellCommodite(Noeud * left, Noeud * right, CellCommodite * list) //**OK
{
  CellCommodite * cc = (CellCommodite *) malloc(sizeof(CellCommodite));
  if (cc != NULL) {
    cc->extrA = left;
    cc->extrB = right;
    if (list != NULL) cc->suiv = list;
  } else { 
    perror("[createCellCommodite] Error allocating memory!\n");
  }
  return cc;
}

CellNoeud * addCellNoeud (Noeud * add_this, CellNoeud * cells) //**OK
{
  CellNoeud * cn = (CellNoeud *) malloc(sizeof(CellNoeud));
  if ( add_this == NULL ) { 
    perror("Invalid argument, you can not add null\n");
  }
  cn->cour = add_this;
  if (cells == NULL) { 
    cn->suiv= NULL; 
  } else { 
    cn->suiv= cells;
  }
  return cn;
}

CellNoeud * addNeighbour(CellNoeud * list, int id, Noeud * add) //**OK
{
  CellNoeud * tmp1 = list; 
  Noeud * tmp = findNoeudID(id, tmp1); 
  tmp->voisins = addCellNoeud(add, tmp->voisins);
  return tmp1;
}

int find_neighbour(Noeud *nd, int u)
{
  int num = -1;
  if ( nd != NULL ){
    CellNoeud * voisin = nd->voisins;
    while ( voisin != NULL && voisin->cour->num != u ) {
      voisin = voisin->suiv;
    }
    if ( voisin != NULL )
      num = voisin->cour->num;
  } else
    fprintf(stderr,"recherche_voisin : Noeud non initialisé\n");
  return num;
}

Noeud * findNoeudID (int id, CellNoeud * list) //**OK
{ 
  CellNoeud * cn = list; 
  Noeud * tmp=list->cour; 
  while (tmp != NULL) {
    if (tmp->num==id) {
      return tmp; 
    } else {
      cn = cn->suiv;
      tmp=cn->cour;
    }
  }
  perror("[findNoeudID] ID not found! Returning current Noeud\n");
  return tmp;
}

Noeud * findNoeudCoords(double x, double y, CellNoeud * list)
{
  CellNoeud * cn = list; 
  Noeud * tmp=list->cour;
  while (tmp != NULL) {
    double nx=tmp->x, ny=tmp->y; // Handles for simplified reading
    if ((x==nx) && (y==ny)) return tmp; 
    else { 
      cn = cn->suiv;
      tmp = cn->cour;
    }
  }
  fprintf(stderr, "[findNoeudCoords] No neud at [%.2f, %.2f] ! Returning current Noeud\n", x, y);
  return tmp;
}
  
Reseau * lectureReseau(FILE * f) //**OK
{
  Reseau * rs = createReseau();
  Noeud 
    * cell= (Noeud*) malloc(sizeof(Noeud)), 
    * tmp1=NULL, 
    * tmp2=NULL;
  CellNoeud * listNoeuds= (CellNoeud*) malloc(sizeof(CellNoeud));
  CellCommodite * commodites = (CellCommodite *) malloc(sizeof(CellCommodite));
  int
    i=0,
    j=0,
    k=0,
    num=-1,
    nbNo=-1, 
    gam=-1;
  double 
    x=0., 
    y=0.;
  char str[1024];
  GetChaine(f, 1023, str); // To get NbChaines
  if(strcmp(str, "NbNoeuds:")==0) {
    Skip(f);
    nbNo=GetEntier(f);
    rs->nbNoeuds=nbNo;
  } else { 
    perror("[nbNoeuds value error] The file is not properly structured, aborting!\n");
  }
  SkipLine(f); // To drop to L2
  GetChaine(f, 1023, str); // To get Gamma
  if(strcmp(str, "Gamma:")==0) {
    Skip(f);
    gam=GetEntier(f);
    rs->gamma=gam;
  } else {
    perror("[Gamma value error] The file is not properly structured, aborting!\n");
  }
  printf("nbNo: %d Gam: %d\n", nbNo, gam);
  SkipLine(f); // To get to L3+

  // Creating Noeuds
  for (i=0; i< nbNo ; i++) 
    { 
      num=GetEntier(f); Skip(f); // #
      x=GetReel(f); Skip(f); // x
      y=GetReel(f); Skip(f); // y
      cell = createNoeud(num, x, y); // Allocate new structure
      listNoeuds = addCellNoeud(cell, listNoeuds); // Update list
      if (DB==1) printf("Cell: %d x: %.2f y: %.2f\n", cell->num, cell->x, cell->y); // Verbose output
    }
  printf("\n"); // Purely aesthetic
  Skip(f); // To get to Cables section 
  char c=ReadChar(f);
  if ( c!='a') perror("[Cables error] The file is not properly structured, aborting!\n");
  ungetc(c, f); // Move parser back
  while (!feof(f)) {
    c=ReadChar(f);
    switch (c) {
    case 'a' : // Cables | Add k to j's neighbour list and j to k's neighbours
      Skip(f); j=GetEntier(f);
      Skip(f); k=GetEntier(f);
      tmp1=findNoeudID(j, listNoeuds); // Find by number
      tmp2=findNoeudID(k, listNoeuds); // Find by number
      listNoeuds = addNeighbour(listNoeuds, k, tmp1);
      listNoeuds = addNeighbour(listNoeuds, j, tmp2);
      //printf("%c %d %d\n",c, j, k);
      Skip(f);
      break; 
      
    case 'o' : // Commodites
      Skip(f); j=GetEntier(f); 
      Skip(f); k=GetEntier(f);
      tmp1=findNoeudID(j, listNoeuds); // Find by number
      tmp2=findNoeudID(k, listNoeuds); // Find by number
      commodites = addCellCommodite(tmp1, tmp2, commodites);
      if (DB==1) { // Verbose output
	printf("left: %d %.2f %.2f\n", commodites->extrA->num, commodites->extrA->x, commodites->extrA->y);
	printf("right: %d %.2f %.2f\n", commodites->extrB->num, commodites->extrB->x, commodites->extrB->y);
	printf("%c %d %d\n\n", c, j, k);
      }
      Skip(f);
      break; 
      
    default : 
      perror("[A/C error] The file is not properly structured, aborting!\n");
      break; 
    }
  }
  rs->noeuds=listNoeuds;
  rs->commodites=commodites;
  return rs;
}

void print_NoeudAndNei (Noeud * n) {  //**OK
  if (n!=NULL) { 
    printf("*\n* Cell %d x: %.2f y: %.2f\n", n->num, n->x, n->y);
    CellNoeud * neig = n->voisins; // Print cour, affect suiv;
    while (neig != NULL) {
      printf("*\t<-> %d x: %.2f y: %.2f\n", neig->cour->num, neig->cour->x, neig->cour->y);
      neig = neig->suiv;
    }
  }
}


void print_all (Reseau * rs)  //**OK
{ 
  if (rs == NULL) {
    perror("[print_all] Reseau NULL!\n");
  } else {
    int click =0;
    // Header  
    printf("***************************************\n");
    printf("* nbNoeuds: %d\n* Gamma: %d\n* Noeuds and neighbours:\n", rs->nbNoeuds, rs->gamma);
    
    // Noeuds
    CellNoeud * cells = rs->noeuds;
    Noeud * pt = cells->cour;
    while (cells != NULL) {
      click=0;
      while (pt != NULL && click==0) {
	pt = cells->cour;
	print_NoeudAndNei(pt);
	click=1;
      }
      cells = cells->suiv;
    }
    printf("***************************************\n");
  }
}


int compteCable(Reseau * r) //**OK
{
  int cables=0;
  // Noeuds
  CellNoeud * cells = r->noeuds;
  Noeud * pt = cells->cour;
  while (pt != NULL) {
    pt = cells->cour;
    if (pt!=NULL) { // Point
      CellNoeud * neig = pt->voisins; 
      while (neig != NULL) {  // Neighbours
	cables++;
	neig = neig->suiv;
      }
    }
    cells = cells->suiv;
  }
  return cables/2;
}


void saveReseau(Reseau * rs) //**OK
{
  FILE * f = fopen("./Data/text1.txt", "w");
  // Header
  fprintf(f, "nbNoeuds: %d\nGamma: %d\n\n", rs->nbNoeuds, rs->gamma);

  // Noeuds
  CellNoeud * cells = rs->noeuds;
  Noeud * pt = cells->cour;
  while (pt != NULL) {
    pt = cells->cour;
    if (pt!=NULL) { // Point
      CellNoeud * neig = pt->voisins; 
      while (neig->suiv != NULL) {  // Neighbours
	neig = neig->suiv;
      }
      fprintf(f, "a %d %d\n", pt->num, neig->cour->num); 
    }
    cells = cells->suiv;
  }
  fprintf(f, "\n");
  // Commodite
  CellCommodite * cc = rs->commodites;
  while (cc!=NULL) {
    if (cc->extrA!=NULL && cc->extrB!=NULL) {
      int a=cc->extrA->num, b = cc->extrB->num;
      fprintf(f, "o %d %d\n", a, b);
      cc=cc->suiv;
    }
    else break;
  }
  fclose(f);
}


void reseauToPlot(Reseau * r, char* name) //**OK
{
  CellNoeud * cells = r->noeuds;
  Noeud * pt = cells->cour;
  char pngpath[512],
    plotdatapath[512],
    configpath[512];

  sprintf(pngpath, "./Data/Plots/%s.png", name);
  sprintf(configpath, "./Data/PlotConfig/%s.txt", name); // Config file path
  sprintf(plotdatapath,"./Data/PlotData/%s_ch.txt", name); // GNUPlot data file path
  FILE 
    * plotconfig = fopen(configpath, "w"), // Create config file
    * plotdata = fopen(plotdatapath, "w"), // Open plot data file 
    * gp= popen(GNUPLOT, "w"); //GNUPlot pipe

  // Header
  fprintf(plotconfig, "set title \"%s\"\nset xlabel \"X\"\nset ylabel\
		       \"Y\"\nset grid\nunset key\nset terminal png\n\
		       set output \"%s\"\nplot ", name, pngpath);

  // GNUPlot data
  while (pt != NULL) {
    pt = cells->cour;
    if (pt!=NULL) { // Point
      CellNoeud * neig = pt->voisins; 
      if (DB==1) printf("%.2f %.2f\n", pt->x, pt->y); // Verbose output
      while (neig != NULL) {  // Neighbours
	fprintf(plotdata, "%.2f %.2f\n", neig->cour->x, neig->cour->y);
	neig = neig->suiv;
      }
    }
    cells = cells->suiv;
  }

  // GNUPlot Config
  fprintf(plotconfig, "\"%s\" using 1:2 title \"%s\" with linespoint ls 1", plotdatapath, plotdatapath);
  fclose(plotconfig); fclose(plotdata); // Close all files to allow GNUPlot to read them
  if(gp == NULL){
    fprintf(stderr, "Oops, I can't find %s.", GNUPLOT);
    exit(EXIT_FAILURE);
  } else {
    fprintf(gp, "load \"%s\"\n", configpath);
  }
  fflush(gp); // Clean buffer
  pclose(gp); // Close pipe
}


/** Hash **/
Hachage * createHashTab(int size) 
{
  Hachage * h = (Hachage *) malloc(sizeof(Hachage)); 
  if (h==NULL) {
    perror("[createHashTab] Error allocating memory for hash!\n");
    exit(EXIT_FAILURE);
  } else {
    h->tab = (CellNoeud **) malloc(sizeof(CellNoeud*)*size);
    if (h->tab==NULL) {
      perror("[createHashTab] Error allocating memory for tab!\n");
    } else {
      h->size=size;
      int i =0;
      for (i=0; i<size ; i++) {
	h->tab[i]=NULL;
      }
    }
    return h; 
  }
}

int compare(double x1, double y1, double x2, double y2)
{
  int val = 0;
  if ( x1 < x2 ||  ( sqrt(pow(x1-x2,2)) < 0.001 && y1 < y2 ) )
    return -1;
  else if ( x1 > x2 ||  ( sqrt(pow(x1-x2,2)) < 0.001 && y1 > y2 ) )
    return 1;
  else return 0;
}

double key (double x,double y) { return (y + ((x+y) * (x+y+1)/2)); }

int hash (double key,int size) {
  double kA = key * ((sqrt(5)-1.)/2.);
  return (int) floor(size*(kA - floor(kA)));
}

int hashcoords (double x, double y, int size) { return hash(key(x,y), size); }

Noeud *findAddHash(Reseau *R, Hachage *H, double x, double y)
{
  Noeud* noeud = NULL;
  CellNoeud 
    *lNoeud, *nNoeud, *nLNoeud;
  int k;
  static int id = 0;
  char found = 0;
  if (R != NULL) {
    if (H != NULL) {
      k = hashcoords(x, y, H->size); // Hash
      lNoeud = *(H->tab+k); // Handle
      found = 0;
      
      while ( lNoeud != NULL && found == 0 ) { // Search
	if ( compare(lNoeud->cour->x,lNoeud->cour->y,x,y) == 0 ) {
	  noeud = lNoeud->cour;
	  found = 1;
	} else 
	  lNoeud = lNoeud->suiv;
      }
      
      if ( found == 0 ) { // Add
	noeud = createNoeud(id, x, y);
	id++;
	nNoeud = addCellNoeud(noeud, nNoeud); 
	nNoeud->suiv = *(H->tab+k);

	*(H->tab+k)= nNoeud; // Populating hash
	
	nLNoeud = addCellNoeud(noeud, nLNoeud); // Populating Reseau
	nLNoeud->suiv = R->noeuds;
	R->noeuds = nLNoeud;
      }
    } else perror("[findAddHash] Hash not initialised\n"); // H!=NULL
  } else perror("[findAddHash] Reseau non [findAddHash] : Hash not initialised\n"); // R!=NULL
  return noeud;
}

Reseau * recreateReseau(Chaines *C)
{
  Reseau * rs = createReseau();
  if (C==NULL) { perror("Chaines not initialized, returning NULL\n"); return rs; }
  
  rs->gamma = C->gamma;
  
  CellChaine * cc = C->chaines;
  CellPoint * pt=NULL;
  CellNoeud * n=NULL;
  Noeud * left, * right;
  CellCommodite * commo = NULL;
  int 
    size = C->gamma*C->nbChaines, 
    end = C->nbChaines,
    i = 0;
  Hachage * h = createHashTab(size);
  
  if (DB == 1) { 
    printf("Created Hash table with size %d\n", size);
    printf("Reseau params:\nGamma: %d\n", rs->gamma);
  }
  
  for (i=0 ; i<end; i++) { // Go through all the chains
    printf("\nNew chain\n");
    commo = (CellCommodite*) malloc(sizeof(CellCommodite));
    pt = cc->points;
    if (DB==1) printf("Hashing x %.2f y %.2f\n", pt->x, pt->y); // Verbose
    left = findAddHash(rs, h, pt->x, pt->y); // extrA of CellCommodite
    pt=pt->suiv; // To not treat the same point twice
    while (pt->suiv != NULL) { // Neighbours
      if (pt!=NULL) {
	if (DB==1) printf("Hashing x %.2f y %.2f\n", pt->x, pt->y); // Verbose
	right = findAddHash(rs, h, pt->x, pt->y); // extrB of CellCommodite, eventually.
	left->voisins=addCellNoeud(right, left->voisins);
	pt=pt->suiv;
      }
    }
    rs->commodites = addCellCommodite(left, right, rs->commodites);
    Noeud * leftb = rs->commodites->extrA, *rightb = rs->commodites->extrB;
    if (DB==1) printf("Added commodity left #%d[%.2f, %.2f] right #%d[%.2f, %.2f] to blockchain\n", leftb->num, leftb->x, leftb->y, rightb->num, rightb->x, rightb->y);
    cc=cc->suiv;
  }
  
  rs->commodites=commo;
  return rs; 
}

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax) 
{
  if (C!=NULL) {
    int i=0, end = C->nbChaines;
    if (DB==1) printf("End = %d\n", end);
    for (i=0 ; i<end ; i++) {
      CellChaine * cc = C->chaines; 
      CellPoint * pt = cc->points;
      while (pt != NULL) {
	if (DB==1) printf("Xmin %.2f Ymin %.2f Xmax %.2f Ymax %.2f PtX %.2f PtY %.2f\n", *xmin, *ymin, *xmax, *ymax, pt->x, pt->y);
	if (*xmin > pt->x) *xmin=pt->x; 
	if (*ymin > pt->y) *ymin=pt->y; 
	if (*xmax < pt->x) *xmax=pt->x; 
	if (*ymax < pt->y) *ymax=pt->y;
	pt=pt->suiv;
      }
      cc=cc->suiv;
    }
  } else printf("\n\n--> Chaines is NULL. ABORT.\n\n");
}

Reseau * hashFromFile(FILE * f) 
{
  Chaines * ch = lectureChaine(f);
  Reseau * r = recreateReseau(ch);
  return r;  
}

void hashAllFiles() 
{
  DIR *d;
  struct dirent *dir;
  clock_t
    start,
    end;
  double cpu_time;
  FILE * f_time = fopen("./Data/Times/Hash.txt", "w");
  d= opendir("./Data/Chain/");
  if (d!=NULL) {
    while ((dir = readdir(d)) != NULL) 
      if (!isalnum(dir->d_name[0]))
	continue;
      else 
	{
	  char chain[128], times[128];
	  sprintf(chain, "./Data/Chain/%s", dir->d_name);
	  FILE * f_ch = fopen(chain, "r");
	  if (DB==1) printf("Hashing %s...\n", chain); // Verbose
	  Chaines * ch = lectureChaine(f_ch); // Before hashing
	  start = clock(); 
	  Reseau * rs = hashFromFile(f_ch);
	  end=clock();
	  cpu_time = ((double)(end-start))/CLOCKS_PER_SEC;
	  printf("Hash file %s in %f\n", dir->d_name, cpu_time);
	  fprintf(f_time, "%d %f\n", comptePointsTotal(ch), cpu_time);
	  fclose(f_ch);
	}
    fclose(f_time);
    closedir(d);
  }
  printf("Output available in ./Data/Times/Hash.txt");
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY)
{
  ArbreQuat * a = (ArbreQuat*) malloc(sizeof(ArbreQuat));
  if (a!=NULL) {
    a->xc=xc;
    a->yc=yc;
    a->coteX=coteX;
    a->coteY=coteY;
    a->noeud=NULL;
    a->so=NULL;
    a->se=NULL;
    a->no=NULL;
    a->ne=NULL;
    return a;
  } else { 
    perror("[creerArbreQuat] Error allocating memory, abort.\n"); exit(EXIT_FAILURE); 
  }  
}

int whichQ (Noeud * n, ArbreQuat * parent) { 
  double // Lots of handles
    xcen = n->x, 
    ycen = n->y, 
    px = parent->xc, 
    py = parent->yc, 
    pw = parent->coteX, 
    ph = parent->coteY;
  if ((xcen<px) && (ycen<py)) return 1;        // SW
  else if ((xcen>=px && ycen<py)) return 2;    // SE
  else if ((xcen<px && ycen >=py)) return 3;   // NW
  else return 4;                               // NE
}


ArbreQuat * insererNoeudArbre ( Noeud * n, ArbreQuat * a, ArbreQuat * parent) 
{
  ArbreQuat * tmp, *ret = parent;
  double // Lots of handles
    nx = n->x,
    ny = n->y, 
    xcen = 0., 
    ycen = 0., 
    px = parent->xc, 
    py = parent->yc, 
    pw = parent->coteX, 
    ph = parent->coteY;
  if (a == NULL) {
    if (xcen<px && ycen<py) { // SW
      if (DB==1) printf("Inserting SW\n");
      xcen = px-((pw-px)/2);
      ycen = (ph-py)/2;
      tmp = creerArbreQuat(xcen, ycen, pw/2, ph/2);
      tmp->noeud=n;
      ret->so=tmp;
    }
    else if (xcen>=px && ycen<py) { // SE
      if (DB==1) printf("Inserting SE\n");
      xcen = px+((pw-px)/2);
      ycen = py-((ph-py)/2);
      tmp = creerArbreQuat(xcen, ycen, pw/2, ph/2);
      tmp->noeud=n;
      ret->se=tmp;
    }
    else if (xcen<px && ycen >=py) { // NW
      if (DB==1) printf("Inserting NW\n");
      xcen = px-((pw-px)/2);
      ycen = py+((ph-py)/2);
      tmp = creerArbreQuat(xcen, ycen, pw/2, ph/2);
      tmp->noeud=n;
      ret->no=tmp;
    }
    else { // NE
      if (DB==1) printf("Inserting NE\n");
      xcen = px+((pw-px)/2);
      ycen = py+((ph-py)/2);
      tmp = creerArbreQuat(xcen, ycen, pw/2, ph/2);
      tmp->noeud=n;
      ret->ne=tmp;
    }
    return ret;
  } else { 
    switch (whichQ(n, parent)) { // Two cases in each switch - Leaf and Inside Cell. Hereforth a!=NULL
      ArbreQuat * tmp=a;
    case 1 : // SW
      if (a->so != NULL) { // Inside cell, meaning there's already something in the cell. Create new tree, insert newand  old one in.
	Noeud * nt = a->so->noeud;
	tmp->so=insererNoeudArbre(n, tmp->so, tmp);
	tmp->so=insererNoeudArbre(nt, tmp->so, tmp);
      } else { // Leaf 
	tmp->so=insererNoeudArbre(n, tmp->so, tmp);
      }
      break;
      
    case 2 : // SE
      if (a->se != NULL) {
	Noeud * nt = a->se->noeud;
	tmp->se=insererNoeudArbre(n, tmp->se, tmp);
	tmp->se=insererNoeudArbre(nt, tmp->se, tmp);
      } else { // Leaf
	tmp->se=insererNoeudArbre(n, tmp->se, tmp);
      }
      break; 
      
    case 3 : // NW
      if (a->no != NULL) {
	Noeud * nt = a->no->noeud;
	tmp->so=insererNoeudArbre(n, tmp->so, tmp);
	tmp->so=insererNoeudArbre(nt, tmp->so, tmp);
      } else { 
	tmp->no=insererNoeudArbre(n, tmp->se, tmp);
      }
      break; 
      
    case 4: // NE
      if (a->ne != NULL) { 
	Noeud * nt = a->ne->noeud;
	tmp->so=insererNoeudArbre(n, tmp->so, tmp);
	tmp->so=insererNoeudArbre(nt, tmp->so, tmp);
      } else { 
	tmp->ne=insererNoeudArbre(n, tmp->ne, tmp);
      }
      break;
      
    default : 
      printf("[%s:%d] Something went wrong!\n", __func__, __LINE__);
    }
  }
  return tmp;
}

Noeud * chercherNoeudArbre(CellPoint * pt, Reseau * R, ArbreQuat ** aptr, ArbreQuat * parent)
{
  Noeud * nd = createNoeud(0, pt->x, pt->y);
  if ( *aptr==NULL) { //1er cas
    return nd;
  }
  if ( (*aptr)->noeud !=NULL) {// Feuille
    if (pt->x == (*aptr)->noeud->x && pt->y == (*aptr)->noeud->y) {
      return (*aptr)->noeud;
    } else { // 
      return (*aptr)->noeud;
    }
  }
  return (*aptr)->noeud;
}

Reseau * recreeReseauArbre(Chaines * C) 
{
  Reseau * r;
  return r;
}

Reseau * qtreeFromFile(FILE * f) 
{
  Chaines * ch = lectureChaine(f);
  Reseau * a = recreeReseauArbre(ch);
  return a;
}

void qtreeAllFiles() 
{
  DIR *d;
  struct dirent *dir;
  clock_t
    start,
    end;
  double cpu_time;
  FILE * f_time = fopen("./Data/Times/QTree.txt", "w");
  d= opendir("./Data/Chain/");
  if (d!=NULL) {
    while ((dir = readdir(d)) != NULL) 
      if (!isalnum(dir->d_name[0]))
	continue;
      else 
	{
	  char chain[128], times[128];
	  sprintf(chain, "./Data/Chain/%s", dir->d_name);
	  FILE * f_ch = fopen(chain, "r");
	  if (DB==1) printf("Hashing %s...\n", chain); // Verbose
	  Chaines * ch = lectureChaine(f_ch); // Before hashing
	  start = clock(); 
	  Reseau * rs = qtreeFromFile(f_ch);
	  end=clock();
	  cpu_time = ((double)(end-start))/CLOCKS_PER_SEC;
	  printf("QuadTree file %s in %f\n", dir->d_name, cpu_time);
	  fprintf(f_time, "%d %f\n", comptePointsTotal(ch), cpu_time);
	  fclose(f_ch);
	}
    fclose(f_time);
    closedir(d);
  }
  printf("Output available in ./Data/Times/Hash.txt");
}
