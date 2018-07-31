//
//  SelectData.hpp
//  LibSolar++
//
//  Created by GUSTAVO CAMPOS on 30/07/18.
//  Copyright © 2018 GUSTAVO CAMPOS. All rights reserved.
//

#ifndef SelectData_hpp
#define SelectData_hpp

#include <Exception.hpp>
#include <Util.hpp>
#include <map>
#include <vector>
#include <list>

#include <stdio.h>


class SelectDataException : public Exception
{
public:
    SelectDataException (std::string strMessage, uint nErrorID);
};


class SelectData
{
private:
    map<string, vector<string>> mapSourceFields;
    map<string, list<vector<string>>> mapSourceData;
    
public:
    
    
};

#endif /* SelectData_hpp */
