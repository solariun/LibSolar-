/*
 *  Graphic.cpp
 *  FSRash
 *
 *  Created by Gustavo Campos on 1/3/10.
 *  Copyright 2010 `. All rights reserved.
 *
 */

#include "graphic.h"

GraphicException::GraphicException (std::string strMessage, uint nErrorID): Exception ("Graphic", strMessage, nErrorID)
{}

/* Interations for 8bits color conversion */
// whole colortable, filled by maketable()
static int initialized=0;
static unsigned char colortable[254][3];

// the 6 value iterations in the xterm color cube
static const unsigned char valuerange[] = { 0x00, 0x5F, 0x87, 0xAF, 0xD7, 0xFF };

// 16 basic colors
static const unsigned char basic16[16][3] =
{
	{ 0x00, 0x00, 0x00 }, // 0
	{ 0xCD, 0x00, 0x00 }, // 1
	{ 0x00, 0xCD, 0x00 }, // 2
	{ 0xCD, 0xCD, 0x00 }, // 3
	{ 0x00, 0x00, 0xEE }, // 4
	{ 0xCD, 0x00, 0xCD }, // 5
	{ 0x00, 0xCD, 0xCD }, // 6
	{ 0xE5, 0xE5, 0xE5 }, // 7
	{ 0x7F, 0x7F, 0x7F }, // 8
	{ 0xFF, 0x00, 0x00 }, // 9
	{ 0x00, 0xFF, 0x00 }, // 10
	{ 0xFF, 0xFF, 0x00 }, // 11
	{ 0x5C, 0x5C, 0xFF }, // 12
	{ 0xFF, 0x00, 0xFF }, // 13
	{ 0x00, 0xFF, 0xFF }, // 14
	{ 0xFF, 0xFF, 0xFF }  // 15
};
/* End of the code */




Color MkColor(uint8_t nR, uint8_t nG, uint8_t nB)
{	
	return MkColor (nR, nG, nB, 0);
}


Color MkColor(uint8_t nR, uint8_t nG, uint8_t nB, uint8_t nAlpha)
{
	Color stColor;
	stColor.nR = nR;
	stColor.nG = nG;
	stColor.nB = nB;
	stColor.nAlpha = nAlpha;
	
	return stColor;
}



Graphic::Graphic (uint32_t nWidth, uint32_t nHeight)
{
	//NOTRACE ("Initilizing....\n");
	
	uint32_t	nArrayLen = (uint32_t) nWidth * nHeight;
	
	ColorBuffer = (Color*) malloc (sizeof (Color) * (nArrayLen + 2));
	if (ColorBuffer == NULL)
		throw ("Error allocing memory for Graphics");
	
	nGlobalX = 0;
	nGlobalX = 0;
	
    SelectDefaultFont (0);
    
	nImageWidth = nWidth;
	nImageHeight = nHeight;
}




Graphic::~Graphic ()
{
	free (ColorBuffer);
}




uint32_t Graphic::GetWidth ()
{
	return nImageWidth;
}




uint32_t Graphic::GetHeight ()
{
	return nImageHeight;
}




double Graphic::Cos(double Degrees)
{
    double ret;
    double off = (Degrees / 30 - round(Degrees / 30));
    
    if (0) return cos (Degrees);
    
    if (off < .0000001 && off > -.0000001)
    {
        int iDegrees = (int)round(Degrees);
        iDegrees = (iDegrees < 0) ? (360 - (-iDegrees % 360))  : (iDegrees % 360);
        switch (iDegrees)
        {
            case 0:   ret=1.0; break;
            case 30:  ret=0.866025403784439; break;
            case 60:  ret=0.5; break;
            case 90:  ret=0.0; break;
            case 120: ret=-0.5; break;
            case 150: ret=-0.866025403784439; break;
            case 180: ret=-1.0; break;
            case 210: ret=-0.866025403784439; break;
            case 240: ret=-0.5; break;
            case 270: ret=0.0; break;
            case 300: ret=0.5; break;
            case 330: ret=0.866025403784439; break;
            case 360: ret=1.0; break;
            default:  ret=cos(Degrees * 3.14159265358979 / 180);  // it shouldn't get here
        }
        return ret;
    }
    else
        return cos(Degrees * 3.14159265358979 / 180);
}



double Graphic::Sin(double Degrees)
{
    return Cos(Degrees+90);
}




/* 
 * Font routines 
 */

void Graphic::SelectDefaultFont(int nFont)
{
    if(nFont)
    {
        strcpy(GFont[1],"br3");
        strcpy(GFont[2],"u1bu2u6r1d6bd2d1br2");
        strcpy(GFont[3],"bu5u3r1nd3br3nd3r1d3bd5br2");
        strcpy(GFont[4],"u2l1nr6r1u2r1nd4u3l1nr6r1u2r1nd4d2r2u2nd4r1nd4l1d9l1nu4r1br4");
        strcpy(GFont[5],"bu1r1d1nr3r1d1r1u1r1nu4e1u2h1l3nu4h1u2e1nr3e1r1f1d2r1nu1br2bd6");
        strcpy(GFont[6],"e9r1g9bu5nu4h1u2e1r2nd4f1d2g1nl2br4bd1nd4g1d2f1r2nu4e1u2h1nl2br3bd4");
        strcpy(GFont[7],"bu1u2e1nd4r1nu5u2l1u2e1r2nd4f1d2g1l1d1nl1d1r1nd3d1r1nd2r1u1d1l1d2nr1nl3r1br2");
        strcpy(GFont[8],"bu6u3r1d3br2bd6");
        strcpy(GFont[9],"d2r1d1nr1u12d1l1nd9e1r1bd9br2");
        strcpy(GFont[10],"bd3r1u12nl1f1d10u2br2");
        strcpy(GFont[11],"br1bu5u1r1u2nl2u1r1d1nr2d2r1d1bd5br2");
        strcpy(GFont[12],"br3bu1u6r1nd6d3nl3r2br2bd4");
        strcpy(GFont[13],"bd1r1u2r1d1br2");
        strcpy(GFont[14],"bu4r3bd4br2");
        strcpy(GFont[15],"u1r1d1br2");
        strcpy(GFont[16],"d2u3r1nd3u4r1nd3u4r1d3bd6br2");
        strcpy(GFont[17],"bu1u1u6e1nd8r3d1nd7r1d7g1nl3br3");
        strcpy(GFont[18],"br2u8nl2u1r1d9br2");
        strcpy(GFont[19],"nr5u2r1nd2l1e4u3d1r1d2ng4l1u3l3d2l1u1br7bd8");
        strcpy(GFont[20],"bu1r1d1r3u4r1nd3l1u1nl2u3r1nd2l1u1l3d1l1br7bd8");
        strcpy(GFont[21],"bu2br1u1nr1u1nr1u1r1u1nr3u1nr3r1u1nr2r1u1r1d9l1nu9u2nnl4d2r1br2");
        strcpy(GFont[22],"bu1r1d1r3nu5e1u3h1l4d1r1u1l1u4r1nd4r4br2bd9");
        strcpy(GFont[23],"bu1u7e1nd9r3d1r1bd4nd3l1u1nl3d5nl3br3");
        strcpy(GFont[24],"br1u2r1nd2u3r1nd2u2r1nd1u2r1nd1nl5br2bd9");
        strcpy(GFont[25],"bu1u3bu2u2e1nd9r3nd9f1d2bd2d3g1nl3u5nl3br3bd5");
        strcpy(GFont[26],"bu1r1d1r3nu9e1u7h1l3nd5g1d3f1r3br3bd4");
        strcpy(GFont[27],"u1bu5u1r1d1bd5d1br2");
        strcpy(GFont[28],"br1u1bu5u1r1d1bd5d1l1d1l1r1u1r1br2");
        strcpy(GFont[29],"br4nh4r1h4l1ne4r1ne4br6bd4");
        strcpy(GFont[30],"bu3r5bu3nl5bd6br2");
        strcpy(GFont[31],"e4r1ng4l1nh4r1nh4br2bd4");
        strcpy(GFont[32],"br2u1r1nd1l1bu2u1r1nd1u1r1nu4e1u2h1l3d1l1d1r1bd7br6");
        strcpy(GFont[33],"bu2u4e1u1nd8r1d1u2r2u1r3d1r2d2u1r1nd7d1f1d4g1l3u6d1r1d2l2d1g1l1nu4u1l1u2r1u1r1u1r2bd7bl6r1nu1d1r2d1r4u1r2bu1br4");
        strcpy(GFont[34],"nr1u1nr1e1nr4u1nr1u1nr1e1u1nr3u1nr3e1nu1r1nu1d1r1d3nr1d1nr1d1r1d1nr1d1r1br2");
        strcpy(GFont[35],"u9r1nd9r4d1r1nd2l1d3nl4d1r1nd3l1d4nl4br3");
        strcpy(GFont[36],"bu2u5e1d7r1d1r3u1r1u1bu5u1l1u1l3d1l1br7bd8");
        strcpy(GFont[37],"u9r1nd9r4d1r1d1nd6r1d5g1l1d1nl4br5");
        strcpy(GFont[38],"u9r1nd9r5bd3bg1nl4br1bd5nl5br2");
        strcpy(GFont[39],"u9r1nd9r5bd3bg1nl4br1bd5br2");
        strcpy(GFont[40],"bu2u5e1nd7r1u1r4d1r1d1bd3nl4nd4l1d3g1l3u1br7bd1");
        strcpy(GFont[41],"u9r1d4nd5r5nu4d5r1nu9br2");
        strcpy(GFont[42],"u9r1d9br2");
        strcpy(GFont[43],"bu1u1r1d2r3u9r1d8bd1br2");
        strcpy(GFont[44],"u9r1nd9d3r1e3r1g4nl2d1f4r1nh5br2");
        strcpy(GFont[45],"u9r1d9r5br2");
        strcpy(GFont[46],"u9r1nd9d2r1d3u1r1d3u1r1d3r1u3d1r1u3d1r1u3r1u2d9r1nu9br2");
        strcpy(GFont[47],"u9r1nd9d1r1d2u1r1d3u1r1d3u1r1d2f1nu9r1nu9br2");
        strcpy(GFont[48],"bu2u5r1u1nd7r1u1r3d1r1nd7f1d5g2nu1l3u1br7bd1");
        strcpy(GFont[49],"u9r1nd9r5nd5f1d3g1nl5br3bd4");
        strcpy(GFont[50],"bu2u5r1u1nd7r1u1r3d1r1nd7f1d5g2u2l1f2nr1l4u1br7bd1");
        strcpy(GFont[51],"u9r1nd9r5nd5f1d3g1nl5nd3d1r1d3r1br2");
        strcpy(GFont[52],"bu1u1r1d2r4u4f1nd2h1l2u1l2nu4h1u2e1r4d2r1u1br2bd8");
        strcpy(GFont[53],"bu9r3nd9r1nd9r3br2bd9");
        strcpy(GFont[54],"bu2u7r1d8r1d1r3u1r1u8r1d7bd2br2");
        strcpy(GFont[55],"bu8u1r1d4u2r1d5u1r1d3r1u3d1r1u5d2r1u4r1d1bd8br2");
        strcpy(GFont[56],"bu7u2r1d4u1r1d3u1r1d4r1u3r1u3d1r1nu4r1nu4r1nu1d2r1nd3r1nd3u1r1nd1u2r1d1u4r1d2bd7br2");
        strcpy(GFont[57],"nr1u1e7u1r1d1g7d1br6r1u1h7u1l1d1f7d1r1br2");
        strcpy(GFont[58],"bu8nf4u1nr1f4nd5r1nd5e4nl1d1g4d4br6");
        strcpy(GFont[59],"nr8u1e8d1ng7u1nl8bd9br2");
        strcpy(GFont[60],"d3nu12r1nr1u12r1br2bd9");
        strcpy(GFont[61],"bu6u3r1d7u3r1d7r1u3d1br2");
        strcpy(GFont[62],"d3nl1nu12r1u12nl2br2bd9");
        strcpy(GFont[63],"bu7r1u1r1nu1r1d1r1bd7br2");
        strcpy(GFont[64],"bd3r4bu3");
        strcpy(GFont[65],"bu6u3r1d3br2bd6");
        strcpy(GFont[66],"bu1u1e2r1g2d2r3u6f1nd5h1l3d1l1bd5br7");
        strcpy(GFont[67],"u9r1d3nd6r3d1nd5r1d4g1nl4br3");
        strcpy(GFont[68],"bu1u4e1d6r3u1r1bu4l1u1l3br6bd6");
        strcpy(GFont[69],"bu1u4e1nr4d6r3nu9r1nu9br2");
        strcpy(GFont[70],"bu1u4e1nd6r3d1nr1d1r1nl4bd3l1d1nl3br3");
        strcpy(GFont[71],"br1u6nl1u2e1nr1d3nr1d6br3");
        strcpy(GFont[72],"bu1u4e1nd6r4nd8l1d6nl3d3l3u1l1br7bu2");
        strcpy(GFont[73],"u9r1d3nd6r3nd6f1d5br2");
        strcpy(GFont[74],"nu6r1u6bu2u1l1d1bd2d6br3");
        strcpy(GFont[75],"nu6r1u6bu2u1l1d1bd2d9nl1u1r1u2br2");
        strcpy(GFont[76],"u9r1d9br1bu2u2e2r1g3d1f2r1nh3br2");
        strcpy(GFont[77],"nu9r1nu9br2");
        strcpy(GFont[78],"u6r1nd6r3nd6r1nd6r3nd6d1r1d5br2");
        strcpy(GFont[79],"u6r1nd6r3nd6f1d5br2");
        strcpy(GFont[80],"bu1u4e1d6r3u6f1nd4h1nl3bd6br3");
        strcpy(GFont[81],"nd3nu6r1nd3u6r3d6nl4e1nu4bd1br2");
        strcpy(GFont[82],"bu1u4e1nd6r4d9l1nu9u3nl3br3");
        strcpy(GFont[83],"u6r1nd6r1nd1r1br2bd6");
        strcpy(GFont[84],"bu1r1d1r3u1r1u1l1u1l3u1l1u1r1u1r3d1r1br2bd5");
        strcpy(GFont[85],"br1bu1u5nl1nu2r1nu2nr1d6r1br2");
        strcpy(GFont[86],"bu1u5r1d6r3nu6r1nu6br2");
        strcpy(GFont[87],"bu5u1r1d3u1r1d2r1nd2r1nd2r1u2d1r1u3r1d1bd5br2");
        strcpy(GFont[88],"bu5u1r1d4u2r1d4u2r1nd2r1nu3r1nu3r1d2r1u4d2r1u4r1d1bd5br2");
        strcpy(GFont[89],"ne6r1ne6br5nh6r1nh6br2");
        strcpy(GFont[90],"bu5u1r1d3u1r1d3u1r1d4l1d1l1br3bu2u3r1nd1u2d1r1u3r1d1bd5br2");
        strcpy(GFont[91],"nr5u1e5d1ng5u1nl5br2bd6");
        strcpy(GFont[92],"d2r1d1nr1u5l1nrdu1nd2u1nl1u5r1nd4u1r1br2bd9");
        strcpy(GFont[93],"d3nu12r1nu12u3br2");
        strcpy(GFont[94],"bd3r1nu5u1r1u5nr1u5l1nd4u1nl1br4bd9");
        strcpy(GFont[95],"bu4u1r2d1r2u1br2bd5");
    }
    else
    {
        strcpy(GFont[1],"br3");
        strcpy(GFont[2],"nr0bu2u5bd7br2");
        strcpy(GFont[3],"bu5u2br2d2bd5br2");
        strcpy(GFont[4],"u2nr4r1u3nl1r1nu2r2nu2l1d3l1d2br4");
        strcpy(GFont[5],"bu1f1r1nd1r1e1u2h1l1nu3nd4l1h1u1e1r2f1br2bd6");
        strcpy(GFont[6],"bu5u1e1r1f1d1g1nl1bd4e1u1e1u1e1u1e1bd4r1f1d1g1l1h1u1br5bd2");
        strcpy(GFont[7],"bu1u1e2h1u1e1r1f1d1g1d1f1nr1d1nf1g1nl2br4");
        strcpy(GFont[8],"bu5nu2bd5br2");
        strcpy(GFont[9],"nf2u5e2bd7br2");
        strcpy(GFont[10],"br2ng2u5nh2br2bd5");
        strcpy(GFont[11],"bu4e1nf1u1nl1nr1nu1bd6br3");
        strcpy(GFont[12],"br2u2nl2nr2nu2d2br4");
        strcpy(GFont[13],"nd2br2");
        strcpy(GFont[14],"bu2r3br2bd2");
        strcpy(GFont[15],"nr0br2");
        strcpy(GFont[16],"u1e1u3e1u1bd7br2");
        strcpy(GFont[17],"bu1u5e1r2f1d5g1nl2br3");
        strcpy(GFont[18],"bu5e2d7br2");
        strcpy(GFont[19],"nr4e4u2h1l2g1br6bd6");
        strcpy(GFont[20],"bu1f1r2e1u2h1nl1e1u1h1l2g1br6bd6");
        strcpy(GFont[21],"bu2nr4u1e1u1e2d7br3");
        strcpy(GFont[22],"bu1f1r2e1u2h1l3u1e1u1r3br2bd7");
        strcpy(GFont[23],"bu1bu5e1r2f1bd3nd2h1l3nu2d3f1r2br4");
        strcpy(GFont[24],"u2e1u1e1u2nl3r1br2bd7");
        strcpy(GFont[25],"bu1u2e1nr2h1u1e1r2f1d1g1f1d2g1nl2br3");
        strcpy(GFont[26],"bu1f1r2e1u2nl3u3h1l2g1d2br6bd4");
        strcpy(GFont[27],"nr0bu5nr0br2bd5");
        strcpy(GFont[28],"nd2bu5nr0br2bd5");
        strcpy(GFont[29],"br4h1l2h1e1r2e1br2bd4");
        strcpy(GFont[30],"bu2nr4bu2r4bd4br2");
        strcpy(GFont[31],"e1r2e1h1l2h1br6bd4");
        strcpy(GFont[32],"br2nr0bu2u1e2u1h1l2g1br6bd6");
        strcpy(GFont[33],"u4e1u1r1e1r4f2d3g2l1u4nl1h1l1g2d2f1r1e2bd2bl6f2r6e1bu1br2");
        strcpy(GFont[34],"u1e1nr4u1e1u2e1f1d2f1d1f1d1br2");
        strcpy(GFont[35],"u7r4f1d2nl4d3g1nl4br3");
        strcpy(GFont[36],"bu2nf2u3e2r2f1bd5g1nl2br3");
        strcpy(GFont[37],"u7r3f2d3g2nl3br4");
        strcpy(GFont[38],"nr4u4nr4u3r4br2bd7");
        strcpy(GFont[39],"u4nr4u3r4br2bd7");
        strcpy(GFont[40],"bu2nf2u3e2r2f2bd2nl2d1g2nl2br4");
        strcpy(GFont[41],"u4nr5u3br5d7br2");
        strcpy(GFont[42],"nu7br2");
        strcpy(GFont[43],"bu1nu1f1r1e1u6br2bd7");
        strcpy(GFont[44],"u3nu4r1ne4br2nu1f1d1f1br2");
        strcpy(GFont[45],"nu7r4br2");
        strcpy(GFont[46],"u7f1d1f1d2f1nd1e1u2e1u1e1d7br2");
        strcpy(GFont[47],"u7f2d1f1d1f2nu7br2");
        strcpy(GFont[48],"bu2nf2u3e2r2f2d3g2nl2br4");
        strcpy(GFont[49],"u3nr3u4r3f1d2bd4br2");
        strcpy(GFont[50],"bu2nf2u3e2r3f2d3g2nl3e1h1nl1f2br2");
        strcpy(GFont[51],"u7r4f1d1g1l1nl3d1f1d1f1br2");
        strcpy(GFont[52],"bu1f1r3e1u1h1l1h1l1h1u1e1r3f1bd6br2");
        strcpy(GFont[53],"br2u7nl2r2bd7br2");
        strcpy(GFont[54],"bu1nu6f1r3e1nu6bd1br2");
        strcpy(GFont[55],"bu6nu1f1d1f1d1f1nd1e1u1e1u1e1nu1bd6br2");
        strcpy(GFont[56],"bu6nu1f1d3f1nd1e1u1e1u2e1f1d2f1d1f1nd1e1u3e1nu1bd6br2");
        strcpy(GFont[57],"e1u1e1u1h1u1h1br5g1d1g1d1f1d1f1br2");
        strcpy(GFont[58],"bu7f1d1f2nd3e2u1e1bd7br2");
        strcpy(GFont[59],"nr5e2u1e1u1e2nl5br2bd7");
        strcpy(GFont[60],"u7nr1d9nr1u2br3");
        strcpy(GFont[61],"br2u1h1u3h1u1br4bd7");
        strcpy(GFont[62],"br1u7nl1d9nl1u2br2");
        strcpy(GFont[63],"bu4e1u1e1f1d1f1bd4br2");
        strcpy(GFont[64],"bd2r4bu2br2");
        strcpy(GFont[65],"bu6br2nh1br2bd6");
        strcpy(GFont[66],"bu1u1e1h1e1r2f1d1nl3d1nd2g2nl1br4");
        strcpy(GFont[67],"nu7u1r1f1r1e1u3h1l1g1l1d4br6");
        strcpy(GFont[68],"bu1u3e1r2f1bd3g1nl2br3");
        strcpy(GFont[69],"bu1u3e1r1f1r1nu3d3nd1l1g1nl1br4");
        strcpy(GFont[70],"bu1u3e1r2f1d1nl4bd2g1nl2br3");
        strcpy(GFont[71],"br1u5nl1nr1u1e1br2bd7");
        strcpy(GFont[72],"bu1u3e1r1f2nu2d1nd3g2l1bl1bd2r3br3bu2");
        strcpy(GFont[73],"u3nu4e2r1f1d4br2");
        strcpy(GFont[74],"u5bu2nr0br2bd7");
        strcpy(GFont[75],"bd2e1u6bu2nr0br2bd7");
        strcpy(GFont[76],"u2nu5ne3br2d1f1br2");
        strcpy(GFont[77],"nu7br2");
        strcpy(GFont[78],"u5f1e1r1nd5r2f1d4br2");
        strcpy(GFont[79],"u5r3f1d4br2");
        strcpy(GFont[80],"bu1u3e1r2f1d3g1nl2br3");
        strcpy(GFont[81],"nd2nu5e1f1r1e1u3h1l1g1br5bd4");
        strcpy(GFont[82],"bu1u3e1r1f2nu2d1nd4g2nl1br4");
        strcpy(GFont[83],"u3nu2e2bd5br2");
        strcpy(GFont[84],"bu1f1r2e1h2l1h1e1r2f1bd4br2");
        strcpy(GFont[85],"nr1u5nl1nr1nu2br3bd5");
        strcpy(GFont[86],"bu1nu4f1r1e2nu3d2br2");
        strcpy(GFont[87],"bu4nu1f1d1f1nd1e1u1e1nu1br2bd4");
        strcpy(GFont[88],"bu4nu1f1d1f1nd1e1u2e1f1d2f1nd1e1u1e1nu1br2bd4");
        strcpy(GFont[89],"e2u1nh2ne2d1f2br2");
        strcpy(GFont[90],"bu4nu1f1d1f1nd2e1u1e1nu1bl2bd5ng1u1br4");
        strcpy(GFont[91],"nr4e2u1e2nl4br2bd5");
        strcpy(GFont[92],"br2bd2h1u3h1e1u2e1br2bd7");
        strcpy(GFont[93],"nd2nu7br2");
        strcpy(GFont[94],"bd2e1u3e1h1u2nh1br2bd6");
        strcpy(GFont[95],"bu3u1r2d1r1e1br2bd4");
    }
}



	

bool Graphic::GetBlock (unsigned int nX, unsigned int nY, unsigned int nBlkWidth, unsigned int nBlkHeight, Color* BlkColor)
{
#define XYRASTER_GETBLOCK  ((nCounty - nY) * nBlkWidth) + (nCountx - nX)

    
	if (BlkColor == NULL) return false;
	
	int nCountx, nCounty;
	int nDataLen;
	
	nDataLen = nBlkWidth * nBlkHeight;
	
	memset (BlkColor, 0, sizeof (Color) * nDataLen);

	for (nCounty = nY; nCounty < nY + nBlkHeight; nCounty++)
		for (nCountx = nX; nCountx <= nX + nBlkWidth; nCountx++)
		{
			if (nCountx >= nImageWidth || nCounty > nImageHeight)
				continue;
			
			BlkColor [XYRASTER_GETBLOCK] = GetPixel (nCountx, nCounty);
			
			
			NOTRACE ("Blocos: [%d] [%d] Array Location: [%d]\n", nCountx - nX, nCounty - nY, XYRASTER_GETBLOCK);
		}
	
	return true;
}




__inline Color Graphic::GetPixel (int nX, int nY)
{	
    Verify (ColorBuffer != NULL, "No Buffer initialized at this time", 1, GraphicException);
	
	Color tmpColor;

	tmpColor.nR = 0L;
	
	
    if (nX < 0 || nY < 0 || nX >= (nImageWidth) || nY >= (nImageHeight)) return tmpColor;
	
    return (Color) ColorBuffer [(size_t) ((nY * nImageWidth) + nX)];
}



__inline Color Graphic::GetAlphaPixel (int nX, int nY, Color stSourceColor, int nAlpha)
{	
	Color TmpColor, TargetColor;
	
	TargetColor = GetPixel (nX, nY);
	TargetColor.nAlpha = nAlpha;
    
    if (nAlpha == 255)
        return stSourceColor;
    
    nAlpha = 255 - nAlpha;
    
    TmpColor.nR = (uint8_t) ((uint32_t) (stSourceColor.nR * nAlpha / 0xFF) + (TargetColor.nR) * (0xFF - nAlpha) / 0xFF);
    TmpColor.nG = (uint8_t) ((uint32_t) (stSourceColor.nG * nAlpha / 0xFF) + (TargetColor.nG) * (0xFF - nAlpha) / 0xFF);
    TmpColor.nB = (uint8_t) ((uint32_t) (stSourceColor.nB * nAlpha / 0xFF) + (TargetColor.nB) * (0xFF - nAlpha) / 0xFF);
    
    /*
	szSourceRGB [0] = (int)( (szSourceRGB [0] * nAlpha / 0xFF) + (szTargerRGB [0]) * (0xff - nAlpha) / 0xFF);
	szSourceRGB [1] = (int)( (szSourceRGB [1] * nAlpha / 0xFF) + (szTargerRGB [1]) * (0xff - nAlpha) / 0xFF);
	szSourceRGB [2] = (int)( (szSourceRGB [2] * nAlpha / 0xFF) + (szTargerRGB [2]) * (0xff - nAlpha) / 0xFF);
	*/
    
	return TmpColor;
}



int Graphic::PutPixel (int nX, int nY, Color stColor)
{
    PutPixel (nX, nY, stColor.nAlpha, stColor);
	
    return true;
}




inline int Graphic::PSet (int nX, int nY, int nAlpha, Color stColor)
{
    if (nGlobalAngle > 0) 
	{ 
		if (1) 
		{
			nX = ABS (nX); 
			nY = ABS (nY);
		} 
		
	}

	NOTRACE ("Setting R:[%-2u] G:[%-2u] B:[%-2u] A:[%-2u]\n", stColor.nR, stColor.nG, stColor.nB, stColor.nAlpha);

	if (nX < 0 || nY < 0 || nX >= (nImageWidth) || nY >= (nImageHeight)) 
		return true;
	
    if (nAlpha > 0)
    {
        stColor = GetAlphaPixel (nX, nY, stColor, nAlpha);
    }
	
    ColorBuffer [(size_t) ((nY * nImageWidth) + nX)] = stColor;
	
	return true;
}




__inline int Graphic::PutPixel (int nX, int nY, int nAlpha, Color stColor) 
{
	if (ColorBuffer == NULL) return -1;
		
	if (nGlobalAngle > 0)
	{
		return PSet ((nX * Cos (nGlobalAngle * 2)) - (nY * Sin (nGlobalAngle * 2) + nGlobalX), (nY * Cos (nGlobalAngle * 2)) + (nX * Sin (nGlobalAngle * 2)) - nGlobalY, nAlpha, stColor);
	}
	else
	{
		return PSet (nX, nY, nAlpha, stColor);
	}
}





void Graphic::SetAngle(int nX, int nY, int nAngle) 
{ 
    nGlobalX = nX; 
    nGlobalY = nY; 
    nGlobalAngle = (nAngle + 540) / 2; 
}


int Graphic::DrawRadiosLine (int nX, int nY,  int nLength, double nAngle, int nAlpha, Color stColor)
{
    int nCount;
    
    for (nCount=0; nCount < nLength; nCount++)
    {
        PutPixelRadios (nX, nY, nCount, nAngle, nAlpha, stColor); 
    }
    return true;
}



int Graphic::PutPixelRadios (int nX, int nY, int nLength, double nAngle, int nAlpha, Color stColor)
{
    int nX1, nY1;
    
    nX1 = nX + (nLength * sin (nAngle));
    nY1 = nY - ((nLength) * cos (nAngle));
    
    PutPixel (nX1, nY1, nAlpha, stColor);
    
	
    return true;
}



int Graphic::IDrawLine (int x0, int y0, int x1, int y1, int nAlpha, Color stColor)
{        
    int dx = x1 - x0;        
    int dy = y1 - y0;        
    
    PutPixel (x0, y0, nAlpha, stColor);
    
    if (dx != 0) 
    {            
        float m = (float) dy / (float) dx;
        float b = y0 - m * x0;
        dx = (x1 > x0) ? 1 : -1;            
        while (x0 != x1)
        { 
            x0 += dx;
            y0 = (int) (m*x0 + b);
            PutPixel (x0, y0, nAlpha, stColor);
        }
    }  
	
	return true;
}




int Graphic::DrawLine (int x1, int y1, int x2, int y2, int nAlpha, Color stColor)
{
    int myx, myy;
    double slope;
    double myslope;
    double curx, cury;
    int done = 0;
    int temp;
    double draw_count = 0.0;
    double on_off_size = 0.0;
    
    /* x2 should always be greater than x1 */
    if ( x2 < x1 ) {
        temp = x2;
        x2 = x1;
        x1 = temp;
        temp = y2;
        y2 = y1;
        y1 = temp;
    }
    
    /* remember, our coordinate system is reversed for y */
    if ( x1 == x2 ) {
        if ( y2 < y1 ) {
            temp = x2;
            x2 = x1;
            x1 = temp;
            temp = y2;
            y2 = y1;
            y1 = temp;
        }
    }
    else
        slope = - ( (double) y2 - (double) y1 ) / ( (double) x1 - (double) x2 );
    curx = (double) x1;
    cury = (double) y1;
    
    PutPixel ( x1, y1, nAlpha, stColor );
    
    /* handle dashes */
    if ( false ) {
        if ( x1 == x2 || ABS ( slope ) < 0.1 || ABS  ( slope ) > 10 ) {
            on_off_size = ON_OFF_PIXELS;
        }
        else {
            myslope = ABS ( slope );
            if ( myslope > 1.0 )
                myslope = 1.0 / myslope;
            /* myslope now between 0 and 1.0 */
            on_off_size = ON_OFF_PIXELS + ( 0.41 * myslope );
        }
    }
    
    while ( ! done ) {
        if ( x1 == x2 ) {
            if ( cury >= (double)y2 )
                done = 1;
            else
                cury += 1.0;
        }
        else if ( slope >= 1.0 ) {
            if ( cury >= (double)y2 )
                done = 1;
            else {
                cury += 1.0;
                curx += ( 1.0 / slope );
            }
        }
        else if ( slope < -1.0 ) {
            if ( cury <= (double)y2 )
                done = 1;
            else {
                cury -= 1.0;
                curx -= ( 1.0 / slope );
            }
        }
        else if ( slope >= 0.0 ) {
            if ( curx >= (double)x2 )
                done = 1;
            else {
                curx += 1.0;
                cury += slope;
            }
        }
        else if ( slope < 0.0 ) {
            if ( curx >= (double)x2 )
                done = 1;
            else {
                curx += 1.0;
                cury += slope;
            }
        }
        
        if ( false ) {
            draw_count += 1.0;
            if ( ( (int)( floor ( draw_count / on_off_size ) ) % 2 ) == 1 )
                continue;
        }
        
        if ( ! done ) {
            myx = (int) curx;
            myy = (int) cury;
            switch (LineWidth) {
                default:
                case 0:
                case 1:
                    PutPixel (myx, myy, nAlpha, stColor);
                    break;
                case 2:
                    PutPixel (myx, myy, nAlpha, stColor);
                    PutPixel (myx - 1, myy, nAlpha, stColor);
                    PutPixel ( myx - 1, myy - 1, nAlpha, stColor);
                    PutPixel ( myx, myy - 1, nAlpha, stColor);
                    break;
                case 3:
                    PutPixel (myx, myy, nAlpha, stColor);
                    PutPixel (myx - 1, myy, nAlpha, stColor);
                    PutPixel (myx + 1, myy - 1, nAlpha, stColor);
                    PutPixel (myx, myy - 1, nAlpha, stColor);
                    PutPixel (myx, myy + 1, nAlpha, stColor);
                    break;
            }
        }
    }
    
    return true;
}



int Graphic::DrawArc (int x, int y, int r1, int r2, int a1, int a2, int nAlpha, Color stColor)
{
    int myx, myy, lastx, lasty, N, loop;
    double a, da;
    
    /* because our y is upside down, make all angles their negative */
    a1 = 360 - a1;
    a2 = 360 - a2;
    
    N = (int) fabs ( a2 - a1 ) + 8;
    a = a1 * 2.0 * PI / 360.0;
    da = ( a2 - a1 ) * ( 2.0 * PI / 360.0 ) / ( N - 1 );
    for ( loop = 0; loop < N ; loop++ ) {
        myx = x + (int)( r1 * cos ( a + loop * da ) );
        myy = y + (int)( r2 * sin ( a + loop * da ) );
        if ( loop )
            DrawLine (lastx, lasty, myx, myy, nAlpha, stColor);
        lastx = myx;
        lasty = myy;
    }
    
    return true;
}



int Graphic::DrawCircle (int nX, int nY, int nLenght, int nIsFill, int nAlpha, Color stColor)
{
    double nCount;        
    int    nLWidth = LineWidth;
    
    if (!nIsFill)
    {  
        DrawArc (nX, nY, nLenght, nLenght, 0.0, 360.0, nAlpha, stColor);
    }
    else
    {
        LineWidth = 2;
        
        for (nCount=0; nCount < (nLenght - 1); nCount++)
        {
            DrawCircle (nX, nY, (int) nCount, false, nAlpha, stColor);
        }
    }
    
    LineWidth = nLWidth;
    
    return true;
}



void Graphic::BasicDrawWidth (int* x, int * y, const char *dr)
{
    int f1=0,f2=0;
    int n,i;
    char c,nc[10]="";
    int ncCount;
    int nInfo;
    
    n=strlen(dr);
    
    nInfo = 0;
    
    for(i=0;i<=n;i++)
    {
        c=toupper(dr[i]);
        /* Controle de Funcao */
        
        if(f2 && !isdigit(c) || i==n)
        {            
            if(f1==0)
            {
                if(f2==1) {*y -= atoi(nc);}
                else if(f2==2) {*y += atoi(nc);}
                else if(f2==3) {*x -= atoi(nc);}
                else if(f2==4) {*x += atoi(nc);}
                else if(f2==5) {*x += atoi(nc);  *y -= atoi(nc);}
                else if(f2==6) {*x += atoi(nc);  *y += atoi(nc);}
                else if(f2==7) {*x -= atoi(nc);  *y += atoi(nc);}
                else if(f2==8) {*x -= atoi(nc);  *y -= atoi(nc);}
            }
            else if(f1==1)
            {
                if(f2==1) {  *y -= atoi(nc);}
                else if(f2==2) {  *y += atoi(nc);}
                else if(f2==3) {  *x -= atoi(nc);}
                else if(f2==4) {  *x += atoi(nc);}
                else if(f2==5) {  *x += atoi(nc);  *y -= atoi(nc);}
                else if(f2==6) {  *x += atoi(nc);  *y += atoi(nc);}
                else if(f2==7) {  *x -= atoi(nc);  *y += atoi(nc);}
                else if(f2==8) {  *x -= atoi(nc);  *y -= atoi(nc);}
            }
			
            f1=0; f2=0;
            nc [0] = '\0';
            ncCount = 0;
        }
        else if(isdigit(c))
        {
            nc [ncCount++] = c;
            nc [ncCount] = '\0';
        }
        else
        {
            ncCount = 0;
            nc [ncCount] ='\0';
        }
        
        /* Controle de acesso */
        if (c=='B') f1=1;
        else if (c=='N') f1=2;
        else if (c=='U') f2=1;
        else if (c=='D') f2=2;
        else if (c=='L') f2=3;
        else if (c=='R') f2=4;
        else if (c=='E') f2=5;
        else if (c=='F') f2=6;
        else if (c=='G') f2=7;
        else if (c=='H') f2=8;
    }
}



void Graphic::GPrint (int nX, int nY, int nAlpha, Color stColor, const char* pszText)
{
    int nXv = nX, nYv = nY;
    
    GPrint (&nXv, &nYv, nAlpha, stColor, pszText);
}


void Graphic::GPrint (int* nX, int* nY, int nAlpha, Color stColor, const char* pszText)
{
    char ch;
    int  nTX;
    
    nTX = *nX;
    while ((ch = (char) pszText [0]) != '\0' && pszText++)
    {
        if (ch == '\n')
        {
            *nY += 12;
            *nX  = nTX;
        }
        else
        {
            BasicDraw (nX, nY, nAlpha, stColor, GFont [ch - (' ' - 1)]);
        }
    }
}



void Graphic::GPrintWidth (int* nX, int* nY, char* pszText)
{
    char *ch;
    int  nTX;
    
    *nX = *nY = 0;
    
    nTX = *nX;
    while (*(ch = pszText++) != '\0')
    {
        if (*ch == '\n')
        {
            *nY += 12;
            *nX  = nTX;
        }
        else
        {
            BasicDrawWidth (nX, nY, GFont [*ch - (' ' - 1)]);
        }
    }
}


void Graphic::BasicDraw (int* x, int * y, int nAlpha, Color stColor, const char *dr)
{
    int f1=0,f2=0;
    int n,i;
    char c,nc[10]="";
    int ncCount;
    int nInfo;
    
    n=strlen(dr);
    
    nInfo = 0;
    
    for(i=0;i<=n;i++)
    {
        c=toupper(dr[i]);
        /* Controle de Funcao */
        
        if(f2 && !isdigit(c) || i==n)
        {            
            if(f1==0)
            {
                if(f2==1) { DrawLine (*x , *y , *x , *y-atoi(nc), nAlpha, stColor);  *y -= atoi(nc);}
                else if(f2==2) { DrawLine (*x, *y, *x, *y+atoi(nc), nAlpha, stColor);  *y += atoi(nc);}
                else if(f2==3) { DrawLine (*x, *y, *x-atoi(nc), *y, nAlpha, stColor);  *x -= atoi(nc);}
                else if(f2==4) { DrawLine (*x, *y, *x+atoi(nc), *y, nAlpha, stColor);  *x += atoi(nc);}
                else if(f2==5) { DrawLine (*x, *y, *x+atoi(nc), *y-atoi(nc), nAlpha, stColor);  *x += atoi(nc);  *y -= atoi(nc);}
                else if(f2==6) { DrawLine (*x, *y, *x+atoi(nc), *y+atoi(nc), nAlpha, stColor);  *x += atoi(nc);  *y += atoi(nc);}
                else if(f2==7) { DrawLine (*x, *y, *x-atoi(nc), *y+atoi(nc), nAlpha, stColor);  *x -= atoi(nc);  *y += atoi(nc);}
                else if(f2==8) { DrawLine (*x, *y, *x-atoi(nc), *y-atoi(nc), nAlpha, stColor);  *x -= atoi(nc);  *y -= atoi(nc);}
            }
            else if(f1==1)
            {
                if(f2==1) {  *y -= atoi(nc);}
                else if(f2==2) {  *y += atoi(nc);}
                else if(f2==3) {  *x -= atoi(nc);}
                else if(f2==4) {  *x += atoi(nc);}
                else if(f2==5) {  *x += atoi(nc);  *y -= atoi(nc);}
                else if(f2==6) {  *x += atoi(nc);  *y += atoi(nc);}
                else if(f2==7) {  *x -= atoi(nc);  *y += atoi(nc);}
                else if(f2==8) {  *x -= atoi(nc);  *y -= atoi(nc);}
            }
            else if(f1==2)
            {
                if(f2==1) { DrawLine (*x, *y, *x, *y-atoi(nc), nAlpha, stColor); }
                else if(f2==2) { DrawLine (*x, *y, *x, *y+atoi(nc), nAlpha, stColor); }
                else if(f2==3) { DrawLine ( *x, *y, *x-atoi(nc), *y, nAlpha, stColor); }
                else if(f2==4) { DrawLine (*x, *y, *x+atoi(nc), *y, nAlpha, stColor); }
                else if(f2==5) { DrawLine ( *x, *y, *x+atoi(nc), *y-atoi(nc), nAlpha, stColor); }
                else if(f2==6) { DrawLine (*x, *y, *x+atoi(nc), *y+atoi(nc), nAlpha, stColor); }
                else if(f2==7) { DrawLine (*x, *y, *x-atoi(nc), *y+atoi(nc), nAlpha, stColor); }
                else if(f2==8) { DrawLine (*x, *y, *x-atoi(nc), *y-atoi(nc), nAlpha, stColor); }
            }
            f1=0; f2=0;
            nc [0] = '\0';
            ncCount = 0;
        }
        else if(isdigit(c))
        {
            nc [ncCount++] = c;
            nc [ncCount] = '\0';
        }
        else
        {
            ncCount = 0;
            nc [ncCount] ='\0';
        }
        
        /* Controle de acesso */
        if (c=='B') f1=1;
        else if (c=='N') f1=2;
        else if (c=='U') f2=1;
        else if (c=='D') f2=2;
        else if (c=='L') f2=3;
        else if (c=='R') f2=4;
        else if (c=='E') f2=5;
        else if (c=='F') f2=6;
        else if (c=='G') f2=7;
        else if (c=='H') f2=8;
    }
}


inline char* Graphic::Tolower (char* pszText)
{
	while ((unsigned char) *pszText != '\0') { pszText [0] = tolower (pszText [0]); pszText++;}
	
	return pszText;
}




int Graphic::DrawXPM (int nPX, int nPY, int nLX, int nLY, int nWidth, int nHeight, int nAlpha, const char* ppszData[])
{
	uint32_t nColors;
    int      nCols;
    int      nX;
    int      nY;
    
    int      nLWidth;
    int      nLHeight;
    uint32_t nColorValue;
	    
    struct Cor
    {
        char   XColorData [5];
		Color     stColor;
    };

    
    int     nCount;
    int     nCountX;
	char    chDummy;
    
	//int     nTpColor;
	
    char szRGBHexa [26];
	
	
    VERIFY (ppszData != NULL, "O Fluxo de dados para imagem fornecido esta nulo.", false);
    
    sscanf (ppszData [0], "%d %d %d %d", &nLWidth, &nLHeight, &nColors, &nCols);
	
	if (nLX == 0 && nLY == 0 && nWidth == 0 && nHeight == 0)
	{
		nLX = 1; nLY = 1;
		nWidth = nLWidth; nHeight = nLHeight;
		
		NOTRACE ("Setting full XPM Image nLX: [%u] nLY: [%u] nWidth: [%u] nHeight: [%u]\n", nLX, nLY, nWidth, nHeight);
	}
	
	VERIFY (nColors < 0xFFFF, "Numero de cores não suportado, máximo de %u", 0xFFFF - 1);
	
	/* Create Cores array into local heap*/
	struct Cor Cores [nColors];
	
 	VERIFY (nCols < sizeof (Cores [0].XColorData), "Numero de cores incompat�vel");
	
	
    for (nCount=0; nCount < nColors; nCount++)
    {		
		strncpy (Cores [nCount].XColorData, ppszData [nCount + 1], nCols);  
        sscanf (ppszData [nCount + 1] + nCols + 1, "%c %s", &chDummy, szRGBHexa);
		
        if (szRGBHexa [0] != '#')
        {   
			Tolower (szRGBHexa);
			
			if (strcmp (szRGBHexa, "none") == 0)
			{
                Cores [nCount].stColor = MkColor (0, 0, 0, 255);
			}
			else
			{
				
				Cores [nCount].stColor = GetColorByName (szRGBHexa);
                Cores [nCount].stColor.nAlpha = nAlpha;
			}
        }
        else 
        {
			nColorValue = strtol (&szRGBHexa [1], NULL, 16);
			
			Cores [nCount].stColor.nR = (nColorValue & 0xff0000L) >> 16;
			Cores [nCount].stColor.nG = (nColorValue & 0x00ff00L) >> 8;
			Cores [nCount].stColor.nB = (nColorValue & 0x0000ffL);
			Cores [nCount].stColor.nAlpha = nAlpha;
            
        }
		
		 NOTRACE ("ADD [%s] : R:[%-2u] G:[%-2u] B:[%-2u]\n", Cores [nCount].XColorData, Cores [nCount].stColor.nR, Cores [nCount].stColor.nG, Cores [nCount].stColor.nB);
    }

	 NOTRACE ("COR: %d - R: %-2u G: %-2u B: %-2u \n",  nCount, Cores [nCount].stColor.nR, Cores [nCount].stColor.nG, Cores [nCount].stColor.nB);                            

	
    for (nCount = nColors + 1 + nLY; nCount < (nLHeight + nColors + 1); nCount++)
    {
        const char* pszLine;
        char  szData [5];
        int   nData=0;
        int   nCounter = 0;
        int   nInfo = 1;
        
        pszLine = ppszData [nCount];
        
        for (nCountX = ((nCols * nLX) - 1); pszLine [nCountX] != '\0' && nCountX <= (nCols * (nLX + nWidth)); nCountX++)
        {
            nData = (nCols - 1) - ((nCountX+1) % nCols);
            
            szData [nData] = pszLine [nCountX];
            szData [nData + 1] = '\0';
            
            for (nCounter=0; nCounter < nColors && nInfo; nCounter++)
            {
                if (Cores [nCounter].XColorData [0] == szData [0] && strncmp (szData, (const char*) Cores [nCounter].XColorData, nCols) == 0)
                {
                    nX = nCountX / nCols; 
                    nY = nCount - (nColors + 1);
                    
                    if (Cores [nCounter].stColor.nAlpha < 255 
						&& nX >= nLX && nX <= nLX + nWidth 
						&& nY >= nLY && nY <= nLY + nHeight)
                    {
                        NOTRACE ("XPM: (%d) X:%u Y:%u C:[%s]\n", nCounter, nX, nY, Cores [nCounter].XColorData);
						
                        PutPixel (nPX - nLX + nX, nPY - nLY + nY, Cores [nCounter].stColor);                                                        
                    }
                    
                    if (nY > nLY + nHeight)
                        nInfo = 0;
                    
                    NOTRACE (".");
                    break;
                }
            }
        }
    }
	
    return true;
}




Color Graphic::GetColorByName (char* pszColor)
{
	int nCount = 0;
	Color stColor;
	
	static struct 
	{
		const char* pszName;
		const char* pszValue;
	} XPM_Color [] = {
		{"10.41", "#000000"}, 
		{"snow", "#FFFAFA"}, 
		{"ghost", "#F8F8FF"}, 
		{"ghostwhite", "#F8F8FF"}, 
		{"white", "#F5F5F5"}, 
		{"whitesmoke", "#F5F5F5"}, 
		{"gainsboro", "#DCDCDC"}, 
		{"floral", "#FFFAF0"}, 
		{"floralwhite", "#FFFAF0"}, 
		{"old", "#FDF5E6"}, 
		{"oldlace", "#FDF5E6"}, 
		{"linen", "#FAF0E6"}, 
		{"antique", "#FAEBD7"}, 
		{"antiquewhite", "#FAEBD7"}, 
		{"papaya", "#FFEFD5"}, 
		{"papayawhip", "#FFEFD5"}, 
		{"blanched", "#FFEBCD"}, 
		{"blanchedalmond", "#FFEBCD"}, 
		{"bisque", "#FFE4C4"}, 
		{"peach", "#FFDAB9"}, 
		{"peachpuff", "#FFDAB9"}, 
		{"navajo", "#FFDEAD"}, 
		{"navajowhite", "#FFDEAD"}, 
		{"moccasin", "#FFE4B5"}, 
		{"cornsilk", "#FFF8DC"}, 
		{"ivory", "#FFFFF0"}, 
		{"lemon", "#FFFACD"}, 
		{"lemonchiffon", "#FFFACD"}, 
		{"seashell", "#FFF5EE"}, 
		{"honeydew", "#F0FFF0"}, 
		{"mint", "#F5FFFA"}, 
		{"mintcream", "#F5FFFA"}, 
		{"azure", "#F0FFFF"}, 
		{"alice", "#F0F8FF"}, 
		{"aliceblue", "#F0F8FF"}, 
		{"lavender", "#E6E6FA"}, 
		{"lavender", "#FFF0F5"}, 
		{"lavenderblush", "#FFF0F5"}, 
		{"misty", "#FFE4E1"}, 
		{"mistyrose", "#FFE4E1"}, 
		{"white", "#FFFFFF"}, 
		{"black", "#000000"}, 
		{"dark", "#2F4F4F"}, 
		{"darkslategray", "#2F4F4F"}, 
		{"dark", "#2F4F4F"}, 
		{"darkslategrey", "#2F4F4F"}, 
		{"dim", "#696969"}, 
		{"dimgray", "#696969"}, 
		{"dim", "#696969"}, 
		{"dimgrey", "#696969"}, 
		{"slate", "#708090"}, 
		{"slategray", "#708090"}, 
		{"slate", "#708090"}, 
		{"slategrey", "#708090"}, 
		{"light", "#778899"}, 
		{"lightslategray", "#778899"}, 
		{"light", "#778899"}, 
		{"lightslategrey", "#778899"}, 
		{"gray", "#BEBEBE"}, 
		{"grey", "#BEBEBE"}, 
		{"light", "#D3D3D3"}, 
		{"lightgrey", "#D3D3D3"}, 
		{"light", "#D3D3D3"}, 
		{"lightgray", "#D3D3D3"}, 
		{"midnight", "#191970"}, 
		{"midnightblue", "#191970"}, 
		{"navy", "#000080"}, 
		{"navy", "#000080"}, 
		{"navyblue", "#000080"}, 
		{"cornflower", "#6495ED"}, 
		{"cornflowerblue", "#6495ED"}, 
		{"dark", "#483D8B"}, 
		{"darkslateblue", "#483D8B"}, 
		{"slate", "#6A5ACD"}, 
		{"slateblue", "#6A5ACD"}, 
		{"medium", "#7B68EE"}, 
		{"mediumslateblue", "#7B68EE"}, 
		{"light", "#8470FF"}, 
		{"lightslateblue", "#8470FF"}, 
		{"medium", "#0000CD"}, 
		{"mediumblue", "#0000CD"}, 
		{"royal", "#4169E1"}, 
		{"royalblue", "#4169E1"}, 
		{"blue", "#0000FF"}, 
		{"dodger", "#1E90FF"}, 
		{"dodgerblue", "#1E90FF"}, 
		{"deep", "#00BFFF"}, 
		{"deepskyblue", "#00BFFF"}, 
		{"sky", "#87CEEB"}, 
		{"skyblue", "#87CEEB"}, 
		{"light", "#87CEFA"}, 
		{"lightskyblue", "#87CEFA"}, 
		{"steel", "#4682B4"}, 
		{"steelblue", "#4682B4"}, 
		{"light", "#B0C4DE"}, 
		{"lightsteelblue", "#B0C4DE"}, 
		{"light", "#ADD8E6"}, 
		{"lightblue", "#ADD8E6"}, 
		{"powder", "#B0E0E6"}, 
		{"powderblue", "#B0E0E6"}, 
		{"pale", "#AFEEEE"}, 
		{"paleturquoise", "#AFEEEE"}, 
		{"dark", "#00CED1"}, 
		{"darkturquoise", "#00CED1"}, 
		{"medium", "#48D1CC"}, 
		{"mediumturquoise", "#48D1CC"}, 
		{"turquoise", "#40E0D0"}, 
		{"cyan", "#00FFFF"}, 
		{"light", "#E0FFFF"}, 
		{"lightcyan", "#E0FFFF"}, 
		{"cadet", "#5F9EA0"}, 
		{"cadetblue", "#5F9EA0"}, 
		{"medium", "#66CDAA"}, 
		{"mediumaquamarine", "#66CDAA"}, 
		{"aquamarine", "#7FFFD4"}, 
		{"dark", "#006400"}, 
		{"darkgreen", "#006400"}, 
		{"dark", "#556B2F"}, 
		{"darkolivegreen", "#556B2F"}, 
		{"dark", "#8FBC8F"}, 
		{"darkseagreen", "#8FBC8F"}, 
		{"sea", "#2E8B57"}, 
		{"seagreen", "#2E8B57"}, 
		{"medium", "#3CB371"}, 
		{"mediumseagreen", "#3CB371"}, 
		{"light", "#20B2AA"}, 
		{"lightseagreen", "#20B2AA"}, 
		{"pale", "#98FB98"}, 
		{"palegreen", "#98FB98"}, 
		{"spring", "#00FF7F"}, 
		{"springgreen", "#00FF7F"}, 
		{"lawn", "#7CFC00"}, 
		{"lawngreen", "#7CFC00"}, 
		{"green", "#00FF00"}, 
		{"chartreuse", "#7FFF00"}, 
		{"medium", "#00FA9A"}, 
		{"mediumspringgreen", "#00FA9A"}, 
		{"green", "#ADFF2F"}, 
		{"greenyellow", "#ADFF2F"}, 
		{"lime", "#32CD32"}, 
		{"limegreen", "#32CD32"}, 
		{"yellow", "#9ACD32"}, 
		{"yellowgreen", "#9ACD32"}, 
		{"forest", "#228B22"}, 
		{"forestgreen", "#228B22"}, 
		{"olive", "#6B8E23"}, 
		{"olivedrab", "#6B8E23"}, 
		{"dark", "#BDB76B"}, 
		{"darkkhaki", "#BDB76B"}, 
		{"khaki", "#F0E68C"}, 
		{"pale", "#EEE8AA"}, 
		{"palegoldenrod", "#EEE8AA"}, 
		{"light", "#FAFAD2"}, 
		{"lightgoldenrodyellow", "#FAFAD2"}, 
		{"light", "#FFFFE0"}, 
		{"lightyellow", "#FFFFE0"}, 
		{"yellow", "#FFFF00"}, 
		{"gold", "#FFD700"}, 
		{"light", "#EEDD82"}, 
		{"lightgoldenrod", "#EEDD82"}, 
		{"goldenrod", "#DAA520"}, 
		{"dark", "#B8860B"}, 
		{"darkgoldenrod", "#B8860B"}, 
		{"rosy", "#BC8F8F"}, 
		{"rosybrown", "#BC8F8F"}, 
		{"indian", "#CD5C5C"}, 
		{"indianred", "#CD5C5C"}, 
		{"saddle", "#8B4513"}, 
		{"saddlebrown", "#8B4513"}, 
		{"sienna", "#A0522D"}, 
		{"peru", "#CD853F"}, 
		{"burlywood", "#DEB887"}, 
		{"beige", "#F5F5DC"}, 
		{"wheat", "#F5DEB3"}, 
		{"sandy", "#F4A460"}, 
		{"sandybrown", "#F4A460"}, 
		{"tan", "#D2B48C"}, 
		{"chocolate", "#D2691E"}, 
		{"firebrick", "#B22222"}, 
		{"brown", "#A52A2A"}, 
		{"dark", "#E9967A"}, 
		{"darksalmon", "#E9967A"}, 
		{"salmon", "#FA8072"}, 
		{"light", "#FFA07A"}, 
		{"lightsalmon", "#FFA07A"}, 
		{"orange", "#FFA500"}, 
		{"dark", "#FF8C00"}, 
		{"darkorange", "#FF8C00"}, 
		{"coral", "#FF7F50"}, 
		{"light", "#F08080"}, 
		{"lightcoral", "#F08080"}, 
		{"tomato", "#FF6347"}, 
		{"orange", "#FF4500"}, 
		{"orangered", "#FF4500"}, 
		{"red", "#FF0000"}, 
		{"hot", "#FF69B4"}, 
		{"hotpink", "#FF69B4"}, 
		{"deep", "#FF1493"}, 
		{"deeppink", "#FF1493"}, 
		{"pink", "#FFC0CB"}, 
		{"light", "#FFB6C1"}, 
		{"lightpink", "#FFB6C1"}, 
		{"pale", "#DB7093"}, 
		{"palevioletred", "#DB7093"}, 
		{"maroon", "#B03060"}, 
		{"medium", "#C71585"}, 
		{"mediumvioletred", "#C71585"}, 
		{"violet", "#D02090"}, 
		{"violetred", "#D02090"}, 
		{"magenta", "#FF00FF"}, 
		{"violet", "#EE82EE"}, 
		{"plum", "#DDA0DD"}, 
		{"orchid", "#DA70D6"}, 
		{"medium", "#BA55D3"}, 
		{"mediumorchid", "#BA55D3"}, 
		{"dark", "#9932CC"}, 
		{"darkorchid", "#9932CC"}, 
		{"dark", "#9400D3"}, 
		{"darkviolet", "#9400D3"}, 
		{"blue", "#8A2BE2"}, 
		{"blueviolet", "#8A2BE2"}, 
		{"purple", "#A020F0"}, 
		{"medium", "#9370DB"}, 
		{"mediumpurple", "#9370DB"}, 
		{"thistle", "#D8BFD8"}, 
		{"snow1", "#FFFAFA"}, 
		{"snow2", "#EEE9E9"}, 
		{"snow3", "#CDC9C9"}, 
		{"snow4", "#8B8989"}, 
		{"seashell1", "#FFF5EE"}, 
		{"seashell2", "#EEE5DE"}, 
		{"seashell3", "#CDC5BF"}, 
		{"seashell4", "#8B8682"}, 
		{"antiquewhite1", "#FFEFDB"}, 
		{"antiquewhite2", "#EEDFCC"}, 
		{"antiquewhite3", "#CDC0B0"}, 
		{"antiquewhite4", "#8B8378"}, 
		{"bisque1", "#FFE4C4"}, 
		{"bisque2", "#EED5B7"}, 
		{"bisque3", "#CDB79E"}, 
		{"bisque4", "#8B7D6B"}, 
		{"peachpuff1", "#FFDAB9"}, 
		{"peachpuff2", "#EECBAD"}, 
		{"peachpuff3", "#CDAF95"}, 
		{"peachpuff4", "#8B7765"}, 
		{"navajowhite1", "#FFDEAD"}, 
		{"navajowhite2", "#EECFA1"}, 
		{"navajowhite3", "#CDB38B"}, 
		{"navajowhite4", "#8B795E"}, 
		{"lemonchiffon1", "#FFFACD"}, 
		{"lemonchiffon2", "#EEE9BF"}, 
		{"lemonchiffon3", "#CDC9A5"}, 
		{"lemonchiffon4", "#8B8970"}, 
		{"cornsilk1", "#FFF8DC"}, 
		{"cornsilk2", "#EEE8CD"}, 
		{"cornsilk3", "#CDC8B1"}, 
		{"cornsilk4", "#8B8878"}, 
		{"ivory1", "#FFFFF0"}, 
		{"ivory2", "#EEEEE0"}, 
		{"ivory3", "#CDCDC1"}, 
		{"ivory4", "#8B8B83"}, 
		{"honeydew1", "#F0FFF0"}, 
		{"honeydew2", "#E0EEE0"}, 
		{"honeydew3", "#C1CDC1"}, 
		{"honeydew4", "#838B83"}, 
		{"lavenderblush1", "#FFF0F5"}, 
		{"lavenderblush2", "#EEE0E5"}, 
		{"lavenderblush3", "#CDC1C5"}, 
		{"lavenderblush4", "#8B8386"}, 
		{"mistyrose1", "#FFE4E1"}, 
		{"mistyrose2", "#EED5D2"}, 
		{"mistyrose3", "#CDB7B5"}, 
		{"mistyrose4", "#8B7D7B"}, 
		{"azure1", "#F0FFFF"}, 
		{"azure2", "#E0EEEE"}, 
		{"azure3", "#C1CDCD"}, 
		{"azure4", "#838B8B"}, 
		{"slateblue1", "#836FFF"}, 
		{"slateblue2", "#7A67EE"}, 
		{"slateblue3", "#6959CD"}, 
		{"slateblue4", "#473C8B"}, 
		{"royalblue1", "#4876FF"}, 
		{"royalblue2", "#436EEE"}, 
		{"royalblue3", "#3A5FCD"}, 
		{"royalblue4", "#27408B"}, 
		{"blue1", "#0000FF"}, 
		{"blue2", "#0000EE"}, 
		{"blue3", "#0000CD"}, 
		{"blue4", "#00008B"}, 
		{"dodgerblue1", "#1E90FF"}, 
		{"dodgerblue2", "#1C86EE"}, 
		{"dodgerblue3", "#1874CD"}, 
		{"dodgerblue4", "#104E8B"}, 
		{"steelblue1", "#63B8FF"}, 
		{"steelblue2", "#5CACEE"}, 
		{"steelblue3", "#4F94CD"}, 
		{"steelblue4", "#36648B"}, 
		{"deepskyblue1", "#00BFFF"}, 
		{"deepskyblue2", "#00B2EE"}, 
		{"deepskyblue3", "#009ACD"}, 
		{"deepskyblue4", "#00688B"}, 
		{"skyblue1", "#87CEFF"}, 
		{"skyblue2", "#7EC0EE"}, 
		{"skyblue3", "#6CA6CD"}, 
		{"skyblue4", "#4A708B"}, 
		{"lightskyblue1", "#B0E2FF"}, 
		{"lightskyblue2", "#A4D3EE"}, 
		{"lightskyblue3", "#8DB6CD"}, 
		{"lightskyblue4", "#607B8B"}, 
		{"slategray1", "#C6E2FF"}, 
		{"slategray2", "#B9D3EE"}, 
		{"slategray3", "#9FB6CD"}, 
		{"slategray4", "#6C7B8B"}, 
		{"lightsteelblue1", "#CAE1FF"}, 
		{"lightsteelblue2", "#BCD2EE"}, 
		{"lightsteelblue3", "#A2B5CD"}, 
		{"lightsteelblue4", "#6E7B8B"}, 
		{"lightblue1", "#BFEFFF"}, 
		{"lightblue2", "#B2DFEE"}, 
		{"lightblue3", "#9AC0CD"}, 
		{"lightblue4", "#68838B"}, 
		{"lightcyan1", "#E0FFFF"}, 
		{"lightcyan2", "#D1EEEE"}, 
		{"lightcyan3", "#B4CDCD"}, 
		{"lightcyan4", "#7A8B8B"}, 
		{"paleturquoise1", "#BBFFFF"}, 
		{"paleturquoise2", "#AEEEEE"}, 
		{"paleturquoise3", "#96CDCD"}, 
		{"paleturquoise4", "#668B8B"}, 
		{"cadetblue1", "#98F5FF"}, 
		{"cadetblue2", "#8EE5EE"}, 
		{"cadetblue3", "#7AC5CD"}, 
		{"cadetblue4", "#53868B"}, 
		{"turquoise1", "#00F5FF"}, 
		{"turquoise2", "#00E5EE"}, 
		{"turquoise3", "#00C5CD"}, 
		{"turquoise4", "#00868B"}, 
		{"cyan1", "#00FFFF"}, 
		{"cyan2", "#00EEEE"}, 
		{"cyan3", "#00CDCD"}, 
		{"cyan4", "#008B8B"}, 
		{"darkslategray1", "#97FFFF"}, 
		{"darkslategray2", "#8DEEEE"}, 
		{"darkslategray3", "#79CDCD"}, 
		{"darkslategray4", "#528B8B"}, 
		{"aquamarine1", "#7FFFD4"}, 
		{"aquamarine2", "#76EEC6"}, 
		{"aquamarine3", "#66CDAA"}, 
		{"aquamarine4", "#458B74"}, 
		{"darkseagreen1", "#C1FFC1"}, 
		{"darkseagreen2", "#B4EEB4"}, 
		{"darkseagreen3", "#9BCD9B"}, 
		{"darkseagreen4", "#698B69"}, 
		{"seagreen1", "#54FF9F"}, 
		{"seagreen2", "#4EEE94"}, 
		{"seagreen3", "#43CD80"}, 
		{"seagreen4", "#2E8B57"}, 
		{"palegreen1", "#9AFF9A"}, 
		{"palegreen2", "#90EE90"}, 
		{"palegreen3", "#7CCD7C"}, 
		{"palegreen4", "#548B54"}, 
		{"springgreen1", "#00FF7F"}, 
		{"springgreen2", "#00EE76"}, 
		{"springgreen3", "#00CD66"}, 
		{"springgreen4", "#008B45"}, 
		{"green1", "#00FF00"}, 
		{"green2", "#00EE00"}, 
		{"green3", "#00CD00"}, 
		{"green4", "#008B00"}, 
		{"chartreuse1", "#7FFF00"}, 
		{"chartreuse2", "#76EE00"}, 
		{"chartreuse3", "#66CD00"}, 
		{"chartreuse4", "#458B00"}, 
		{"olivedrab1", "#C0FF3E"}, 
		{"olivedrab2", "#B3EE3A"}, 
		{"olivedrab3", "#9ACD32"}, 
		{"olivedrab4", "#698B22"}, 
		{"darkolivegreen1", "#CAFF70"}, 
		{"darkolivegreen2", "#BCEE68"}, 
		{"darkolivegreen3", "#A2CD5A"}, 
		{"darkolivegreen4", "#6E8B3D"}, 
		{"khaki1", "#FFF68F"}, 
		{"khaki2", "#EEE685"}, 
		{"khaki3", "#CDC673"}, 
		{"khaki4", "#8B864E"}, 
		{"lightgoldenrod1", "#FFEC8B"}, 
		{"lightgoldenrod2", "#EEDC82"}, 
		{"lightgoldenrod3", "#CDBE70"}, 
		{"lightgoldenrod4", "#8B814C"}, 
		{"lightyellow1", "#FFFFE0"}, 
		{"lightyellow2", "#EEEED1"}, 
		{"lightyellow3", "#CDCDB4"}, 
		{"lightyellow4", "#8B8B7A"}, 
		{"yellow1", "#FFFF00"}, 
		{"yellow2", "#EEEE00"}, 
		{"yellow3", "#CDCD00"}, 
		{"yellow4", "#8B8B00"}, 
		{"gold1", "#FFD700"}, 
		{"gold2", "#EEC900"}, 
		{"gold3", "#CDAD00"}, 
		{"gold4", "#8B7500"}, 
		{"goldenrod1", "#FFC125"}, 
		{"goldenrod2", "#EEB422"}, 
		{"goldenrod3", "#CD9B1D"}, 
		{"goldenrod4", "#8B6914"}, 
		{"darkgoldenrod1", "#FFB90F"}, 
		{"darkgoldenrod2", "#EEAD0E"}, 
		{"darkgoldenrod3", "#CD950C"}, 
		{"darkgoldenrod4", "#8B6508"}, 
		{"rosybrown1", "#FFC1C1"}, 
		{"rosybrown2", "#EEB4B4"}, 
		{"rosybrown3", "#CD9B9B"}, 
		{"rosybrown4", "#8B6969"}, 
		{"indianred1", "#FF6A6A"}, 
		{"indianred2", "#EE6363"}, 
		{"indianred3", "#CD5555"}, 
		{"indianred4", "#8B3A3A"}, 
		{"sienna1", "#FF8247"}, 
		{"sienna2", "#EE7942"}, 
		{"sienna3", "#CD6839"}, 
		{"sienna4", "#8B4726"}, 
		{"burlywood1", "#FFD39B"}, 
		{"burlywood2", "#EEC591"}, 
		{"burlywood3", "#CDAA7D"}, 
		{"burlywood4", "#8B7355"}, 
		{"wheat1", "#FFE7BA"}, 
		{"wheat2", "#EED8AE"}, 
		{"wheat3", "#CDBA96"}, 
		{"wheat4", "#8B7E66"}, 
		{"tan1", "#FFA54F"}, 
		{"tan2", "#EE9A49"}, 
		{"tan3", "#CD853F"}, 
		{"tan4", "#8B5A2B"}, 
		{"chocolate1", "#FF7F24"}, 
		{"chocolate2", "#EE7621"}, 
		{"chocolate3", "#CD661D"}, 
		{"chocolate4", "#8B4513"}, 
		{"firebrick1", "#FF3030"}, 
		{"firebrick2", "#EE2C2C"}, 
		{"firebrick3", "#CD2626"}, 
		{"firebrick4", "#8B1A1A"}, 
		{"brown1", "#FF4040"}, 
		{"brown2", "#EE3B3B"}, 
		{"brown3", "#CD3333"}, 
		{"brown4", "#8B2323"}, 
		{"salmon1", "#FF8C69"}, 
		{"salmon2", "#EE8262"}, 
		{"salmon3", "#CD7054"}, 
		{"salmon4", "#8B4C39"}, 
		{"lightsalmon1", "#FFA07A"}, 
		{"lightsalmon2", "#EE9572"}, 
		{"lightsalmon3", "#CD8162"}, 
		{"lightsalmon4", "#8B5742"}, 
		{"orange1", "#FFA500"}, 
		{"orange2", "#EE9A00"}, 
		{"orange3", "#CD8500"}, 
		{"orange4", "#8B5A00"}, 
		{"darkorange1", "#FF7F00"}, 
		{"darkorange2", "#EE7600"}, 
		{"darkorange3", "#CD6600"}, 
		{"darkorange4", "#8B4500"}, 
		{"coral1", "#FF7256"}, 
		{"coral2", "#EE6A50"}, 
		{"coral3", "#CD5B45"}, 
		{"coral4", "#8B3E2F"}, 
		{"tomato1", "#FF6347"}, 
		{"tomato2", "#EE5C42"}, 
		{"tomato3", "#CD4F39"}, 
		{"tomato4", "#8B3626"}, 
		{"orangered1", "#FF4500"}, 
		{"orangered2", "#EE4000"}, 
		{"orangered3", "#CD3700"}, 
		{"orangered4", "#8B2500"}, 
		{"red1", "#FF0000"}, 
		{"red2", "#EE0000"}, 
		{"red3", "#CD0000"}, 
		{"red4", "#8B0000"}, 
		{"deeppink1", "#FF1493"}, 
		{"deeppink2", "#EE1289"}, 
		{"deeppink3", "#CD1076"}, 
		{"deeppink4", "#8B0A50"}, 
		{"hotpink1", "#FF6EB4"}, 
		{"hotpink2", "#EE6AA7"}, 
		{"hotpink3", "#CD6090"}, 
		{"hotpink4", "#8B3A62"}, 
		{"pink1", "#FFB5C5"}, 
		{"pink2", "#EEA9B8"}, 
		{"pink3", "#CD919E"}, 
		{"pink4", "#8B636C"}, 
		{"lightpink1", "#FFAEB9"}, 
		{"lightpink2", "#EEA2AD"}, 
		{"lightpink3", "#CD8C95"}, 
		{"lightpink4", "#8B5F65"}, 
		{"palevioletred1", "#FF82AB"}, 
		{"palevioletred2", "#EE799F"}, 
		{"palevioletred3", "#CD6889"}, 
		{"palevioletred4", "#8B475D"}, 
		{"maroon1", "#FF34B3"}, 
		{"maroon2", "#EE30A7"}, 
		{"maroon3", "#CD2990"}, 
		{"maroon4", "#8B1C62"}, 
		{"violetred1", "#FF3E96"}, 
		{"violetred2", "#EE3A8C"}, 
		{"violetred3", "#CD3278"}, 
		{"violetred4", "#8B2252"}, 
		{"magenta1", "#FF00FF"}, 
		{"magenta2", "#EE00EE"}, 
		{"magenta3", "#CD00CD"}, 
		{"magenta4", "#8B008B"}, 
		{"orchid1", "#FF83FA"}, 
		{"orchid2", "#EE7AE9"}, 
		{"orchid3", "#CD69C9"}, 
		{"orchid4", "#8B4789"}, 
		{"plum1", "#FFBBFF"}, 
		{"plum2", "#EEAEEE"}, 
		{"plum3", "#CD96CD"}, 
		{"plum4", "#8B668B"}, 
		{"mediumorchid1", "#E066FF"}, 
		{"mediumorchid2", "#D15FEE"}, 
		{"mediumorchid3", "#B452CD"}, 
		{"mediumorchid4", "#7A378B"}, 
		{"darkorchid1", "#BF3EFF"}, 
		{"darkorchid2", "#B23AEE"}, 
		{"darkorchid3", "#9A32CD"}, 
		{"darkorchid4", "#68228B"}, 
		{"purple1", "#9B30FF"}, 
		{"purple2", "#912CEE"}, 
		{"purple3", "#7D26CD"}, 
		{"purple4", "#551A8B"}, 
		{"mediumpurple1", "#AB82FF"}, 
		{"mediumpurple2", "#9F79EE"}, 
		{"mediumpurple3", "#8968CD"}, 
		{"mediumpurple4", "#5D478B"}, 
		{"thistle1", "#FFE1FF"}, 
		{"thistle2", "#EED2EE"}, 
		{"thistle3", "#CDB5CD"}, 
		{"thistle4", "#8B7B8B"}, 
		{"gray0", "#000000"}, 
		{"grey0", "#000000"}, 
		{"gray1", "#030303"}, 
		{"grey1", "#030303"}, 
		{"gray2", "#050505"}, 
		{"grey2", "#050505"}, 
		{"gray3", "#080808"}, 
		{"grey3", "#080808"}, 
		{"gray4", "#0A0A0A"}, 
		{"grey4", "#0A0A0A"}, 
		{"gray5", "#0D0D0D"}, 
		{"grey5", "#0D0D0D"}, 
		{"gray6", "#0F0F0F"}, 
		{"grey6", "#0F0F0F"}, 
		{"gray7", "#121212"}, 
		{"grey7", "#121212"}, 
		{"gray8", "#141414"}, 
		{"grey8", "#141414"}, 
		{"gray9", "#171717"}, 
		{"grey9", "#171717"}, 
		{"gray10", "#1A1A1A"}, 
		{"grey10", "#1A1A1A"}, 
		{"gray11", "#1C1C1C"}, 
		{"grey11", "#1C1C1C"}, 
		{"gray12", "#1F1F1F"}, 
		{"grey12", "#1F1F1F"}, 
		{"gray13", "#212121"}, 
		{"grey13", "#212121"}, 
		{"gray14", "#242424"}, 
		{"grey14", "#242424"}, 
		{"gray15", "#262626"}, 
		{"grey15", "#262626"}, 
		{"gray16", "#292929"}, 
		{"grey16", "#292929"}, 
		{"gray17", "#2B2B2B"}, 
		{"grey17", "#2B2B2B"}, 
		{"gray18", "#2E2E2E"}, 
		{"grey18", "#2E2E2E"}, 
		{"gray19", "#303030"}, 
		{"grey19", "#303030"}, 
		{"gray20", "#333333"}, 
		{"grey20", "#333333"}, 
		{"gray21", "#363636"}, 
		{"grey21", "#363636"}, 
		{"gray22", "#383838"}, 
		{"grey22", "#383838"}, 
		{"gray23", "#3B3B3B"}, 
		{"grey23", "#3B3B3B"}, 
		{"gray24", "#3D3D3D"}, 
		{"grey24", "#3D3D3D"}, 
		{"gray25", "#404040"}, 
		{"grey25", "#404040"}, 
		{"gray26", "#424242"}, 
		{"grey26", "#424242"}, 
		{"gray27", "#454545"}, 
		{"grey27", "#454545"}, 
		{"gray28", "#474747"}, 
		{"grey28", "#474747"}, 
		{"gray29", "#4A4A4A"}, 
		{"grey29", "#4A4A4A"}, 
		{"gray30", "#4D4D4D"}, 
		{"grey30", "#4D4D4D"}, 
		{"gray31", "#4F4F4F"}, 
		{"grey31", "#4F4F4F"}, 
		{"gray32", "#525252"}, 
		{"grey32", "#525252"}, 
		{"gray33", "#545454"}, 
		{"grey33", "#545454"}, 
		{"gray34", "#575757"}, 
		{"grey34", "#575757"}, 
		{"gray35", "#595959"}, 
		{"grey35", "#595959"}, 
		{"gray36", "#5C5C5C"}, 
		{"grey36", "#5C5C5C"}, 
		{"gray37", "#5E5E5E"}, 
		{"grey37", "#5E5E5E"}, 
		{"gray38", "#616161"}, 
		{"grey38", "#616161"}, 
		{"gray39", "#636363"}, 
		{"grey39", "#636363"}, 
		{"gray40", "#666666"}, 
		{"grey40", "#666666"}, 
		{"gray41", "#696969"}, 
		{"grey41", "#696969"}, 
		{"gray42", "#6B6B6B"}, 
		{"grey42", "#6B6B6B"}, 
		{"gray43", "#6E6E6E"}, 
		{"grey43", "#6E6E6E"}, 
		{"gray44", "#707070"}, 
		{"grey44", "#707070"}, 
		{"gray45", "#737373"}, 
		{"grey45", "#737373"}, 
		{"gray46", "#757575"}, 
		{"grey46", "#757575"}, 
		{"gray47", "#787878"}, 
		{"grey47", "#787878"}, 
		{"gray48", "#7A7A7A"}, 
		{"grey48", "#7A7A7A"}, 
		{"gray49", "#7D7D7D"}, 
		{"grey49", "#7D7D7D"}, 
		{"gray50", "#7F7F7F"}, 
		{"grey50", "#7F7F7F"}, 
		{"gray51", "#828282"}, 
		{"grey51", "#828282"}, 
		{"gray52", "#858585"}, 
		{"grey52", "#858585"}, 
		{"gray53", "#878787"}, 
		{"grey53", "#878787"}, 
		{"gray54", "#8A8A8A"}, 
		{"grey54", "#8A8A8A"}, 
		{"gray55", "#8C8C8C"}, 
		{"grey55", "#8C8C8C"}, 
		{"gray56", "#8F8F8F"}, 
		{"grey56", "#8F8F8F"}, 
		{"gray57", "#919191"}, 
		{"grey57", "#919191"}, 
		{"gray58", "#949494"}, 
		{"grey58", "#949494"}, 
		{"gray59", "#969696"}, 
		{"grey59", "#969696"}, 
		{"gray60", "#999999"}, 
		{"grey60", "#999999"}, 
		{"gray61", "#9C9C9C"}, 
		{"grey61", "#9C9C9C"}, 
		{"gray62", "#9E9E9E"}, 
		{"grey62", "#9E9E9E"}, 
		{"gray63", "#A1A1A1"}, 
		{"grey63", "#A1A1A1"}, 
		{"gray64", "#A3A3A3"}, 
		{"grey64", "#A3A3A3"}, 
		{"gray65", "#A6A6A6"}, 
		{"grey65", "#A6A6A6"}, 
		{"gray66", "#A8A8A8"}, 
		{"grey66", "#A8A8A8"}, 
		{"gray67", "#ABABAB"}, 
		{"grey67", "#ABABAB"}, 
		{"gray68", "#ADADAD"}, 
		{"grey68", "#ADADAD"}, 
		{"gray69", "#B0B0B0"}, 
		{"grey69", "#B0B0B0"}, 
		{"gray70", "#B3B3B3"}, 
		{"grey70", "#B3B3B3"}, 
		{"gray71", "#B5B5B5"}, 
		{"grey71", "#B5B5B5"}, 
		{"gray72", "#B8B8B8"}, 
		{"grey72", "#B8B8B8"}, 
		{"gray73", "#BABABA"}, 
		{"grey73", "#BABABA"}, 
		{"gray74", "#BDBDBD"}, 
		{"grey74", "#BDBDBD"}, 
		{"gray75", "#BFBFBF"}, 
		{"grey75", "#BFBFBF"}, 
		{"gray76", "#C2C2C2"}, 
		{"grey76", "#C2C2C2"}, 
		{"gray77", "#C4C4C4"}, 
		{"grey77", "#C4C4C4"}, 
		{"gray78", "#C7C7C7"}, 
		{"grey78", "#C7C7C7"}, 
		{"gray79", "#C9C9C9"}, 
		{"grey79", "#C9C9C9"}, 
		{"gray80", "#CCCCCC"}, 
		{"grey80", "#CCCCCC"}, 
		{"gray81", "#CFCFCF"}, 
		{"grey81", "#CFCFCF"}, 
		{"gray82", "#D1D1D1"}, 
		{"grey82", "#D1D1D1"}, 
		{"gray83", "#D4D4D4"}, 
		{"grey83", "#D4D4D4"}, 
		{"gray84", "#D6D6D6"}, 
		{"grey84", "#D6D6D6"}, 
		{"gray85", "#D9D9D9"}, 
		{"grey85", "#D9D9D9"}, 
		{"gray86", "#DBDBDB"}, 
		{"grey86", "#DBDBDB"}, 
		{"gray87", "#DEDEDE"}, 
		{"grey87", "#DEDEDE"}, 
		{"gray88", "#E0E0E0"}, 
		{"grey88", "#E0E0E0"}, 
		{"gray89", "#E3E3E3"}, 
		{"grey89", "#E3E3E3"}, 
		{"gray90", "#E5E5E5"}, 
		{"grey90", "#E5E5E5"}, 
		{"gray91", "#E8E8E8"}, 
		{"grey91", "#E8E8E8"}, 
		{"gray92", "#EBEBEB"}, 
		{"grey92", "#EBEBEB"}, 
		{"gray93", "#EDEDED"}, 
		{"grey93", "#EDEDED"}, 
		{"gray94", "#F0F0F0"}, 
		{"grey94", "#F0F0F0"}, 
		{"gray95", "#F2F2F2"}, 
		{"grey95", "#F2F2F2"}, 
		{"gray96", "#F5F5F5"}, 
		{"grey96", "#F5F5F5"}, 
		{"gray97", "#F7F7F7"}, 
		{"grey97", "#F7F7F7"}, 
		{"gray98", "#FAFAFA"}, 
		{"grey98", "#FAFAFA"}, 
		{"gray99", "#FCFCFC"}, 
		{"grey99", "#FCFCFC"}, 
		{"gray100", "#FFFFFF"}, 
		{"grey100", "#FFFFFF"}, 
		{"dark", "#A9A9A9"}, 
		{"darkgrey", "#A9A9A9"}, 
		{"dark", "#A9A9A9"}, 
		{"darkgray", "#A9A9A9"}, 
		{"dark", "#00008B"}, 
		{"darkblue", "#00008B"}, 
		{"dark", "#008B8B"}, 
		{"darkcyan", "#008B8B"}, 
		{"dark", "#8B008B"}, 
		{"darkmagenta", "#8B008B"}, 
		{"dark", "#8B0000"}, 
		{"darkred", "#8B0000"}, 
		{"light", "#90EE90"}, 
		{"lightgreen", "#90EE90"}, 
		{NULL, NULL },
	};
	
	
	while (XPM_Color [nCount].pszName != NULL && (pszColor [0] == XPM_Color [nCount].pszName [0] && strcmp (pszColor, XPM_Color [nCount].pszName) != 0)) nCount++;
	
	if (XPM_Color [nCount].pszName != NULL)
	{
		uint32_t nColorValue;
		
		nColorValue = strtoll (XPM_Color [nCount].pszName, NULL, 16);
		
		stColor.nR = (nColorValue & 0xff0000L) >> 16;
		stColor.nG = (nColorValue & 0x00ff00L) >> 8;
		stColor.nB = (nColorValue & 0x0000ffL);
	}
	else 
	{
		stColor.nR = 0;
		stColor.nG = 0;
		stColor.nB = 0;
		stColor.nAlpha = 0;
	}

	
	return stColor;
}


int Graphic::DrawBox (int x, int y, int x2, int y2, int nAlpha, Color stColor)
{    
    DrawLine (x, y, x2, y, nAlpha, stColor);
    DrawLine (x, y, x, y2, nAlpha, stColor);
    DrawLine (x, y2, x2, y2, nAlpha, stColor);
    DrawLine (x2, y, x2, y2, nAlpha, stColor);
	
    return true;
}


int Graphic::DrawFillBox (int x, int y, int x2, int y2, int nAlpha, Color stColor)
{
    int row, col;
    
    
    for ( row = y; row < y2; row++ ) {
        for ( col = x; col < x2; col++ ) {
            if ( row >= 0 && col >= 0 ) {
                PutPixel (col, row, nAlpha, stColor);
            }
        }
    }
    
    return true;
}


// convert an xterm color value (0-253) to 3 unsigned chars rgb
inline void Graphic::XTerm2RGB(unsigned char color, unsigned char* rgb)
{
	// 16 basic colors
	if(color<16)
	{
		rgb[0] = basic16[color][0];
		rgb[1] = basic16[color][1];
		rgb[2] = basic16[color][2];
	}
	
	// color cube color
	if(color>=16 && color<=232)
	{
		color-=16;
		rgb[0] = valuerange[(color/36)%6];
		rgb[1] = valuerange[(color/6)%6];
		rgb[2] = valuerange[color%6];
	}
	
	// gray tone
	if(color>=233 && color<=253)
	{
		rgb[0]=rgb[1]=rgb[2] = 8+(color-232)*0x0a;
	}
	
	if (color > 253) rgb[0]=rgb[1]=rgb[2] = 255;
}



// fill the colortable for use with rgb2xterm
void Graphic::MakeTable()
{
	unsigned char c, rgb[3];
	for(c=0;c<=253;c++)
	{
		XTerm2RGB(c,rgb);
		colortable[c][0] = rgb[0];
		colortable[c][1] = rgb[1];
		colortable[c][2] = rgb[2];
	}
}

// selects the nearest xterm color for a 3xBYTE rgb value
unsigned char Graphic::Get8bitsColor(int nX, int nY)
{
	unsigned char c, best_match=0;
	double d, smallest_distance;
	Color stColor;
	
	stColor = GetPixel (nX, nY);

	if(!initialized)
		MakeTable();
	
	smallest_distance = 10000000000.0;
	
	for(c=0;c<=253;c++)
	{
		d = pow(colortable[c][0]-stColor.nR,2.0) + 
		pow(colortable[c][1]-stColor.nG,2.0) + 
		pow(colortable[c][2]-stColor.nB,2.0);
		
		//d = pow(colortable[c][0]-rgb[0],2.0) + 
		//pow(colortable[c][1]-rgb[1],2.0) + 
		//pow(colortable[c][2]-rgb[2],2.0);
		
		if(d<smallest_distance)
		{
			smallest_distance = d;
			best_match=c;
		}
	}
	
	
	 NOTRACE ("For nX: [%-4d], nY:[%-4d] Color: R:%-2u G:%-2u B:%-2u: Color: [%u]\n",
		   nX, nY, stColor.nR, stColor.nG, stColor.nB, best_match);
		   
	return best_match;
}




//Hide for development procedures 

#if 0

// read a hex-rgb-color like "CCAABB"
// output are 3 unsigned chars
// returns 0 on failure and 1 on success
inline int Graphic::ReadColor(const char* rgb_string, unsigned char* output)
{
	char Xr[3], Xg[3], Xb[3];
	
	// string -> bytes
	if(strlen(rgb_string)!=6) return 0;
	strncpy(Xr,rgb_string+0,2);
	strncpy(Xg,rgb_string+2,2);
	strncpy(Xb,rgb_string+4,2);
	output[0] = (unsigned char) strtoll(Xr, NULL, 16);
	output[1] = (unsigned char) strtoll(Xg, NULL, 16);
	output[2] = (unsigned char) strtoll(Xb, NULL, 16);
	
	return 1;
}





inline long long Graphic::XTerm2RGB2 (unsigned char color)
{		
	//long long nRetValue;
	unsigned char szValue [10];
	
	XTerm2RGB (color, szValue);
	
	return (long long) (szValue [0] + (0xff + szValue [1] + 1) + (0xff00 + szValue [2] + 1));
}





/*
 int Graphic::FillArc (int x, int y, int r1, int r2, double a1, double a2 )
 {
 int N, loop;
 double a, da;
 IPoint *points;
 
 //* because our y is upside down, make all angles their negative 
 a1 = 360 - a1;
 a2 = 360 - a2;
 
 N = (int) fabs ( a2 - a1 ) + 9;
 a = a1 * 2.0 * PI / 360.0;
 da = ( a2 - a1 ) * ( 2.0 * PI / 360.0 ) / ( N - 1 );
 points = (IPoint *) malloc ( sizeof ( IPoint ) * ( N + 1 ) );
 for ( loop = 0; loop < N ; loop++ ) {
 points[loop].x = x + (int)( r1 * cos ( a + loop * da ) );
 points[loop].y = y + (int)( r2 * sin ( a + loop * da ) );
 }
 
 // if we're not drawing a circle, add in the center point 
 if ( a2 - a1 < 359.9 ) {
 points[N].x = x;
 points[N].y = y;
 IFillPolygon (points, N + 1 );
 } else {
 IFillPolygon (points, N );
 }
 
 free ( points );
 
 return ( INoError );
 }
 */





/*
 int main (int nArgs, char** ppszArgs)
 {
 int nX, nY;
 
 GIF_Create (NULL, 400, 400, 256, atoi (ppszArgs [1]));
 
 GIF_DrawFillBox (0, 0, 400, 400, 0, 15);
 
 GIF_DrawCircle (160, 120, 60, true, 0,  1);
 
 GIF_DrawCircle (160, 120, 10, true, 0,  2);
 
 GIF_DrawCircle (160, 120, 40, false, 0, 3);
 
 GIF_DrawCircle (160, 120, 70, false, 0, 3);
 
 nX=5; nY=20; GIF_GPrint (&nX, &nY, 0, 1, "TESTE DE COR E TEXTO");
 
 GIF_SelectDefaultFont (1);
 
 nX=5; nY=40; GIF_GPrint (&nX, &nY, 0, 2, "TESTE DE COR E TEXTO\n\n");
 nX=3; nY=38; GIF_GPrint (&nX, &nY, 0, 2, "TESTE DE COR E TEXTO\n\n");
 
 
 GIF_LoadXPM (1, 50, 180 + 2, 324 + 2, 36, 100, 0, wether_xpm);
 
 GIF_LoadXPM (1, 90, 1, 1, 400, 400, 200, vista_xpm);
 
 GIF_LoadXPM (1, 130, 1, 1, 200, 120, 100,  wether_xpm);
 
 GIF_LoadXPM (1, 360, 1, 1, 190, 39, 242, solariun_xpm);
 
 GIF_LoadXPM (180, 130, 1, 1, 120, 160, 170, GU_xpm);
 
 GIF_LoadXPM (370, 370, 1, 1, 26, 26, 100, Badge_xpm);
 
 GIF_CompressImage (1, 1, 400, 400, NULL);
 
 GIF_Close ();
 }
 */

#endif 
