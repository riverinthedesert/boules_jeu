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
 * \brief abscisse de hole en haut
 */
#define HOLE_TOP_X 644
/**
 * \brief ordonne de hole en haut
 */
#define HOLE_TOP_Y 40
/**
 * \brief abscisse de hole en bas
 */
#define HOLE_BOTTOM_X 644
/**
 * \brief ordonne de hole en bas
 */
#define HOLE_BOTTOM_Y 685



/**
 * \brief Représentation du monde du jeu
 */



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
  int diapar;
};
typedef struct ball_s ball_t;




/**
 * 
 * \brief world les données du monde
 */
struct world_s{
    SDL_Surface* table;
    SDL_Surface* boules;
    SDL_Surface* boulesa;
    int gameover;
    ball_t ball;
    ball_t balla;
    hole_t hole[NB_HOLES];
    };

typedef struct world_s world_t;




/**
 * \brief La fonction initialise les données du trou
 */
void ini_trou(hole_t * hole,double x, double y){
    hole->troux=x;
    hole->trouy=y;
}
void ini_trous(world_t *world){
    ini_trou(&world->hole[0],75,75);
    ini_trou(&world->hole[1],644,40);
    ini_trou(&world->hole[2],1213,75);
    ini_trou(&world->hole[3],1213,650);
    ini_trou(&world->hole[4],644,685);
    ini_trou(&world->hole[5],75,650);
}




/**
 * \brief La fonction initialise les données du boule blanche
 */
void ini_boule_blanche(world_t *world){
    world->ball.x=SCREEN_WIDTH/3-BALL_SIZE;
    world->ball.y=SCREEN_HEIGHT/2-BALL_SIZE;
    world->ball.vx=0;
    world->ball.vy=0;
    world->ball.diapar=1;
}




/**
 * \brief La fonction initialise les données du boule jaune
 */ 
void ini_boule_jaune(world_t *world){
    world->balla.x=2*SCREEN_WIDTH/3-BALL_SIZE;
    world->balla.y=SCREEN_HEIGHT/2-BALL_SIZE;
    world->balla.vx=0;
    world->balla.vy=0;
    world->balla.diapar=1;
}




/**
 * \brief La fonction initialise les données du monde du jeu
 * \param world les données du monde
 */
void init_data(world_t * world){
    world->table=load_image("ressources/table.bmp");
    world->boules=load_image("ressources/boules.bmp");
    world->boulesa=load_image("ressources/boules.bmp");
    world->gameover = 0;
    ini_boule_blanche(world);
    ini_boule_jaune(world);
    /*express initial
    (world->x+BALL_SIZE/2)=SCREEN_WIDTH/3-BALL_SIZE/2;
    (world->y+BALL_SIZE/2)=SCREEN_HEIGHT/2-BALL_SIZE/2;*/
    ini_trous(world);

}



/**
 * \brief La fonction nettoie les données du monde
 * \param world les données du monde
 */
void clean_data(world_t *world){
    SDL_FreeSurface(world->table);
    SDL_FreeSurface(world->boules);
    SDL_FreeSurface(world->boulesa);
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
 * \brief La fonction met à jour les données en tenant compte de la physique du monde du boule blanche
 */
void update_data_boule_blache(world_t *world){
    world->ball.x=world->ball.x+world->ball.vx;
    world->ball.y=world->ball.y+world->ball.vy;
    /* express initial
    (world->x+BALL_SIZE/2)=(world->x+BALL_SIZE/2)+world->vx;
    (world->y+BALL_SIZE/2)=(world->y+BALL_SIZE/2)+world->vy;*/
    world->ball.vx=world->ball.vx*(1-FRICTION);
    world->ball.vy=world->ball.vy*(1-FRICTION);
}



/**
 * \brief La fonction met à jour les données en tenant compte de la physique du monde du boule jaune
 */
void update_data_boule_jaune(world_t *world){
    world->balla.x=world->balla.x+world->balla.vx;
    world->balla.y=world->balla.y+world->balla.vy;
    /* express initial
    (world->x+BALL_SIZE/2)=(world->x+BALL_SIZE/2)+world->vx;
    (world->y+BALL_SIZE/2)=(world->y+BALL_SIZE/2)+world->vy;*/
    world->balla.vx=world->balla.vx*(1-FRICTION);
    world->balla.vy=world->balla.vy*(1-FRICTION);
}




/**
 * \brief La fonction met à jour les données en tenant compte de la physique du monde
 * \param les données du monde
 */
void update_data(world_t *world){
    update_data_boule_blache(world);
    update_data_boule_jaune(world);
}




/**
 * \brief La fonction initialise les transparences des différentes surfaces
 * \param screen la surface correspondant à l'écran de jeu
 * \param world les données du monde
 */
void  init_graphics(SDL_Surface *screen, world_t *world){
    set_transparence(screen,world->boules, 255, 0, 255);
    set_transparence(screen,world->boulesa, 255, 0, 255);
}
void apply_table(SDL_Surface *table, SDL_Surface *screen){
    apply_surface(table,screen,0,0);
}




/**
 * \brief La fonction qui calcule et renvoie la distance euclidienne entreles deux boulesb1etb2
 */
double distance(ball_t *b1, ball_t *b2){
  return sqrt((b1->x-b2->x)*(b1->x-b2->x)+(b1->y-b2->y)*(b1->y-b2->y));
}



/**
 * \brief La fonction qui  prend comme parametres deux boules et renvoie 1 si elles se touchent, 0 sinon.
 */
int touche(ball_t *b1, ball_t *b2){
  if (distance(b1,b2)<=BALL_SIZE ){
  
    return 1;
  }
  return 0;
}



/**
 * \brief La fonction qui  prend comme parametres deux boules et renvoie 1 si elles se touchent mais trop inferieur, 0 sinon.
 */
int touche_trop_inferieur(ball_t *b1, ball_t *b2){
  if (distance(b1,b2)<=BALL_SIZE*5/6 ){
  
    return 1;
  }
  return 0;
}



/**
 * \brief La fonction qui reposition de ball
 */
void set_pos(ball_t *ball, double x, double y)
{
  ball->x=x;
  ball->y=y;
}



/**
 * \brief La fonction qui retourner la distance
 */
double dis(double x,double y){
  return sqrt(x*x+y*y);
}



/**
 * \brief La fonction quand la boule blanche rencontre la boule de couleur, elle collision.
 */
void touche_collision(world_t *world){
  if (touche(&world->ball,&world->balla)==1){
    collision(world->ball.x,world->ball.y,&world->ball.vx,&world->ball.vy,world->balla.x,world->balla.y,&world->balla.vx,&world->balla.vy);
  }
}



/**
 * \brief La fonction quand la boule blanche rencontre trop inférieur la boule de couleur, elle reposition.
 */
void touche_reposition(ball_t *b1, ball_t *b2){
  if (touche_trop_inferieur(b1,b2)==1){
    double a=b1->x;
    double b=b1->y;
    double c=b2->x;
    double d=b2->y;
    b1->vx=0;
    b1->vy=0;
    double alpha=(BALL_SIZE+1)/dis(c-a,d-b);
    c=alpha*c+(1-alpha)*a;
    d=alpha*d+(1-alpha)*b;
    set_pos(b2,c,d);
  }
}



/**
 * \brief La fonction qui retourner la distance entre trou et boule
 */
double dist(ball_t *b1,hole_t *hole){
  return sqrt((b1->x+BALL_SIZE/2-hole->troux)*(b1->x+BALL_SIZE/2-hole->troux)+(b1->y+BALL_SIZE/2-hole->trouy)*(b1->y+BALL_SIZE/2-hole->trouy));
}
 /*coordonne  centre de la boule est (world->x+BALL_SIZE/2,world->y+BALL_SIZE/2)*/
 
 
 
/**
 * \brief La fonction qui renvoie 1 si une boule tombe dans un trou donn ́e en param`etres, 0 sinon.
 */
int tomb_trou(ball_t *b1,hole_t *hole){
  if (dist(b1,hole)<=HOLE_RADIUS+BALL_SIZE/2){
    return 1;
  }
  return 0;
}



/**
 * \brief La fonction de rafraichissementutilisera cette information pour afficher ou pas la boule.
 */
void dispa(world_t *world){
  int i;
  for (i=0;i<=5;i++){
  if (tomb_trou(&world->ball,&world->hole[i])==1){
      world->ball.diapar=0;
    }
    
  }
} 




/**
 * \brief La fonction qui renvoie 1 si elle touche lebord en bas, 0 sinon
 * \param screen laboule_bord(&world); surface de l'écran de jeu
 * \param world les données du monde
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
 * \brief La fonction qui renvoie 1 si elle touche lebord en bas, 0 sinon
 * \param screen laboule_bord(&world); surface de l'écran de jeu
 * \param world les données du monde
 */
int boule_bord_hole_top(ball_t *ball){
  if (sqrt((ball->x+BALL_SIZE/2-HOLE_TOP_X)*(ball->x+BALL_SIZE/2-HOLE_TOP_X)+(ball->y+BALL_SIZE/2-HOLE_TOP_Y)*(ball->y+BALL_SIZE/2-HOLE_TOP_Y))<=HOLE_RADIUS+BALL_SIZE/2)
  {
    return 1;
    }
return 0;
}
int boule_bord_hole_bottom(ball_t *ball){
  if (sqrt((ball->x+BALL_SIZE/2-HOLE_BOTTOM_X)*(ball->x+BALL_SIZE/2-HOLE_BOTTOM_X)+(ball->y+BALL_SIZE/2-HOLE_BOTTOM_Y)*(ball->y+BALL_SIZE/2-HOLE_BOTTOM_Y))<=HOLE_RADIUS+BALL_SIZE/2)
  {
    return 1;
    }
return 0;
}



/**
 * \brief La fonction qui met a jour la vitesse d’une boule ball avec les valeurs vx et vy
 * \param screen la surface de l'écran de dispajeu
 * \param world les données du monde
 */
void set_speed(ball_t *ball, double vx, double vy){
if(ball->x+BALL_SIZE/2<=HOLE_TOP_X-BALL_SIZE/2||ball->x+BALL_SIZE/2>=HOLE_TOP_X+BALL_SIZE/2){ 
  if (boule_bord_right(ball)==1||boule_bord_left(ball)==1)
{
  ball->vx=vx;
}
  if(boule_bord_bottom(ball)==1||boule_bord_top(ball)==1)
  {
 ball->vy=vy;
  }
 }
}



/**
 * \brief La fonction qui met a jour la position d’une bouleballavec les coordonnees (x,y)
 * \param screen la surface de l'écran de jeu
 * \param world les données du monde
 */
void set_position(ball_t *ball, double x, double y){
  if(ball->x+BALL_SIZE/2>=HOLE_TOP_X+BALL_SIZE/2||ball->x+BALL_SIZE/2<=HOLE_TOP_X-BALL_SIZE/2){
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
 * \brief La fonction rafrachit l'écran en fonction de l'état des données du monde
 * \param screen la surface de l'écran de jeu(world->y+BALL_SIZE/2)
 * \param world les données du monde
 */
void refresh_graphics(SDL_Surface *screen, world_t *world){
    apply_table(world->table,screen);
    if (world->ball.diapar==1){
    apply_sub_surface(world->boules,screen,0,0, BALL_SIZE, BALL_SIZE,world->ball.x,world->ball.y);
    }
    apply_sub_surface(world->boulesa,screen,BALL_SIZE,0, BALL_SIZE, BALL_SIZE,world->balla.x,world->balla.y);
    /*coordonne  centre de la boule est (world->x+BALL_SIZE/2,world->y+BALL_SIZE/2)*/ 
    
    refresh_surface(screen);
}



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
	world->ball.vx=SPEED_COEF*(mouseX-(world->ball.x+BALL_SIZE/2));
	world->ball.vy=SPEED_COEF*(mouseY-(world->ball.y+BALL_SIZE/2));
        
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
        set_speed(&world.ball,-world.ball.vx,-world.ball.vy);
	set_position(&world.ball,0,0);
	touche_collision(&world);
	touche_reposition(&world.ball,&world.balla);
	dispa(&world);
        refresh_graphics(screen,&world);
        SDL_Delay(10);
    }
    
    clean_data(&world);
    quit_sdl();
    
    
    return 0;
}
