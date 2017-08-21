/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "MyConvertor.h"
#include "MyLog.h"

void BmpConvertor::color_2_rgb565(Vec3b &color, Vec3b &pixel565)
{
    pixel565[0] = color[0] >> 3;// R
    pixel565[1] = color[1] >> 2;// G
    pixel565[2] = color[2] >> 3;// B
}

void BmpConvertor::rgb565_2_color(Vec3b &pixel565, Vec3b &color)
{
    color[0] = pixel565[0] << 3;// R
    color[1] = pixel565[1] << 2;// G
    color[2] = pixel565[2] << 3;// B
}

void BmpConvertor::bmpConvert(Mat &srcMat, Mat &dstMat, bmp_convert_mode mode, bool reverse)
{
    if(!srcMat.empty())
    {
        // clear the output
        if(!dstMat.empty())
        {
            dstMat.release();
        }

        // log
        LOG(INFO_LV, "start to convert!");

        switch(mode)
        {
            case RGB888_2_565:
                bmp_convert_888_2_565(srcMat, dstMat, reverse);
                break;
            case ARGB8888_2_565:
                bmp_convert_8888_2_565(srcMat, dstMat, false, reverse);
                break;
            case RGBA8888_2_RGB565:
                bmp_convert_8888_2_565(srcMat, dstMat, true, reverse);
                break;
            case RGB565_2_888:
                bmp_convert_565_2_888(srcMat, dstMat, reverse);
                break;
            case ARGB8888_2_RGB888:
                bmp_convert_8888_2_888(srcMat, dstMat, false, reverse);
                break;
            case RGBA8888_2_RGB888:
                bmp_convert_8888_2_888(srcMat, dstMat, true, reverse);
                break;
            case JUST_REVERSE:
                bmp_convert_do_reverse(srcMat, dstMat);
                break;
            default:
                break;
        }
    }
}

void BmpConvertor::bmp_convert_888_2_565(Mat &src, Mat &dst, bool reverse)
{
    if(src.type() != CV_8UC3)
    {
        LOG(ERR_LV, "the src mat is not CV_8UC3!");
    }

    // make sure the dst mat's size and type are same with src mat
    dst.create(src.rows, src.cols, CV_8UC3);

    // convert
    for(int i=0; i<src.rows; ++i)
    {
        for(int j=0; j<src.cols; ++j)
        {
            color_2_rgb565(src.at<Vec3b>(i, j), dst.at<Vec3b>((reverse ? (src.rows - i - 1) : i), j));
        }
    }
}

void BmpConvertor::bmp_convert_8888_2_565(Mat &src, Mat &dst, bool alphaFirst, bool reverse)
{
    if(src.type() != CV_8UC4)
    {
        LOG(ERR_LV, "the src mat is not CV_8UC4!");
    }

    // make sure the dst mat's size and type are same with src mat
    dst.create(src.rows, src.cols, CV_8UC3);

    // convert;
    Vec3b colorPixel;
    int index = alphaFirst ? 0 : 1;
    for(int i = 0;i < src.rows;++i)
    {
        for(int j = 0;j < src.cols;++j)
        {
            // ARGB
            colorPixel[0] = src.at<Vec4b>(i, j)[index];
            colorPixel[1] = src.at<Vec4b>(i, j)[index + 1];
            colorPixel[2] = src.at<Vec4b>(i, j)[index + 2];
            color_2_rgb565(colorPixel, dst.at<Vec3b>((reverse ? (src.rows - i - 1) : i), j));
        }
    }
}

void BmpConvertor::bmp_convert_8888_2_888(Mat &src, Mat &dst, bool alphaFirst, bool reverse)
{
    if(src.type() != CV_8UC4)
    {
        LOG(ERR_LV, "the src mat is not CV_8UC4!");
    }

    // make sure the dst mat's size and type are same with src mat
    dst.create(src.rows, src.cols, CV_8UC3);

    // convert
    // alphaFirst shows whither the alpha is the first byte.
    int index = alphaFirst ? 0 : 1;
    for(int i = 0;i < src.rows;++i)
    {
        for(int j = 0;j < src.cols;++j)
        {
            // ARGB
            dst.at<Vec3b>((reverse ? (src.rows - i - 1) : i), j)[0] = src.at<Vec4b>(i, j)[index];
            dst.at<Vec3b>((reverse ? (src.rows - i - 1) : i), j)[1] = src.at<Vec4b>(i, j)[index + 1];
            dst.at<Vec3b>((reverse ? (src.rows - i - 1) : i), j)[2] = src.at<Vec4b>(i, j)[index + 2];
        }
    }
}

void BmpConvertor::bmp_convert_565_2_888(Mat &src, Mat &dst, bool reverse)
{
    if(src.type() != CV_8UC3)
    {
        LOG(ERR_LV, "the src mat is not CV_8UC3!");
    }

    // make sure the dst mat's size and type are same with src mat
    dst.create(src.rows, src.cols, CV_8UC3);

    // convert
    for(int i = 0;i < src.rows;++i)
    {
        for(int j = 0;j < src.cols;++j)
        {
            rgb565_2_color(src.at<Vec3b>(i, j), dst.at<Vec3b>((reverse ? (src.rows - i - 1) : i), j));
        }
    }
}

void BmpConvertor::bmp_convert_do_reverse(Mat &src, Mat &dst)
{
    // make sure the dst mat's size and type are same with src mat
    dst.create(src.rows, src.cols, src.type());

    // convert
    for(int i = 0;i < src.rows;++i)
    {
        for(int j = 0;j < src.cols;++j)
        {
            if(CV_8UC4 == src.type())
            {
                // ARGB
                Vec4b &colorPixel = dst.at<Vec4b>(src.rows - i - 1, j);
                colorPixel[0] = src.at<Vec4b>(i, j)[0];
                colorPixel[1] = src.at<Vec4b>(i, j)[1];
                colorPixel[2] = src.at<Vec4b>(i, j)[2];
                colorPixel[3] = src.at<Vec4b>(i, j)[3];
            }
            else if(CV_8UC3 == src.type())
            {
                Vec3b &colorPixel = dst.at<Vec3b>(src.rows - i - 1, j);
                colorPixel[0] = src.at<Vec3b>(i, j)[0];
                colorPixel[1] = src.at<Vec3b>(i, j)[1];
                colorPixel[2] = src.at<Vec3b>(i, j)[2];
            }
        }
    }
}