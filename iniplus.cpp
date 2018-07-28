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
#include <cctype>

iniplusException::iniplusException (std::string strMessage, uint nErrorID): Exception ("iniplus", strMessage, nErrorID)
{}


iniplus::iniplus (const string strINIFileName) : strFileName(strINIFileName)
{
    
    ifstream* ifsFile;
    
    Verify(Util::getFileSize(strINIFileName) > 0, "Errro, file is zero or does not exist.", EXCEPT_INI_FILE_DOSENT_EXIST_OR_ZERO, iniplusException);
    
    ifsFile = new ifstream (strINIFileName);
    //int errn = errno;
    
    Verify((ifsFile->rdstate() & std::ifstream::failbit) == 0, "File could not be read.", EXCEPT_INI_FILE_NOT_READ, iniplusException);
    
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
    string strTempValue = "";
    
    uint nType = none_tag;
    
    if (strPath.length() > 0)
    {
        NOTRACE << "Entering level (" << nDepth << ") [" << strPath << "]" << endl;
    }
    
    while (getNextLexicalItem(lexItem) != NULL)
    {
        NOTRACE  << ">>>>> PARSER <<<<< :" << "strPath: [" << strPath.length() << "] Received Lex Item: (" << lexItem.nType << ") [" << lexItem.strValue << "]" << endl;
        
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
                    NOTRACE << "---------------------------------" << endl;
                    NOTRACE << "Leving Level " << nDepth << "..." << endl << endl;

                    return;
                }
            }
            if (nType == value_tag)
            {
                Verify (strAttribute.length() > 0, "Error, no Attribute value available.", EXCEPT_INI_INVALID_ATTRIBUTE_VALUE, iniplusException);
                
                if (lexItem.nType == attributive_tag)
                {
                    Verify (getNextLexicalItem(lexItem) != NULL && lexItem.nType == string_tag, "Syntatic Error, no correct value given", EXCEPT_INI_SYNT_ERROR_INVALID_VALUE, iniplusException);
                    
                    strTempValue = strPath + "." + strAttribute;
                    
                    mapIniData.insert (pair<string, string> (Util::strToUpper(strTempValue), lexItem.strValue));
                    
                    NOTRACE << "Adding: [" << strPath << "." << strAttribute << "] = [" << lexItem.strValue << "]" << endl;
                    
                    nType = none_tag;
                }
                else if (lexItem.nType == open_struct_tag)
                {
                    parseINI(strPath + "." + strAttribute, nDepth + 1);
                    
                    NOTRACE << "Returned to: [" << strPath << "]" << endl;
                    nType = none_tag;
                }
            }
        }
        else
        {
            Verify (strPath.length() == 0 && lexItem.nType == session_tag, "Error, Trying to add values with no session open, please revise the ini structure.", EXCEPT_INI_FAIL_ADD_VALUE_NO_SESSION, iniplusException);
        }
    }
}

inline bool isAlphaData (char chChar)
{
    return (isdigit (chChar) || isalpha(chChar) || Util::isBetween(chChar, "_-\\", 3));
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
    
    
    while (isIn->good())
    {
        chChar = isIn->get();
        stzTemp [0] = chChar; //securely stringfing chChar...
        
        NOTRACE  "boolString: [" << boolString << "], " << chChar << "(" << static_cast<int>(chChar) << ") tp: " << nType << " : String: [" << strData << "]" <<  " EOF: " << isIn->eof () << endl;
        
        
        if (boolDiscartComment == true)
        {
            if (chChar == '\n')
            {
                boolDiscartComment = false;
                isIn->putback(chChar);
            }
            
            NOTRACE << chChar;
        }
        else if (nType != string_quote_tag && chChar == '#')
        {
            boolDiscartComment = true;
            
            NOTRACE << "Starting Discarting..." << endl;
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
            else if (isAlphaData(chChar) == true)
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
            else if (isgraph (chChar) != 0)
            {
                nType = string_line_tag;
                
                isIn->putback(chChar);
                
                boolString = true;
            }
        }
        else if (nType == string_tag && (! isAlphaData (chChar)))
        {
            nType = none_tag;
            
            iniParserItem.assign (string_tag, strData);
            
            return &iniParserItem;
        }
        else if (nType == string_quote_tag && boolOverload == false && Util::isBetween(chChar, "\"\\", 2))
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
            if (nType == string_quote_tag || chChar == '\t' || chChar >= ' ') strData += chChar;
            
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
bool iniplus::Exists (const string strINIPath)
{
    return mapIniData.find(Util::strToUpper (strINIPath)) != mapIniData.end() ? true : false;
}



/*
 * Conversion types lookup functions functions 
 */

void iniplus::getStringFromRef (string& strRet, const string strINIPath)
{
    strRet = getRawString(strINIPath);
}

/*
    getRawString
 
    This funciton will return the same data available at the INI without interpreting
    any value at all....
 
*/
string iniplus::getRawString (const string strINIPath)
{
    
    map<string,string>::iterator mapPos;
    
    string strValue = strINIPath;
    
    mapPos = mapIniData.find( Util::strToUpper(strValue).c_str());
    
    NOTRACE << "Looking for: [" << Util::strToUpper(strValue) << "]" << endl << endl;
    
    Verify (mapPos != mapIniData.end(), "Error, element (" + Util::strToUpper(strValue) + ") was not found.", EXCEPT_INI_NO_INIPATH_FOUND, iniplusException);
    
    return mapPos->second;
}



int iniplus::getInteger (const string strINIPath)
{
    return std::stoi (((const string) getRawString (strINIPath)), nullptr, 0);
}


/*
 *  The Long and Long Long conversion hereby implemented
 *  are capable of selecting from decimal and hexadecimal automatically
 *  and extra is supplied for binary only conversion. I know one can argue
 *  the auto select could be implemented for binary too, but it could pose
 *  an extra layer of processing, losing precious time that could be, otherwise,
 *  used for processing, thus, I decided against it.
 */

long  iniplus::getLong (const string strINIPath)
{
    return std::stol ((const string)getRawString (strINIPath), nullptr, 0);
}

long long iniplus::getLongLong (const string strINIPath)
{
    return std::stoll (getRawString (strINIPath).c_str(), nullptr, 0);
}

long long iniplus::getULongLongFromBinary (const string strINIPath)
{
    return std::stoll (getRawString (strINIPath).c_str(), nullptr, 2);
}


long  iniplus::getULong (const string strINIPath)
{
    return std::stoul (getRawString (strINIPath).c_str());
}

long long iniplus::getULongLong (const string strINIPath)
{
    return std::stoull (getRawString (strINIPath).c_str());
}


float iniplus::getfloat (const string strINIPath)
{
     return std::stof (getRawString (strINIPath).c_str());
}
 
double iniplus::getDouble (const string strINIPath)
{
     return std::stof (getRawString (strINIPath).c_str());
}



/*
    getString
 
    This is by far the most advanced capability from this lib, since
    it can interpret 3 levels of information, replacing any ${look_up_date},
    for one found on the 3 level of looking up data.
 
    The looking up priority is as followed:
 
 Dynamic:
    1. pVarMap, by default it is null, but once providade, it can overide
    a INI path while it is in use by the getString, no data will EVER change
    the in memory data stored.
 
 Statics:
    2. The in memory iniPath data stored, provided by the INI read itself.
 
    3. the Environment variables, is the lower orther and can be overide by both
        iniPath and pVarMap.
 
*/
std::string iniplus::getString(string strINIPath, map<std::string, std::string> *pVarMap)
{
    string strValueof = getRawString(strINIPath);
    string strVariableName;
    string strReplace;
    size_t nStart, nStop;

    
    while ((nStart= strValueof.find("${", 0)) !=  string::npos)
    {
        NOTRACE << endl << endl << "Value: [" << strValueof << "]" << endl;

        if ((nStop = strValueof.find("}", 0)) != string::npos)
        {
            strVariableName = strValueof.substr (nStart+2, (nStop - nStart) - 2);
            strReplace = "";

            if (pVarMap != nullptr && pVarMap->find(strVariableName) != pVarMap->end())
            {
                strReplace = (*pVarMap)[strVariableName];
            }
            else if (Exists(strVariableName.c_str()))
            {
                strReplace = getRawString(strVariableName.c_str());
            }
            else if (getenv(strVariableName.c_str()) != nullptr)
            {
                strReplace = getenv(strVariableName.c_str());
            }
            
            NOTRACE << "Found nStart: (" << nStart << "), nStop: (" << nStop << "), Variable: [" << strVariableName << "] = [" << strReplace << "]" << endl;
            strValueof.replace(nStart, (nStop-nStart)+1, strReplace);
        }
    }
    
    NOTRACE "String: [" << strValueof << "]" << endl;
    
    return strValueof;
}



//protocolo correios 2931802247702
