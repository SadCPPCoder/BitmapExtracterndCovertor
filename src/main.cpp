/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: zhangbob
 *
 * Created on 2017年4月13日, 下午1:44
 */

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <string>

#include "MyLog.h"
#include "MyImread.h"
#include "MyConvertor.h"
#include "MyImwriter.h"
#include "Type.h"

using namespace cv;

#define WIN_HEIGHT  (1050)
#define WIN_WIDTH   (1680)
#define _16U_MAX_VAL (65536)
#define _8U_MAX_COLOR (255*255*255)

std::string getPixelTypeName(pixel_type type)
{
    switch(type)
    {
        case ARGB8888:
        case RGBA8888:
            return "RGB32";
            break;
        case RGB888:
            return "RGB24";
            break;
        case RGB565:
            return "RGB16";
            break;
        default:
            return "Invalid Type";
            break;
    }
}

void showUsage()
{
    std::cout << "Usage:\n"
            << "\t bmptool CONVERT ImagePath InImageType OutImageType [REVERSE]\n"
            << "\t bmptool EXTRACT ImagePath InImageType [REVERSE]"
            << "\nImagePath: the path of image.\n"
            << "\tex. c:\\user\\image.bmp\n"
            << "InImageType: type of input image, ARGB8888/RGBA8888/RGB888/RGB565.\n"
            << "OutImageType: same with InImageType.\n"
            << "REVERSE: set output picture reversed.\n";
}

bool readPar(parameters &par, int argc, char **argv)
{
    int argIDX = 1;
    LOG(INFO_LV, "\n\n\tWelcome to use the BmpTool, version:1.0, author: Bob, Zhang, Date:20170622\n");
    if(argc < 2)
    {
        LOG(ERR_LV, "parameter num error.");
        showUsage();
        return false;
    }

    std::string cmd = argv[argIDX++];// mode
    if("CONVERT" == cmd)
    {
        par.Mode = CONVERT;
        if(argc < 5)
        {
            LOG(ERR_LV, "CONVERT mode should hvae 4 paramters!");
            showUsage();
            return false;
        }
    }
    else if("EXTRACT" == cmd)
    {
        par.Mode = EXTRACT;
        if(argc < 4)
        {
            LOG(ERR_LV, "EXTRACT mode should hvae 5 paramters!");
            showUsage();
            return false;
        }
    }
    else
    {
        LOG(ERR_LV, "error mode: ", cmd);
        showUsage();
        return false;
    }

    cmd = argv[argIDX++];// input file
    size_t posWindows = cmd.find_last_of('\\');
    size_t posLinux = cmd.find_last_of('/');
    if(std::string::npos != posWindows)
    {
        par.Path = cmd.substr(0, posWindows + 1);
        par.FileName = cmd.substr(posWindows + 1);
    }
    else if(std::string::npos != posLinux)
    {
        par.Path = cmd.substr(0, posLinux + 1);
        par.FileName = cmd.substr(posLinux + 1);
    }
    else
    {
        par.Path = "";
        par.FileName = cmd;
    }

    par.ConvertFileName = "Out_" + par.FileName;
    par.RgbFileName = par.ConvertFileName + ".rgb";

    // type 
    //    ARGB8888=0,
    //    RGBA8888=1,
    //    RGB888=2,
    //    RGB565=3,
    cmd = argv[argIDX++];// input file type
    if("ARGB8888" == cmd)
    {
        par.PixelType = ARGB8888;
    }
    else if("RGBA8888" == cmd)
    {
        par.PixelType = RGBA8888;
    }
    else if("RGB888" == cmd)
    {
        par.PixelType = RGB888;
    }
    else if("RGB565" == cmd)
    {
        par.PixelType = RGB565;
    }
    else
    {
        LOG(ERR_LV, "input file type error: ", cmd);
        showUsage();
        return false;
    }

    if(CONVERT == par.Mode)
    {
        // get output file type
        cmd = argv[argIDX++];
        if("ARGB8888" == cmd)
        {
            par.CovtType = ARGB8888;
        }
        else if("RGBA8888" == cmd)
        {
            par.CovtType = RGBA8888;
        }
        else if("RGB888" == cmd)
        {
            par.CovtType = RGB888;
        }
        else if("RGB565" == cmd)
        {
            par.CovtType = RGB565;
        }
        else
        {
            LOG(ERR_LV, "converted file type error: ", cmd);
            showUsage();
            return false;
        }

        //    RGB888_2_565 = 0,
        //    ARGB8888_2_565,
        //    RGBA8888_2_RGB565,
        //    RGB565_2_888,
        if(RGB888 == par.PixelType && RGB565 == par.CovtType)
        {
            par.CovtMode = RGB888_2_565;
        }
        else if(ARGB8888 == par.PixelType && RGB565 == par.CovtType)
        {
            par.CovtMode = ARGB8888_2_565;
        }
        else if(RGBA8888 == par.PixelType && RGB565 == par.CovtType)
        {
            par.CovtMode = RGBA8888_2_RGB565;
        }
        else if(RGB565 == par.PixelType && RGB888 == par.CovtType)
        {
            par.CovtMode = RGB565_2_888;
        }
        else if(ARGB8888 == par.PixelType && RGB888 == par.CovtType)
        {
            par.CovtMode = ARGB8888_2_RGB888;
        }
        else if(RGBA8888 == par.PixelType && RGB888 == par.CovtType)
        {
            par.CovtMode = RGBA8888_2_RGB888;
        }
        else
        {
            LOG(ERR_LV, "convert type(", getPixelTypeName(par.PixelType), ") to type(", getPixelTypeName(par.CovtType), ") not supported!");
        }
    }

    // reverse
    par.Reverse = false;
    if(argc > argIDX)
    {
        cmd = argv[argIDX];
        if("REVERSE" == cmd)
        {
            par.Reverse = true;
        }
    }

    return true;
}

/*
 * 显示一个正方形，从中心向四周扩散颜色
 */
int main(int argc, char** argv)
{
    parameters par;
    if(!readPar(par, argc, argv))
    {
        return -1;
    }

    Mat input;
    //read
    ColorBmpReader bmpReader;
    bmpReader.colorBmpRead(par.Path, par.FileName, input, par.PixelType);
    Log::instance().showMat("test.bmp Mat", input);

    if(EXTRACT == par.Mode)
    {
        PixelWriter output;
        if(par.Reverse)
        {
            Mat extract;
            BmpConvertor bmpConvert;
            bmpConvert.bmpConvert(input, extract, JUST_REVERSE, par.Reverse);
            output.ouputPixelData(extract, par.Path, par.RgbFileName, par.PixelType);
        }
        else
        {
            output.ouputPixelData(input, par.Path, par.RgbFileName, par.PixelType);
        }

    }
    else if(CONVERT == par.Mode)
    {
        Mat convert;
        BmpConvertor bmpConvert;
        bmpConvert.bmpConvert(input, convert, par.CovtMode, par.Reverse);
        Log::instance().showMat("test.bmp Mat", convert);
        PixelWriter output;
        output.ouputPixelData(convert, par.Path, par.RgbFileName, par.CovtType);
        output.outputBmp(convert, par.Path, par.ConvertFileName, par.CovtType);
    }

    waitKey(0);
    return 0;
}

