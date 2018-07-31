//
//  Util.hpp
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


#ifndef Util_hpp
#define Util_hpp

#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <cstring>

#include "Exception.hpp"

using namespace std;
//#include "MetaException.hpp"

#define NOTRACE /* DISABLES CODE */ true ? cerr : _LOG

#define TRACE if (getDebugState() == true) _LOG

void setDebug(bool nState);

bool getDebugState();



namespace Util
{
    bool isBetween  (char chChar, const char* pszCharList, ssize_t nMaxCharList);

    long getFileSize(std::string filename);

    void ltrim(std::string &s);

    void rtrim(std::string &s);

    void trim(std::string &s);

    std::string ltrim_copy(std::string s);

    std::string rtrim_copy(std::string s);

    std::string trim_copy(std::string s);

    const std::string strToUpper (std::string strData);
    
    const std::string strToLower (std::string strData);

    const char* ToUpper (char* strData, uint nDataLen);
    
    const char* ToLower (char* strData, uint nDataLen);

    const string getLogLikeTimeStamp ();
    
    const string getStandardErrorHeader (const char* pszClass, int nLine, const char* pszFunction);
    
    void  PrintDataToDebug (uint8_t* szSectionData, long int nDataLen);
    
    
    /*
     *  getFilds* funcitons are destinated to work in mult-spepareted like
     *  command line but variable...
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
    const vector<std::string>& getFields (const std::string& strData, const std::string strTokens, vector<std::string>& vecData,  bool boolAutoRTrim = false);
    
    void PrintStandardTypeSizes();
    
   // void strToUpper (string& strData);
    //void strToLower (string& strData);
}



#endif /* Util_hpp */

