//
//  InMemDB.hpp
//  LibSolar++
//
//  Created by GUSTAVO CAMPOS on 13/06/18.
//  Copyright © 2018 GUSTAVO CAMPOS. All rights reserved.
//

#ifndef InMemDB_hpp
#define InMemDB_hpp

#include "Exception.hpp"
#include <stdio.h>
#include <map>
#include <string>
#include <list>
#include <vector>



namespace InMemDBType
{

    enum enumTP
    {
        InMemDB_TP_TEXT = 1,
        InMemDB_TP_INTEGER,
        InMemDB_TP_REAL,
        InMemDB_TP_BLOB
    };

    /* Interface */
    class Field
    {
    protected:
        string strName;
        int typeField;
        int nID;
    };
    
    /* Data Types */
    class FieldValue : public Field
    {
    protected:
        string strValue;
        long double doubleValue;
    };
    
    
}

using namespace InMemDBType;



/*
 *  Database defination paramiter
 */
class Database : Field
{
protected:
    map<string, Field>      mapFiledSet;
    uint64_t                nRecords;
    
public:
    
    void addField (const string& strName, int nFieldType);
};


/*
 * RecordSet def for selecting returning
 */

class RecordSets
{

};


/*
 * Database Exception
 */



/*
 * Main In Memory DB Class and controler
 */

class InMemDB : public Exception, public RecordSets
{
private:
    
    map<string, Database> mapDatabases;
    
public:

    InMemDB ();
    
    void addDatabase(const Database& dataBase);
    
};

#endif /* InMemDB_hpp */
