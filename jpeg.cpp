/*
 *  jpeg.cpp
 *  FSRash
 *
 *  Created by Gustavo Campos on 1/4/10.
 *  Copyright 2010 `. All rights reserved.
 *
 */

#include "Exception.hpp"
#include "jpeg.hpp"

/*
 #ifdef __cplusplus
 //extern "C" {
 #endif
 */

/*
 #ifdef __cplusplus
 //}
 #endif
 */

jpegException::jpegException (std::string strMessage, uint nErrorID): Exception ("jpg", strMessage, nErrorID)
{}

jpeg::jpeg (): pGraphic(*(new Graphic(1,1))) { /* private */}
/*
 RGB to YCbCr Conversion:
 */
// Y = 0.299*R + 0.587*G + 0.114*B
constexpr int16_t RGB2Y(const color r, const color g, const color b)
{
    return (color) ((153*r + 301*g + 58*b)>>9);
}
// Cb = -0.1687*R - 0.3313*G + 0.5*B + 128
constexpr int16_t RGB2Cb(const color r, const color g, const color b)
{
    return (color) ((65536 - 86*r - 170*g + 256*b)>>9);
}
// Cr = 0.5*R - 0.4187*G - 0.0813*B + 128
constexpr int16_t RGB2Cr(const color r, const color g, const color b)
{
    return (color) ((65536 + 256*r - 214*g - 42*b)>>9);
}



/* tables from JPEG standard
 const uint8_t qtable_std_lum[8][8] =
 {
 {16, 11, 10, 16, 24, 40, 51, 61},
 {12, 12, 14, 19, 26, 58, 60, 55},
 {14, 13, 16, 24, 40, 57, 69, 56},
 {14, 17, 22, 29, 51, 87, 80, 62},
 {18, 22, 37, 56, 68,109,103, 77},
 {24, 35, 55, 64, 81,104,113, 92},
 {49, 64, 78, 87,103,121,120,101},
 {72, 92, 95, 98,112,100,103, 99}
 };
 
 const uint8_t qtable_std_chrom[8][8] =
 {
 {17, 18, 24, 47, 99, 99, 99, 99},
 {18, 21, 26, 66, 99, 99, 99, 99},
 {24, 26, 56, 99, 99, 99, 99, 99},
 {47, 66, 99, 99, 99, 99, 99, 99},
 {99, 99, 99, 99, 99, 99, 99, 99},
 {99, 99, 99, 99, 99, 99, 99, 99},
 {99, 99, 99, 99, 99, 99, 99, 99},
 {99, 99, 99, 99, 99, 99, 99, 99}
 };
 
 // Windows Paint tables
 const uint8_t qtable_paint_lum[8][8] =
 {
 { 8,  6,  5,  8, 12, 20, 26, 31},
 { 6,  6,  7, 10, 13, 29, 30, 28},
 { 7,  7,  8, 12, 20, 29, 35, 28},
 { 7,  9, 11, 15, 26, 44, 40, 31},
 { 9, 11, 19, 28, 34, 55, 52, 39},
 {12, 18, 28, 32, 41, 52, 57, 46},
 {25, 32, 39, 44, 52, 61, 60, 51},
 {36, 46, 48, 49, 56, 50, 52, 50}
 };
 
 const uint8_t qtable_paint_chrom[8][8] =
 {
 { 9,  9, 12, 24, 50, 50, 50, 50},
 { 9, 11, 13, 33, 50, 50, 50, 50},
 {12, 13, 28, 50, 50, 50, 50, 50},
 {24, 33, 50, 50, 50, 50, 50, 50},
 {50, 50, 50, 50, 50, 50, 50, 50},
 {50, 50, 50, 50, 50, 50, 50, 50},
 {50, 50, 50, 50, 50, 50, 50, 50},
 {50, 50, 50, 50, 50, 50, 50, 50}
 };
 */


#define QTAB_DIV    512

// as you can see I use Paint tables
static const uint8_t qtable_0_lum[8][8] =
{
    { 8,  6,  5,  8, 12, 20, 26, 31},
    { 6,  6,  7, 10, 13, 29, 30, 28},
    { 7,  7,  8, 12, 20, 29, 35, 28},
    { 7,  9, 11, 15, 26, 44, 40, 31},
    { 9, 11, 19, 28, 34, 55, 52, 39},
    {12, 18, 28, 32, 41, 52, 57, 46},
    {25, 32, 39, 44, 52, 61, 60, 51},
    {36, 46, 48, 49, 56, 50, 52, 50}
};

static const uint8_t qtable_0_chrom[8][8] =
{
    { 9,  9, 12, 24, 50, 50, 50, 50},
    { 9, 11, 13, 33, 50, 50, 50, 50},
    {12, 13, 28, 50, 50, 50, 50, 50},
    {24, 33, 50, 50, 50, 50, 50, 50},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {50, 50, 50, 50, 50, 50, 50, 50}
};

// 1024/qtable_0_lum[][]
static const uint8_t qtable_lum[8][8] =
{
    {128,171,205,128, 85, 51, 39, 33},
    {171,171,146,102, 79, 35, 34, 37},
    {146,146,128, 85, 51, 35, 29, 37},
    {146,114, 93, 68, 39, 23, 26, 33},
    {114, 93, 54, 37, 30, 19, 20, 26},
    { 85, 57, 37, 32, 25, 20, 18, 22},
    { 41, 32, 26, 23, 20, 17, 17, 20},
    { 28, 22, 21, 21, 18, 20, 20, 20}
};

// 1024/qtable_0_chrom[][]
static const uint8_t qtable_chrom[8][8] =
{
    {114,114, 85, 43, 20, 20, 20, 20},
    {114, 93, 79, 31, 20, 20, 20, 20},
    { 85, 79, 37, 20, 20, 20, 20, 20},
    { 43, 31, 20, 20, 20, 20, 20, 20},
    { 20, 20, 20, 20, 20, 20, 20, 20},
    { 20, 20, 20, 20, 20, 20, 20, 20},
    { 20, 20, 20, 20, 20, 20, 20, 20},
    { 20, 20, 20, 20, 20, 20, 20, 20}
};

// zig-zag table
static const uint8_t zig[64] =
{
    0,
    1, 8,
    16, 9, 2,
    3,10,17,24,
    32,25,18,11, 4,
    5,12,19,26,33,40,
    48,41,34,27,20,13, 6,
    7,14,21,28,35,42,49,56,
    57,50,43,36,29,22,15,
    23,30,37,44,51,58,
    59,52,45,38,31,
    39,46,53,60,
    61,54,47,
    55,62,
    63
};

static const uint8_t std_dc_luminance_nrcodes[17] =
{
    0,0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0
};
static const uint8_t std_dc_luminance_values[12] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};

static const uint8_t std_dc_chrominance_nrcodes[17] =
{
    0,0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0
};
static const uint8_t std_dc_chrominance_values[12] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};

static const uint8_t std_ac_luminance_nrcodes[17] =
{
    0,0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,0x7d
};

static const uint8_t std_ac_luminance_values[162] =
{
    0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
    0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
    0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
    0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
    0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
    0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
    0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
    0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
    0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
    0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
    0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
    0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
    0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
    0xf9, 0xfa
};

static const uint8_t std_ac_chrominance_nrcodes[17] =
{
    0,0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,0x77
};

static const uint8_t std_ac_chrominance_values[162] =
{
    0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
    0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
    0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
    0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
    0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
    0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
    0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
    0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
    0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
    0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
    0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
    0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
    0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
    0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
    0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
    0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
    0xf9, 0xfa
};

static const uint8_t HYDClen[12] =
{
    0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x05,
    0x06, 0x07, 0x08, 0x09
};

static const uint8_t HCDClen[12] =
{
    0x02, 0x02, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b
};

static const uint16_t HYDCbits[12] =
{
    0x0000, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x000e, 0x001e,
    0x003e, 0x007e, 0x00fe, 0x01fe
};

static const uint16_t HCDCbits[12] =
{
    0x0000, 0x0001, 0x0002, 0x0006, 0x000e, 0x001e, 0x003e, 0x007e,
    0x00fe, 0x01fe, 0x03fe, 0x07fe
};


static const uint8_t HYAClen[16][12] =
{
    {0x04, 0x02, 0x02, 0x03, 0x04, 0x05, 0x07, 0x08, 0x0a, 0x10, 0x10, 0x00},    // 00 - 0f
    {0x00, 0x04, 0x05, 0x07, 0x09, 0x0b, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 10 - 1f
    {0x00, 0x05, 0x08, 0x0a, 0x0c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 20 - 2f
    {0x00, 0x06, 0x09, 0x0c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 30 - 3f
    {0x00, 0x06, 0x0a, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 40 - 4f
    {0x00, 0x07, 0x0b, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 50 - 5f
    {0x00, 0x07, 0x0c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 60 - 6f
    {0x00, 0x08, 0x0c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 70 - 7f
    {0x00, 0x09, 0x0f, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 80 - 8f
    {0x00, 0x09, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 90 - 9f
    {0x00, 0x09, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // a0 - af
    {0x00, 0x0a, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // b0 - bf
    {0x00, 0x0a, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // c0 - cf
    {0x00, 0x0b, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // d0 - df
    {0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // e0 - ef
    {0x0b, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00}
};

static const uint16_t HYACbits[16][12] =
{
    {0xFFFA, 0xFFF0, 0xFFF1, 0xFFF4, 0xFFFB, 0xFFFA, 0xFFF8, 0xFFF8, 0xFFF6, 0xFF82, 0xFF83, 0x0000},    // 00 - 0f
    {0x0000, 0xFFFC, 0xFFFB, 0xFFF9, 0xFFF6, 0xFFF6, 0xFF84, 0xFF85, 0xFF86, 0xFF87, 0xFF88, 0x0000},    // 10 - 1f
    {0x0000, 0xFFFC, 0xFFF9, 0xFFF7, 0xFFF4, 0xFF89, 0xFF8A, 0xFF8B, 0xFF8C, 0xFF8D, 0xFF8E, 0x0000},    // 20 - 2f
    {0x0000, 0xFFFA, 0xFFF7, 0xFFF5, 0xFF8F, 0xFF90, 0xFF91, 0xFF92, 0xFF93, 0xFF94, 0xFF95, 0x0000},    // 30 - 3f
    {0x0000, 0xFFFB, 0xFFF8, 0xFF96, 0xFF97, 0xFF98, 0xFF99, 0xFF9A, 0xFF9B, 0xFF9C, 0xFF9D, 0x0000},    // 40 - 4f
    {0x0000, 0xFFFA, 0xFFF7, 0xFF9E, 0xFF9F, 0xFFA0, 0xFFA1, 0xFFA2, 0xFFA3, 0xFFA4, 0xFFA5, 0x0000},    // 50 - 5f
    {0x0000, 0xFFFB, 0xFFF6, 0xFFA6, 0xFFA7, 0xFFA8, 0xFFA9, 0xFFAA, 0xFFAB, 0xFFAC, 0xFFAD, 0x0000},    // 60 - 6f
    {0x0000, 0xFFFA, 0xFFF7, 0xFFAE, 0xFFAF, 0xFFB0, 0xFFB1, 0xFFB2, 0xFFB3, 0xFFB4, 0xFFB5, 0x0000},    // 70 - 7f
    {0x0000, 0xFFF8, 0xFFC0, 0xFFB6, 0xFFB7, 0xFFB8, 0xFFB9, 0xFFBA, 0xFFBB, 0xFFBC, 0xFFBD, 0x0000},    // 80 - 8f
    {0x0000, 0xFFF9, 0xFFBE, 0xFFBF, 0xFFC0, 0xFFC1, 0xFFC2, 0xFFC3, 0xFFC4, 0xFFC5, 0xFFC6, 0x0000},    // 90 - 9f
    {0x0000, 0xFFFA, 0xFFC7, 0xFFC8, 0xFFC9, 0xFFCA, 0xFFCB, 0xFFCC, 0xFFCD, 0xFFCE, 0xFFCF, 0x0000},    // a0 - af
    {0x0000, 0xFFF9, 0xFFD0, 0xFFD1, 0xFFD2, 0xFFD3, 0xFFD4, 0xFFD5, 0xFFD6, 0xFFD7, 0xFFD8, 0x0000},    // b0 - bf
    {0x0000, 0xFFFA, 0xFFD9, 0xFFDA, 0xFFDB, 0xFFDC, 0xFFDD, 0xFFDE, 0xFFDF, 0xFFE0, 0xFFE1, 0x0000},    // c0 - cf
    {0x0000, 0xFFF8, 0xFFE2, 0xFFE3, 0xFFE4, 0xFFE5, 0xFFE6, 0xFFE7, 0xFFE8, 0xFFE9, 0xFFEA, 0x0000},    // d0 - df
    {0x0000, 0xFFEB, 0xFFEC, 0xFFED, 0xFFEE, 0xFFEF, 0xFFF0, 0xFFF1, 0xFFF2, 0xFFF3, 0xFFF4, 0x0000},    // e0 - ef
    {0xFFF9, 0xFFF5, 0xFFF6, 0xFFF7, 0xFFF8, 0xFFF9, 0xFFFA, 0xFFFB, 0xFFFC, 0xFFFD, 0xFFFE, 0x0000}
};

static const uint8_t HCAClen[16][12] =
{
    {0x02, 0x02, 0x03, 0x04, 0x05, 0x05, 0x06, 0x07, 0x09, 0x0a, 0x0c, 0x00},    // 00 - 0f
    {0x00, 0x04, 0x06, 0x08, 0x09, 0x0b, 0x0c, 0x10, 0x10, 0x10, 0x10, 0x00},    // 10 - 1f
    {0x00, 0x05, 0x08, 0x0a, 0x0c, 0x0f, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 20 - 2f
    {0x00, 0x05, 0x08, 0x0a, 0x0c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 30 - 3f
    {0x00, 0x06, 0x09, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 40 - 4f
    {0x00, 0x06, 0x0a, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 50 - 5f
    {0x00, 0x07, 0x0b, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 60 - 6f
    {0x00, 0x07, 0x0b, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 70 - 7f
    {0x00, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 80 - 8f
    {0x00, 0x09, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // 90 - 9f
    {0x00, 0x09, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // a0 - af
    {0x00, 0x09, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // b0 - bf
    {0x00, 0x09, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // c0 - cf
    {0x00, 0x0b, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // d0 - df
    {0x00, 0x0e, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},    // e0 - ef
    {0x0a, 0x0f, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00}
};

static const uint16_t HCACbits[16][12] =
{
    {0x0000, 0x0001, 0x0004, 0x000a, 0x0018, 0x0019, 0x0038, 0x0078, 0x01f4, 0x03f6, 0x0ff4, 0x0000},    // 00 - 0f
    {0x0000, 0x000b, 0x0039, 0x00f6, 0x01f5, 0x07f6, 0x0ff5, 0xff88, 0xff89, 0xff8a, 0xff8b, 0x0000},    // 10 - 1f
    {0x0000, 0x001a, 0x00f7, 0x03f7, 0x0ff6, 0x7fc2, 0xff8c, 0xff8d, 0xff8e, 0xff8f, 0xff90, 0x0000},    // 20 - 2f
    {0x0000, 0x001b, 0x00f8, 0x03f8, 0x0ff7, 0xff91, 0xff92, 0xff93, 0xff94, 0xff95, 0xff96, 0x0000},    // 30 - 3f
    {0x0000, 0x003a, 0x01f6, 0xff97, 0xff98, 0xff99, 0xff9a, 0xff9b, 0xff9c, 0xff9d, 0xff9e, 0x0000},    // 40 - 4f
    {0x0000, 0x003b, 0x03f9, 0xff9f, 0xffa0, 0xffa1, 0xFFA2, 0xFFA3, 0xFFA4, 0xFFA5, 0xFFA6, 0x0000},    // 50 - 5f
    {0x0000, 0x0079, 0x07f7, 0xffa7, 0xffa8, 0xffa9, 0xffaa, 0xffab, 0xFFAc, 0xFFAf, 0xFFAe, 0x0000},    // 60 - 6f
    {0x0000, 0x007a, 0x07f8, 0xffaf, 0xffb0, 0xFFB1, 0xFFB2, 0xFFB3, 0xFFB4, 0xFFB5, 0xFFB6, 0x0000},    // 70 - 7f
    {0x0000, 0x00f9, 0xffb7, 0xFFB8, 0xFFB9, 0xFFBa, 0xFFBb, 0xFFBc, 0xFFBd, 0xFFBe, 0xFFBf, 0x0000},    // 80 - 8f
    {0x0000, 0x01f7, 0xffc0, 0xffc1, 0xFFC2, 0xFFC3, 0xFFC4, 0xFFC5, 0xFFC6, 0xFFC7, 0xFFC8, 0x0000},    // 90 - 9f
    {0x0000, 0x01f8, 0xffc9, 0xFFCa, 0xFFCb, 0xFFCc, 0xFFCd, 0xFFCe, 0xFFCf, 0xFFd0, 0xFFd1, 0x0000},    // a0 - af
    {0x0000, 0x01f9, 0xFFD2, 0xFFD3, 0xFFD4, 0xFFD5, 0xFFD6, 0xFFD7, 0xFFD8, 0xFFD9, 0xFFDa, 0x0000},    // b0 - bf
    {0x0000, 0x01fa, 0xFFDb, 0xFFDc, 0xFFDd, 0xFFDe, 0xFFDf, 0xFFe0, 0xFFe1, 0xFFe2, 0xFFe3, 0x0000},    // c0 - cf
    {0x0000, 0x07f9, 0xFFE4, 0xFFE5, 0xFFE6, 0xFFE7, 0xFFE8, 0xFFE9, 0xFFEa, 0xFFEb, 0xFFEc, 0x0000},    // d0 - df
    {0x0000, 0x3fe0, 0xffed, 0xFFEe, 0xFFEf, 0xFFf0, 0xFFF1, 0xFFF2, 0xFFF3, 0xFFF4, 0xFFF5, 0x0000},    // e0 - ef
    {0x03fa, 0x7fc3, 0xFFF6, 0xFFF7, 0xFFF8, 0xFFF9, 0xFFFA, 0xFFFB, 0xFFFC, 0xFFFD, 0xFFFE, 0x0000}
};

huffman_t huffman_ctx[3] =
{
    {HYAClen, HYACbits, HYDClen, HYDCbits, (const uint8_t *) qtable_lum,   0}, // Y
    {HCAClen, HCACbits, HCDClen, HCDCbits, (const uint8_t *) qtable_chrom, 0}, // Cb
    {HCAClen, HCACbits, HCDClen, HCDCbits, (const uint8_t *) qtable_chrom, 0}, // Cr
};




/******************************************************************************
 **  dct
 **  --------------------------------------------------------------------------
 **  Fast DCT - Discrete Cosine Transform.
 **  This function converts 8x8 pixel block into frequencies.
 **  Lowest frequencies are at the upper-left corner.
 **  The input and output could point at the same array, in this case the data
 **  will be overwritten.
 **
 **  ARGUMENTS:
 **      pixels  - 8x8 pixel array;
 **      data    - 8x8 freq block;
 **
 **  RETURN: -
 ******************************************************************************/
inline void dct(int16_t pixels[8][8], int16_t data[8][8])
{
    int16_t rows[8][8];
    unsigned i;
    
    static const int
    c1 = 1004,  /* cos(pi/16) << 10 */
    s1 = 200,   /* sin(pi/16) */
    c3 = 851,   /* cos(3pi/16) << 10 */
    s3 = 569,   /* sin(3pi/16) << 10 */
    r2c6 = 554, /* sqrt(2)*cos(6pi/16) << 10 */
    r2s6 = 1337,/* sqrt(2)*sin(6pi/16) << 10 */
    r2 = 181;   /* sqrt(2) << 7*/
    
    /* transform rows */
    for (i = 0; i < 8; i++)
    {
        int x0,x1,x2,x3,x4,x5,x6,x7,x8;
        
        x0 = pixels[i][0];
        x1 = pixels[i][1];
        x2 = pixels[i][2];
        x3 = pixels[i][3];
        x4 = pixels[i][4];
        x5 = pixels[i][5];
        x6 = pixels[i][6];
        x7 = pixels[i][7];
        
        /* Stage 1 */
        x8=x7+x0;
        x0-=x7;
        x7=x1+x6;
        x1-=x6;
        x6=x2+x5;
        x2-=x5;
        x5=x3+x4;
        x3-=x4;
        
        /* Stage 2 */
        x4=x8+x5;
        x8-=x5;
        x5=x7+x6;
        x7-=x6;
        x6=c1*(x1+x2);
        x2=(-s1-c1)*x2+x6;
        x1=(s1-c1)*x1+x6;
        x6=c3*(x0+x3);
        x3=(-s3-c3)*x3+x6;
        x0=(s3-c3)*x0+x6;
        
        /* Stage 3 */
        x6=x4+x5;
        x4-=x5;
        x5=r2c6*(x7+x8);
        x7=(-r2s6-r2c6)*x7+x5;
        x8=(r2s6-r2c6)*x8+x5;
        x5=x0+x2;
        x0-=x2;
        x2=x3+x1;
        x3-=x1;
        
        /* Stage 4 and output */
        rows[i][0]=x6;
        rows[i][4]=x4;
        rows[i][2]=x8>>10;
        rows[i][6]=x7>>10;
        rows[i][7]=(x2-x5)>>10;
        rows[i][1]=(x2+x5)>>10;
        rows[i][3]=(x3*r2)>>17;
        rows[i][5]=(x0*r2)>>17;
    }
    
    /* transform columns */
    for (i = 0; i < 8; i++)
    {
        int x0,x1,x2,x3,x4,x5,x6,x7,x8;
        
        x0 = rows[0][i];
        x1 = rows[1][i];
        x2 = rows[2][i];
        x3 = rows[3][i];
        x4 = rows[4][i];
        x5 = rows[5][i];
        x6 = rows[6][i];
        x7 = rows[7][i];
        
        /* Stage 1 */
        x8=x7+x0;
        x0-=x7;
        x7=x1+x6;
        x1-=x6;
        x6=x2+x5;
        x2-=x5;
        x5=x3+x4;
        x3-=x4;
        
        /* Stage 2 */
        x4=x8+x5;
        x8-=x5;
        x5=x7+x6;
        x7-=x6;
        x6=c1*(x1+x2);
        x2=(-s1-c1)*x2+x6;
        x1=(s1-c1)*x1+x6;
        x6=c3*(x0+x3);
        x3=(-s3-c3)*x3+x6;
        x0=(s3-c3)*x0+x6;
        
        /* Stage 3 */
        x6=x4+x5;
        x4-=x5;
        x5=r2c6*(x7+x8);
        x7=(-r2s6-r2c6)*x7+x5;
        x8=(r2s6-r2c6)*x8+x5;
        x5=x0+x2;
        x0-=x2;
        x2=x3+x1;
        x3-=x1;
        
        /* Stage 4 and output */
        data[0][i]=((x6+16)>>3);
        data[4][i]=((x4+16)>>3);
        data[2][i]=((x8+16384)>>13);
        data[6][i]=((x7+16384)>>13);
        data[7][i]=((x2-x5+16384)>>13);
        data[1][i]=((x2+x5+16384)>>13);
        data[3][i]=(((x3>>8)*r2+8192)>>12);
        data[5][i]=(((x0>>8)*r2+8192)>>12);
    }
}

// simple but fast DCT
inline void dct3(int16_t pixels[8][8], int16_t data[8][8])
{
    /*CACHE_ALIGN*/ int16_t rows[8][8];
    unsigned i;
    
    static const int16_t // Ci = cos(i*PI/16)*(1 << 14);
    C1 = 16070,
    C2 = 15137,
    C3 = 13623,
    C4 = 11586,
    C5 = 9103,
    C6 = 6270,
    C7 = 3197;
    
    /* transform rows */
    for (i = 0; i < 8; i++)
    {
        int16_t s07,s16,s25,s34,s0734,s1625;
        int16_t d07,d16,d25,d34,d0734,d1625;
        
        s07 = pixels[i][0] + pixels[i][7];
        d07 = pixels[i][0] - pixels[i][7];
        s16 = pixels[i][1] + pixels[i][6];
        d16 = pixels[i][1] - pixels[i][6];
        s25 = pixels[i][2] + pixels[i][5];
        d25 = pixels[i][2] - pixels[i][5];
        s34 = pixels[i][3] + pixels[i][4];
        d34 = pixels[i][3] - pixels[i][4];
        
        rows[i][1] = (C1*d07 + C3*d16 + C5*d25 + C7*d34) >> 14;
        rows[i][3] = (C3*d07 - C7*d16 - C1*d25 - C5*d34) >> 14;
        rows[i][5] = (C5*d07 - C1*d16 + C7*d25 + C3*d34) >> 14;
        rows[i][7] = (C7*d07 - C5*d16 + C3*d25 - C1*d34) >> 14;
        
        s0734 = s07 + s34;
        d0734 = s07 - s34;
        s1625 = s16 + s25;
        d1625 = s16 - s25;
        
        rows[i][0] = (C4*(s0734 + s1625)) >> 14;
        rows[i][4] = (C4*(s0734 - s1625)) >> 14;
        
        rows[i][2] = (C2*d0734 + C6*d1625) >> 14;
        rows[i][6] = (C6*d0734 - C2*d1625) >> 14;
    }
    
    /* transform columns */
    for (i = 0; i < 8; i++)
    {
        int16_t s07,s16,s25,s34,s0734,s1625;
        int16_t d07,d16,d25,d34,d0734,d1625;
        
        s07 = rows[0][i] + rows[7][i];
        d07 = rows[0][i] - rows[7][i];
        s16 = rows[1][i] + rows[6][i];
        d16 = rows[1][i] - rows[6][i];
        s25 = rows[2][i] + rows[5][i];
        d25 = rows[2][i] - rows[5][i];
        s34 = rows[3][i] + rows[4][i];
        d34 = rows[3][i] - rows[4][i];
        
        data[1][i] = (C1*d07 + C3*d16 + C5*d25 + C7*d34) >> 16;
        data[3][i] = (C3*d07 - C7*d16 - C1*d25 - C5*d34) >> 16;
        data[5][i] = (C5*d07 - C1*d16 + C7*d25 + C3*d34) >> 16;
        data[7][i] = (C7*d07 - C5*d16 + C3*d25 - C1*d34) >> 16;
        
        s0734 = s07 + s34;
        d0734 = s07 - s34;
        s1625 = s16 + s25;
        d1625 = s16 - s25;
        
        data[0][i] = (C4*(s0734 + s1625)) >> 16;
        data[4][i] = (C4*(s0734 - s1625)) >> 16;
        
        data[2][i] = (C2*d0734 + C6*d1625) >> 16;
        data[6][i] = (C6*d0734 - C2*d1625) >> 16;
    }
}



/******************************************************************************
 **  quantize
 **  --------------------------------------------------------------------------
 **  DCT coeficients quantization to discard weak high frequencies.
 **  To avoid division function uses quantization coefs amplified by 2^10 and
 **  then shifts the product by 10 bits to the right.
 **  To make this operation a bit faster some tricks are used but it is just
 **    return round(data[i]/qt0[i]);
 **
 **  ARGUMENTS:
 **      data    - DCT freq. component;
 **      qt      - quantization coeficient;
 **
 **  RETURN: quantized freq. component.
 ******************************************************************************/
int16_t jpeg::quantize(const int16_t data, const uint8_t qt)
{
    return (data*qt + QTAB_DIV-((unsigned)data>>31))>>10;
}



void jpeg::writebyte(const uint8_t b)
{
    jpgbuff[jpgn++] = b;
    
    if (jpgn == sizeof(jpgbuff)) {
        jpgn = 0;
        write_jpeg((const uint8_t*) jpgbuff, sizeof(jpgbuff));
    }
}


void jpeg::writeword(const uint16_t w)
{
    writebyte(w >> 8); writebyte(w);
}



void jpeg::write_APP0info(void)
{
    writeword(0xFFE0); //marker
    writeword(16);     //length
    writebyte('J');
    writebyte('F');
    writebyte('I');
    writebyte('F');
    writebyte(0);
    writebyte(1);//versionhi
    writebyte(1);//versionlo
    writebyte(0);//xyunits
    writeword(1);//xdensity
    writeword(1);//ydensity
    writebyte(0);//thumbnwidth
    writebyte(0);//thumbnheight
}

// should set width and height before writing
void jpeg::write_SOF0info(const int16_t height, const int16_t width)
{
    writeword(0xFFC0);    //marker
    writeword(17);        //length
    writebyte(8);        //precision
    writeword(height);    //height
    writeword(width);    //width
    writebyte(3);        //nrofcomponents
    writebyte(1);        //IdY
    writebyte(0x22);    //HVY, 4:2:0 subsampling
    writebyte(0);        //QTY
    writebyte(2);        //IdCb
    writebyte(0x11);    //HVCb
    writebyte(1);        //QTCb
    writebyte(3);        //IdCr
    writebyte(0x11);    //HVCr
    writebyte(1);        //QTCr
}



void jpeg::write_SOSinfo(void)
{
    writeword(0xFFDA);    //marker
    writeword(12);        //length
    writebyte(3);        //nrofcomponents
    writebyte(1);        //IdY
    writebyte(0);        //HTY
    writebyte(2);        //IdCb
    writebyte(0x11);    //HTCb
    writebyte(3);        //IdCr
    writebyte(0x11);    //HTCr
    writebyte(0);        //Ss
    writebyte(0x3F);    //Se
    writebyte(0);        //Bf
}



void jpeg::write_DQTinfo(void)
{
    unsigned i;
    
    writeword(0xFFDB);
    writeword(132);
    writebyte(0);
    
    for (i = 0; i < 64; i++)
        writebyte(((uint8_t *)qtable_0_lum)[zig[i]]); // zig-zag order
    
    writebyte(1);
    
    for (i = 0; i < 64; i++)
        writebyte(((uint8_t *)qtable_0_chrom)[zig[i]]); // zig-zag order
}



void jpeg::write_DHTinfo(void)
{
    unsigned i;
    
    writeword(0xFFC4); // marker
    writeword(0x01A2); // length
    
    writebyte(0); // HTYDCinfo
    for (i = 0; i < 16; i++)
        writebyte(std_dc_luminance_nrcodes[i+1]);
    for (i = 0; i < 12; i++)
        writebyte(std_dc_luminance_values[i]);
    
    writebyte(0x10); // HTYACinfo
    for (i = 0; i < 16; i++)
        writebyte(std_ac_luminance_nrcodes[i+1]);
    for (i = 0; i < 162; i++)
        writebyte(std_ac_luminance_values[i]);
    
    
    writebyte(1); // HTCbDCinfo
    for (i = 0; i < 16; i++)
        writebyte(std_dc_chrominance_nrcodes[i+1]);
    for (i = 0; i < 12; i++)
        writebyte(std_dc_chrominance_values[i]);
    
    writebyte(0x11); // HTCbACinfo = 0x11;
    for (i = 0; i < 16; i++)
        writebyte(std_ac_chrominance_nrcodes[i+1]);
    for (i = 0; i < 162; i++)
        writebyte(std_ac_chrominance_values[i]);
}



/******************************************************************************
 **  writebits
 **  --------------------------------------------------------------------------
 **  Write bits into bit-buffer.
 **  If the number of bits exceeds 16 the result is unpredictable.
 **
 **  ARGUMENTS:
 **      pbb     - pointer to bit-buffer context;
 **      bits    - bits to write;
 **      nbits   - number of bits to write, 0-16;
 **
 **  RETURN: -
 ******************************************************************************/
void jpeg::writebits(bitbuffer_t *const pbb, uint16_t bits, uint16_t nbits)
{
    // shift old bits to the left, add new to the right
    pbb->buf = (pbb->buf << nbits) | (bits & ((1 << nbits)-1));
    
    nbits += pbb->n;
    
    // flush whole bytes
    while (nbits >= 8) {
        uint8_t b;
        
        nbits -= 8;
        b = pbb->buf >> nbits;
        
        writebyte(b);
        
        if (b == 0xFF) // replace 0xFF with 0xFF00
            writebyte(0);
    }
    
    pbb->n = nbits;
}





/******************************************************************************
 **  flushbits
 **  --------------------------------------------------------------------------
 **  Flush bits into bit-buffer.
 **  If there is not an integer number of bytes in bit-buffer - add zero bits
 **  and write these bytes.
 **
 **  ARGUMENTS:
 **      pbb     - pointer to bit-buffer context;
 **
 **  RETURN: -
 ******************************************************************************/
void jpeg::flushbits(bitbuffer_t *pbb)
{
    if (pbb->n & 3)
        writebits(pbb, 0, 8-(pbb->n & 3));
}




/******************************************************************************
 **  huffman_bits
 **  --------------------------------------------------------------------------
 **  Converst amplitude into the representation suitable for Huffman encoder.
 **
 **  ARGUMENTS:
 **      value    - DCT amplitude;
 **
 **  RETURN: huffman bits
 ******************************************************************************/
uint16_t jpeg::huffman_bits(const int16_t value)
{
    return value - ((unsigned)value >> 31);
}



/******************************************************************************
 **  huffman_magnitude
 **  --------------------------------------------------------------------------
 **  Calculates magnitude of an amplitude - the number of bits that are enough
 **  to represent given value.
 **
 **  ARGUMENTS:
 **      value    - DCT amplitude;
 **
 **  RETURN: magnitude
 ******************************************************************************/
uint16_t jpeg::huffman_magnitude(const int16_t value)
{
    unsigned x = (value < 0)? -value: value;
    unsigned m = 0;
    
    while (x >> m) ++m;
    
    return m;
}




/******************************************************************************
 **  huffman_start
 **  --------------------------------------------------------------------------
 **  Starts Huffman encoding by writing Start of Image (SOI) and all headers.
 **  Sets image size in Start of File (SOF) header before writing it.
 **
 **  ARGUMENTS:
 **      height  - image height (pixels);
 **      width   - image width (pixels);
 **
 **  RETURN: -
 ******************************************************************************/
void jpeg::huffman_start(int16_t height, int16_t width)
{
    writeword(0xFFD8); // SOI
    write_APP0info();
    write_DQTinfo();
    write_SOF0info(height, width);
    write_DHTinfo();
    write_SOSinfo();
    
    huffman_ctx[2].dc =
    huffman_ctx[1].dc =
    huffman_ctx[0].dc = 0;
}



/******************************************************************************
 **  huffman_stop
 **  --------------------------------------------------------------------------
 **  Finalize Huffman encoding by flushing bit-buffer, writing End of Image (EOI)
 **  into output buffer and flusing this buffer.
 **
 **  ARGUMENTS: -
 **
 **  RETURN: -
 ******************************************************************************/
void jpeg::huffman_stop(void)
{
    flushbits(&bitbuf);
    writeword(0xFFD9); // EOI - End of Image
    write_jpeg(jpgbuff, jpgn);
    jpgn = 0;
}




/******************************************************************************
 **  huffman_encode
 **  --------------------------------------------------------------------------
 **  Quantize and Encode a 8x8 DCT block by JPEG Huffman lossless coding.
 **  This function writes encoded bit-stream into bit-buffer.
 **
 **  ARGUMENTS:
 **      ctx     - pointer to encoder context;
 **      data    - pointer to 8x8 DCT block;
 **
 **  RETURN: -
 ******************************************************************************/
void jpeg::huffman_encode(huffman_t *const ctx, const int16_t data[])
{
    unsigned magn, bits;
    unsigned zerorun, i;
    int16_t    diff;
    
    int16_t    dc = quantize(data[0], ctx->qtable[0]);
    
    // difference between old and new DC
    diff = dc - ctx->dc;
    ctx->dc = dc;
    
    bits = huffman_bits(diff);
    magn = huffman_magnitude(diff);
    
    writebits(&bitbuf, ctx->hdcbit[magn], ctx->hdclen[magn]);
    writebits(&bitbuf, bits, magn);
    
    for (zerorun = 0, i = 1; i < 64; i++)
    {
        const int16_t ac = quantize(data[zig[i]], ctx->qtable[zig[i]]);
        
        if (ac) {
            while (zerorun >= 16) {
                zerorun -= 16;
                // ZRL
                writebits(&bitbuf, ctx->hacbit[15][0], ctx->haclen[15][0]);
            }
            
            bits = huffman_bits(ac);
            magn = huffman_magnitude(ac);
            
            writebits(&bitbuf, ctx->hacbit[zerorun][magn], ctx->haclen[zerorun][magn]);
            writebits(&bitbuf, bits, magn);
            
            zerorun = 0;
        }
        else zerorun++;
    }
    
    if (zerorun) { // EOB - End Of Block
        writebits(&bitbuf, ctx->hacbit[0][0], ctx->haclen[0][0]);
    }
}



// chroma subsampling, i.e. converting a 16x16 RGB block into 8x8 Cb and Cr
void jpeg::subsample(const Color rgb[16][16], int16_t cb[8][8], int16_t cr[8][8])
{
    color R, G, B;
    for (unsigned r = 0; r < 8; r++)
        for (unsigned c = 0; c < 8; c++)
        {
            unsigned rr = (r<<1);
            unsigned cc = (c<<1);
            
            // calculating average values
            R = (rgb[rr][cc].nR + rgb[rr][cc+1].nR
                       + rgb[rr+1][cc].nR + rgb[rr+1][cc+1].nR) >> 2;
            G = (rgb[rr][cc].nG + rgb[rr][cc+1].nG
                       + rgb[rr+1][cc].nG + rgb[rr+1][cc+1].nG) >> 2;
            B = (rgb[rr][cc].nB + rgb[rr][cc+1].nB
                       + rgb[rr+1][cc].nB + rgb[rr+1][cc+1].nB) >> 2;
            
            cb[r][c] = RGB2Cb(R, G, B)-128;
            cr[r][c] = RGB2Cr(R, G, B)-128;
        }
}



/*
 WARNING: You must provide a function to perform as write interface
 */

//jpeg::jpeg (Graphic* pGraphic, void (*write_jpeg)(const uint8_t buff[], const unsigned size))

jpeg::jpeg (Graphic& pGraphic) : pGraphic(pGraphic)
{
    
    nWidth  = this->pGraphic.GetWidth();
    nHeight = this->pGraphic.GetHeight();
    
    jpgn = 0;
    
    return;
}


bool jpeg::GetBlock (unsigned int nX, unsigned int nY, Color* BlkColor)
{
#define XYRASTER_GETBLOCK  ((nCounty - nY) * nBlkWidth) + (nCountx - nX)
    
    
    if (BlkColor == NULL) return false;
    
    unsigned int nBlkWidth = 16;
    unsigned int nBlkHeight = 16;
    
    uint nCountx, nCounty;
    uint nDataLen;
    uint nOffset;
    
    int nImageWidth = pGraphic.GetWidth();
    int nImageHeight = pGraphic.GetHeight();
    
    nDataLen = nBlkWidth * nBlkHeight;
    
    //memset (BlkColor, 0, sizeof (Color) * nDataLen);
    
    for (nCounty = nY; nCounty < nY + (nBlkHeight); nCounty++)
        for (nCountx = nX; nCountx < nX + (nBlkWidth); nCountx++)
        {
            //printf ("Blocos: (%d,%d) [%d] [%d] Array Location: [%d]\n", nX, nY, nCountx - nX, nCounty - nY, XYRASTER_GETBLOCK);

            if (nCountx >= nImageWidth || nCounty > nImageHeight)
                continue;
            
            nOffset = (int) XYRASTER_GETBLOCK;
            
            pGraphic.GetPixel (nCountx, nCounty, BlkColor [nOffset]);
            
            //_LOG <<"    -> R: [" << (int) BlkColor [nOffset].nR << "], G: [" << (int) BlkColor [nOffset].nG << "], B: [" << (int) BlkColor [nOffset].nB << "]" << endl;
            
        }
    
    return true;
}


bool jpeg::CompressImage ()
{
    //TRACE ("Vale: Width: [%lu], Height: [%lu]\n", this->nWidth, this->nHeight);
    
    //this->write_jpeg = write_jpeg;
    
    
    Color   RGB16x16[16][16];
    
    int16_t Y8x8[2][2][8][8]; // four 8x8 blocks - 16x16
    int16_t Cb8x8[8][8];
    int16_t Cr8x8[8][8];
    
    uint16_t rr;
    uint16_t cc;
    
    color R, G, B;
    
    huffman_start (nHeight & -16, nWidth & -16);
    
    
    if (1) for (unsigned int nY = 0; nY < nHeight - 15; nY += 16)
        for (unsigned int nX = 0; nX < nWidth - 15; nX += 16)
        {
            Verify (GetBlock (nX, nY, (Color*) RGB16x16), "Error getting GC block samples", 1, jpegException);
            
            for (unsigned nCounti = 0; nCounti < 2; nCounti++)
                for (unsigned nCountj = 0; nCountj < 2; nCountj++)
                {
                    for (unsigned nCountr = 0; nCountr < 8; nCountr++)
                        for (unsigned nCountc = 0; nCountc < 8; nCountc++)
                        {
                            rr = (nCounti<<3) + nCountr;
                            cc = (nCountj<<3) + nCountc;
                            
                            R = RGB16x16[rr][cc].nR;
                            G = RGB16x16[rr][cc].nG;
                            B = RGB16x16[rr][cc].nB;

                            printf ("Pixel: (%d, %d) R:[%u], G:[%u], B:[%u]\n\n", rr, cc, RGB16x16[rr][cc].nR, RGB16x16[rr][cc].nG, RGB16x16[rr][cc].nB);
                            
                            // converting RGB into Y (luminance)
                            Y8x8[nCounti][nCountj][nCountr][nCountc] = (RGB2Y(R, G, B)-128);
                        }
                }
            
            
            // getting subsampled Cb and Cr
            subsample(RGB16x16, Cb8x8, Cr8x8);
            
            // 1 Y-compression
            dct(Y8x8[0][0], Y8x8[0][0]);
            huffman_encode(HUFFMAN_CTX_Y, (int16_t*)Y8x8[0][0]);
            // 2 Y-compression
            dct(Y8x8[0][1], Y8x8[0][1]);
            huffman_encode(HUFFMAN_CTX_Y, (int16_t*)Y8x8[0][1]);
            // 3 Y-compression
            dct(Y8x8[1][0], Y8x8[1][0]);
            huffman_encode(HUFFMAN_CTX_Y, (int16_t*)Y8x8[1][0]);
            // 4 Y-compression
            dct(Y8x8[1][1], Y8x8[1][1]);
            huffman_encode(HUFFMAN_CTX_Y, (int16_t*)Y8x8[1][1]);
            // Cb-compression
            dct(Cb8x8, Cb8x8);
            huffman_encode(HUFFMAN_CTX_Cb, (int16_t*)Cb8x8);
            // Cr-compression
            dct(Cr8x8, Cr8x8);
            huffman_encode(HUFFMAN_CTX_Cr, (int16_t*)Cr8x8);
        }
    
    
    huffman_stop();
    
    return true;
}

/*
 void jpeg::write_jpeg (const uint8_t buff[], const unsigned size)
 {
 Verify (write(this->nFD, (const void*) buff, size) >= 0, "", 2, jpegException);
 }
 */




/* LIVE EXAMPLE
 
 
 Graphic* pGraphic;
 int      nCountx, nCounty;
 Color  stColor;
 int    nColor;
 jpeg*  pJPEG;
 Color  BlkColors [16][16];
 
 
 pGraphic = new Graphic (400,400);
 
 stColor = MkColor (255,255,255);
 
 pGraphic.DrawFillBox (1,1, 400, 400, 0, MkColor (255,255,255));
 pGraphic.DrawXPM (1, 1, 0, 0, 0, 0, 0, DSC_xpm);
 pGraphic.DrawXPM (100, 100, 0, 0, 0, 0, 130, GU_xpm);
 
 
 pGraphic.PutPixel (20,20, MkColor (255,255,255));
 pGraphic.PutPixel (0,0, stColor);
 
 //stColor = pGraphic.GetPixel (200,200);
 stColor = pGraphic.GetPixel (0,0);
 
 
 //pGraphic.GetBlock (390,390, 16, 16, (Color*) BlkColors);
 
 pJPEG = new jpeg (pGraphic, fileno (stdout));
 
 pJPEG->CompressImage();
 
 
 exit (0);
 
 GIF_Create ("Teste.gif", 400, 400, 256, 8);
 
 GIF_MakeTable();
 
 GIF_DrawFillBox (0, 0, 400, 400, 0, 15);
 
 for (nCountx=0; nCountx < 400; nCountx++)
 {
 for (nCounty=0; nCounty < 400; nCounty++)
 {
 nColor = pGraphic.Get8bitsColor (nCountx, nCounty);
 NOTRACE (" Cor: nX:%u nY:%u [%u]\n", nCountx, nCounty, nColor);
 GIF_PutPixel (nCountx, nCounty, nColor);
 }
 }
 
 
 GIF_CompressImage (1, 1, 400, 400, NULL);
 GIF_Close ();
 */
