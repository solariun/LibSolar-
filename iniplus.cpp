//
//  iniplus.cpp
//  LibSolar++
//
//  Created by GUSTAVO CAMPOS on 26/03/18.
//  Copyright © 2018 GUSTAVO CAMPOS. All rights reserved.
//

#include "iniplus.hpp"
#include "Util.hpp"


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



void iniplus::parseINI ()
{
    iniParserItemRet lexItem;
    
    while (getNextLexicalItem(lexItem) != NULL)
    {
        cerr << "Received Lex Item: (" << lexItem.inieType << ") " << lexItem.strValue << endl;
    }
}


iniParserItemRet* iniplus::getNextLexicalItem (iniParserItemRet& iniParserItem)
{
    string  strData = "";
    
    char    chChar = '\0';
    bool    bAddNext = false;
    
    if (isIn->eof()) return NULL;
    
    bool    boolString = false;
    
    while (isIn->good())
    {
        chChar = isIn->get();
        
        //cerr << chChar << " tp: " << nType << " : String: [" << strData << "]" <<  " EOF: " << isIn->eof () << endl;
        
        if (nType == none_tag)
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
            //else if (chChar)
        }
        else if (nType == open_session_tag && chChar == ']')
        {
            nType = none_tag;
            
            iniParserItem.assign (session_tag, strData);
            
            return &iniParserItem;
        }
        else if (boolString == true)
        {
            strData += chChar;
        }
    }
    
    isIn->close();
    
    return NULL;
}


