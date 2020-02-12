/**
 * \file main.c
 * \brief Programme principal du niveau 0, à compléter
 * \author Mathieu Constant
 * \version 0.1
 * \date 13 mars 2019
 */

#include "sdl-light.h"

/**
 * \brief Largeur de l'écran de jeu
 */
#define SCREEN_WIDTH 640

/**
 * \brief Hauteur de l'écran de jeu
 */
#define SCREEN_HEIGHT 480

/**
 * \brief Taille d'une bulle
 */
#define BUB_SIZE 40

/**
 * \brief Taille du bonhomme (sprite)
 */
#define SPRITE_SIZE 32

/**
 * \brief Pas de déplacement du sprite
 */

#define MOVING_STEP 5
#define WIND_VX 1
#define VX_BUB 150
#define VY_BUB 100


/**
 * \brief Représentation du monde du jeu
 */

struct world_s{
    SDL_Surface* background;
    SDL_Surface* sprite;
  SDL_Surface* bub_blue;
    int gameover;
    int x;
    int y;
    int vx;
    int vy;

    
    };

typedef struct world_s world_t;



/**
 * \brief La fonction initialise les données du monde du jeu
 * \param world les données du monde
 */


void init_data(world_t * world){
    world->background = load_image( "ressources/background.bmp" );
    world->sprite = load_image( "ressources/sprite.bmp" );
    world->bub_blue=load_image("ressources/bub_blue.bmp");
    world->gameover = 0;
    world->x=SCREEN_WIDTH/2-SPRITE_SIZE/2;
    world->y=SCREEN_HEIGHT/2-SPRITE_SIZE/2;
    world->vx=VX_BUB-BUB_SIZE/2;
    world->vy=VY_BUB-BUB_SIZE/2;
    
    
}


/**
 * \brief La fonction nettoie les données du monde
 * \param world les données du monde
 */


void clean_data(world_t *world){
    SDL_FreeSurface(world->background);
    SDL_FreeSurface(world->sprite);
    SDL_FreeSurface(world->bub_blue);
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

void update_data(world_t *world)
{
  world->x=world->x+WIND_VX;
}


/**
 * \brief La fonction initialise les transparences des différentes surfaces
 * \param screen la surface correspondant à l'écran de jeu
 * \param world les données du monde
 */

void  init_graphics(SDL_Surface *screen, world_t *world){
   /* On indique que la couleur (255, 0, 255) est transparente */
  set_transparence(screen,world->sprite, 255, 0, 255);
set_transparence(screen,world->bub_blue, 255, 0, 255);
}


/**
 * \brief La fonction applique la surface de l'image de fond à quatre positions différentes sur l'écran de jeu, de sorte de complètement couvrir ce dernier
 * \param screen l'écran de jeu
 * \param bg la surface de l'image de fond
 */
void apply_background(SDL_Surface *bg, SDL_Surface *screen){
    apply_surface(bg,screen,0,0);
    apply_surface(bg, screen,SCREEN_WIDTH/2, 0);
    apply_surface(bg, screen, 0, SCREEN_HEIGHT/2 );
    apply_surface(bg, screen, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 );
}





/**
 * \brief La fonction rafrachit l'écran en fonction de l'état des données du monde
 * \param screen la surface de l'écran de jeu
 * \param world les données du monde
 */

void refresh_graphics(SDL_Surface *screen, world_t *world){
    apply_background(world->background,screen);
    apply_surface(world->sprite,screen,world->x,world->y);
    apply_sub_surface(world->bub_blue,screen,0,0,BUB_SIZE,BUB_SIZE,world->vx,world->vy);
    
    
    
    refresh_surface(screen);
}



/**
 * \brief La fonction gère les évènements ayant eu lieu et qui n'ont pas encore été traités
 * \param event paramètre qui contient les événements
 * \param world les données du monde
 */


void handle_events(SDL_Event *event,world_t *world){
    Uint8 *keystates;
    int mouseX, mouseY;
    while( SDL_PollEvent( event ) ) {
        //Si l'utilisateur a cliqué sur le X de la fenêtre
        if( event->type == SDL_QUIT ) {
            //On quitte le programme
            world->gameover = 1;
        }
        
        /* gestion des evenements clavier */
        keystates = SDL_GetKeyState( NULL );
	/* Si l'utilisateur appuie sur
         la touche flèche  */
        if( keystates[ SDLK_UP ] ) {
	  world->y=world->y-10;
	}
        if( keystates[ SDLK_DOWN ] ) {
	  world->y=world->y+10;
	}
	if( keystates[ SDLK_LEFT ] ) {
	  world->x=world->x-10;
	}
	if( keystates[ SDLK_RIGHT ] ) {
	  world->x=world->x+10;
	}
        if(event->type == SDL_MOUSEBUTTONUP){
        //on recupère les coordonnées de le souris
        SDL_GetMouseState(&mouseX, &mouseY);
        //on affiche les coordonnées de la souris
	world->vx=mouseX;
	world->vy=mouseY;
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
        refresh_graphics(screen,&world);
        SDL_Delay(10);
    }
    
    clean_data(&world);
    quit_sdl();
    
    
    return 0;
}
