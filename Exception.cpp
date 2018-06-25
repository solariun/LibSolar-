//
//  Exception.cpp
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


 

#include "Exception.hpp"
#include <sstream>
#include "Util.hpp"

Exception::Exception(const Exception& exException) noexcept
{
    (*this) = exException;
    return;
}


Exception& Exception::operator=(const Exception& exException) noexcept
{
    this->nExID = exException.nExID;
    this->strExText.assign(exException.strExText);
    
    return (*this);
}

Exception::~Exception() noexcept
{
    return;
}

const uint32_t Exception::getExceptionID ()
{
    return (const uint32_t) nExID;
}

const char* Exception::getExMessage ()
{
    return strExText.c_str();
}


const char* Exception::what() const noexcept
{
    Util::PrintDataToDebug ((uint8_t*) this->strExText.c_str(), this->strExText.length());
    
    return (const char*) this->strExText.c_str();
}


const string Exception::getExtendedErrorHeader (const char* pszClass, int nLine, const char* pszFunction)
{
    string strValue;
    
    strValue = strValue + pszClass + "." + pszFunction + "(" + std::to_string(nLine) + ")";
    
    return strValue;
}


Exception::Exception(std::string strType, std::string strMessage, uint nErrorID) : strType (strType), strExText(strMessage), nExID(nErrorID)
{
    CLASSLOG << "Message: " << strExText << endl;
}


Exception::Exception(std::string strMessage, uint nErrorID) : strType ("Exception"), strExText(strMessage), nExID(nErrorID)
{
    CLASSLOG << "Message: " << strExText << endl;
}



