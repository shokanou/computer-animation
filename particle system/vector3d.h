//
//  vector3d.h
//  particle-sys
//
//  Created by Ousyoukan on 16/1/15.
//  Copyright (c) 2016年 Ousyoukan. All rights reserved.
//

#ifndef Vector3D_H
#define Vector3D_H
#include <iostream>
#include <cmath>
using namespace std;


class Vector3D
{

public:
    float x;
    float y;
    float z;

    Vector3D();
    ~Vector3D();

    
    Vector3D(float posX, float posY, float posZ);
    
    void getInfo();
    
    
    
    //矢量加法
    Vector3D operator+(Vector3D v);
    //矢量减法
    Vector3D operator-(Vector3D v);
    //数乘
    Vector3D operator*(float n);
    //数除
    Vector3D operator/(float n);
    
    
    //向量点积
    float dotMul(Vector3D v2) const;
    //向量叉乘
    Vector3D crossMul(Vector3D v2);
    //绝对值化
    Vector3D abs();
    //获取分量中的最大值
    float max();
    //获取分量的最小值
    float min();
    //获取矢量长度
    float getLength();
    //向量单位化
    Vector3D normalize();
    //求两点之间的距离
    float getDist(Vector3D v);
    //返回零向量
    static Vector3D zero();
    //返回反向的向量
    Vector3D negate();
    //打印向量的分量值
    void show();

};

#endif // Vector3D_H
