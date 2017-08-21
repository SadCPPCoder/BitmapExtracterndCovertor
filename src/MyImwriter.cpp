/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "MyImwriter.h"
#include "MyLog.h"

void PixelWriter::ouputPixelData(Mat &pixelMat, const String& path, const String& fileName, pixel_type type)
{
    if(!pixelMat.empty())
    {
        LOG(INFO_LV, path, fileName);
        std::ofstream outFile((path + fileName).c_str(), std::ios::out | std::ios::binary);
        if(!outFile.good())
        {
            LOG(ERR_LV, "open file error.");
            outFile.close();
            return;
        }
        else
        {
            LOG(INFO_LV, "start to write file.");
        }

        // convert
        switch(type)
        {
            case ARGB8888:
            case RGBA8888:
                outputARGB(pixelMat, outFile);
                break;
            case RGB888:
                outputRGB888(pixelMat, outFile);
                break;
            case RGB565:
                outputRGB565(pixelMat, outFile);
                break;
            default:
                break;
        }

        outFile.close();
    }
}

void PixelWriter::outputARGB(const Mat &pixelMat, std::ofstream &outFile)
{
    MatConstIterator_<Vec4b> pixel, colorend;
    for(pixel = pixelMat.begin<Vec4b>(), colorend = pixelMat.end<Vec4b>();pixel != colorend;++pixel)
    {
        outFile.write((char*) &(*pixel)[0], 1);
        outFile.write((char*) &(*pixel)[1], 1);
        outFile.write((char*) &(*pixel)[2], 1);
        outFile.write((char*) &(*pixel)[3], 1);
    }
}

void PixelWriter::outputRGB888(const Mat &pixelMat, std::ofstream &outFile)
{
    int count = 0;
    int alignNum = FILL_BYTES_PER_LINE(pixelMat.cols, 3);
    int alignData = 0;
    MatConstIterator_<Vec3b> pixel, colorend;
    for(pixel = pixelMat.begin<Vec3b>(), colorend = pixelMat.end<Vec3b>();pixel != colorend;++pixel)
    {
        outFile.write((char*) &(*pixel)[0], 1);
        outFile.write((char*) &(*pixel)[1], 1);
        outFile.write((char*) &(*pixel)[2], 1);
        if(++count % pixelMat.cols == 0)
        {
            // 4 bytes align
            outFile.write((char *) &alignData, alignNum);
        }
    }
}

void PixelWriter::outputRGB565(const Mat &pixelMat, std::ofstream &outFile)
{
    ushort pixel565 = 0;
    int count = 0;
    int alignNum = FILL_BYTES_PER_LINE(pixelMat.cols, 2);
    int alignData = 0;
    MatConstIterator_<Vec3b> pixel, colorend;
    for(pixel = pixelMat.begin<Vec3b>(), colorend = pixelMat.end<Vec3b>();pixel != colorend;++pixel)
    {
        // RGB
        pixel565 = (ushort) (((unsigned((*pixel)[2]) << 11) & 0xF800) |
                ((unsigned((*pixel)[1]) << 5) & 0x7E0) |
                             (unsigned((*pixel)[0])));
        outFile.write((char*) &pixel565, 2);
        if(++count % pixelMat.cols == 0)
        {
            // 4 bytes align
            outFile.write((char *) &alignData, alignNum);
        }
    }
}

void PixelWriter::initBmpHeader(color_bmp_header &header, pixel_type type, int width, int height)
{
    switch(type)
    {
        case ARGB8888:
        case RGBA8888:
            header.biBitCount = 32;
            break;
        case RGB888:
            header.biBitCount = 24;
            break;
        case RGB565:
            header.biBitCount = 16;
            header.biCompression = 3;
            break;
        default:
            break;
    }

    // mask length count
    int maks_len = (3 == header.biCompression) ? sizeof(bmp_color_mask) : 0;

    header.bfType = BFTYPE;
    header.biSize = BIHEADER_SIZE + maks_len;
    header.biWidth = width;
    header.biHeight = height;

    // count the align data size
    int alignNum = FILL_BYTES_PER_LINE(width, header.biBitCount >> 3);
    int totalAlignFillNum = alignNum * height;

    // the image should be rectangle
    header.biSizeImage = width * height * (header.biBitCount >> 3) + totalAlignFillNum;
    header.bfSize = BFHEADER_SIZE + header.biSize + header.biSizeImage;
    header.bfOffBits = BFHEADER_SIZE + header.biSize;
    header.bfReserved2 = 1;
    header.biPlanes = 1;
}

void PixelWriter::outputBmpHeader(color_bmp_header &header, std::ofstream &outFile)
{
    char *p = (char *) &header;
    for(int i = 0;i<sizeof(header);++i)
    {
        outFile.write(p, 1);
        ++p;
    }
}

void PixelWriter::outputBmpColorMask(bmp_color_mask &mask, std::ofstream &outFile)
{
    char *p = (char *) &mask;
    for(int i = 0;i<sizeof(mask);++i)
    {
        outFile.write(p, 1);
        ++p;
    }
}

void PixelWriter::outputBmp(Mat &pixelMat, const String& path, const String& fileName, pixel_type type)
{
    if(!pixelMat.empty())
    {
        std::ofstream outFile((path + fileName).c_str(), std::ios::out | std::ios::binary);
        if(!outFile.good())
        {
            LOG(ERR_LV, "open file error.");
            outFile.close();
            return;
        }
        else
        {
            LOG(INFO_LV, "start to write file.");
        }

        // output header
        color_bmp_header bmpHeader = {0};
        initBmpHeader(bmpHeader, type, pixelMat.cols, pixelMat.rows);
        outputBmpHeader(bmpHeader, outFile);
        if(3 == bmpHeader.biCompression) // should add the color mask
        {
            bmp_color_mask mask = {RGB565_R_MASK, RGB565_G_MASK, RGB565_B_MASK, 0};
            outputBmpColorMask(mask, outFile);
        }

        // init header
        switch(type)
        {
            case ARGB8888:
            case RGBA8888:
                outputARGB(pixelMat, outFile);
                break;
            case RGB888:
                outputRGB888(pixelMat, outFile);
                break;
            case RGB565:
                outputRGB565(pixelMat, outFile);
                break;
            default:
                break;
        }

        outFile.close();
    }
}