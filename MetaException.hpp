//
//  MetaException.hpp
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


#ifndef MetaException_hpp
#define MetaException_hpp

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>

using namespace std;


#define VERIFY(cond,id,text) MetaException::verify (cond, "typeid (this).name()", __FILE__, __LINE__, __PRETTY_FUNCTION__, #cond, id, text)

class MetaException
{
private:
    uint32_t nExID;
    string   strExText;
    string   strType;
    
public:
    MetaException(const string& strType, const uint32_t nExID,  const string&) noexcept;
    MetaException(const string& strType, const uint32_t nExID,  const char*) noexcept;
    
    MetaException(const MetaException&) noexcept;
    MetaException& operator=(const MetaException&) noexcept;
    
    ~MetaException() noexcept;
    
    virtual const char* what() const noexcept;
    
    static void verify(bool bCriteria, const char* pszType, const char* pszFile, const size_t nFileLine, const char* pszFuncion, const char* pszCode, const u_int32_t nExID, const char* pszStringValue);
    
    const uint32_t getExceptionID ();
    const char* getExMessage ();
};

#endif /* MetaException_hpp */

