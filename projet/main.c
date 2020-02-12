/**
 * \file main.c
 * \author Yunfan CAI
 * \version 0.2
 * \date 25 mars 2019
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
 * \brief Taille d'une BALL
 */
#define SPEED_COEF 0.1

#define FRICTION 0.05
/**
 * \brief REDUIRE VITESS
 */
#define LEFT 625

#define RIGHT 663

#define LEFT_LIMIT 85
/**
 * \brief abscisse du bord droit
 */
#define RIGHT_LIMIT 1202
/**
 * \brief abscisse du bord gauche
 */
#define TOP_LIMIT 85
/**
 * \brief ordonne du bord haut
 */
#define BOTTOM_LIMIT 640
/**
 * \brief ordonne du bord en bas
 */
#define HOLE_RADIUS 35

#define NB_HOLES 6



/**
 * \brief Représentation du monde du jeu
 */

struct world_s{
    SDL_Surface* table;
    SDL_Surface* boules;
    SDL_Surface* boules1;
    struct ball_s* ball_t;
    struct ball_s* ball1;
    int gameover;

    };

typedef struct world_s world_t;

typedef struct ball_s
{
    int x;
    int y;
    int vx;
    int vy;
}ball_t,ball1;


/**
 * \brief La fonction initialise les données du monde du jeu
 * \param world les données du monde
 */


void init_data(world_t * world){
   world->table=load_image("ressources/table.bmp");
    world->boules=load_image("ressources/boules.bmp");
    world->boules1=load_image("ressources/boules.bmp");
    world->gameover = 0;
    world->ball_t->x=SCREEN_WIDTH/3;
    world->ball_t->y=SCREEN_HEIGHT/2;
    world->ball_t->vx=0;
    world->ball_t->vy=0;
    world->ball1->x=2*SCREEN_WIDTH/3;
    world->ball1->y=SCREEN_HEIGHT/2;
    world->ball1->vx=0;
    world->ball1->vy=0;
    
}


/**
 * \brief La fonction nettoie les données du monde
 * \param world les données du monde
 */


void clean_data(world_t *world){
    SDL_FreeSurface(world->table);
    SDL_FreeSurface(world->boules);
    SDL_FreeSurface(world->boules1);
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
 * \brief La fonction met à jour les données en tenant compte de la physique du monde
 * \param les données du monde
 */

void update_data(world_t *world){
  world->ball_t->x=world->ball_t->x+world->ball_t->vx;
  world->ball_t->y=world->ball_t->y+world->ball_t->vy;
/* express initial
  (world->x+BALL_SIZE/2)=(world->x+BALL_SIZE/2)+world->vx;
  (world->y+BALL_SIZE/2)=(world->y+BALL_SIZE/2)+world->vy;*/
  world->ball_t->vx=world->ball_t->vx*(1-FRICTION);
  world->ball_t->vy=world->ball_t->vy*(1-FRICTION);
  world->ball1->x=world->ball1->x+world->ball1->vx;
  world->ball1->y=world->ball1->y+world->ball1->vy;
/* express initial
  (world->x+BALL_SIZE/2)=(world->x+BALL_SIZE/2)+world->vx;
  (world->y+BALL_SIZE/2)=(world->y+BALL_SIZE/2)+world->vy;*/
  world->ball_t->vx=world->ball1->vx*(1-FRICTION);
  world->ball_t->vy=world->ball1->vy*(1-FRICTION);
}


/**
 * \brief La fonction initialise les transparences des différentes surfaces
 * \param screen la surface correspondant à l'écran de jeu
 * \param world les données du monde
 */

void  init_graphics(SDL_Surface *screen, world_t *world){
    set_transparence(screen,world->boules, 255, 0, 255);
    set_transparence(screen,world->boules, 255, 0, 255);
}


void apply_table(SDL_Surface *table, SDL_Surface *screen){
    apply_surface(table,screen,0,0);
}






/**
 * \brief La fonction rafrachit l'écran en fonction de l'état des données du monde
 * \param screen la surface de l'écran de jeu(world->y+BALL_SIZE/2)
 * \param world les données du monde
 */




void refresh_graphics(SDL_Surface *screen, world_t *world){
    apply_table(world->table,screen);
    apply_sub_surface(world->boules,screen,0,0, BALL_SIZE, BALL_SIZE,world->ball_t->x,world->ball_t->y);
     apply_sub_surface(world->boules,screen,BALL_SIZE,0, BALL_SIZE, BALL_SIZE,world->ball1->x,world->ball1->y);
     
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
	world->ball_t->vx=SPEED_COEF*(mouseX-(world->ball_t->x+BALL_SIZE/2));
	world->ball_t->vy=SPEED_COEF*(mouseY-(world->ball_t->y+BALL_SIZE/2));
         /* world->ball1->vx=SPEED_COEF*(mouseX-world->ball1->x);
	    world->ball1->vy=SPEED_COEF*(mouseY-world->ball1->y);*/
        
        }
        
        
    }
    
}

int boule_bord_droit(ball_t *ball )
{
 if(ball->x>=RIGHT_LIMIT-BALL_SIZE)
 {
 return 1;
 }
 else
 {
  return 0;
 }
}

int boule_bord_gauche(ball_t *ball )
{
if(ball->x<=LEFT_LIMIT)
 {
 return 1;
 }
 else
 {
  return 0;
 }
}

int boule_bord_haut(ball_t *ball )
{
if(ball->x<=TOP_LIMIT)
 {
 return 1;
 }
 else
 {
  return 0;
 }
}

int boule_bord_bas(ball_t *ball)
{
if(ball->x>=BOTTOM_LIMIT-BALL_SIZE)
 {
 return 1;
 }
 else
 {
  return 0;
 }
}

void set_speed(ball_t *ball, double vx, double vy)
{
  if(boule_bord_droit(ball)==1||boule_bord_gauche(ball)==1)
 {
   ball->vx=vx;
 }

  if(boule_bord_haut(ball)==1||boule_bord_bas(ball)==1)
 {
   ball->vy=vy;
 }
}

void set_position(ball_t *ball, double x, double y)
{
  if(ball->x+BALL_SIZE>RIGHT_LIMIT)
  {
    ball->x=x-BALL_SIZE;
  }
 if(ball->x<LEFT_LIMIT)
  {
    ball->x=RIGHT_LIMIT;
  }
 if(ball->y<TOP_LIMIT)
  {
    ball->y=y;
  }
 if(ball->y+BALL_SIZE>BOTTOM_LIMIT)
  {
    ball->y=y-BALL_SIZE;
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
        refresh_graphics(screen,&world);
        SDL_Delay(10);
    }
    
    clean_data(&world);
    quit_sdl();
    
    
    return 0;
}
