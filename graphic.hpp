/*
 *  Graphic.h
 *  FSRash
 *
 *  Created by Gustavo Campos on 1/3/10.
 *  Copyright 2010 `. All rights reserved.
 *
 */


#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "Exception.hpp"
#include "Util.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif /* PI */

#ifndef VERIFY
#define VERIFY(x,y,z) if (!(x)) { UINT nExpError = errno; TRACE ("Exception: %s (%d): %s [%s] - %s\n", __FILE__, __LINE__, __FUNCTION__,\
#x, y [0] == '\0' ? strerror (nExpError) : y); return z; }
#endif

#ifndef TRACE
#define TRACE printf
#define YYTRACE printf
#define NOTRACE if (0) printf
#endif

#ifndef UINT
#define UINT unsigned int
#endif


#define ON_OFF_PIXELS	3.0

#define ABS(i) i<0?(i*(-1)):i

/*
 Define a structure of a Color
 */

typedef union
{
    uint32_t nRGB;
    struct
    {
        uint8_t  nAlpha;
        uint8_t  nB;
        uint8_t  nG;
        uint8_t  nR;
    };
} Color;


Color MkColor(uint8_t nR, uint8_t nG, uint8_t nB);
Color MkColor(uint8_t nR, uint8_t nG, uint8_t nB, uint8_t nAlpha);
Color MkColor(const string strColor);

class GraphicException : public Exception
{
public:
    GraphicException (std::string strMessage, uint nErrorID);
};


class Graphic 
{
protected:
	
	/*
	 ** Defines a structure for specifying a point.
	 */
	typedef struct 
	{
		int x;
		int y;
	} IPoint;
	
	/*
	 ** Line drawing styles.
	 ** Applies to: IDrawLine, IDrawRectangle
	 */
	typedef enum 
	{
		ILINE_SOLID,			/* default */
		ILINE_ON_OFF_DASH,		/* dashes (every 3 pixels) */
		ILINE_DOUBLE_DASH		/* not yet implemented */
	} ILineStyle;
	
	char GFont[96][145];

private:
    
    uint32_t nArrayLen = 0;

	uint32_t nGlobalX = 0, nGlobalY = 0;
	
    Color* ColorBuffer;
	
    uint32_t nGlobalAngle = 0;
    
	uint32_t nImageWidth = 0, nImageHeight = 0;
	
	
	double Cos(double Degrees);
	double Sin(double Degrees);

	inline char* Tolower (char* pszText);
	Color GetColorByName (char* pszColor);

	void MakeTable();
	inline void XTerm2RGB(unsigned char color, unsigned char* rgb);

	inline int PSet (int nX, int nY, int nAlpha, Color stColor);
	
    Graphic();
    
public:
	uint8_t LineWidth;
	
	uint32_t GetWidth ();
	uint32_t GetHeight ();
	
    
	void SetAngle(int nX, int nY, int nAngle);

	bool GetBlock (unsigned int nX, unsigned int nY, unsigned int nBlkWidth, unsigned int nBlkHeight, Color* BlkColor);
	
	void SelectDefaultFont(int nFont);
    
	Color GetPixel (int nX, int nY);
    void  GetPixel (int nX, int nY, Color& color);
    
	Color GetAlphaPixel (int nX, int nY, Color stColor, int nAlpha);

	int PutPixel (int nX, int nY, Color stColor);
	int PutPixel (int nX, int nY, int nAlpha, Color stColor);

	int DrawRadiosLine (int nX, int nY,  int nLength, double nAngle, int nAlpha, Color stColor);
	int PutPixelRadios (int nX, int nY, int nLength, double nAngle, int nAlpha, Color stColor);
	int IDrawLine (int x0, int y0, int x1, int y1, int nAlpha, Color stColor);
	int DrawLine (int x1, int y1, int x2, int y2, int nAlpha, Color stColor);
	int DrawArc (int x, int y, int r1, int r2, int a1, int a2, int nAlpha, Color stColor);
	int DrawCircle (int nX, int nY, int nLenght, int nIsFill, int nAlpha, Color stColor);
	
	void BasicDrawWidth (int* x, int * y, const char *dr);
	void GPrint (int* nX, int* nY, int nAlpha, Color stColor, const char* pszText);
    void GPrint (int nX, int nY, int nAlpha, Color stColor, const char* pszText);
	void GPrintWidth (int* nX, int* nY, char* pszText);
	void BasicDraw (int* x, int * y, int nAlpha, Color stColor, const char *dr);
	int  DrawBox (int x, int y, int x2, int y2, int nAlpha, Color stColor);
	int  DrawFillBox (int x, int y, int x2, int y2, int nAlpha, Color stColor);

	
	int DrawXPM (int nPX, int nPY, int nLX, int nLY, int nWidth, int nHeight, int nAlpha, const char* ppszData[]);
	unsigned char Get8bitsColor(int nX, int nY);

    uint8_t* getRGBBuffer();
    
	//Creating procedures
	
	Graphic (uint32_t nWidth, uint32_t nHeight);
	~Graphic ();

};





#endif
