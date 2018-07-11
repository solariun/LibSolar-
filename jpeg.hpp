/*
 *  jpeg.h
 *  FSRash
 *
 *  Created by Gustavo Campos on 1/4/10.
 *  Copyright 2010 `. All rights reserved.
 *
 */

#ifndef JPEG_H
#define JPEG_H

#include "Exception.hpp"
#include "Util.h"
//#include <Exception.h>
#include "graphic.hpp"

    
typedef uint8_t color;
    
#ifdef _MSC_VER
#    include <intrin.h>
#    include <emmintrin.h>
#    include <io.h>
#    pragma intrinsic(__rdtsc)
#    define CACHE_ALIGN __declspec(align(32))
    typedef unsigned __int64 uint64_t;
#else
#    include <stdint.h>
#    include <unistd.h>
    
    
#    define CACHE_ALIGN __attribute__ ((aligned(32)))
#    define O_BINARY    0
#endif
    
    
    typedef struct {
        color Red;
        color Green;
        color Blue;
        color Alpha;
    } RGBA;
    
    typedef struct {
        color Blue;
        color Green;
        color Red;
        color Alpha;
    } BGRA;
    
    typedef struct {
        color Blue;
        color Green;
        color Red;
    } BGR;
    
    typedef struct {
        uint8_t Blue:5;
        uint8_t Green:5;
        uint8_t Red:5;
        uint8_t Reserved:1;
    } BGR16;
    
    //---------------- J P E G ---------------
    
    // Application should provide this function for JPEG stream flushing
    //void write_jpeg(const uint8_t buff[], const unsigned size);
    
    typedef struct huffman_s
    {
        const uint8_t  (*haclen)[12];
        const uint16_t (*hacbit)[12];
        const uint8_t  *hdclen;
        const uint16_t *hdcbit;
        const uint8_t  *qtable;
        int16_t             dc;
    }
    huffman_t;
    
    typedef struct bitbuffer_s
    {
        uint32_t buf;
        uint32_t n;
    }
    bitbuffer_t;
    
    
    extern huffman_t huffman_ctx[3];
    
#define    HUFFMAN_CTX_Y    &huffman_ctx[0]
#define    HUFFMAN_CTX_Cb    &huffman_ctx[1]
#define    HUFFMAN_CTX_Cr    &huffman_ctx[2]
    
    



class jpegException : public Exception
{
public:
    jpegException (std::string strMessage, uint nErrorID);
};



class jpeg
{
protected:
    
    Graphic& pGraphic;
    bitbuffer_t bitbuf;
    
    
    void huffman_start(int16_t height, int16_t width);
    void huffman_stop(void);
    void huffman_encode(huffman_t *const ctx, const int16_t data[]);
    void subsample(const Color rgb[16][16], int16_t cb[8][8], int16_t cr[8][8]);
    
    int32_t nWidth, nHeight;
private:
    
    //void (*write_jpeg) (const uint8_t buff[], const unsigned size);
    
    //void write_jpeg (const uint8_t buff[], const unsigned size);
    
    int16_t quantize(const int16_t data, const uint8_t qt);
    // code-stream output counter
    uint16_t jpgn;
    // code-stream output buffer, adjust its size if you need
    uint8_t jpgbuff[1024];
    
    
    void writebyte(const uint8_t b);
    void writeword(const uint16_t w);
    void write_APP0info(void);
    void write_SOF0info(const int16_t height, const int16_t width);
    void write_SOSinfo(void);
    void write_DQTinfo(void);
    void write_DHTinfo(void);
    void writebits(bitbuffer_t *const pbb, uint16_t bits, uint16_t nbits);
    void flushbits(bitbuffer_t *pbb);
    uint16_t huffman_bits(const int16_t value);
    uint16_t huffman_magnitude(const int16_t value);
    
    jpeg ();
    
    bool GetBlock (unsigned int nX, unsigned int nY, Color* BlkColor);

public:
    
    //function<void(string)
    //jpeg (Graphic* pGraphic, void (*write_jpeg)(const unsigned char* buff, const unsigned size));
    /* EXEMPLO DE
     jpeg jpegImage (&gChartContext);
     */
    jpeg (Graphic& pGraphic);
    
    
    /*
     * since it is pure virtual, making this object almost abstract
     * this fcuntion MUST BE set in order to be used.
     */
    //void (*write_jpeg)(const uint8_t buff[], const unsigned size) = nullptr;
    virtual void write_jpeg (const uint8_t* data, const uint32_t size) = 0;
    
    
    /*
     COMPREESS Needs to be called with a linear writing data
     done with lambida...
     
     Ex:
     jpegImage.CompressImage([&](const unsigned char* buff, const unsigned size) -> void
     {
     write(nFD, (const void*) buff, size);
     });
     */
    
    bool CompressImage ();
};




#endif

