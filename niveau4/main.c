#include "collision.c"
#include <math.h>
/**
 * \file main.c
 * \author Yunfan CAI
 * \version 0.2
 * \date 02 avril 2019
 */

#include "sdl-light.h"

/**
 * \brief Largeur de l'écran de jeu
 */
#define SCREEN_WIDTH 1288

/**
 * \brief Hauteur de l'écran de jeu
 */
#define SCREEN_HEIGHT 725

/**
 * \brief Taille d'une boule
 */
#define BALL_SIZE 42
/**
 * \brief VITESS_coefficient
 */
#define SPEED_COEF 0.1
/**
 * \brief REDUIRE VITESS
 */
#define FRICTION 0.05
/**
 * \brief abscisse du bord gauche
 */
#define LEFT_LIMIT 85
/**
 * \brief abscisse du bord droit
 */
#define RIGHT_LIMIT 1202
/**
 * \brief ordonne du bord haut
 */
#define TOP_LIMIT 85
/**
 * \brief ordonne du bord en bas
 */
#define BOTTOM_LIMIT 640
/**
 * \brief radius de hole
 */
#define HOLE_RADIUS 35
/**
 * \brief  nombres des holes
 */
#define NB_HOLES 6
/**
 * \brief abscisse de hole en bas a gauche
 */
#define HOLE_TOP_LEFT_X 75
/**
 * \brief ordonne de hole en bas a gauche
 */
#define HOLE_TOP_LEFT_Y 75
/**
 * \brief abscisse de hole en haut en milieu
 */
#define HOLE_TOP_MID_X 644
/**
 * \brief ordonne de hole en haut en milieu
 */
#define HOLE_TOP_MID_Y 40
/**
 * \brief abscisse de hole en haut a droit
 */
#define HOLE_TOP_RIGHT_X 1213
/**
 * \brief ordonne de hole en haut a droit
 */
#define HOLE_TOP_RIGHT_Y 75
/**
 * \brief abscisse de hole en bas a gauche
 */
#define HOLE_BOTTOM_LEFT_X 75
/**
 * \brief ordonne de hole en bas a gauche
 */
#define HOLE_BOTTOM_LEFT_Y 650
/**
 * \brief ordoone de hole en bas a droit
 */
#define HOLE_BOTTOM_RIGHT_Y 650
/**
 * \brief abscisse de hole en bas a droit
 */
#define HOLE_BOTTOM_RIGHT_X 1213
/**
 * \brief abscisse de hole en bas en milieu
 */
#define HOLE_BOTTOM_MID_X 644
/**
 * \brief ordonne de hole en bas en milieu
 */
#define HOLE_BOTTOM_MID_Y 685
/**
 * \brief nombre des boules
 */
#define NB_BALLS 16
/**
 * \brief abscisse de queuebillard
 */
#define WIDTH_QUEUEBILLARD 514
/**
 * \brief ordonne de queuebillard
 */
#define HEIGHT_QUEUEBILLARD 17
/**
 * \brief abscisse de boule blanche
 */
#define WIDTH_BOUBLE_BLANCHE 900
/**
 * \brief nombre maximale de ligne de boule couleur
 */
#define NB_MAX_LIGNE 5
/**
 * \brief nombre maximale de colonne de boule couleur
 */
#define NB_MAX_COLONNE 5

/**
 * \brief Représentation du monde du jeu
 */

/**
 * \brief initialise gobal de ordonne dans la point de jeu
 */

    int som1=0;/*somme point de joueur1*/
    int som2=0;/*somme point de joueur2*/
    int tour=0;/* verifier joueur1 ou joueur2*/
    int som_boule=0;/* pour verifier si tous les boule sont dans la hole*/
    int ig;/*pour numero de boule*/
    int ib;/*dans cas boule blache entre la hole ,plus7*/
    int iy;/*dans cas boule blache entre la hole,plus nombre de point de boule couleur*/


/**
 * \brief La fonction initialise de hole
 * \param world les données du monde
 */
struct hole_s {
  double troux;
  double trouy;
};
typedef struct hole_s hole_t;




/**
 * \brief La fonction initialise de boule
 * \param world les données du monde
 */
struct ball_s {
  double vx;
  double vy;
  double x;
  double y;
  int dispar;
};
typedef struct ball_s ball_t;




/**
 * \brief La fonction initialise de queuebillard
 * \param world les données du monde
 */
struct queuebillard_s {
  double x;
  double y;
};
typedef struct queuebillard_s queuebillard_t;



/**
 * 
 * \brief world les données du monde
 * \param world les données du monde
 */
struct world_s{
    SDL_Surface* table;
    SDL_Surface* boules[NB_BALLS];
    SDL_Surface* queuebillard;
    int gameover;
    queuebillard_t billard;
    ball_t ball[NB_BALLS];
    hole_t hole[NB_HOLES];
    };

typedef struct world_s world_t;




/**
 * \brief La fonction initialise les données du trou
 * \param hole les données du hole,
 * \param x les données qui va initialise dans la hole,
 * \param y les données qui va initialise dans la hole,
 */
void ini_trou(hole_t * hole,double x, double y){
    hole->troux=x;
    hole->trouy=y;
}
void ini_trous(world_t *world){
    ini_trou(&world->hole[0],0,0);/* il y problem de ajouter valeur à hole[0]*/
    ini_trou(&world->hole[1],HOLE_TOP_LEFT_X,HOLE_TOP_LEFT_Y);
    ini_trou(&world->hole[2],HOLE_TOP_MID_X,HOLE_TOP_MID_Y);
    ini_trou(&world->hole[3],HOLE_TOP_RIGHT_X,HOLE_TOP_RIGHT_Y);
    ini_trou(&world->hole[4],HOLE_BOTTOM_RIGHT_X,HOLE_BOTTOM_RIGHT_Y);
    ini_trou(&world->hole[5],HOLE_BOTTOM_MID_X,HOLE_BOTTOM_MID_Y);
    ini_trou(&world->hole[6],HOLE_BOTTOM_LEFT_X,HOLE_BOTTOM_LEFT_Y);
}
/*a cause de symetrie on peut reduire definition de HOLE :exemple HOLE_BOTTOM_LEFT_X=HOLE_TOP_LEFT_X*/



/**
 * \brief La fonction initialise les données du boule blanche
 * \param world les données du monde
 */
void ini_boule_blanche(world_t *world){
    world->ball[0].x=SCREEN_WIDTH/2-BALL_SIZE;
    world->ball[0].y=SCREEN_HEIGHT/2-BALL_SIZE;
    world->ball[0].vx=0;
    world->ball[0].vy=0;
    world->ball[0].dispar=1;
}




/**
 * \brief La fonction initialise les données du boule couleur
 * \param world les données du monde
 */ 
void ini_boule_couleur(world_t *world){
    world->ball[1].x=SCREEN_WIDTH*2/3-BALL_SIZE;
    world->ball[1].y=SCREEN_HEIGHT/2-BALL_SIZE;
    world->ball[1].vx=0;
    world->ball[1].vy=0;
    world->ball[1].dispar=1;
    int i,j;
    int m=2;
    int nb=0;world->ball[0].x=SCREEN_WIDTH/2-BALL_SIZE;
    world->ball[0].y=SCREEN_HEIGHT/2-BALL_SIZE;
    int a,b;
    a=world->ball[1].x;
    b=world->ball[1].y;
    for(i=2;i<=NB_MAX_COLONNE;i++){
      world->ball[m].x=a+sqrt(3)*BALL_SIZE/2;
      world->ball[m].y=b-BALL_SIZE/2;
      world->ball[m].vx=0;
      world->ball[m].vy=0;
      world->ball[m].dispar=1;
      a=world->ball[m].x;
      b=world->ball[m].y;
      m=m+1;
      nb=nb+1;
      for(j=1;j<=nb;j++){
	world->ball[m].x=world->ball[m-1].x;
	world->ball[m].y=world->ball[m-1].y+BALL_SIZE;
	world->ball[m].vx=0;
	world->ball[m].vy=0;
	world->ball[m].dispar=1;
	m=m+1;
      }
    }
}


/**
 * \brief La fonction initialise les données du QueueBillard
 */ 
void ini_queuebillard(world_t *world){
    world->billard.x=world->ball[0].x;
    world->billard.y=world->ball[0].y;
}







/**
 * \brief La fonction initialise les données du monde du jeu
 * \param world les données du monde
 */
void init_data(world_t * world){
    world->table=load_image("ressources/table.bmp");
    world->boules[0]=load_image("ressources/boules.bmp");
    world->queuebillard=load_image("ressources/QueueBillard.bmp");
    int i;
    for (i=1;i<NB_BALLS;i++){
      world->boules[i]=load_image("ressources/boules.bmp");
    }
    world->gameover = 0;
    ini_trous(world);
    ini_boule_blanche(world);
    ini_boule_couleur(world);
    /*express initial
    (world->x+BALL_SIZE/2)=SCREEN_WIDTH/3-BALL_SIZE/2;
    (world->y+BALL_SIZE/2)=SCREEN_HEIGHT/2-BALL_SIZE/2;*/

}



/**
 * \brief La fonction nettoie les données du monde
 * \param world les données du monde
 */
void clean_data(world_t *world){
    SDL_FreeSurface(world->table);
    SDL_FreeSurface(world->boules[0]);
    int i;
    for (i=1;i<NB_BALLS;i++){
      SDL_FreeSurface(world->boules[i]);
    }
    SDL_FreeSurface(world->queuebillard);
}



/**
 * \brief La fonction indique si le jeu est fini en fonction des données du monde
 * \param world les données du monde
 * \return 1 si le jeu est fini, 0 sinon
 */
int is_game_over(world_t *world){
    return world->gameover;
}



/**
 * \brief La fonction met à jour les données en tenant compte de la physique du monde du boule ensembles
 * \param world les données du monde
 */
void update_data_boule(world_t *world){
    int i;
    for(i=0;i<=NB_BALLS;i++){
    world->ball[i].x=world->ball[i].x+world->ball[i].vx;
    world->ball[i].y=world->ball[i].y+world->ball[i].vy;
    /* express initial
    (world->x+BALL_SIZE/2)=(world->x+BALL_SIZE/2)+world->vx;
    (world->y+BALL_SIZE/2)=(world->y+BALL_SIZE/2)+world->vy;*/
    world->ball[i].vx=world->ball[i].vx*(1-FRICTION);
    world->ball[i].vy=world->ball[i].vy*(1-FRICTION);
    }
}








/**
 * \brief La fonction met à jour les données en tenant compte de la physique du monde
 * \param les données du monde
 */
void update_data(world_t *world){
    update_data_boule(world);

}




/**
 * \brief La fonction initialise les transparences des différentes surfaces
 * \param screen la surface correspondant à l'écran de jeu
 * \param world les données du monde
 */
void  init_graphics(SDL_Surface *screen, world_t *world){

    set_transparence(screen,world->boules[0], 255, 0, 255);
    int i;
    for (i=1;i<NB_BALLS;i++){
          set_transparence(screen,world->boules[i], 255, 0, 255);
    }
    set_transparence(screen,world->queuebillard, 255, 0, 255);

}
void apply_table(SDL_Surface *table, SDL_Surface *screen){
    apply_surface(table,screen,0,0);
}




/**
 * \brief La fonction qui calcule et renvoie la distance euclidienne entreles deux boulesb1etb2
 * \param b1 les données du boule1
 * \param b2 les données du boule2
 */
double distance(ball_t *b1, ball_t *b2){
  return sqrt((b1->x-b2->x)*(b1->x-b2->x)+(b1->y-b2->y)*(b1->y-b2->y));
}



/**
 * \brief La fonction qui  prend comme parametres deux boules et renvoie 1 si elles se touchent, 0 sinon.
 * \param b1 les données du boule1
 * \param b2 les données du boule2
 */
int touche(ball_t *b1, ball_t *b2){
  if (distance(b1,b2)<=BALL_SIZE){
  
    return 1;
  }
  return 0;
}



/**
 * \brief La fonction qui  prend comme parametres deux boules et renHOLE_RADIUS+BALL_SIZE/2voie 1 si elles se touchent mais trop inferieur, 0 sinon.
 * \param b1 les données du boule1
 * \param b2 les données du boule2
 */
int touche_trop_inferieur(ball_t *b1, ball_t *b2){
  if (distance(b1,b2)<=BALL_SIZE*49/50){
  
    return 1;
  }
  return 0;
}



/**
 * \brief La fonction qui reposition de ball
 * \param ball les donnees du boule
 * \param x le abscisse du boule
 * \param y l'ordonne du boule
 */
void set_pos(ball_t *ball, double x, double y)
{
  ball->x=x;
  ball->y=y;
}



/**
 * \brief La fonction qui retourner la distance
 * \param x le abscisse du boule
 * \param y l'ordonne du boule
 */
double dis(double x,double y){
  return sqrt(x*x+y*y);
}





/**
 * \brief La fonction quand la boule blanche rencontre trop inférieur la boule de couleur, elle reposition.
 * \param b1 les données du boule1
 * \param b2 les données du boule2
 */
void touche_reposition(ball_t *b1, ball_t *b2){
  if (touche_trop_inferieur(b1,b2)==1){
    double a=b1->x;
    double b=b1->y;
    double c=b2->x;
    double d=b2->y;
   /* b1->vx=0;
    b1->vy=0;*/
    double alpha=(BALL_SIZE+1)/dis(c-a,d-b);
    c=alpha*c+(1-alpha)*a;
    d=alpha*d+(1-alpha)*b;
    set_pos(b2,c,d);
    
  }
}


/**
 * \brief La fonction touche reposition apply a les boules
 * \param world les données du monde
 */
void touche_reposition_boules(world_t *world){
  int i,j;
  for (i=0;i<NB_BALLS;i++){
    for(j=i+1;j<NB_BALLS;j++){
      touche_reposition(&world->ball[i],&world->ball[j]);
    }
  }
}



/**
 * \brief La fonction quand les boules rencontrent, elle collision.
 * \param b1 les données du boule1
 * \param b2 les données du boule2
 */
void touche_collision(ball_t *b1, ball_t *b2){
  if (touche(b1,b2)==1){
    double a=b1->x;
    double b=b1->y;
    double c=b2->x;
    double d=b2->y;
    double e=b1->vx;
    double f=b1->vy;
    double g=b2->vx;
    double h=b2->vy;
    b1->vx=0;
    b1->vy=0;
    b2->vx=0;
    b2->vy=0;
  collision(a,b,&e,&f,c,d,&g,&h);
     b1->x=a;
     b1->y=b;
     b2->x=c;
     b2->y=d;
     b1->vx=e;
     b1->vy=f;
     b2->vx=g;
     b2->vy=h;
  }
}


/**
 * \brief La fonction collision apply a les boules
 * \param world les données du monde
 */
void touche_collision_boules(world_t *world){
  int i,j;
  for (i=0;i<NB_BALLS;i++){
    for(j=i+1;j<NB_BALLS;j++){
      touche_collision(&world->ball[i],&world->ball[j]);
    }
  }
}



/**
 * \brief La fonction qui retourner la distance entre trou et boule
 * \param ball les donnees du boule
 * \param hole les donnees du hole
 */
double dist(ball_t *b1,hole_t *hole){
  return sqrt((b1->x+BALL_SIZE/2-hole->troux)*(b1->x+BALL_SIZE/2-hole->troux)+(b1->y+BALL_SIZE/2-hole->trouy)*(b1->y+BALL_SIZE/2-hole->trouy));
}
 /*coordonne  centre de la boule est (world->x+BALL_SIZE/2,world->y+BALL_SIZE/2)*/
 
 
 




/**
 * \brief La fonction qui renvoie 1 si elle touche lebord en bas, 0 sinon
 * \param ball les donnees du boule
 */
int boule_bord_right(ball_t *ball){
  if ((ball->x+BALL_SIZE/2)+BALL_SIZE/2>=RIGHT_LIMIT){
    return 1;
    }
return 0;
}
int boule_bord_left(ball_t *ball){
if ((ball->x+BALL_SIZE/2)-BALL_SIZE/2<=LEFT_LIMIT){
  return 1;
  }
return 0;
}
int boule_bord_bottom(ball_t *ball){
if ((ball->y+BALL_SIZE/2)+BALL_SIZE/2>=BOTTOM_LIMIT){
  return 1;
  }
return 0;
}
int boule_bord_top(ball_t *ball){
if ((ball->y+BALL_SIZE/2)-BALL_SIZE/2<=TOP_LIMIT){
  return 1;
  }
return 0;
}




/**
 * \brief La fonction qui renvoie 1 si une boule tombe dans un trou donn ́e en param`etres, 0 sinon.
 * \param ball les donnees du boule
 * \param hole les donnees du hole
 */
int tomb_trou(ball_t *b1,hole_t *hole){
  if (dist(b1,hole)<=HOLE_RADIUS+BALL_SIZE/2){
    return 1;
  }
  return 0;
}



/**
 * \brief La fonction de rafraichissementutilisera cette information pour afficher ou pas la boule.
 * \param world les données du monde
 */
void dispa(world_t *world){
  int i,j;
  for (j=0;j<NB_BALLS;j++){
    for (i=1;i<=NB_HOLES;i++){
      if (tomb_trou(&world->ball[j],&world->hole[i])==1){
	if(j==0)
	{
	    world->ball[j].x=SCREEN_WIDTH/2-BALL_SIZE;
	    world->ball[j].y=SCREEN_HEIGHT/2-BALL_SIZE;
	    world->ball[j].vx=0;
	    world->ball[j].vy=0;
	    ib=1;
	}
	else{
	    world->ball[j].x=0;
	    world->ball[j].y=SCREEN_HEIGHT/2-BALL_SIZE;
	    world->ball[j].vx=0;
	    world->ball[j].vy=0;
	    world->ball[j].dispar=0;
	}
      }
    }
  }
} 




/**
 * \brief La fonction qui met a jour la vitesse d’une boule ball avec les valeurs vx et vy
 * \param ball les donnees du boule
 * \param vx la VITESS qui va mettre du boule
 * \param vy la VITESS qui va mettre du boule
 */
void set_speed(ball_t *ball, double vx, double vy){
if(ball->x+BALL_SIZE/2<=HOLE_TOP_MID_X-BALL_SIZE/2||ball->x+BALL_SIZE/2>=HOLE_TOP_MID_X+BALL_SIZE/2){
  if (boule_bord_right(ball)==1||boule_bord_left(ball)==1)
{
  ball->vx=vx;
}
  else if(boule_bord_bottom(ball)==1||boule_bord_top(ball)==1)
  {
 ball->vy=vy;
  }
}
}


/**
 * \brief La fonction set speed apply a les boules
 * \param world les données du monde
 */
void set_speed_boules(world_t *world){
  int i;
  for (i=0;i<NB_BALLS;i++){
      set_speed(&world->ball[i],-world->ball[i].vx,-world->ball[i].vy);
  }
}




/**
 * \brief La fonction qui met a jour la position d’une bouleballavec les coordonnees (x,y)
 * \param ball les donnees du boule
 * \param vx la position qui va mettre du boule
 * \param vy la position qui va mettre du boule
 */
void set_position(ball_t *ball, double x, double y){
 if(ball->x+BALL_SIZE/2<=HOLE_TOP_MID_X-BALL_SIZE/2||ball->x+BALL_SIZE/2>=HOLE_TOP_MID_X+BALL_SIZE/2){
  if (boule_bord_right(ball)==1){
  ball->x=RIGHT_LIMIT-BALL_SIZE;
  ball->y=ball->y;
  }
  if (boule_bord_left(ball)==1){
  ball->x=LEFT_LIMIT;
  ball->y=ball->y;
  }
  if (boule_bord_bottom(ball)==1){
  ball->y=BOTTOM_LIMIT-BALL_SIZE;
  ball->x=ball->x;
  }
  if (boule_bord_top(ball)==1){
  ball->y=TOP_LIMIT;
  ball->x=ball->x;
}

  }
}


/**
 * \brief La fonction set position apply a les boules
 * \param world les données du monde
 */
void set_position_boules(world_t *world){
  int i;
  for (i=0;i<NB_BALLS;i++){
      set_position(&world->ball[i],0,0);
    }
}



/**
 * \brief La fonction rafrachit l'écran en fonction de l'état des données du monde
 * \param screen la surface de l'écran de jeu(world->y+BALL_SIZE/2)
 * \param world les données du monde
 */
void refresh_graphics(SDL_Surface *screen, world_t *world){
    apply_table(world->table,screen);
    if (world->ball[0].dispar==1){
    apply_sub_surface(world->boules[0],screen,0,0, BALL_SIZE,BALL_SIZE,world->ball[0].x,world->ball[0].y);
    }
    int i;
    for (i=1;i<NB_BALLS;i++){
	 if (world->ball[i].dispar==1){
	  apply_sub_surface(world->boules[i],screen,i*BALL_SIZE,0, BALL_SIZE,BALL_SIZE,world->ball[i].x,world->ball[i].y);
	 }
      
    }
    apply_sub_surface(world->queuebillard,screen,0,0,WIDTH_QUEUEBILLARD,HEIGHT_QUEUEBILLARD,world->ball[0].x,world->ball[0].y);

    /*coordonne  centre de la boule est (world->x+BALL_SIZE/2,world->y+BALL_SIZE/2)*/ 
    
    refresh_surface(screen);
}


/**
 * \brief La fonction qui trait les points des deux jouers
 * \param world les données du monde
 *//*
void point(world_t *world){
  int som1=0;
int som2=0;
int tour=0;
int som_boule=0;
int ig;

    tour=tour+1;
    for(ig=0;ig<NB_BALLS;ig++){
	  while(world->ball[ig].dispar==0){
	    world->ball[ig].dispar=2;*//*indique il est tombé et calcule*//*
	    if (tour/2==1)
	    {
	      if(ig==0)
	      {
		som2=som2+7;
	      }
	      som1=som1+ig;
	    }
	    if (tour/2==0)
	    {
	      if(ig==0)
	      {
		som1=som1+7;
	      }
	      som2=som2+ig;
	    }
	  }
	  printf("jour1:%d,jouer2:%d",som1,som2);
	}
	for(ig=1;ig<NB_BALLS;ig++)
	{
	  som_boule=som_boule+world->ball[ig].dispar;
	}
       if(som_boule==30)
       {
	 if(som1>som2)
	 {
	   printf("jouer1 a gagne ,point : %d",som1);
	 }
	 if(som1<som2)
	 {
	   printf("jouer2 a gagne, point : %d",som2);
	 }
	 if(som1==som2)
	 {
	   printf("aucune a gagne, point : %d",som1);
	 }
       }
        
        
  
    
}
*/




/**
 * \brief La fonction gère les évènements ayant eu lieu et qui n'ont pas encore été traités
 * \param event paramètre qui contient les événements
 * \param world les données du monde
 */
void handle_events(SDL_Event *event,world_t *world){

    int mouseX, mouseY;
    while( SDL_PollEvent( event ) ) {
        //Si l'utilisateur a cliqué sur le X de la fenêtre
        if( event->type == SDL_QUIT ) {
            //On quitte le programme
            world->gameover = 1;
        }
	if(event->type == SDL_MOUSEBUTTONUP){
        //on recupère les coordonnées de le souris
        SDL_GetMouseState(&mouseX, &mouseY);
        //on affiche les coordonnées de la souris
	world->ball[0].vx=SPEED_COEF*(mouseX-(world->ball[0].x+BALL_SIZE/2));
	world->ball[0].vy=SPEED_COEF*(mouseY-(world->ball[0].y+BALL_SIZE/2));
	tour=tour+1;
	if (ib==1)
	{
	  if (tour%2==1)
	    {
	      som2=som2+7;
	      iy=1;
	    }
	   else{
	     som1=som1+7;
	      iy=1;
	   }
	   ib=0;
	}
	  
	
      for(ig=0;ig<NB_BALLS;ig++){
	while(world->ball[ig].dispar==0){
	    if (tour%2==1)
	    {
	      if(iy==1){
		som2=som2+ig;
	      }
	      som1=som1+ig;
	    }
	    if (tour%2==0)
	    {
	       if(iy==1){
		som1=som1+ig;
	      }
	      som2=som2+ig;
	    }
	    world->ball[ig].dispar=2;/*indique il est tombé et calcule*/
	  }
	}
	iy=0;
	for(ig=1;ig<NB_BALLS;ig++)
	{
	  som_boule=som_boule+world->ball[ig].dispar;
	}
       if(som_boule==30)
       {
	 if(som1>som2)
	 {
	   printf("jouer1 a gagne ,point : %d",som1);
	 }
	 if(som1<som2)
	 {
	   printf("jouer2 a gagne, point : %d",som2);
	 }
	 if(som1==som2)
	 {
	   printf("aucune a gagne, point : %d",som1);
	 }
       }
       som_boule=0;
       printf("jour1:%d\njouer2:%d\n",som1,som2);
        printf("*******\n");
        }
        
        
    }
    
}






/**
 *  \brief programme principal qui implémente la boucle du jeu
 */

int main( int argc, char* args[] )
{

    SDL_Event event;
    world_t world;
    SDL_Surface *screen;

    screen = init_sdl(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    init_data(&world);
    init_graphics(screen,&world);
    
    
    while(!is_game_over(&world)){
        handle_events(&event,&world);
        update_data(&world);
        set_speed_boules(&world);
	set_position_boules(&world);
	touche_collision_boules(&world);
	touche_reposition_boules(&world);
	dispa(&world);
        refresh_graphics(screen,&world);
        SDL_Delay(10);
    }
    
    clean_data(&world);
    quit_sdl();
    
    return 0;
}
