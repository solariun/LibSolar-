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
#include <regex>
#include <map>

#include "MetaException.hpp"
#include "Util.hpp"


using namespace std;



enum iniElements_t
{
    none_tag,
    struct_tag,
    open_struct_tag,
    close_struct_tag,
    attributive_tag,
    string_line_tag,
    string_quote_tag,
    value_tag,
    string_tag,
    session_tag,
    open_session_tag,
    close_session_tag
};



/*
 * ERROR COLDES
 */

#define EXCEPT_INI_FILE_NOT_READ                200
#define EXCEPT_INI_FILE_DOSENT_EXIST_OR_ZERO    201

#define EXCEPT_INI_FAIL_ADD_VALUE_NO_SESSION    202
#define EXCEPT_INI_SYNT_ERROR_INVALID_VALUE     203
#define EXCEPT_INI_INVALID_ATTRIBUTE_VALUE      204

#define EXCEPT_INI_NO_INIPATH_FOUND             210

/*
 * Type used for returning the Lexical ITem
 */


class iniParserItemRet
{
public:
    iniParserItemRet(iniElements_t  inieType, string& strValue) : nType(inieType), strValue(strValue){cout << "init class: " << typeid(this).name() << endl; };
    
    iniParserItemRet(){};
    
    iniParserItemRet* assign (iniElements_t  inieType, string& strValue)
    {
        this->nType = inieType;
        this->strValue = strValue;
        
        return this;
    }
    
    iniElements_t   nType = none_tag;
    string           strValue = "";
};



class iniplus
{
protected:
    
    map<string, string> mapIniData;
    
    ifstream*   isIn = NULL;
    string      strFileName;
    int32_t     nType= none_tag;

    
private:
    
    void parseINI (string strPath="", uint32_t nDepth=0);
    
    iniParserItemRet* getNextLexicalItem (iniParserItemRet& iniParserITem);
    
public:
    
    iniplus (const char* pszINIFileName);
    
    string getStringFromRef (string& strRet, const char* pszINIPath);
    string getString (const char* pszINIPath);
    
    int    getInteger (const char* pszINIPath);

    long  getULong (const char* pszINIPath);
    long long getULongLong (const char* pszINIPath);

    long long getULongLongFromBinary (const char* pszINIPath);

    long  getLong (const char* pszINIPath);
    long long getLongLong (const char* pszINIPath);
    
    float getfloat (const char* pszINIPath);
    double getDouble (const char* pszINIPath);
};


#endif /* iniplus_hpp */

