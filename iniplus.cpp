//
//  iniplus.cpp
//  LibSolar++
//
//  Created by GUSTAVO CAMPOS on 26/03/18.
//  Copyright © 2018 GUSTAVO CAMPOS. All rights reserved.
//

#include "iniplus.hpp"

#include <iostream>
#include <new>
#include <sstream>
#include <stdlib.h>


iniplus::iniplus (const char* pszINIFileName) : strFileName(pszINIFileName)
{
    
    ifstream* ifsFile;
    
    VERIFY(Util::getFileSize(pszINIFileName) > 0, EXCEPT_INI_FILE_DOSENT_EXIST_OR_ZERO, "Errro, file is zero or does not exist.");
    
    ifsFile = new ifstream (pszINIFileName);
    //int errn = errno;
    
    VERIFY ((ifsFile->rdstate() & std::ifstream::failbit) == 0, EXCEPT_INI_FILE_NOT_READ, "File could not be read.");
    
    this->isIn = ifsFile;
    
    char chCinbuffer [64 * 1024]; // 64k buffer;
    
    this->isIn->rdbuf()->pubsetbuf(chCinbuffer, sizeof (chCinbuffer));
    
    //Starting creating the structure of the data
    
    try {
        parseINI();
    }
    catch (...)
    {
        this->isIn->close();
        throw;
    }
    
    
    this->isIn->close();
}



void iniplus::parseINI (string strPath, uint32_t nDepth)
{
    iniParserItemRet lexItem;
    
    string strValue = "";
    string strAttribute = "";
    
    uint nType = none_tag;
    
    if (strPath.length() > 0)
    {
        TRACE << "Entering level (" << nDepth << ") [" << strPath << "]" << endl;
    }
    
    while (getNextLexicalItem(lexItem) != NULL)
    {
        //TRACE << "strPath: [" << strPath.length() << "] Received Lex Item: (" << lexItem.nType << ") [" << lexItem.strValue << "]" << endl;
        
        if (lexItem.nType == session_tag)
        {
            strPath = lexItem.strValue;
        }
        else if (strPath.length() > 0)
        {
            if (nType == none_tag)
            {
                if (lexItem.nType == string_tag)
                {
                    strAttribute = lexItem.strValue;
                    nType = value_tag;
                }
                else if (lexItem.nType == close_struct_tag)
                {
                    //TRACE << "---------------------------------" << endl;
                    //TRACE << "Leving Level " << nDepth << "..." << endl << endl;

                    return;
                }
            }
            if (nType == value_tag)
            {
                VERIFY(strAttribute.length() > 0, EXCEPT_INI_INVALID_ATTRIBUTE_VALUE, "Error, no Attribute value available.");
                
                if (lexItem.nType == attributive_tag)
                {
                    VERIFY(getNextLexicalItem(lexItem) != NULL && lexItem.nType == string_tag, EXCEPT_INI_SYNT_ERROR_INVALID_VALUE, "Syntatic Error, no correct value given");
                    
                    mapIniData.insert (pair<string, string> (strPath + "." + strAttribute, lexItem.strValue));
                    
                    TRACE << "Adding: [" << strPath << "." << strAttribute << "] = [" << lexItem.strValue << "]" << endl;
                    
                    nType = none_tag;
                }
                else if (lexItem.nType == open_struct_tag)
                {
                    parseINI(strPath + "." + strAttribute, nDepth + 1);
                    
                    //TRACE << "Returned to: [" << strPath << "]" << endl;
                    nType = none_tag;
                }
            }
        }
        else
        {
            VERIFY (strPath.length() == 0 && lexItem.nType == session_tag, EXCEPT_INI_FAIL_ADD_VALUE_NO_SESSION, "Error, Trying to add values with no session open, please revise the ini structure.");
        }
    }
}


iniParserItemRet* iniplus::getNextLexicalItem (iniParserItemRet& iniParserItem)
{
    string  strData = "";
    
    char    stzTemp [2]= { '\0', '\0' };
    char    chChar;

    if (isIn->eof()) return NULL;
    
    bool    boolString = false;
    bool    boolOverload = false;
    bool    boolDiscartComment = false;
    
    static regex regexDigits ("[[:graph:]]");
    static regex regexAlphaName ("[0-9A-Za-z_\\-]");
    
    
    while (isIn->good())
    {
        chChar = isIn->get();
        stzTemp [0] = chChar; //securely stringfing chChar...
        
        //TRACE << chChar << " tp: " << nType << " : String: [" << strData << "]" <<  " EOF: " << isIn->eof () << endl;
        
        if (boolDiscartComment == true)
        {
            if (chChar == '\n')
            {
                boolDiscartComment = false;
                isIn->putback(chChar);
            }
            
            TRACE << chChar;
        }
        else if (nType != string_quote_tag && chChar == '#')
        {
            boolDiscartComment = true;
            
            TRACE << "Starting Discarting..." << endl;
        }
        else if (nType == none_tag)
        {
            if (chChar == '[')
            {
                nType = open_session_tag;
                boolString = true;
            }
            else if (chChar == '{')
            {
                strData = "{";
                
                iniParserItem.assign (open_struct_tag, strData);
                
                return &iniParserItem;
            }
            else if (chChar == '}')
            {
                strData = "}";
                
                iniParserItem.assign (close_struct_tag, strData);
                
                return &iniParserItem;
            }
            else if (chChar == '=')
            {
                nType = attributive_tag;
                
                strData = "=";
                
                iniParserItem.assign (attributive_tag, strData);
                
                return &iniParserItem;
            }
            else if (regex_match (stzTemp, regexAlphaName) == true)
            {
                if (chChar == '"')
                    nType = string_quote_tag;
                else
                    nType = string_tag;
                
                isIn->putback(chChar);
                
                boolString = true;
            }
        }
        else if (nType == attributive_tag)
        {
            if (chChar == '"')
            {
                nType = string_quote_tag;
                boolString = true;
            }
            else if (regex_match (stzTemp, regexDigits) == true)
            {
                nType = string_line_tag;
                
                isIn->putback(chChar);
                
                boolString = true;
            }
        }
        else if (nType == string_tag && regex_match (stzTemp, regexAlphaName) == false)
        {
            nType = none_tag;
            
            iniParserItem.assign (string_tag, strData);
            
            return &iniParserItem;
        }
        else if (nType == string_quote_tag && boolOverload == false)
        {
            if(chChar == '\\')
            {
                boolOverload = true;
                continue;
            }
            else if (chChar == '"')
            {
                nType = none_tag;
                
                iniParserItem.assign (string_tag, strData);
                
                return &iniParserItem;
            }
        }
        else if (nType == string_line_tag && chChar == '\n')
        {
            nType = none_tag;
            
            Util::trim(strData);
            
            iniParserItem.assign (string_tag, strData);
            
            return &iniParserItem;
        }
        else if (nType == open_session_tag && chChar == ']')
        {
            nType = none_tag;
            
            iniParserItem.assign (session_tag, strData);
            
            return &iniParserItem;
        }
        else if (boolString == true)
        {
            if (chChar == '\t' || chChar >= ' ') strData += chChar;
            
            boolOverload = false;
        }
    }
    
    if (nType == string_line_tag && strData.length() > 0)
    {
        iniParserItem.assign (string_tag, strData);
        
        return &iniParserItem;
    }
    
    nType = none_tag;
        
    return NULL;
}



/*
 * Check if exists any inipath;
 */
bool iniplus::Exists (const char* pszINIPath)
{
    return mapIniData.find(pszINIPath) != mapIniData.end() ? true : false;
}



/*
 * Conversion types lookup functions functions 
 */

string iniplus::getStringFromRef (string& strRet, const char* pszINIPath)
{
    return (strRet = getString(pszINIPath));
}

string iniplus::getString (const char* pszINIPath)
{
    map<string,string>::iterator mapPos;
    
    mapPos = mapIniData.find(pszINIPath);
    
    VERIFY (mapPos != mapIniData.end(), EXCEPT_INI_NO_INIPATH_FOUND, "Error, element not found.");
    
    return mapPos->second;
}


int iniplus::getInteger (const char* pszINIPath)
{
    return std::stoi (((const string) getString (pszINIPath)), nullptr, 0);
}


/*
 *  The Long and Long Long conversion hereby implemented
 *  are capable of selecting from decimal and hexadecimal automatically
 *  and extra is supplied for binary only conversion. I know one can argue
 *  the auto select could be implemented for binary too, but it could pose
 *  an extra layer of processing, losing precious time that could be, otherwise,
 *  used for processing, thus, I decided against it.
 */

long  iniplus::getLong (const char* pszINIPath)
{
    return std::stol ((const string)getString (pszINIPath), nullptr, 0);
}

long long iniplus::getLongLong (const char* pszINIPath)
{
    return std::stoll ((const string)getString (pszINIPath), nullptr, 0);
}

long long iniplus::getULongLongFromBinary (const char* pszINIPath)
{
    return std::stoll ((const string)getString (pszINIPath), nullptr, 2);
}


long  iniplus::getULong (const char* pszINIPath)
{
    return std::stoul ((const string)getString (pszINIPath));
}

long long iniplus::getULongLong (const char* pszINIPath)
{
    return std::stoull ((const string)getString (pszINIPath));
}


float iniplus::getfloat (const char* pszINIPath)
{
     return std::stof ((const string)getString (pszINIPath));
}
 
double iniplus::getDouble (const char* pszINIPath)
{
     return std::stof ((const string)getString (pszINIPath));
}


//protocolo correios 2931802247702
