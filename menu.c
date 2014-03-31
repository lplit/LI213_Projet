#include "menu.h"

void p_menu_gen ()
{
  printf("------- Menu ------\n");
  printf("------ Part 1 -----\n");
  printf("----- Chaines -----\n");
  printf("1: Points\n");
  printf("2: Lines\n");
  printf("3: File\n");
  printf("------ Part 2 -----\n");
  printf("------  Hash  -----\n");
  printf("4: Create hash from all files and save output\n");
  printf("------ Part 3 -----\n");
  printf("----- Quadtre -----\n");
  printf("5: Not yet implemented\n");
  printf("------ Part 4 -----\n");
  printf("----- Randoms -----\n");
  printf("6: Generate random\n");
  printf("0: Exit\n");
}

void p_menu_1_points ()
{
  printf("------ Points ------\n");
  printf("1: Add points manually\n");
  printf("2: Add randomly generated points\n");
  printf("3: Print the structure\n");
  printf("0: Exit\n");
}

void p_menu_2_lines ()
{
  printf("------ Lines ------\n");
  printf("1: Create/Add a random (<20) lines of points.\n");
  printf("2: Print points\n");
  printf("0: Exit\n");
}

void p_menu_3_file ()
{
  printf("------ File ------\n");
  printf("1: Read and print File\n");
  printf("2: Create plot\n");
  printf("3: Generate config files for all the files\n");
  printf("4: Generate all plots (do #3 before)\n");
  printf("0: Exit\n");
}


void chainesMenu ()
{
  int 
    ans1 = -1,
    ans2 = -1,
    size=-1,
    i=0;
  float 
    x=0., 
    y=0.;
  CellPoint * points = NULL;
  CellChaine * cc = NULL;
  char 
    *tmp = (char *) malloc(sizeof(char)*1024),
    tmp2[1024];
  FILE 
    *f = fopen("./Data/Chain/00014_burma.cha", "r"),
    *gp = NULL;
  srand(time(NULL));

  while (ans1 != 0) { // Main menu loop
    p_menu_gen();
    scanf("%d", &ans1);
    switch (ans1) {
    case 1 : // Points
      p_menu_1_points();
      scanf("%d", &ans2);
      while (ans2 != 0) { 
	switch(ans2) {
	case 1 : // Manually add one point
	  printf("[addPoint]: Coordonates? ");
	  scanf("%f %f", &x, &y);
	  points=addPoint(x, y, points);
	  ans2=0;
	  break;
	case 2 : // Add randomly generated points
	  printf("Add how many points?\n");
	  scanf("%d", &size);
	  srand48(time(NULL));
	  for (i=0 ; i<size ; i++) { 
	    float x = drand48()*10;
	    float y = drand48()*10;
	    points=addPoint(x, y, points);
	  }
	  ans2=0;
	  break;
	case 3 : // Print list of points
	  printListPoints(points);
	  ans2=0;
	  break;
	case 0 : // Back
	  ans2=0;
	  break;
	default : 
	  printf("[Points] Wrong unput\n");
	  ans2=0;
	  break;
	}
      }
      break;
    
    case 2 : // Lines menu
      p_menu_2_lines();
      scanf("%d", &ans2);

      while (ans2!=0) { 
	switch (ans2) { 
	case 1 : // Add number of random points lines
	  printf("How many lines?\n");
	  scanf("%d", &size);
	  int j=0;
	  for (j=0 ; j < size ; j++) { 
	    points=(CellPoint*) malloc(sizeof(CellPoint));
	    for (i=0 ; i< rand()%20 ; i++) {
	      srand48(time(NULL));
	      float x = drand48()*10;
	      float y = drand48()*10;
	      points=addPoint(x, y, points);
	    }
	    cc=addCellChaine(cc, j, points);
	  }
	  ans2=0;
	  break;
	case 2 : // Print points
	  while (cc != NULL) {
	    printf("%d", cc->numero);
	    printListPoints(cc->points);
	    cc=cc->suiv;
	  }
	  ans2=0;
	  break; 
	case 0 :
	  printf("Back to main menu\n");
	  ans2=0;
	  break;
	default :
	  printf("[2: Lines] Wrong imput\n");
	  break; 
	}
      }
      break;
      
    case 3 : // File menu
      p_menu_3_file();
      scanf("%d", &ans2);
      Chaines * ch = NULL;
      switch (ans2) {
      case 1: // Read file
	printf("Available files\n");
	system("ls ./Data/Chain");
	scanf("%s", tmp2);
	char str[512]=("./Data/Chain/");
	strcat(str, tmp2);
	printf("Opening file %s\n", str);
	f = fopen(str, "r");
	if (f!=NULL) {
	  ch = lectureChaine(f);
	  printf("File read succesfuly:\n");
	  printChaines(ch);
	  ans2=0;
	} else {
	  printf("Error opening file!\nPlease try again\n");
	  ans2=1;
	}
	break;
      case 2 : // Generate plot 
	gp= popen(GNUPLOT, "w");
	if(gp == NULL){
	  fprintf(stderr, "Oops, I can't find /usr/local/bin/gnuplot");
	  exit(EXIT_FAILURE);
	}
	char configname[100];
	printf("Which config file?\n");
	scanf("%s", configname);
	fprintf(gp, "cd './Data/PlotConfigs/ '\n load \"%s\"\n", configname);
	fflush(gp); /* On oublie pas le buffer. */
	getchar();
	pclose(gp);
	printf("Plot created, available in ./Data/Plots/\n");
	break;
      case 3 : // Generate Data Files, Config Files
	createConfigFiles();
	ans2=0;
	break; 
      case 4 : // Generate Data Files, Config FilesGenerate all plots
	generatePlots();
	ans2=0;
      case 0 : // Exit
	ans2=0;
	break;
      default : 
	printf("[File] Wrong menu input\n");
	break;
      }
      break; 

    case 4 : // Create hash from all files and save output
      hashAllFiles();
      ans2=0;
      break; 

    case 5 : // Tree
      printf("Quart tree\n");
      f = fopen("./Data/Chain/00014_burma.cha", "r");
      double minX=10000., maxX=0., minY=10000., maxY=0.;
      Chaines * tr = lectureChaine(f);
      printf("minX %.2f maxX %.2f minY %.2f maxY %.2f\n", minX, maxX, minY, maxY);
      chaineCoordMinMax(tr, &minX, &minY, &maxX, &maxY);
      printf("minX %.2f maxX %.2f minY %.2f maxY %.2f\n", minX, maxX, minY, maxY);
      break;

    case 6 : // Add random points
      ans2=0;
      int tabsize=0, ch_nb=0, xmax=0, ymax=0;
      printf("How many chaines? [nbChaines]\n");
      scanf("%d", &tabsize);
      printf("How many points per chain? [nbPointsChaine]\n");
      scanf("%d", &ch_nb);
      printf("xMax? [xmax]\n");
      scanf("%d", &xmax);
      printf("yMax? [ymax]\n");
      scanf("%d", &ymax);
      printf("Values chosen:\n[nbChaines] %d\n[nbPointsChaine] %d\n[xMax] %d\n[yMax] %d\n", tabsize, ch_nb, xmax, ymax);
      Chaines * alea = generationAleatoire(tabsize, ch_nb, xmax, ymax);
      ans2=0; 
      break;

    case 0 : // Exit program
      exit (EXIT_SUCCESS);
    default : // Main loop wrong input
      printf("Wrong input!\n");
    }
  }
  exit (EXIT_SUCCESS);
}
