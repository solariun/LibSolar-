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
 
 class iniplusException : public Exception
 {
 public:
    iniplusException (std::string strMessage, uint nErrorID);
 };

 -------------------------------------------
 .cppp
 
 iniplusException::iniplusException (std::string strMessage, uint nErrorID): Exception ("iniplus", strMessage, nErrorID)
 {}

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

#define CLASSLOG cerr << ClassErrorHeader() << ":"
#define _LOG cerr << FuncErrorHeader() << ":"

#define ClassErrorHeader() Exception::getExtendedErrorHeader (typeid(*this).name(), __LINE__, __FUNCTION__)
#define FuncErrorHeader() Exception::getExtendedErrorHeader ("Function", __LINE__, __FUNCTION__)

#define _Verify(cond, text, id, except) { if(!(cond)) throw except (FuncErrorHeader() + "cond:[" + #cond + "]: " + text, id); }
#define Verify(cond, text, id, except) { if(!(cond)) throw except (ClassErrorHeader() + "cond:[" + #cond + "]: " + "-" + text, id); }

#define VERIFY(cond, text, ret) {if(!(cond)) { _LOG << ":" << text << endl; return ret; }}

#define STRINGFY_(x) #x
#define STRINGFY__(x) STRINGFY_(x)
#define STRINGFY(x)STRINGFY__(x)

#define NOTRACE

#define TRACE if (getDebugState() == true) std::cerr


void setDebug(bool nState);

bool getDebugState();




class Exception
{
private:
    size_t nExID;
    string   strExText;
    
    static string strTypeValue;

protected:
    
    string strType;

public:

    static const string getExtendedErrorHeader (const char* pszClass, int nLine, const char* pszFunction);
    
    explicit Exception (string strType, string strMessage, uint nErrorID);

    explicit Exception (string strMessage, uint nErrorID);

    Exception(const Exception&) noexcept;
    Exception& operator=(const Exception&) noexcept;
    
    ~Exception() noexcept;
    
    virtual const char* what() const noexcept;
    
    const uint32_t getExceptionID ();
    const char* getExMessage ();
    
};

#endif /* Exception_hpp */

