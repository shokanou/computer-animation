//
//  mass.h
//  particle-test
//
//  Created by Ousyoukan on 16/1/15.
//  Copyright (c) 2016年 Ousyoukan. All rights reserved.
//

#ifndef particle_test_mass_h
#define particle_test_mass_h
#include "vector3d.h"

class Mass
{
public:
    Mass();
    Mass(float _m,float _size){m=_m;size=_size;};
    void init();
    void simulate(float dt);
    void checkBnd(float x1,float x2,float y1,float y2);
    void setForce(Vector3D _force){force=_force;};
    void setVel(Vector3D _vel){vel=_vel;};
    virtual ~Mass();
    inline Vector3D getPos(){return pos;};
    inline Vector3D getVel(){return vel;};
    inline Vector3D getForce(){return force;};
    inline float getSize(){return size;};
    
    
protected:
private:
    float size;                         //大小
    float m;                            // 质量
    Vector3D pos;                       // 位置
    Vector3D vel;                       // 速度
    Vector3D force;                     // 力
};


#endif
