/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyImwriter.h
 * Author: zhangbob
 *
 * Created on 2017年6月20日, 上午9:42
 */

#ifndef MYIMWRITER_H
#define MYIMWRITER_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <iostream>
using namespace cv;

#include "Type.h"

class PixelWriter
{
public:
    void ouputPixelData(Mat &pixelMat, const String& path,  const String& fileName, pixel_type type);
    void outputBmp(Mat &pixelMat, const String& path,  const String& fileName, pixel_type type);
protected:
    void outputARGB(const Mat &pixelMat, std::ofstream &outFile);
    void outputRGB888(const Mat &pixelMat, std::ofstream &outFile);
    void outputRGB565(const Mat &pixelMat, std::ofstream &outFile);
    void initBmpHeader(color_bmp_header &header, pixel_type type, int width, int height);
    void outputBmpHeader(color_bmp_header &header, std::ofstream &outFile);
    void outputBmpColorMask(bmp_color_mask &mask, std::ofstream &outFile);
};

#endif /* MYIMWRITER_H */

