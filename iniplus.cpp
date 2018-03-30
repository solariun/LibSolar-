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


iniplus::iniplus (const char* pszINIFileName) : strFileName(pszINIFileName)
{
    
    ifstream* ifsFile;
    
    VERIFY(Util::getFileSize(pszINIFileName) > 0, 100, "Errro, file is zero or does not exist.");
    
    ifsFile = new ifstream (pszINIFileName);
    //int errn = errno;
    
    VERIFY ((ifsFile->rdstate() & std::ifstream::failbit) == 0, 101, "File could not be read becasuse ");
    
    this->isIn = ifsFile;
    
    char chCinbuffer [64 * 1024]; // 64k buffer;
    
    this->isIn->rdbuf()->pubsetbuf(chCinbuffer, sizeof (chCinbuffer));
    
    //Starting creating the structure of the data
    
    parseINI();
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
                VERIFY(strAttribute.length() > 0, 201, "Error, no Attribute value available.");
                
                if (lexItem.nType == attributive_tag)
                {
                    VERIFY(getNextLexicalItem(lexItem) != NULL && lexItem.nType == string_tag, 202, "Syntatic Error, no correct value given");
                    
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
            VERIFY (strPath.length() == 0 && lexItem.nType == session_tag, 200, "Error, Trying to add values with no session open, please revise the ini structure.");
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
    
    isIn->close();
    
    return NULL;
}


//protocolo correios 2931802247702
