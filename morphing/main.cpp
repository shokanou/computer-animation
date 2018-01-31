//
//  main.cpp
//  morrhingtest
//
//  Created by Ousyoukan on 16/1/13.
//  Copyright (c) 2016年 Ousyoukan. All rights reserved.
//

#include <iostream>

#include<string>
#include"morphing.h"

string first_image_name ;
string second_image_name;
string new_image_name;
string filepath = "/Users/Ousyoukan/Desktop/morphingtest/";

using namespace std;

struct Image{
    int frame_index;//FRAME OF PIC
    IplImage *image;
    IplImage *new_image;
    IplImage *test_image;
    
    Image(int i);
    void LoadImage(string image_name);
    CvScalar bilinear(IplImage *image , double X  , double Y );
    void Warp();
};


Image::Image(int i){
    frame_index=i;
    CvSize ImageSize = cvSize(width,height);
    new_image = cvCreateImage(ImageSize,IPL_DEPTH_8U,3);
    test_image = cvCreateImage(ImageSize,IPL_DEPTH_8U,3);
}


CvScalar Image::bilinear(IplImage *image , double X  , double Y ){
    int x_floor = (int)X ;
    int y_floor = (int)Y ;
    int x_ceil = x_floor + 1 ;
    int y_ceil = y_floor + 1 ;
    double a = X - x_floor ;
    double b = Y - y_floor ;
    if(x_ceil >= width-1)
        x_ceil = width-1 ;
    if(y_ceil >= height-1)
        y_ceil = height-1 ;
    CvScalar output_scalar ;
    CvScalar leftdown = cvGet2D(image,y_floor,x_floor);
    CvScalar lefttop = cvGet2D(image,y_ceil,x_floor);
    CvScalar rightdown = cvGet2D(image,y_floor,x_ceil);
    CvScalar righttop = cvGet2D(image,y_ceil,x_ceil);
    for(int i = 0 ; i < 4 ; i ++){
        output_scalar.val[i] = (1-a)*(1-b)*leftdown.val[i] + a*(1-b)*rightdown.val[i] + a*b*righttop.val[i] + (1-a)*b*lefttop.val[i];
    }
    return output_scalar ;
}


void Image::Warp(){
    double ratio=(double)(frame_index+1)/(frame_count+1);
    
    IplImage *ori_leftImage,*ori_rightImage;
    ori_leftImage=cvLoadImage(first_image_name.c_str());
    ori_rightImage=cvLoadImage(second_image_name.c_str());
    for(int x = 0 ; x < width ; x++){
        for(int y = 0 ; y < height ; y++){
            CvPoint2D32f dst_point ;
            dst_point.x = x ;
            dst_point.y = y;
            double leftXSum_x = 0.0;
            double leftXSum_y = 0.0;
            double leftWeightSum = 0.0;
            double rightXSum_x = 0.0;
            double rightXSum_y = 0.0;
            double rightWeightSum = 0.0;
            for(int i = 0 ; i < pairs.size() ; i++){

                Line src_line = pairs[i].leftLine;
                Line dst_line = pairs[i].warpLine[frame_index];
                
                double new_u = dst_line.Getu(dst_point);
                double new_v = dst_line.Getv(dst_point);
                
                CvPoint2D32f src_point = src_line.Get_Point(new_u , new_v);
                double src_weight = dst_line.Get_Weight(dst_point);
                leftXSum_x = leftXSum_x + (double)src_point.x * src_weight ;
                leftXSum_y = leftXSum_y + (double)src_point.y * src_weight ;
                leftWeightSum = leftWeightSum + src_weight ;
                
                src_line = pairs[i].rightLine;
                
                new_u = dst_line.Getu(dst_point);
                new_v = dst_line.Getv(dst_point);
                
                src_point = src_line.Get_Point(new_u , new_v);
                src_weight = dst_line.Get_Weight(dst_point);
                rightXSum_x = rightXSum_x + (double)src_point.x * src_weight ;
                rightXSum_y = rightXSum_y + (double)src_point.y * src_weight ;
                rightWeightSum = rightWeightSum + src_weight ;
            }
            double left_src_x = leftXSum_x / leftWeightSum;
            double left_src_y = leftXSum_y / leftWeightSum;
            double right_src_x = rightXSum_x / rightWeightSum;
            double right_src_y = rightXSum_y / rightWeightSum;
            
            
            if(left_src_x<0)
                left_src_x=0;
            if(left_src_y<0)
                left_src_y=0;
            if(left_src_x>=width)
                left_src_x=width-1;
            if(left_src_y>=height)
                left_src_y=height-1;
            if(right_src_x<0)
                right_src_x=0;
            if(right_src_y<0)
                right_src_y=0;
            if(right_src_x>=width)
                right_src_x=width-1;
            if(right_src_y>=height)
                right_src_y=height-1;
            
            
            //CvScalar left_scalar=cvGet2D(ori_leftImage,left_src_y,left_src_x);
            //CvScalar right_scalar=cvGet2D(ori_rightImage,right_src_y,right_src_x);
            CvScalar left_scalar = bilinear(ori_leftImage,left_src_x,left_src_y);
            CvScalar right_scalar=bilinear(ori_rightImage,right_src_x,right_src_y);
            CvScalar new_scalar;
            new_scalar.val[0]=(1-ratio)*left_scalar.val[0] + ratio*right_scalar.val[0];
            new_scalar.val[1]=(1-ratio)*left_scalar.val[1] + ratio*right_scalar.val[1];
            new_scalar.val[2]=(1-ratio)*left_scalar.val[2] + ratio*right_scalar.val[2];
            new_scalar.val[3]=(1-ratio)*left_scalar.val[3] + ratio*right_scalar.val[3];
            cvSet2D(new_image,y,x,new_scalar);
            cvSet2D(test_image,y,x,left_scalar);//test
            

        }
        
    }
    char win_name[16];
    char img_name[50];
    char test_name[16];
    sprintf(win_name,"frame[%d]",frame_index);
    sprintf(img_name,"%s_%d.jpg",new_image_name.c_str(),frame_index);
    
    //sprintf(test_name,"test[%d]",frame_index);
    cvShowImage(win_name,new_image);
    cvSaveImage(img_name,new_image);
    
    return ;
}
//======================================

void runWarp()
{
    for(int i=0;i<frame_count;i++)
    {
        Image curImage=Image(i);
        printf("warping %d...\n",i);
        curImage.Warp();
    }
}


int main(int argc, char** argv){
    
    cout << "Please input the first image name : "  ;
    cin >> first_image_name;
    cout << "Please input the second image name : "  ;
    cin >> second_image_name;
    cout << "Please input the frame number : ";
    cin >> frame_count;
    cout << "Please input the new image name : ";
    cin >> new_image_name;
    /*parameter*/
    cout << "Please input the parameter a , b , p( a = 1 , b = 2 , p = 0 is recommended)  " << endl;
    cout << " a = " ;
    cin >> parameter_a ;
    cout << " b = " ;
    cin >> parameter_b ;
    cout << " p = " ;
    cin >> parameter_p ;
    
    first_image_name = filepath+first_image_name;
    second_image_name = filepath+second_image_name;
    new_image_name = filepath+new_image_name;
    
    leftImage = cvLoadImage(first_image_name.c_str());
    rightImage = cvLoadImage(second_image_name.c_str());
    height = leftImage->height;
    width = leftImage->width;
    leftImageTmp = cvLoadImage(first_image_name.c_str());
    rightImageTmp = cvLoadImage(second_image_name.c_str());
    
    cvNamedWindow("left",1);
    cvMoveWindow("left",10,10);
    cvNamedWindow("right",1);
    cvMoveWindow("right",300,10);
    
    cvSetMouseCallback( "left", on_mousel, 0 );
    cvSetMouseCallback( "right", on_mouser, 0 );
    
    cvShowImage("left",leftImage);
    cvShowImage("right",rightImage);
    
    while(true)
    {
        key=cvWaitKey(0);
        if(key=='d')
            counter=counter+2;
        else if(key=='w')
            runWarp();
        else if(key=='q')
            break;
    }
    
    cvDestroyWindow("left");
    cvDestroyWindow("right");
    return EXIT_SUCCESS;
}