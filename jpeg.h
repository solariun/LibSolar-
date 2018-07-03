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
#include "graphic.h"

#ifdef __cplusplus
extern "C" {
#endif
	
	typedef unsigned char color;
	
#ifdef _MSC_VER
#	include <intrin.h>
#	include <emmintrin.h>
#	include <io.h>
#	pragma intrinsic(__rdtsc) 
#	define CACHE_ALIGN __declspec(align(32))
	typedef unsigned __int64 uint64_t;
#else
#	include <stdint.h>
#	include <unistd.h>
	

#	define CACHE_ALIGN __attribute__ ((aligned(32)))
#	define O_BINARY	0
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
		unsigned short int Blue:5;
		unsigned short int Green:5;
		unsigned short int Red:5;
		unsigned short int Reserved:1;
	} BGR16;
	
	//---------------- J P E G ---------------
	
	// Application should provide this function for JPEG stream flushing
	void write_jpeg(const unsigned char buff[], const unsigned size);
	
	typedef struct huffman_s
	{
		const unsigned char  (*haclen)[12];
		const unsigned short (*hacbit)[12];
		const unsigned char  *hdclen;
		const unsigned short *hdcbit;
		const unsigned char  *qtable;
		short                dc;
	}
	huffman_t;
	
	typedef struct bitbuffer_s
	{
		unsigned buf;
		unsigned n;
	}
	bitbuffer_t;

	
	extern huffman_t huffman_ctx[3];
	
#define	HUFFMAN_CTX_Y	&huffman_ctx[0]
#define	HUFFMAN_CTX_Cb	&huffman_ctx[1]
#define	HUFFMAN_CTX_Cr	&huffman_ctx[2]
	
	
#ifdef __cplusplus
}
#endif

	
class jpegException : public Exception
{
public:
    jpegException (std::string strMessage, uint nErrorID);
};



class jpeg 
{
protected:
	
	Graphic* pGraphic;
	bitbuffer_t bitbuf;
	
	
	void huffman_start(short height, short width);
	void huffman_stop(void);
	void huffman_encode(huffman_t *const ctx, const short data[]);
	void subsample(const Color rgb[16][16], short cb[8][8], short cr[8][8]);
	
    int32_t nWidth, nHeight;
private:
	
	int nFD;
	
	//void (*write_jpeg) (const unsigned char buff[], const unsigned size);
	
	void write_jpeg (const unsigned char buff[], const unsigned size);
	
	short quantize(const short data, const unsigned char qt);
	// code-stream output counter
	unsigned jpgn;
	// code-stream output buffer, adjust its size if you need
	unsigned char jpgbuff[2048];

	
	void writebyte(const unsigned char b);
	void writeword(const unsigned short w);
	void write_APP0info(void);
	void write_SOF0info(const short height, const short width);
	void write_SOSinfo(void);
	void write_DQTinfo(void);
	void write_DHTinfo(void);
	void writebits(bitbuffer_t *const pbb, unsigned bits, unsigned nbits);
	void flushbits(bitbuffer_t *pbb);
	unsigned huffman_bits(const short value);
	unsigned huffman_magnitude(const short value);

public:
	
	jpeg (Graphic* pGraphic, int nFileDescriptor);
	
	//virtual void write_jpeg(const unsigned char buff[], const unsigned size) = 0;
	
	bool CompressImage ();
};
	



#endif


