//
//  main.cpp
//  particle-sys
//
//  Created by Ousyoukan on 16/1/15.
//  Copyright (c) 2016年 Ousyoukan. All rights reserved.
//

#include <SDL/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "particalsystem.h"
#include "mass.h"
SDL_Surface *screen;
ParticalSystem ps;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT =800;
const int SCREEN_BPP = 32;
//Whether the window is windowed or not
bool windowed;

void quit( int code )
{
    SDL_Quit( );
    //exit
    exit( code );
}




void handleKeyEvent( SDL_keysym* keysym )
{
    switch( keysym->sym )
    {
        case SDLK_ESCAPE:
            quit( 0 );
            break;
        case SDLK_SPACE:
            break;
        default:
            break;
    }
}


void resizeGL(int width,int height)
{
    if ( height == 0 )
    {
        height = 1;
    }
    //Reset View
    glViewport( 0, 0, (GLint)width, (GLint)height );
    //Choose the Matrix mode
    glMatrixMode( GL_PROJECTION );
    //reset projection
    glLoadIdentity();
    
    //set perspection
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );
    //choose Matrix mode
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}


void handleEvents()
{
    // SDL event placeholder
    SDL_Event event;
    
    while( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
            case SDL_KEYDOWN:
                handleKeyEvent( &event.key.keysym );
                break;
            case SDL_QUIT:
                quit( 0 );
                break;
                
        }
    }
}

void initSDL(int width,int height,int bpp,int flags)
{
    // First, initialize SDL's video subsystem.
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        fprintf( stderr, "Video initialization failed: %s\n",
                SDL_GetError( ) );
        quit( 1 );
    }
    atexit(SDL_Quit);
    
    //Set Attribute of OpenGL in SDL
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    

    screen= SDL_SetVideoMode( width, height, bpp,flags);
    if(!screen )
    {
        fprintf( stderr, "Video mode set failed: %s\n",SDL_GetError( ) );
        quit( 1 );
        windowed=false;
    }
    else windowed=true;
    
    resizeGL(screen->w, screen->h);
    //Set caption
    SDL_WM_SetCaption( "Particle System", NULL );
    
}

void renderGL()
{
    // Clear the color and depth buffers.
    glClear(GL_DEPTH_BUFFER_BIT );
    
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    
    
    glTranslatef(0.0f,0.0f,-35.0f);
    
    ps.render();
    
    SDL_GL_SwapBuffers( );
}


void initGL( int width, int height )
{
    float ratio = (float) width / (float) height;
    
    //shade model
    glShadeModel( GL_SMOOTH );
    
    
    glClearColor( 0, 0, 0, 0 );
    glViewport( 0, 0, width, height );
    
    //texture
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode( GL_PROJECTION );
    // 设置深度缓存
    glClearDepth( 1.0 );
    
    glEnable( GL_DEPTH_TEST );
    // 所作深度测试的类型
    glDepthFunc( GL_LEQUAL );
    
    // 告诉系统对透视进行修正
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glLoadIdentity();
    gluPerspective( 60.0, ratio, 1.0, 100.0 );
}

//change xpos of particle
float posTransX(int posX)
{
    return (posX-400)/20.0;
    
}

//change ypos of particle
float posTransY(int posY)
{
    return (400-posY)/20.0;
}


int main( int argc, char* argv[] )
{
    int posX,posY;
    int para= SDL_OPENGL|SDL_RESIZABLE;
    
    initSDL(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,para);
    
    initGL(SCREEN_WIDTH, SCREEN_HEIGHT );
    
    ps=ParticalSystem(100,-15.0);
    ps.init();//first
    
    
    while(true)
    {

        handleEvents( );
        SDL_GetMouseState(&posX, &posY);
        ps.simulate(0.01,posTransX(posX),posTransY(posY));
        //draw
        renderGL( );
    }
    return 0;
}
