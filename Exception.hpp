//
//  Exception.hpp
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

/*
 Implementation example
 
 .hpp
 
 #ifdef __CLASS_NAME__
 #undef __CLASS_NAME__
 #endif
 
 #define __CLASS_NAME__ BidirectExec
 
 class BidirectExecException : public Exception
 {
 public:
 BidirectExecException (const char* pszFile, const size_t nFileLine, const char* pszFuncion, const char* pszCode, const size_t nExID, const char* pszStringValue);
 
 const char* what() noexcept override;
 };
 
 -------------------------------------------
 .cppp
 
 BidirectExecException::BidirectExecException (const char* pszFile, const size_t nFileLine, const char* pszFuncion, const char* pszCode, const size_t nExID, const char* pszStringValue) : Exception (what(), pszFile, nFileLine, pszFuncion, pszCode, nExID, pszStringValue)
 {}
 
 const char* BidirectExecException::what() noexcept
 {
 return STRINGFY (__CLASS_NAME__);
 }


 */



#ifndef Exception_hpp
#define Exception_hpp

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>

using namespace std;


#define __CLASS_NAME__ Exception


#define STRINGFY_(x) #x
#define STRINGFY__(x) STRINGFY_(x)
#define STRINGFY(x)STRINGFY__(x)

#define EXCEPTIONCALL(strCode, nExID, strErrorDescr) __FILE__, __LINE__, __FUNCTION__,  strCode, nExID, strErrorDescr

#define Verify_(cond,id,text, class) if (!(cond)) { throw class ## Exception(EXCEPTIONCALL(#cond, id, text)); }

#define Verify__(cond,id,text, class) Verify_(cond, id, text, class)

#define Verify(cond,id,text) Verify__(cond, id, text, __CLASS_NAME__)

#define NOTRACE

#define TRACE if (getDebugState() == true) std::cerr


void setDebug(bool nState);

bool getDebugState();


class Exception
{
private:
    const char* pszType;
    size_t nExID;
    string   strExText;
    
    static string strTypeValue;

public:
    
    explicit Exception (const char* pszType, const char* pszFile, const size_t nFileLine, const char* pszFuncion, const char* pszCode, const size_t nExID, const char* pszStringValue);
    
    Exception(const Exception&) noexcept;
    Exception& operator=(const Exception&) noexcept;
    
    ~Exception() noexcept;
    
    virtual const char* what() const noexcept;
    
    
    const uint32_t getExceptionID ();
    const char* getExMessage ();
    
};

#endif /* Exception_hpp */

