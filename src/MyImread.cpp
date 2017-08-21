/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "MyImread.h"
#include "MyLog.h"

#include <sstream>

void ColorBmpReader::colorBmpRead(const String& path, const String& filename, Mat& pixelMat, pixel_type type)
{
    this->open((path + filename).c_str(), std::ios::in | std::ios::binary);

    if(this->good())
    {
        // read header
        char *p = (char *) &m_header;
        for(int i = 0;i<sizeof(m_header);++i)
        {
            this->read(p, 1);
            ++p;
        }
        p = NULL;

        if((32 != m_header.biBitCount) && (24 != m_header.biBitCount) && (16 != m_header.biBitCount))
        {
            LOG(ERR_LV, "This is not a colorful BMP picture!");
            this->close();
            return;
        }
        else
        {
            LOG(INFO_LV, "This is a colorful BMP picture!");
        }

        // judge the input file
        if(!((32 == m_header.biBitCount && (type == ARGB8888) || (type == RGBA8888))
           || (24 == m_header.biBitCount && type == RGB888)
             || (16 == m_header.biBitCount && type == RGB565)))
        {
            pixel_type inputFileType = (32 == m_header.biBitCount) ? ARGB8888 : ((24 == m_header.biBitCount) ? RGB888 : RGB565);
            LOG(ERR_LV, "the input file(", getPixelTypeName(inputFileType), ")is no same with input type(", getPixelTypeName(type), ").");
            this->close();
            return;
        }

        // allocate room
        int matType = 32 == m_header.biBitCount ? CV_8UC4 : CV_8UC3;
        pixelMat.create(m_header.biHeight, m_header.biWidth, matType);

        // read image data
        this->seekg(m_header.bfOffBits);
        switch(type)
        {
            case ARGB8888:
            case RGBA8888:
                argbRead(pixelMat);
                break;
            case RGB888:
                rgb888Read(pixelMat);
                break;
            case RGB565:
                rgb565Read(pixelMat);
                break;
            default:
                break;
        }
    }
    else
    {
        LOG(ERR_LV, "There are some errors, when read file: " + path + filename);
        this->close();
        return;
    }

    this->close();
    LOG(INFO_LV, "Read finished.");
}

void ColorBmpReader::rgb565Read(Mat &pixelMat)
{
    unsigned short data = 0;
    Vec3b pixel;
    int alignNum = FILL_BYTES_PER_LINE(pixelMat.cols, 2);
    int alignData = 0;
    for(int i = 0;i < pixelMat.rows;++i)
    {
        for(int j = 0;j < pixelMat.cols;++j)
        {
            this->read((char*) &data, sizeof(data));

            if(this->eof() || !this->good())
            {
                LOG(ERR_LV, "file read error!");
                exit(-1);
            }

            // decode
            pixel[2] = data >> 11;
            pixel[1] = (data >> 5) & 0x3F;
            pixel[0] = data & 0x1F;

            pixelMat.at<Vec3b>(i, j) = pixel;
            if((pixelMat.cols - 1) == j)
            {
                // 4 bytes align
                this->read((char *) &alignData, alignNum);
            }
        }
    }
}

void ColorBmpReader::rgb888Read(Mat &pixelMat)
{
    Vec3b pixel;
    int count = 0;
    int alignNum = FILL_BYTES_PER_LINE(pixelMat.cols, 3);
    int alignData = 0;
    for(int i = 0;i < pixelMat.rows;++i)
    {
        for(int j = 0;j < pixelMat.cols;++j)
        {
            this->read((char*) &pixel[0], 1);
            this->read((char*) &pixel[1], 1);
            this->read((char*) &pixel[2], 1);

            pixelMat.at<Vec3b>(i, j) = pixel;
            if(++count % pixelMat.cols == 0)
            {
                // 4 bytes align
                this->read((char *) &alignData, alignNum);
            }
        }
    }
}

void ColorBmpReader::argbRead(Mat &pixelMat)
{
    Vec4b pixel;
    for(int i = 0;i < pixelMat.rows;++i)
    {
        for(int j = 0;j < pixelMat.cols;++j)
        {
            this->read((char*) &pixel[0], 1);
            this->read((char*) &pixel[1], 1);
            this->read((char*) &pixel[2], 1);
            this->read((char*) &pixel[3], 1);

            pixelMat.at<Vec4b>(i, j) = pixel;
        }
    }
}