#include "chaine.h"

// CellPoint functions
CellPoint *newPoint(double x, double y)  // OK**
{
  CellPoint *p = (CellPoint *) malloc(sizeof(CellPoint));
  if ( p != NULL ) {
    p->x=x;
    p->y=y;
    p->suiv = NULL; 
  }
  return p;
}

CellPoint * addPoint(double x, double y, CellPoint *p) // OK**
{
  CellPoint *new = newPoint(x, y);
  if ( p != NULL) { 
    new->suiv=p;
  }
  return new;
}


CellPoint * pop (CellPoint * p) // OK**
{
  CellPoint * popped = NULL;
  if (p != NULL) {
    popped = p->suiv;
    printf("[pop] Popped(%.2f, %.2f)!\n", p->x, p->y);
    free(p);

  }
  return popped;
}

CellPoint * deleteListPoints(CellPoint *p) // OK**
{
  if (p==NULL) { 
    printf("--> [deleteListPoints] Liste vide!\n");
    return NULL;
  } else {
    CellPoint * tmp = p;
    while (tmp!=NULL) {
      printf("Deleting point of coords: x: %.2f y: %.2f\n", tmp->x,tmp->y);
      tmp=pop(tmp);
    }
    free(tmp);
    tmp=NULL;
    return tmp;
  }
}

void printListPoints(CellPoint *p)  // OK**
{
  CellPoint *tmp;
  if (p == NULL) {
    printf("--> [printListPoints] Empty list.\n");
  } else {
    tmp=p;
    while (tmp!=NULL) {
      printf("%.2f %.2f ", tmp->x, tmp->y); 
      if (tmp->suiv != NULL) 
	tmp = tmp->suiv;
      else if (tmp->suiv == NULL) 
	return;
    }
  }
}

// CellChaine - chain (LINE) functions
CellChaine * createChain(int n, CellPoint * points) // OK**
{
  CellChaine * ch = (CellChaine *) malloc(sizeof(CellChaine));
  if (ch != NULL) 
    {
      ch->numero=n;
      ch->points = points; 
      ch->suiv = NULL;
    }
  return ch;
}

CellChaine * addCellChaine(CellChaine * lines, int nb, CellPoint * points) // OK**
{
  CellChaine * cc = createChain(nb, points);
  if (points != NULL) 
    cc->suiv=lines;
  return cc;
}

void deleteChain(CellChaine * ch) // OK**
{
  if (ch != NULL ) {
    ch->points=deleteListPoints(ch->points);
    free(ch);
  }
}

CellChaine * deleteListChains(CellChaine * c) // OK**
{
  CellChaine * next;
  while (c != NULL) {
    next = c->suiv;
    deleteChain(c);
    c=next;
  }
  return c; 
}

// List of Chaine (FILE) functions
Chaines * createChainList(int gam, int nbCh) // OK**
{
  Chaines * list = (Chaines*) malloc(sizeof(Chaines));
  if ( list != NULL ) {
    list->gamma=gam;
    list->nbChaines=nbCh;
    list->chaines = NULL; 
  }
  return list;
}

void deleteChainList(Chaines *ls) // OK**
{
  if (ls != NULL) {
    ls->chaines=deleteListChains(ls->chaines);
    free(ls);
  }
}

void printChaines (Chaines * ch) { // OK**
  if (ch != NULL) {
    int i =0;
    CellChaine * cc = ch->chaines;
    while ( cc != NULL) { 
      printf("\n#%d ", cc->numero);
      printListPoints(cc->points);
      cc=cc->suiv;
    }
  }
}

// IO
Chaines * lectureChaine(FILE * f) // OK**
{
  Chaines * ch = NULL;
  CellChaine * tmp = NULL;
  int 
    i=0,
    j=0,
    nbCh=-1,
    nrCh=-1,
    nbPt=-1,
    gam=-1;
  double 
    x=-1.,
    y=-1.;
  char str[1024];
  
  GetChaine(f, 1023, str); // To get NbChaines
   if(strcmp(str, "NbChaines:")==0) {
    Skip(f);
    nbCh=GetEntier(f);
  } else { 
    perror("[nbChaines value error] The file is not properly structured, aborting!\n");
  }
  SkipLine(f); // To drop to L2
  GetChaine(f, 1023, str); // To get Gamma
  if(strcmp(str, "Gamma:")==0) {
    Skip(f);
    gam=GetEntier(f);
  } else {
    perror("[Gamma value error] The file is not properly structured, aborting!\n");
  }
  printf("nbCh: %d, Gam: %d\n", nbCh, gam);
  SkipLine(f); // To get to L3+
  for (j=0 ; j<nbCh ; j++) { // Iteration on lines
    CellChaine *cc = (CellChaine *) malloc(sizeof(CellChaine));
    cc->points = NULL;
    nrCh=GetEntier(f);
    Skip(f);
    nbPt=GetEntier(f);
    // Points
    for (i=0 ; i<nbPt ; i++) { // Adding every lines' points
      Skip(f); 
      x=GetReel(f); 
      Skip(f);
      y=GetReel(f);
      cc->points=addPoint(x, y, cc->points);
    }
    cc->numero=j;
    cc->suiv=tmp;
    tmp=cc;
    Skip(f); // Drop to new line
  }
  ch = createChainList(gam, nbCh); // Concatenate to Chaine
  ch->chaines = tmp;
  return ch;
}

double longueurChaine(CellChaine *l) // OK**
{
  double longueur = 0;
  if ( l != NULL ) {
    CellPoint *point = l->points;
    while ( point != NULL && point->suiv != NULL ) {
      longueur += sqrt(pow((point->y-point->suiv->y),2) + pow((point->x-point->suiv->x),2));
      point = point->suiv;
    }
  } else { 
    fprintf(stderr,"[longueurChaine] Chaine not initialised\n");
  }
  return longueur;
}

double longueurTotale(Chaines *C) // OK**
{
  double longueur = 0;
  if ( C != NULL ) {
    CellChaine *chaine = C->chaines;
    while ( chaine != NULL ) {
      longueur += longueurChaine(chaine);
      chaine = chaine->suiv;
    }
  } else {
    fprintf(stderr,"[longueurTotale] Chaine not initialised\n");
  }
  return longueur;
}

int nbPoints (CellPoint * points) // OK**
{ 
  int i =0; 
  CellPoint * tmp = points;
  while (tmp != NULL) {
    i++;
    tmp=tmp->suiv;    
  }
  return i; 
}

int comptePointsTotal(Chaines *C) // OK**
{
  int nbPoint = 0;
  if ( C != NULL ) {
    CellPoint *point;
    CellChaine *chaine = C->chaines;
    while ( chaine != NULL ) {
      point = chaine->points;
      while ( point != NULL ) {
	nbPoint++;
	point = point->suiv;
      }
      chaine = chaine->suiv;
    }
  } else {
    fprintf(stderr,"[comptePointsTotal] Chaine not initialised\n");
  }
  return nbPoint;
}

void saveChaine (Chaines * ch) { // OK**
  FILE *f = fopen("./Data/Generated/file1.txt", "w");
  char str;
  if (f==NULL) { 
    perror("[saveChaine] Error opening file!\n");
  }
  fprintf(f, "NbChaines: %d\nGamma: %d\n", ch->nbChaines, ch->gamma); // First two lines
  if (ch != NULL) {
    int i =0;
    CellChaine * cc = ch->chaines; // Lines
    CellPoint *tmp;
    while ( cc != NULL) { 
      if (cc->points == NULL) {
	printf("[saveChaine] File saved..\n");
	break;
      } else { // Line not empty, printing points to file.
	printf("Saving line %d...\n", cc->numero);
	i=nbPoints(cc->points);
	fprintf(f, "%d %d ", cc->numero, i); // Write line number and number of points
	tmp=cc->points;
	while (tmp!=NULL) { // Points writing
	  fprintf(f, "%.2f %.2f ", tmp->x, tmp->y); 
	  if (tmp->suiv != NULL) // If next point exists goto
	    tmp = tmp->suiv;
	  else if (tmp->suiv == NULL) {  // End of line
	    fprintf(f, "\n");
	    break;
	  }
	} // Points writing
      }
      cc=cc->suiv;
    }
  } else { // Ch null
    printf("[saveChaine] --> struct NULL!\n");
  }
}


char *remove_extension(const char *filename) {
  size_t len = strlen(filename);
  char *newfilename = malloc(len-3);
  if (!newfilename) perror("Malloc error\n"); /* handle error */
  memcpy(newfilename, filename, len-3);
  newfilename[len - 4] = 0;
  return newfilename;
}



void createConfigFiles()
{
  DIR           *d;
  struct dirent *dir;
  
  int i=0;
  char 
    configpath[512],
    datapath[512], 
    plotdatapath[512],
    pngpath[512];
  
  d = opendir("./Data/Chain/"); // Open dir
  if (d!=NULL) 
    {
      while ((dir = readdir(d)) != NULL) 
	{
	  if (!isalnum(dir->d_name[0]))
	    continue;
	  else 
	    { // File by file dir parsing
	      char * basename = remove_extension(dir->d_name);

	      sprintf(datapath, "./Data/Chain/%s.cha", basename); // Data file path
	      sprintf(configpath, "./Data/PlotConfig/%s.txt", basename); // Config file path
	      sprintf(plotdatapath,"./Data/PlotData/%s_ch%d.txt", basename, i); // GNUPlot data file path
	      sprintf(pngpath, "./Data/Plots/%s.png", basename); // PNG path
	      FILE 
		* data = fopen(datapath, "r"), // Open data file
		* plotconfig = fopen(configpath, "w"), // Create config file
		* plotdata = fopen(plotdatapath, "w"); // Open plot data file 
	      
	      Chaines * ch = (Chaines *)malloc(sizeof(Chaines));
	      ch=lectureChaine(data); // Create struct
	      CellChaine * cc = ch->chaines;
	      CellPoint * pt = cc->points;
	      i=0;
	      
	      // Initial GNUPlot config file
	      fprintf(plotconfig, "set title \"%s\"\nset xlabel \"X\"\nset ylabel\
       \"Y\"\nset grid\nunset key\nset terminal png\n			\
       set output \"%s\"\nplot ", basename, pngpath);
	      fclose(plotconfig);
	      
	      printf("Basename: %s\n", basename);
	      printf("\nConfig:\t%s\n", configpath); 
	      printf("Data\t%s\n", datapath);
	      printf("Plot Data\t%s\n", plotdatapath);
	      
	      while (cc != NULL) { 
		plotconfig=fopen(configpath, "a+");
		sprintf(plotdatapath,"./Data/PlotData/%s_ch%d.txt", basename, i);
		fprintf(plotconfig, "\"./Data/PlotData/%s_ch%d.txt\"  using 1:2 title\
         \"Chaine %d\" with linespoint ls 1, \\\n ", basename, i, i);
		printf("\n\nCreating files\n");
		printf("Config:\t%s\n", configpath); 
		printf("Data\t%s\n", datapath); // Path to data file
		printf("Plot Data\t%s\n", plotdatapath);
		plotdata = fopen(plotdatapath, "w");
		pt = cc->points;
		
		while (pt != NULL) {
		  //if (DB==1) printf("%.2f %.2f\n", pt->x, pt->y);
		  fprintf(plotdata, "%.2f %.2f\n", pt->x, pt->y);
		  pt=pt->suiv;
		}
		
		fclose(plotdata);
		fclose(plotconfig);
		cc=cc->suiv;
		i++;
	      }
	      fclose(data);
	    }
	}
      closedir(d);
    }    
  printf("\n\n************************************\n\
    ---> [SUCCESS] All files created ! *\n	    \
    ************************************\n");
}

void generatePlots() 
{
  DIR           *d;
  struct dirent *dir;

  d = opendir("./Data/PlotConfig/"); // Open dir
  if (d!=NULL) {
    while ((dir = readdir(d)) != NULL) 
      if (!isalnum(dir->d_name[0]))
	continue;
      else 
	{
	  FILE *gp;
	  gp = popen(GNUPLOT, "w");
	  if(gp == NULL){
	    fprintf(stderr, "Oops, I can't find %s.", GNUPLOT);
	    exit(EXIT_FAILURE);
	  }
	  if (DB==1) printf("Generating plot for %s...\n", dir->d_name);
	  fprintf(gp, "load \"./Data/PlotConfig/%s\"\n",dir->d_name);
	  fflush(gp); /* On oublie pas le buffer. */
	  pclose(gp);
	}
    closedir(d);  
  }
}

Chaines * generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax) 
{
  double x=0., y=0.;
  int i =0, j=0;
  CellChaine * ch = NULL;
  Chaines * ret = createChainList(10, nbChaines);
  for (i=0 ; i< nbChaines ; i++) { // Create new chain 
    CellPoint * pt = NULL;
    for (j=0 ; j<nbPointsChaine ; j++) { // Create new point
      x = rand()%xmax; 
      y = rand()%ymax;
      if (DB==1) printf("x: %.2f y: %.2f\n", x, y);
      pt = addPoint(x, y, pt);
    }
    ret->chaines = addCellChaine(ch, i, pt);
  }
  ret->chaines = ch; 
  return ret; 
}
