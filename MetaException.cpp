//
//  MetaException.cpp
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


#include "MetaException.hpp"
#include <sstream>

#ifndef _DEBUG
static bool _nDebug = false;
#else
static bool _nDebug = true;
#endif

void setDebug(bool nState) {_nDebug = nState;}

bool getDebugState() {return _nDebug; };


inline std::string className(const std::string& prettyFunction)
{
    
    size_t colons = prettyFunction.find("(");
    
    if (colons == std::string::npos)
        return "__BODY__";
    size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
    size_t end = colons - begin;
    
    string strClassFunc = prettyFunction.substr(begin,end);
    
    colons = strClassFunc.find("::");
    if (colons == std::string::npos)
        return "__BODY__";
    begin = strClassFunc.substr(0,colons).rfind(" ") + 1;
    end = colons - begin;
    
    return strClassFunc.substr(begin,end);
}


#define __CLASS_NAME__ className(__PRETTY_FUNCTION__)

MetaException::MetaException(const string& strType, const uint32_t nExID,  const string strValue) noexcept: strExText(strValue), nExID(nExID), strType(strType)
{
    return;
}

MetaException::MetaException(const string& strType, const uint32_t nExID,  const char* pszValue)  noexcept: strExText(pszValue), nExID(nExID), strType(strType)
{
    return;
}

MetaException::MetaException(const uint32_t nExID,  const string strValue) noexcept: strExText(strValue), nExID(nExID)
{
    return;
}

MetaException::MetaException(const uint32_t nExID,  const char* pszValue)  noexcept: strExText(pszValue), nExID(nExID)
{
    return;
}


MetaException::MetaException(const MetaException& exMetaException) noexcept
{
    (*this) =exMetaException;
    return;
}

MetaException& MetaException::operator=(const MetaException& exMetaException) noexcept
{
    this->nExID = exMetaException.nExID;
    this->strExText.assign(exMetaException.strExText);
    
    return (*this);
}


MetaException::~MetaException() noexcept
{
    return;
}

const char* MetaException::what() const noexcept
{
    return strType.c_str();
}


void MetaException::verify(bool bCriteria, const char* pszType, const char* pszFile, const size_t nFileLine, const char* pszFuncion, const char* pszCode, const u_int32_t nExID, const char* sStringValue)
{
    if (!bCriteria)
    {
        stringstream strsValue;
        string       strClassName = pszType;
        
        
        if (strClassName.size() == 0) strClassName = "__BODY__";
        
        strsValue << " " << strClassName << " at " << pszFile << "(" << nFileLine << "), Func: [" << pszFuncion << "], Code: [" << pszCode << "], ID:(" << nExID << "):" << sStringValue;
        
        if (errno != 0)
        {
            strsValue << ": ERROR: [ (" << errno << ") : " << strerror(errno)<< "]" ;
        }
        
        //std::abort();
        
        throw new MetaException (strClassName, nExID, strsValue.str());
    }
}


const uint32_t MetaException::getExceptionID ()
{
    return (const uint32_t) nExID;
}

const char* MetaException::getExMessage ()
{
    return strExText.c_str();
}

const char* MetaException::getExType ()
{
    return strType.c_str();
}
