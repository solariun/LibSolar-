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

#include "MetaException.hpp"

#define CLASSLOG cerr << ClassErrorHeader() << ":"

#define ClassErrorHeader() Util::getStandardErrorHeader (typeid(*this).name(), __FILE__, __LINE__, __FUNCTION__)
#define FuncErrorHeader() Util::getStandardErrorHeader ("Function", __LINE__, __FUNCTION__)

namespace Util
{
    bool isBetween  (char chChar, const char* pszCharList, int32_t nMaxCharList);

    long getFileSize(std::string filename);

    void ltrim(std::string &s);

    void rtrim(std::string &s);

    void trim(std::string &s);

    std::string ltrim_copy(std::string s);

    std::string rtrim_copy(std::string s);

    std::string trim_copy(std::string s);

    std::string& strToUpper (std::string& strData);
    
    uint getCSVlikeParser (std::string& strData, const char* pszToken, uint nTokenSize, std::vector<std::string>& listContainer);

    const string getLogLikeTimeStamp ();
    
    const string getStandardErrorHeader (const char* pszClass, int nLine, const char* pszFunction);
}



#endif /* Util_hpp */

