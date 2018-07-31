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


#ifndef _DEBUG
static bool _nDebug = false;
#else
static bool _nDebug = true;
#endif


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

bool Util::isBetween (char chChar, const char* pszCharList, ssize_t nMaxCharList=0)
{
    nMaxCharList = (int32_t)(nMaxCharList == 0 ? strlen(pszCharList) : nMaxCharList) - 1;
    
    while ((nMaxCharList) >= 0 && pszCharList [nMaxCharList] != chChar)
    {
        //std::cerr << "comparing: (" << nMaxCharList<< ") [" << (int) pszCharList [nMaxCharList+1] << "] [" << (int) chChar  <<  "]" << std::endl;
        nMaxCharList--;
    };
    
    return nMaxCharList < 0 ? false : true;
}


long Util::getFileSize(std::string filename)
{
    struct stat stat_buf;
    
    int rc = stat(filename.c_str(), &stat_buf);
    
    return rc == 0 ? stat_buf.st_size : -1;
}


const std::string Util::strToUpper (std::string strData)
{
    for (char& c : strData) c = toupper(c);
    
    return strData;
}

const std::string Util::strToLower (std::string strData)
{
    for (char& c : strData) c = tolower(c);
    
    return strData;
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



/*
 *  getFilds* funcitons are destinated to work in mult-spepareted like
 *  CSV or command like fasion, up to the way it will be used.
 *
 *  How it works:
 *      Token is formed by an token char followed by Delimiters if needed
 *      so, ";'\"" in sthis static string the token is ";" and the
 *      Delimiters " e ', so A;"A B C";'123 123'"2340234"sdf;E;";;;;;;\""
 *
 *      for this the result vectori would bring
 *  it would have
 *  [1] A
 *  [2] A B C
 *  [3] 123 1232340234sdf
 *  [4] E
 *  [5] ;;;;;;"
 *
 *  Note that scape is also permited and will interpret \n \r and \t, all the
 *  will be set independetly if it is a Delimiter or Token, it will be overloaded.
 *
 *  if Token is ' '(space) it will automaticaly perform ltrim while looking for a char
 *  or Delimiter if, ONLY IF, --> boolAutoTrim <-- is true;
 */

const vector<std::string>& Util:: getFields (const std::string& strOriData, const std::string strTokens, vector<std::string>& vecData,  bool boolAutoRTrim)
{
    string strTempData;
    vecData.clear();
    
    if (strOriData.length() != 0 && strTokens.length() > 0)
    {
        char chToken = strTokens[0];
        
        //This will make things faster
        char szTokens [strTokens.length()+1];
        strncpy(szTokens, strTokens.c_str(), strTokens.length());

        
        NOTRACE << "\n\n----------------------------------------------\n";
        NOTRACE << "Starting processing line: [" << strOriData << "]" << endl;
        
        char chLookup = 0;
        bool boolScapeChar = false;
        bool boolLookup = false;

        string strItem;
        
        for (const char& chItem : strOriData)
        {
            NOTRACE << "Item: [" << chItem <<"], chLookup: [" << chLookup << "], boolLookup: (" << boolLookup << "), boolScapeChar:(" << boolScapeChar << ")" << endl;
            NOTRACE << " >>>> Original: [" << strOriData << "]" << endl;
            NOTRACE << " >>>> strItem:  [" << strItem << "]" << endl << endl;
            
            if (boolScapeChar == false)
            {
                if (chLookup == 0)
                {
                    boolLookup = false;
                    
                    if (chToken == ' ' && ((boolAutoRTrim == true && isspace(chItem)) || chItem == chToken))
                    {
                        chLookup = ' ';
                        continue;
                    }
                    else if (isBetween(chItem, szTokens, strTokens.length()) == true)
                    {
                        boolLookup = true;
                        chLookup = chItem;
                        continue;
                    }
                    else
                    {
                        boolLookup = true;
                        chLookup = chToken;
                    }
                }
                else
                {
                    if (boolLookup == false && chToken == ' ')
                    {
                        if (boolAutoRTrim == true && isspace(chItem))
                        {
                            continue;
                        }
                        else
                        {
                            boolLookup = true;
                        }
                    }
                    else if (boolLookup == true)
                    {
                        if (chItem == '\\')
                        {
                            boolScapeChar = true;
                            continue;
                        }
                        else if (chLookup == chToken)
                        {
                            if (strTokens.length()>1 && isBetween(chItem, szTokens+1, strTokens.length()-1) == true)
                            {
                                boolLookup = true;
                                chLookup = chItem;
                                
                                continue;
                            }
                            else if (chItem == chLookup)
                            {
                                NOTRACE << "ADDING: [" << strItem << "]" << endl;
                                vecData.push_back(strItem);
                                
                                chLookup = 0;
                                boolLookup = false;
                                strItem = "";
                                
                                continue;
                            }
                        }
                        else if (chItem == chLookup) //Delimiter handler
                        {
                            chLookup = chToken;
                            continue;
                        }
                    }
                }
            }
            
            if (boolScapeChar == true || chItem == '\t' || chItem >= ' ')
            {
                strItem += (boolScapeChar == true ?
                            chItem == 'n' ? '\n' :
                            chItem == 'r' ? '\r' :
                            chItem == 't' ? 'r' : chItem
                            : chItem);
                
                if (boolScapeChar == true) boolScapeChar = false;
            }
        }
        
        if (chLookup != 0)
        {
            NOTRACE << "LATE ADDING: [" << strItem << "]" << endl;
            vecData.push_back(strItem);
        }
        
        NOTRACE << "FINAL -> chLookup: [" << chLookup << "], boolLookup: (" << boolLookup << "), boolScapeChar:(" << boolScapeChar << ")" << endl;
        NOTRACE << " >>>> Original: [" << strOriData << "]" << endl;
        NOTRACE << " >>>> strItem:  [" << strItem << "]" << endl << endl;
    }
    

    return vecData;
}


void Util::PrintStandardTypeSizes()
{
    printf ("short             : %ld\n", sizeof (short)*8);
    printf ("short int         : %ld\n", sizeof (short int)*8);
    printf ("int               : %ld\n", sizeof (int)*8);
    printf ("long int          : %ld\n", sizeof (long int)*8);
    printf ("long long int     : %ld\n", sizeof (long long int)*8);
    printf ("signed            : %ld\n", sizeof (signed)*8);
    printf ("unsigned          : %ld\n", sizeof (unsigned)*8);
    printf ("long              : %ld\n", sizeof (long)*8);
    printf ("double            : %ld\n", sizeof (double)*8);
    printf ("float             : %ld\n", sizeof (float)*8);
    printf ("long double       : %ld\n", sizeof (long double)*8);
}


const char* Util::ToUpper (char* strData, uint nDataLen)
{
    while (nDataLen > 0) strData [nDataLen-1] = toupper(strData [nDataLen-1]);
    
    return strData;
}


const char* Util::ToLower (char* strData, uint nDataLen)
{
    while (nDataLen > 0) strData [nDataLen-1] = tolower (strData [nDataLen-1]);
    
    return strData;
}
