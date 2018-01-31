//
//  particalsystem.cpp
//  particle-sys
//
//  Created by Ousyoukan on 16/1/15.
//  Copyright (c) 2016年 Ousyoukan. All rights reserved.
//

#include "particalsystem.h"

ParticalSystem::ParticalSystem()
{
    //
}

ParticalSystem::~ParticalSystem()
{
    //
}


//init particle
void ParticalSystem::init()
{
    int i;
    srand(unsigned(time(0)));
    Color colors[3]={{1,0,0.8,1},{0,0.8,0.6,1}};
    
    for(i=0;i<ptlCount;i++)
    {
        
        Particle tmp={Vector3D(0,0,0),Vector3D(((rand()%50)-26.0f),((rand()%50)-26.0f),((rand()%50)-26.0f)),Vector3D(0,0,0),colors[rand()%2],0.0f,static_cast<float>(1+0.1*(rand()%10)),0.3f};
        particles.push_back(tmp);
    }
    
    myobject=gluNewQuadric();
}


void ParticalSystem::simulate(float dt,float posX,float posY)
{
    aging(dt,posX,posY);
    applyGravity();
    kinematics(dt);
    checkBump(-15,15,-15,15);
}


void ParticalSystem::aging(float dt,float posX,float posY)
{
    for(vector<Particle>::iterator iter=particles.begin();iter!=particles.end();iter++)
    {
        iter->age+=dt;
        if(iter->age>iter->life)
        {
            iter->position=Vector3D(posX,posY,0);
            iter->age=0.0;
            iter->velocity=Vector3D(((rand()%30)-15.0f),((rand()%30)-11.0f),((rand()%30)-15.0f));
        }
    }
}


void ParticalSystem::applyGravity()
{
    for(vector<Particle>::iterator iter=particles.begin();iter!=particles.end();iter++)
            iter->acceleration=Vector3D(0,gravity,0);
}

void ParticalSystem::kinematics(float dt)
{
    for(vector<Particle>::iterator iter=particles.begin();iter!=particles.end();iter++)
    {
        iter->position = iter->position+iter->velocity*dt;
        iter->velocity = iter->velocity+iter->acceleration*dt;
    }
}


void ParticalSystem::checkBump(float x1,float x2,float y1,float y2)
{
    for(vector<Particle>::iterator iter=particles.begin();iter!=particles.end();iter++)
        {
            if (iter->position.x - iter->size < x1 || iter->position.x +iter->size > x2) iter->velocity.x = -iter->velocity.x;
            if (iter->position.y - iter->size < y1 || iter->position.y + iter->size > y2) iter->velocity.y = -iter->velocity.y;
        }
}


void ParticalSystem::render()
{

    for(vector<Particle>::iterator iter=particles.begin();iter!=particles.end();iter++)
    {
        float alpha = 1 - iter->age / iter->life;
        Vector3D tmp=iter->position;
        glColor4f(iter->color.r,iter->color.g,iter->color.b,alpha);
        
        glPushMatrix();
        glTranslatef(tmp.x,tmp.y,tmp.z);
        gluSphere(myobject,iter->size, 32, 16);
        glPopMatrix();
    }


    glColor4f(0.0f,0.0f,0.0f,0.1);
    glBegin(GL_QUADS);
    glVertex3f(-20.0f  , -20.0f  ,  20.0f  );
    glVertex3f( 20.0f  , -20.0f  ,  20.0f  );
    glVertex3f( 20.0f  ,  20.0f  ,  20.0f  );
    glVertex3f(-20.0f  ,  20.0f  ,  20.0f  );
    glEnd();
}
