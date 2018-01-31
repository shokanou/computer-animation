//
//  particle.h
//  particle-sys
//
//  Created by Ousyoukan on 16/1/15.
//  Copyright (c) 2016年 Ousyoukan. All rights reserved.
//

//define parameters of particle

#ifndef PARTICLE_H
#define PARTICLE_H
#include "vector3d.h"
typedef struct
{
    float r;
    float g;
    float b;
    float alpha;
}Color;


typedef struct
{
    Vector3D position;
    Vector3D velocity;
    Vector3D acceleration;
    Color color;
    float age;
    float life;
    float size;
}Particle;

#endif // PARTICLE_H
