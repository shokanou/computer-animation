//
//  particlesystem.h
//  particle-sys
//
//  Created by Ousyoukan on 16/1/15.
//  Copyright (c) 2016年 Ousyoukan. All rights reserved.
//

#ifndef PARTICALSYSTEM_H
#define PARTICALSYSTEM_H
#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include "particle.h"
#define PI 3.1415926
using namespace std;


class ParticalSystem
{
    public:
        ParticalSystem();
        ParticalSystem(int _count,float _gravity){ptlCount=_count;gravity=_gravity;};
        void init();
        void simulate(float dt,float posX,float posY);
        void aging(float dt,float posX,float posY);
        void applyGravity();
        void kinematics(float dt);
        void checkBump(float x1,float x2,float y1,float y2);
        void render();
        virtual ~ParticalSystem();
    
    
    protected:
    
    
    private:
        int ptlCount;
        float gravity;
        GLUquadricObj *myobject;
        vector<Particle> particles;
};

#endif // PARTICALSYSTEM_H
