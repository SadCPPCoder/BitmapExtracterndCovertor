/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyImread.h
 * Author: zhangbob
 *
 * Created on 2017年6月19日, 上午11:13
 */

#ifndef MYIMREAD_H
#define MYIMREAD_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/cvstd.hpp>
#include <fstream>
#include <iostream>

#include "Type.h"

using namespace cv;

class ColorBmpReader : public std::ifstream
{
private:
    color_bmp_header m_header;
public:   
    void colorBmpRead( const String& path, const String& filename, Mat& pixelMat, pixel_type type);
protected:
    void rgb565Read(Mat &pixel);
    void rgb888Read(Mat &pixel);
    void argbRead(Mat &pixel);
};

#endif /* MYIMREAD_H */

