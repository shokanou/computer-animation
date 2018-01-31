//
//  morphing.h
//  morrhingtest
//
//  Created by Ousyoukan on 16/1/13.
//  Copyright (c) 2016年 Ousyoukan. All rights reserved.
//

#ifndef morrhingtest_morphing_h
#define morrhingtest_morphing_h
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <string>
#include <math.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define PI 3.14159265


using namespace std;

int counter=0;
int frame_count = 0;
IplImage *leftImage, *rightImage;
IplImage *leftImageTmp, *rightImageTmp;
int height = 0;
int width = 0;
CvScalar Color=CV_RGB(0,120,0);
int Thickness=2;
int Shift=0;
int key;

double parameter_a = 0.0;
double parameter_b = 0.0;
double parameter_p = 0.0;

//======================================


struct Line
{
    CvPoint2D32f P; //start
    CvPoint2D32f Q; //end
    CvPoint2D32f M; //mid
    double len;
    float degree;
    
    
    void PQtoMLD();
    void MLDtoPQ();
    void show();
    
    double Getu(CvPoint2D32f X);
    double Getv(CvPoint2D32f X);
    CvPoint2D32f Get_Point(double u , double v);
    double Get_Weight(CvPoint2D32f X);
};

//draw line
void Line::PQtoMLD()
{
    //CvPoint2D32f tmpP=cvPointTo32f(P);
    //CvPoint2D32f tmpQ=cvPointTo32f(Q);
    
    
    M.x=(P.x+Q.x)/2;
    M.y=(P.y+Q.y)/2;
    
    float tmpx = Q.x - P.x;
    float tmpy = Q.y - P.y;
    
    len=sqrt( tmpx*tmpx + tmpy*tmpy );
    degree=atan2(tmpy,tmpx);
    
    
    return;
}


void Line::MLDtoPQ()
{
    float tmpx=0.5*len*cos(degree);
    float tmpy=0.5*len*sin(degree);
    
    CvPoint2D32f tmpP;
    CvPoint2D32f tmpQ;
    tmpP.x = M.x - tmpx;
    tmpP.y = M.y - tmpy;
    tmpQ.x = M.x + tmpx;
    tmpQ.y = M.y + tmpy;
    
    P = tmpP ;
    Q = tmpQ ;
    
    
    //return;
}

//output the line parameter
void Line::show()
{
    printf("P(%lf,%lf) Q(%lf,%lf) M(%lf,%lf)\n \tlen=%lf degree=%f\n",P.x,P.y,Q.x,Q.y,M.x,M.y,len,degree);
    //return;
}


double Line::Getu(CvPoint2D32f X){
    /*calculate u*/
    double X_P_x = X.x - P.x;
    double X_P_y = X.y - P.y;
    double Q_P_x = Q.x - P.x;
    double Q_P_y = Q.y - P.y;
    double u = ((X_P_x * Q_P_x) + (X_P_y * Q_P_y)) / (len * len)  ;
    return u ;
}


double Line::Getv(CvPoint2D32f X){
    double X_P_x = X.x - P.x;
    double X_P_y = X.y - P.y;
    double Q_P_x = Q.x - P.x;
    double Q_P_y = Q.y - P.y;
    double Perp_Q_P_x = Q_P_y ;
    double Perp_Q_P_y = -Q_P_x ;
    double v = ((X_P_x * Perp_Q_P_x) + (X_P_y * Perp_Q_P_y))/len ;
    return v ;
}


CvPoint2D32f Line::Get_Point(double u , double v){
    double Q_P_x = Q.x - P.x;
    double Q_P_y = Q.y - P.y;
    double Perp_Q_P_x = Q_P_y ;
    double Perp_Q_P_y = -Q_P_x ;
    double Point_x = P.x + u * (Q.x - P.x) + ((v * Perp_Q_P_x)/len) ;
    double Point_y = P.y + u * (Q.y - P.y) + ((v * Perp_Q_P_y)/len) ;
    CvPoint2D32f X;
    X.x = Point_x;
    X.y = Point_y;
    return X ;
}


double Line::Get_Weight(CvPoint2D32f X ){
    double a = parameter_a;
    double b = parameter_b;
    double p = parameter_p;
    double d = 0.0;
    
    double u = Getu(X);
    if(u > 1.0 )
        d = sqrt((X.x - Q.x) * (X.x - Q.x) + (X.y - Q.y) * (X.y - Q.y));
    else if(u < 0)
        d = sqrt((X.x - P.x) * (X.x - P.x) + (X.y - P.y) * (X.y - P.y));
    else
        d = abs(Getv(X));
    
    
    double weight = pow( pow(len,p)/(a + d) , b);
    return weight;
}


//======================================
struct LinePair
{
    Line leftLine;
    Line rightLine;
    vector<struct Line> warpLine;//use vector
    
    void genWarpLine();
    void showWarpLine();
};


void LinePair::genWarpLine()
{
    while(leftLine.degree-rightLine.degree>PI)
        rightLine.degree=rightLine.degree+PI;
    while(rightLine.degree-leftLine.degree>PI)
        leftLine.degree=leftLine.degree+PI;
    for(int i=0;i<frame_count;i++)
    {
        double ratio=(double)(i+1)/(frame_count+1);
        Line curLine;
        
        curLine.M.x=(1-ratio)*leftLine.M.x + ratio*rightLine.M.x;
        curLine.M.y=(1-ratio)*leftLine.M.y + ratio*rightLine.M.y;
        curLine.len=(1-ratio)*leftLine.len + ratio*rightLine.len;
        curLine.degree=(1-ratio)*leftLine.degree + ratio*rightLine.degree;
        
        curLine.MLDtoPQ();
        warpLine.push_back(curLine);
    }
    return;
}


void LinePair::showWarpLine()
{
    int size=warpLine.size();
    for(int i=0;i<size;i++)
    {
        printf("warpLine[%d]:",i);
        warpLine[i].show();
        cvLine(leftImage,cvPointFrom32f(warpLine[i].P),cvPointFrom32f(warpLine[i].Q),Color,Thickness,CV_AA,Shift);
        cvLine(rightImage,cvPointFrom32f(warpLine[i].P),cvPointFrom32f(warpLine[i].Q),Color,Thickness,CV_AA,Shift);
    }
    leftImageTmp = cvCloneImage(leftImage);
    rightImageTmp = cvCloneImage(rightImage);
    cvShowImage("left",leftImage);
    cvShowImage("right",rightImage);
    return;
}


//======================================
vector<struct LinePair> pairs;
LinePair curLinePair;

//======================================

void show_pairs()
{
    int len=pairs.size();
    printf("pairs size=%d\n",len);
    for(int i=0;i<len;i++)
    {
        printf("leftLine:");
        pairs[i].leftLine.show();
        printf("rightLine:");
        pairs[i].rightLine.show();
        printf("\n");
    }
}

void on_mousel( int event, int x, int y, int flag, void* param )//LEFT PIC
{
    if (counter % 2 == 0 && counter > 0)
    {
        curLinePair.warpLine.clear();
        if(event==CV_EVENT_LBUTTONDOWN||event==CV_EVENT_RBUTTONDOWN){//
            printf("Left image( %d, %d) ",x,y);
            printf("The Event is : %d ",event);
            printf("The flags is : %d ",flag);
            printf("The param is : %d\n",param);
            curLinePair.leftLine.P.x = x;
            curLinePair.leftLine.P.y = y;
        }
        if(event==CV_EVENT_LBUTTONUP||event==CV_EVENT_RBUTTONUP){//
            printf("Left image( %d, %d) ",x,y);
            printf("The Event is : %d ",event);
            printf("The flags is : %d ",flag);
            printf("The param is : %d\n",param);
            curLinePair.leftLine.Q.x = x;
            curLinePair.leftLine.Q.y = y;
            curLinePair.leftLine.PQtoMLD();
            cvLine(leftImage,cvPointFrom32f(curLinePair.leftLine.P),cvPointFrom32f(curLinePair.leftLine.Q),Color,Thickness,CV_AA,Shift);
            leftImageTmp = cvCloneImage(leftImage);
            counter--;
        }
        if(flag==CV_EVENT_FLAG_LBUTTON||flag==CV_EVENT_FLAG_RBUTTON){//
            curLinePair.leftLine.Q.x = x;
            curLinePair.leftLine.Q.y = y;
            cvReleaseImage(&leftImage);//
            leftImage = cvCloneImage(leftImageTmp);
            cvLine(leftImage,cvPointFrom32f(curLinePair.leftLine.P),cvPointFrom32f(curLinePair.leftLine.Q),Color,Thickness,CV_AA,Shift);
            cvShowImage("left",leftImage);
        }
    }
    else
    {
        //cout << "you have selected the wrong image" << endl;//IT REQUIRE USER TO DRAW ONE AFTER ONE
    }
}

void on_mouser( int event, int x, int y, int flag, void* param )//RIGHT PIC
{
    if (counter % 2 == 1 && counter > 0)
    {
        if(event==CV_EVENT_LBUTTONDOWN||event==CV_EVENT_RBUTTONDOWN){//
            printf("right image( %d, %d) ",x,y);
            printf("The Event is : %d ",event);
            printf("The flags is : %d ",flag);
            printf("The param is : %d\n",param);
            curLinePair.rightLine.P.x = x;
            curLinePair.rightLine.P.y = y;
        }
        if(event==CV_EVENT_LBUTTONUP||event==CV_EVENT_RBUTTONUP){//
            printf("right image( %d, %d) ",x,y);
            printf("The Event is : %d ",event);
            printf("The flags is : %d ",flag);
            printf("The param is : %d\n",param);
            curLinePair.rightLine.Q.x = x;
            curLinePair.rightLine.Q.y = y;
            curLinePair.rightLine.PQtoMLD();
            cvLine(rightImage,cvPointFrom32f(curLinePair.rightLine.P),cvPointFrom32f(curLinePair.rightLine.Q),Color,Thickness,CV_AA,Shift);
            rightImageTmp = cvCloneImage(rightImage);
            counter--;
            curLinePair.genWarpLine();
            pairs.push_back(curLinePair);
            
            printf("\n");
            show_pairs();
            //curLinePair.showWarpLine();
        }
        if(flag==CV_EVENT_FLAG_LBUTTON||flag==CV_EVENT_FLAG_RBUTTON){//
            curLinePair.rightLine.Q.x = x;
            curLinePair.rightLine.Q.y = y;
            cvReleaseImage(&rightImage);//
            rightImage = cvCloneImage(rightImageTmp);
            cvLine(rightImage,cvPointFrom32f(curLinePair.rightLine.P),cvPointFrom32f(curLinePair.rightLine.Q),Color,Thickness,CV_AA,Shift);
            cvShowImage("right",rightImage);
        }
    }
    else
    {
        //cout << "you have selected the wrong image" << endl;
    }
}


#endif
