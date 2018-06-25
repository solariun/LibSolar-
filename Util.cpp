//
//  Util.cpp
//  xml2xpath
//
//  Created by GUSTAVO CAMPOS on 26/02/18.
//  Copyright © 2018 GUSTAVO CAMPOS. All rights reserved.
//
/*
 MIT License
 
 Copyright (c) [year] [fullname]
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */


#include "Util.hpp"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <algorithm>
#include <cctype>
#include <locale>
#include <vector>
#include <ctime>


#ifndef _DEBUG
static bool _nDebug = false;
#else
static bool _nDebug = true;
#endif


void setDebug(bool nState) {_nDebug = nState;}

bool getDebugState() {return _nDebug; };



// trim from start (in place)
void Util::ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
    {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void Util::rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch)
    {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void Util::trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
std::string Util::ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

// trim from end (copying)
std::string Util::rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

// trim from both ends (copying)
std::string Util::trim_copy(std::string s)
{
    trim(s);
    return s;
}

bool Util::isBetween (char chChar, const char* pszCharList, int32_t nMaxCharList=0)
{
    while ((nMaxCharList) >= 0 && pszCharList [nMaxCharList] != chChar)
    {
        //std::cout << "comparing: (" << nMaxCharList<< ") [" << (int) pszCharList [nMaxCharList+1] << "] [" << (int) chChar  <<  "]" << std::endl;
        nMaxCharList--;
    };
    
    //std::cout << "Returning at: " << nMaxCharList << std::endl;
    
    return nMaxCharList < 0 ? false : true;
}


long Util::getFileSize(std::string filename)
{
    struct stat stat_buf;
    
    int rc = stat(filename.c_str(), &stat_buf);
    
    return rc == 0 ? stat_buf.st_size : -1;
}


std::string& Util::strToUpper (std::string& strData)
{
    for (auto& c : strData) toupper(c);
    
    return strData;
}

/*
 * CSV style parser, it will parser
 * it will parser de data taking care of strings between ' " '
 *
 * it is based on vector to get things easier to parser for
 * developer, use resize to approvisionate space
 * it will only clear to no change the memory already allocated.
 */

uint Util::getCSVlikeParser (std::string& strData, const char* pszTokens, uint nTokenSize, std::vector<std::string>& listContainer)
{
    
    listContainer.clear();
    
    std::string strWork = "";
    
    int typeText = 0;
    bool boolOverload = false;
    bool boolAddChar = false;
 
    boolAddChar = false;
    
    uint nCount=0;
    
    for (auto chChar : strData)
    {
        if (typeText == 0 && isBetween(chChar, pszTokens, nTokenSize) == false)
        {
            if (chChar == '\"')
            {
                typeText = 2;
                continue;
            }
            else
            {
                typeText = 1;
            }
        }
        
        if (typeText == 1)
        {
            if (isBetween(chChar, pszTokens, nTokenSize))
            {
                listContainer.push_back (strWork);
                TRACE << "Adding line: [" << strWork << "]" << std::endl;
                
                nCount++;
                
                strWork = "";
                typeText = 0;
                
                boolAddChar = false;
            }
            else if (boolAddChar == false)
            {
                boolAddChar = true;
            }
        }
        else if (typeText == 2)
        {
            if (boolOverload == true)
            {
                strWork += chChar;
                
                boolOverload = true;
            }
            else if (chChar == '\\')
            {
                boolOverload = true;
            }
            else if (chChar == '"')
            {
                typeText = 1;
                continue;
            }
            else
            {
                boolAddChar = true;
            }
        }
        
        if (boolAddChar == true)
        {
            strWork += chChar;
        }
    }
    
    if (strWork.length() > 0)
    {
        listContainer.push_back (strWork);
        TRACE << "LAST: Adding line: [" << strWork << "]" << std::endl;
        nCount++;
    }
    
    return nCount;
}


const string Util::getLogLikeTimeStamp ()
{
    time_t timeNow;
    struct tm* tmInfo;
    char szBuffer [20];
    
    time (&timeNow);
    tmInfo = localtime(&timeNow);
    
    strftime(szBuffer, sizeof (szBuffer), "%F.%T", tmInfo);
    
    return string(szBuffer);
}



const string Util::getStandardErrorHeader (const char* pszClass, int nLine, const char* pszFunction)
{
    string strValue;
    
    strValue.resize(200);
    
    strValue = strValue + pszClass + "." + pszFunction + "(" + std::to_string(nLine) + ")";
    
    return strValue;
}

void  Util::PrintDataToDebug (uint8_t* szSectionData, long int nDataLen)
{
    long int nCount;
    long int nCount1;
    long int  nLen;
    char szPData [20];
    
    fprintf (stderr, "%s : Total Visualizing: [%-8lu]\n", "Debug ", nDataLen);
    
    for (nCount=0; nCount < nDataLen; nCount = nCount + 16)
    {
        nLen = nCount + 16 > nDataLen ? nDataLen - nCount : 16;
        
        fprintf (stderr, "%s : Addr: [%-.10lu] ", "Debug ", nCount);
        for (nCount1=0; nCount1 < 16; nCount1++)
        {
             if (nCount1 % 8 == 0) printf ("  ");
            
            if (nCount1 + nCount < nDataLen)
            {
               
                
                fprintf (stderr, "%-.2X ", (uint8_t) szSectionData [nCount + nCount1]);
                szPData [nCount1] = szSectionData [nCount + nCount1] < 32 || szSectionData [nCount + nCount1] >= 127 ? '.' : szSectionData [nCount + nCount1];
            }
            else
            {
                fprintf (stderr, ".. "); szPData [nCount1] = '.';
            }
            
        }
        
        szPData [nCount1] = '\0';
        
        fprintf (stderr, "  [%s]\n", szPData);
    }
}



const vector<std::string> getFields (const std::string& strData, const std::string strTokens)
{
    vector<std::string> vecData;
    string strTempData;
    
    if (strData.length() != 0)
    {
        const char* pszData = strData.c_str();
        char chValue = '\0';
        
        
        while ((chValue = *pszData++) != '\0')
        {
            
        }
    }
    
    return vecData;
}
