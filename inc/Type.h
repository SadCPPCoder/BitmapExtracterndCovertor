/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Type.h
 * Author: zhangbob
 *
 * Created on 2017年6月21日, 下午4:10
 */

#ifndef TYPE_H
#define TYPE_H

#include <string>

#define BFTYPE (0x4D42)
#define BFHEADER_SIZE (14)
#define BIHEADER_SIZE (40)
#define BMP_ALIGN_BYTES (4)

#define FILL_BYTES_PER_LINE(width, bytePerPixel)  ((BMP_ALIGN_BYTES - (((width) * (bytePerPixel)) % BMP_ALIGN_BYTES))%BMP_ALIGN_BYTES)

typedef enum
{
    ARGB8888=0,
    RGBA8888,
    RGB888,
    RGB565,
}pixel_type;

std::string getPixelTypeName(pixel_type type);

#pragma pack (1)
typedef struct
{
    short bfType;   // "BM"
    int bfSize;     // file size
    short bfReserved1;// 0
    short bfReserved2;// 1
    int bfOffBits;  // Start address
    int biSize;     // size of bitmap info header always 0x28=40 bytes
    int biWidth;    // width of bmp
    int biHeight;   // height
    short biPlanes; // 0
    short biBitCount;// color depth bits, bits per pixel
    int biCompression;// 0-no compress; 1-RLE8; 2-RLE4; 3-masked
    int biSizeImage; // image data size
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    int biClrUsed;  // 0-all used, for 256 colors, 0x100
    int biClrImportant;
}color_bmp_header;

#define RGB565_R_MASK (0xF800)
#define RGB565_G_MASK (0x07E0)
#define RGB565_B_MASK (0x001F)

typedef struct
{
    int RedMask;
    int GreenMask;
    int BlueMask;
    int AlphaMask;
}bmp_color_mask;
#pragma pack ()

typedef enum
{
    RGB888_2_565 = 0,
    ARGB8888_2_565,
    RGBA8888_2_RGB565,
    RGB565_2_888,
    ARGB8888_2_RGB888,
    RGBA8888_2_RGB888,
    JUST_REVERSE,
} bmp_convert_mode;

typedef enum
{
    EXTRACT=0,
    CONVERT,
}mode;

typedef struct 
{
    std::string FileName;
    std::string RgbFileName;
    std::string ConvertFileName;
    std::string Path;
    pixel_type PixelType;
    pixel_type CovtType;
    bmp_convert_mode CovtMode;
    mode Mode;
    bool Reverse;
}parameters;

#endif /* TYPE_H */

