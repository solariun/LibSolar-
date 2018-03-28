//
//  iniplus.hpp
//  LibSolar++
//
//  Created by GUSTAVO CAMPOS on 26/03/18.
//  Copyright © 2018 GUSTAVO CAMPOS. All rights reserved.
//

#ifndef iniplus_hpp
#define iniplus_hpp

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include "MetaException.hpp"

using namespace std;



enum iniElements_t
{
    init_tag,
    none_tag,
    struct_tag,
    open_struct_tag,
    close_struct_tag,
    attributive_tag,
    set_tag,
    value_tag,
    string_quote_tag,
    string_tag,
    limit_tag,
    array_tag,
    session_tag,
    open_session_tag,
    close_session_tag
};

/*
 * Type used for returning the Lexical ITem
 */


class iniParserItemRet
{
public:
    iniParserItemRet(iniElements_t  inieType, string& strValue) : inieType(inieType), strValue(strValue){cout << "init class: " << typeid(this).name() << endl; };
    
    iniParserItemRet(){};
    
    iniParserItemRet* assign (iniElements_t  inieType, string& strValue)
    {
        this->inieType = inieType;
        this->strValue = strValue;
        
        return this;
    }
    
    iniElements_t   inieType = none_tag;
    string           strValue = "";
};



class iniplus
{
protected:
    
    ifstream*   isIn = NULL;
    string      strFileName;
    int32_t     nType= none_tag;
    
private:
    
    void parseINI ();
    
    iniParserItemRet* getNextLexicalItem (iniParserItemRet& iniParserITem);
    
public:
    
    iniplus (const char* pszINIFileName);
};


#endif /* iniplus_hpp */

