/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyConvertor.h
 * Author: zhangbob
 *
 * Created on 2017年6月20日, 上午9:15
 */

#ifndef MYCONVERTOR_H
#define MYCONVERTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/cvstd.hpp>
#include <fstream>
#include <iostream>
using namespace cv;

#include "Type.h"

class BmpConvertor
{
public:
    void color_2_rgb565(Vec3b &color, Vec3b &pixel565);
    void rgb565_2_color(Vec3b &pixel565, Vec3b &color);
    void bmpConvert(Mat &srcMat, Mat &dstMat, bmp_convert_mode mode, bool reverse);
protected:
    void bmp_convert_888_2_565(Mat &src, Mat &dst, bool reverse);
    void bmp_convert_8888_2_565(Mat &src, Mat &dst, bool alphaFirst, bool reverse);
    void bmp_convert_8888_2_888(Mat &src, Mat &dst, bool alphaFirst, bool reverse);
    void bmp_convert_565_2_888(Mat &src, Mat &dst, bool reverse);
    void bmp_convert_do_reverse(Mat &src, Mat &dst);
};

#endif /* MYCONVERTOR_H */

