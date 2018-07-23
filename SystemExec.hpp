//
//  BidirectExec.hpp
//  MSANTools
//
//  Created by GUSTAVO CAMPOS on 07/06/18.
//  Copyright © 2018 GUSTAVO CAMPOS. All rights reserved.
//

#ifndef BidirectExec_hpp
#define BidirectExec_hpp


#include <sys/times.h>

#include <Util.hpp>
#include <Exception.hpp>
#include <fdstream.hpp>

#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>
#include <cstdint>
#include <map>
#include <list>

using namespace std;


enum _BDEXEC_Exceptions
{
    BDEXEC_NOT_EXECUTING    = 10,
    BDEXEC_PROCESS_RUNNING,
    BDEXEC_PROCESS_NOT_FOKED,
    BDEXEC_SPAWN_FILEP_ERR,
    
    BDEXEC_READ_TIMEOUT     = 20,
    BDEXEC_WRITE_TIMEOUT,   //21
    BDEXEC_READ_ERROR,
    BDEXEC_READ_RET_NULL,
    DBEXEC_READ_TIMEOUT,
    
    BDEXEC_FAIL_STARTING_PROCESS = 30
};

class SystemExecException : public Exception
{
public:
    SystemExecException (std::string strMessage, uint nErrorID);
};

//class BidirectExec;

class SystemExec
{
private:
    unsigned int nReadTimeout = 10;   //seconds
    unsigned int nWriteTimeout = 10;     //seconds
    
    int nReadChild;
    int nWriteChild;
    
    int nWaitSigINT = 1;
    int nWaitSigKill = 1;

    stringbuf  strbufRead;
    iostream   iostreamRead;
    
    boost::fdistream*  fdsRead = nullptr;
    
protected:
    pid_t nPid = 0;
    
    static map<int, SystemExec> nPidObjects;
    
    friend void signalChieldHandler (int nSignal);
    
public:
    
    SystemExec ();
    
    ~SystemExec ();

    void spaw (const string strExecute);

    void kill ();
    
    void setIOTimeouts (unsigned int nReadTimeout , unsigned int nWriteTimeout);
    unsigned int  getReadTimeout ();
    unsigned int  getWriteTimeout ();
    
    bool isReadReady (int nSecTimeout=0);
    bool isWriteReady (int nSecTimeout=0);

    bool isExecuting ();
    
    struct rusage getStatistics(int* nStatLoc = nullptr);
    
    void getStatistics(struct rusage& rUsage, int* nStatLoc = nullptr);
    
    ssize_t read (char* pszReturn, size_t nMaxRead, int nSecTimeout=0);
    ssize_t read (string& strValue, size_t nMaxRead, int nSecTimeout=0);

    ssize_t write (const string strValue, size_t nMaxWrite, int nSecTimeout=0);
    
    size_t Expect (const string strExpected, string &strMatchRet, list<string>& listLines, int nReadTimeout);
    
    string Expect (const string strExpected, string &strMatchRet, int nReadTimeout);

    int ExpectIteractor (const string strExpected, string &strMatchRet, string& strReturn, int nReadTimeout);
    
    size_t readLine (string& strData, int nSecTimeout=0);
};

#endif /* BidirectExec_hpp */
