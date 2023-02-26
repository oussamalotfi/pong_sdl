#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>


#define WIDTH 1280
#define HEIGHT 720
#define PADDLE_HEIGHT 100
#define PADDLE_WIDTH 20
#define MARGIN 50
#define PADDLE_SPEED 15

typedef struct ball {
  int size;
  int xpos;
  int ypos;
  int xspeed;
  int yspeed;
}ball;


typedef struct paddle{
  int xpos;
  int ypos;
}paddle;



int main(int argc,char *argv[]){
  // initialize SDL
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow("sdl_pong",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,SDL_WINDOW_OPENGL);

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  bool running = true;
  SDL_Event event;
  
  // initialize ball
  srand(time(0));

  ball myball;
  myball.size = 20;
  myball.xpos = WIDTH/2 - myball.size/2;
  myball.ypos = HEIGHT/2 - myball.size/2;
  int random1 = rand()%2;
  int random2 = rand()%2;

  if(random1 == 0) random1 = -1;
  if(random2 == 0) random2 = -1;
  myball.xspeed = random1*3;
  myball.yspeed = random2*3;

 
  // initialize paddles

  paddle rightp,leftp;
  leftp.ypos = HEIGHT/2 - PADDLE_HEIGHT/2;
  leftp.xpos = MARGIN;
  rightp.ypos = HEIGHT/2 - PADDLE_HEIGHT/2;
  rightp.xpos = WIDTH - MARGIN - PADDLE_WIDTH;


  //game ending boolean
  bool game_end = false;
  bool right_won = false;
  bool left_won = false;



  while(running){
    while(SDL_PollEvent(&event)) if(event.type == SDL_QUIT) running = false;

    //clear screen
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);

    //draw

    if(myball.xpos >= WIDTH-myball.size || myball.xpos <= 0 ) myball.xspeed *= -1; 
    if(myball.ypos >= HEIGHT-myball.size || myball.ypos <= 0 ) myball.yspeed *= -1;

    //check collision with left paddle

    if (myball.xpos + myball.size >= leftp.xpos 
      && leftp.xpos + PADDLE_WIDTH >= myball.xpos
      && myball.ypos + myball.size >= leftp.ypos 
      && leftp.ypos + PADDLE_HEIGHT >= myball.ypos)
        {myball.xspeed *= -1;
        printf("collision with left paddle\n");}

    //check collision with right paddle

    if (myball.xpos + myball.size >= rightp.xpos 
      && rightp.xpos + PADDLE_WIDTH >= myball.xpos
      && myball.ypos + myball.size >= rightp.ypos 
      && rightp.ypos + PADDLE_HEIGHT >= myball.ypos)
        {myball.xspeed *= -1;
        printf("collision with right paddle\n");}

    // move the ball
    myball.xpos = myball.xpos + myball.xspeed;
    myball.ypos = myball.ypos + myball.yspeed; 
  

    // move right paddle
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    
      if(state[SDL_SCANCODE_DOWN] && rightp.ypos < HEIGHT - PADDLE_HEIGHT ){
	      rightp.ypos += PADDLE_SPEED;
	      printf("DOWN is pressed\n");
      }
      if(state[SDL_SCANCODE_UP] && rightp.ypos > 0){
	      rightp.ypos -= PADDLE_SPEED;
        printf("UP is pressed\n");

      }

      // move left paddle    
      if(state[SDL_SCANCODE_S] && leftp.ypos < HEIGHT - PADDLE_HEIGHT ){
	      leftp.ypos += PADDLE_SPEED;
	      printf("S is pressed\n");
      }
      if(state[SDL_SCANCODE_W] && leftp.ypos > 0){
	      leftp.ypos -= PADDLE_SPEED;
        printf("W is pressed\n");

      }

      //check win and loss
      if(myball.xpos == 0 ){
        game_end = true;
        right_won = true;
        myball.xspeed = 0;
        myball.yspeed = 0;
      }
      if(myball.xpos + myball.size == WIDTH){
        game_end = true;
        left_won = true;
        myball.xspeed = 0;
        myball.yspeed = 0;
      }

    // draw ball and paddles

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect_ball = {myball.xpos,myball.ypos,myball.size,myball.size};
    SDL_RenderFillRect(renderer, &rect_ball);

    if(right_won){
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    }else if(left_won){
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    else{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }
    SDL_Rect rect_rightpaddle = {rightp.xpos,rightp.ypos,PADDLE_WIDTH,PADDLE_HEIGHT};
    SDL_RenderFillRect(renderer, &rect_rightpaddle);
    

    if(left_won){
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    }else if(right_won){
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    else{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }
    SDL_Rect rect_leftpaddle = {leftp.xpos,leftp.ypos,PADDLE_WIDTH,PADDLE_HEIGHT};
    SDL_RenderFillRect(renderer, &rect_leftpaddle);

    //show what was drawn
    SDL_RenderPresent(renderer);

  }

  //release resources
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return(0);
}