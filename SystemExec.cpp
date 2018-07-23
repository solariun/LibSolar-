//
//  BidirectExec.cpp
//  MSANTools
//
//  Created by GUSTAVO CAMPOS on 07/06/18.
//  Copyright © 2018 GUSTAVO CAMPOS. All rights reserved.
//

#include "SystemExec.hpp"
#include "Util.hpp"

#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#include <termios.h>
#include <fcntl.h>


#ifdef __linux__
#include <pty.h>
#elif __APPLE__
#include <util.h>
#endif

#include <cstring>
#include <cerrno>
#include <vector>
#include <sstream>

#include <iostream>
#include <fstream>

#include <regex>

static bool boolSignalHandler = false;


void signalChieldHandler (int nSignal)
{
    TRACE << "Chield Notification: ";
    
    int nPid;
    int nStatusLoc;
    struct rusage rUsage;
    
    while ((nPid = wait3(&nStatusLoc, WNOHANG, &rUsage)) > 0)
    {
        TRACE << "Pid:[" << nPid << "], ";
    }
    
    TRACE << "DONE." << endl;
}

constexpr int getPipeReadFD(int (&nFD)[2])
{
    return nFD[0];
}

constexpr int getPipeWriteFD(int (&nFD)[2])
{
    return nFD[1];
}

const char** getExecArgvs(const string& strExec)
{
    vector<string> vecFields = Util::getFields(strExec, " '\"");
    char **ppstrArgv = nullptr;
    
    if (vecFields.size() > 0)
    {
        ppstrArgv = new char*[vecFields.size()+1];
        int nCounter=0;
        
        for (const string& strValue : vecFields)
        {
            ppstrArgv [nCounter++] = new char [strValue.length()+1];
            std::strncpy(ppstrArgv[nCounter-1], strValue.c_str(), strValue.length());
            
            ppstrArgv[nCounter-1][strValue.length()] = '\0';
            
            TRACE << "Add: " << ppstrArgv[nCounter-1] << endl;
        }
        
        TRACE << "Add: NULL Terminated...." << endl;
        ppstrArgv [nCounter] = NULL;
    }
    
    return (const char**) ppstrArgv;
}



SystemExecException::SystemExecException (std::string strMessage, uint nErrorID) : Exception ("BidirectExec", strMessage, nErrorID)
{
    if (boolSignalHandler == false)
    {
        boolSignalHandler = true;
        
        signal (SIGCHLD, signalChieldHandler);
    }
    
}


SystemExec::SystemExec() : iostreamRead(&strbufRead)
{}


void SystemExec::spaw(const std::string strExecute)
{
    Verify(nPid == 0, "There is a process running, please kill first.", BDEXEC_PROCESS_RUNNING, SystemExecException);
    
    int naReadPipe [2];
    int naWritePipe [2];
    
    Verify (pipe(naReadPipe) == 0, "Error creating pipe: " + std::strerror(errno), errno, SystemExecException);
    Verify (pipe(naWritePipe) == 0, "Error creating pipe: " + std::strerror(errno), errno, SystemExecException);
    
    //Creating process
    int master;
   
    Verify ( (nPid = forkpty (&master, NULL, NULL, NULL)) >= 0, "No Fork process created." + std::strerror(errno), BDEXEC_PROCESS_NOT_FOKED, SystemExecException);

    //Verify ( (nPid = fork()) >= 0, "No Fork process created." + std::strerror(errno), BDEXEC_PROCESS_NOT_FOKED, SystemExecException);

    TRACE << "PID: " << nPid << endl;
    
    if ( nPid == 0)
    {
        setsid();
        
        // remove the echo
        struct termios tios;
        tcgetattr(master, &tios);
        tios.c_lflag &= ~(ECHO | ECHONL);
        tcsetattr(master, TCSAFLUSH, &tios);
     
        /*
        //process Child initializations...
        TRACE << "Starting redircting PIDs" << endl;
        
        Verify (dup2(getPipeWriteFD(naReadPipe), fileno(stdout)) >= 0, "Child: Erro redirecting stdout to readPipe write side: [" + std::strerror(errno) + "]", errno, SystemExecException);

        //Verify (dup2(getPipeWriteFD(naReadPipe), fileno(stderr)) >= 0, "Child: Erro redirecting stdout to readPipe write side: [" + std::strerror(errno) + "]", errno, SystemExecException);

        Verify (dup2(getPipeReadFD(naWritePipe), fileno(stdin)) >= 0, "Child: Erro redirecting stdin to writePipe read side: [" + std::strerror(errno) + "]", errno, SystemExecException);
        
        //since stdin and out has been
        //redirected, the remaining
        //must be deleted...
        close (naReadPipe[0]);
        close (naReadPipe[1]);
        close (naWritePipe[0]);
        close (naWritePipe[1]);

        TRACE << "Finished redirecting stdin and stdout" << endl;
         */
        
        /*
         * Since this will be replaced by
         * the new pid, no delete will be
         * needed, this will be done
         * automatically by the Kernel.
         */
        
        
        const char** ppszArgv = getExecArgvs(strExecute);
        
        CLASSLOG << "Application: [" << ppszArgv [0] << "]" << endl << endl;
        
        execvp (ppszArgv[0], (char* const*) &ppszArgv[0]);
        
        CLASSLOG << Util::getLogLikeTimeStamp() << ": Error: " << std::strerror(errno) << endl;
        
        exit(0);
    }
    else if (nPid > 0)
    {
        //process Paratent procedures to read process intercomm
        
        //fcntl(master, F_SETFL, fcntl(master, F_GETFL, 0) | O_NONBLOCK);

        nReadChild  = master; //getPipeReadFD(naReadPipe);
        nWriteChild = master; //getPipeWriteFD(naWritePipe);

        if (fdsRead != nullptr)
            free (fdsRead);
        
        fdsRead = new boost::fdistream (master);
        
        close (getPipeWriteFD(naReadPipe));
        close (getPipeReadFD(naWritePipe));
    }
    
    
}

SystemExec::~SystemExec()
{
    if (fdsRead != nullptr)
    {
        delete fdsRead;
    }
    //TODO - cleaning ups and process kill in case of execution still.
}


/*
 * Timeout set/get funcitons
 */
void SystemExec::setIOTimeouts(unsigned int nReadTimeout, unsigned int nWriteTimeout)
{
    this->nWriteTimeout = nWriteTimeout;
    this->nReadTimeout = nReadTimeout;
}

unsigned int SystemExec::getReadTimeout()
{
    return nReadTimeout;
}

unsigned int SystemExec::getWriteTimeout()
{
    return nWriteTimeout;
}


bool SystemExec::isReadReady(int nSecTimeout)
{
    Verify(nPid > 0, "PID not initializaed, no Child attached.", BDEXEC_NOT_EXECUTING, SystemExecException);
    
    fd_set fdRead;
    fd_set fdError;
    
    if (fdsRead->rdbuf()->in_avail() > 0)
        return true;
    
    struct timeval tvalTimeout;
    tvalTimeout.tv_sec = nSecTimeout;
    tvalTimeout.tv_usec = 10;

    FD_ZERO (&fdRead);
    FD_SET (nReadChild, &fdRead);
    
    FD_ZERO (&fdError);
    FD_SET (nReadChild, &fdError);
    
    int nRet = 0;
    
    if ((nRet = select (nReadChild+1, &fdRead, NULL, &fdError, &tvalTimeout)) <= 0)
    {
        if (nRet == 0)
        {
            if (nSecTimeout > 0)
                Verify (nRet != 0, " " + to_string(nSecTimeout) + " seconds timed-out.", DBEXEC_READ_TIMEOUT, SystemExecException);
        }
        else
        {
            TRACE << "selec error: " << errno << ", [" << std::strerror(errno) << "]" << endl;
            kill();
        }
        
        return false;
    }
    
    NOTRACE << "READREADY: nRet:[" << nRet << "]" << endl << endl;
    
    if (nRet > 0)
    {
        if (FD_ISSET(nReadChild, &fdRead))
        {
            return true;
        }
        else if (FD_ISSET (nReadChild, &fdError))
        {
            return false;
        }
    }
    
    return false;
}


bool SystemExec::isWriteReady(int nSecTimeout)
{
    Verify(nPid > 0, "PID not initializaed, no Child attached.", BDEXEC_NOT_EXECUTING, SystemExecException);
    
    fd_set fdWrite;
    fd_set fdError;
    
    struct timeval tvalTimeout;
    tvalTimeout.tv_sec = nSecTimeout;
    tvalTimeout.tv_usec = 10;
    
    
    FD_ZERO (&fdWrite);
    FD_SET (nWriteChild, &fdWrite);
    
    FD_ZERO (&fdError);
    FD_SET (nWriteChild, &fdError);
    
    int nRet = 0;
    
    if ((nRet = select (nWriteChild+1, NULL, &fdWrite, &fdError, &tvalTimeout)) <= 0)
    {
        if (nRet == 0)
        {
            if (nSecTimeout > 0)
                Verify (nRet != 0, " " + to_string(nSecTimeout) + " seconds timed-out.", DBEXEC_READ_TIMEOUT, SystemExecException);
        }
        else
        {
            TRACE << "selec error: " << errno << ", [" << std::strerror(errno) << "]" << endl;
            kill();
        }
        
        errno = 0;
        
        return false;
    }
    
    if (nRet > 0)
    {
        if (FD_ISSET(nWriteChild, &fdWrite))
        {
            return true;
        }
        else if (FD_ISSET (nWriteChild, &fdError))
        {
            return false;
        }
    }
    

    return false;
}



bool SystemExec::isExecuting()
{
    if (nPid == 0 || nReadChild == 0 || nWriteChild == 0)
        return false;
    
    int nStatLoc;
    getStatistics(&nStatLoc);
    
    if (WIFEXITED(nStatLoc))
        return false;
    
    fd_set fdError;
    
    struct timeval tvalTimeout;
    tvalTimeout.tv_sec = 0;
    tvalTimeout.tv_usec = 100;

    
    FD_ZERO (&fdError);
    FD_SET (nReadChild, &fdError);
    
    
    if (select (nReadChild+1, NULL, NULL, &fdError, &tvalTimeout) < 0)
    {
        CLASSLOG << "selec error: " << errno << ", [" << std::strerror(errno) << "]" << endl;
        return false;
    }
    
    if (FD_ISSET (nReadChild, &fdError))
    {
        return false;
    }
    
    return true;
}


struct rusage SystemExec::getStatistics(int* pnStatLoc)
{
    Verify(nPid > 0, "PID not initializaed, no Child attached.", BDEXEC_NOT_EXECUTING, SystemExecException);
    
    struct rusage rUsage;
    
    int nStatLoc = 0;
    
    wait4(nPid, &nStatLoc, (int) WNOHANG | WUNTRACED, & rUsage);
    
    if (pnStatLoc != NULL)
    {
        *pnStatLoc = nStatLoc;
    }
    
    return rUsage;
}



void SystemExec::getStatistics(struct rusage& rUsage, int* nStatLoc)
{
    
}



ssize_t SystemExec::read(std::string &strValue, size_t nMaxRead, int nSecTimeout)
{
    char pszReturn [nMaxRead + 1];
    
    ssize_t nRetLen = read (pszReturn, nMaxRead, nSecTimeout);
    
    if (nRetLen == 0)
    {
        strValue.clear();
    }
    else
    {
        strValue.assign(pszReturn, nRetLen);
    }
    
    return nRetLen;
}



__inline__ ssize_t SystemExec::read(char* pszReturn, size_t nMaxRead, int nSecTimeout)
{
    Verify (pszReturn != nullptr, "Error, data is NULL", BDEXEC_READ_RET_NULL, SystemExecException);
    Verify(nPid > 0, "PID not initializaed, no Child attached.", BDEXEC_NOT_EXECUTING, SystemExecException);
    
    if (isReadReady(nSecTimeout))
    {
        ssize_t nRetLen = 0;
        
        errno = 0;
        
        //nRetLen = fread ((void*) pszReturn, 1, nMaxRead, fpReadChild);
        //nRetLen = ::read (nReadChild, (void*) pszReturn, (size_t) nMaxRead);
        fdsRead->read(pszReturn, (size_t) nMaxRead);
        
        nRetLen = fdsRead->gcount();
        
        NOTRACE  << "Len: " << nRetLen << " bytes, Buffer: " << fdsRead->rdbuf()->in_avail() << "                \r" ;
        
        if (errno != 0 || (fdsRead->fail() && fdsRead->eof() != true))
        {
            Verify (errno == 0, "Error (" + to_string(errno) +") writing data: " + strerror(errno), BDEXEC_READ_ERROR, SystemExecException);
        }
        
        return nRetLen;
    }
    
    return 0;
}



ssize_t SystemExec::write(const std::string strValue, size_t nMaxWrite, int nSecTimeout)
{
    Verify(nPid > 0, "PID not initializaed, no Child attached.", BDEXEC_NOT_EXECUTING, SystemExecException);

    if (isWriteReady(nSecTimeout))
    {
        ssize_t nRetLen  = 0;
        
        
        TRACE << ">>> WRITE<<< -> Size: (" << nMaxWrite << "):[" << strValue << "]" << endl;
        
        errno=0;
        
        //nRetLen = fwrite (strValue.c_str(),  1, nMaxWrite, fPipe);
        nRetLen = ::write (nWriteChild,  strValue.c_str(),  nMaxWrite);
        
        Verify (errno == 0, "Error (" + to_string(errno) +") writing data: " + strerror(errno), BDEXEC_READ_ERROR, SystemExecException);
        
        return nRetLen;
    }
    
    return 0;
}



void SystemExec::kill()
{
    if (nPid == 0) return;
    
    int nStatLoc = 0;
    
    getStatistics(&nStatLoc);

    nPid = 0;
    
    if (WIFEXITED(nStatLoc))
        return;
    
    ::kill ((pid_t) nPid, SIGINT);
    
    delete fdsRead;
    
    fdsRead = nullptr;
    
    close (nReadChild); //since nReadChild and nWriteChild point out to the same FD
                        //it is safe to close one.
}


int SystemExec::ExpectIteractor(const std::string strExpected, std::string &strMatchRet, std::string &strData, int nReadTimeout)
{
    Verify(nPid > 0, "PID not initializaed, no Child attached.", BDEXEC_NOT_EXECUTING, SystemExecException);
    
    char chChar;
    
    regex rgxMatch (strExpected);
    
    smatch smatchData;
    
    size_t nRetLen;
    
    strMatchRet = "";
    
    strData.clear();
    
    //vector<string> vecLookup = Util::getFields(strExpected, "|");
    
    while (isReadReady(nReadTimeout) && (nRetLen = read(&chChar, sizeof (chChar))) > 0)
    {
        if (chChar == '\n')
        {
            NOTRACE << "Return: [" << strData << "]" << endl;
            
            return 0;
        }
        if (chChar != '\t' && chChar < ' ')
        {
            continue;
        }
        
        strData += chChar;
        
        NOTRACE << "Received so far: [ " << strData << "]" << endl;
        
        
        if (regex_search  (strData, rgxMatch))
        {
            regex_search  (strData, smatchData, rgxMatch);
            
            strMatchRet = smatchData[0];
            
            TRACE << "Found: [" << strMatchRet << "]" << endl << endl;
            
            return 1;
            break;
        }
        
        
        NOTRACE << "Char [" << chChar << "], Line: [" << strMatchRet.length() << "] Size :("  << strData.length() << "): [" << strData << "]"<< endl;
    }
    
    NOTRACE << "Return false : [" << strData << "]" << endl;
    
    return -1;
}


string SystemExec::Expect (const string strExpected, string &strMatchRet, int nReadTimeout)
{
    string strLine;
    
    stringstream strsReturn;
    
    while (isReadReady(nReadTimeout) && ExpectIteractor(strExpected, strMatchRet, strLine, nReadTimeout) == 0)
    {
        strsReturn << strLine << '\n';
    }
    
    strsReturn << strLine;
    
    return strsReturn.str();
}


size_t SystemExec::Expect(const std::string strExpected, string &strMatchRet, list<string>& listLines, int nReadTimeout)
{
    Verify(nPid > 0, "PID not initializaed, no Child attached.", BDEXEC_NOT_EXECUTING, SystemExecException);

    listLines.clear();
    string strLine;
    
    while (isReadReady(nReadTimeout) && ExpectIteractor(strExpected, strMatchRet, strLine, nReadTimeout) == 0)
    {
        listLines.push_back(strLine);
    }

    listLines.push_back(strLine);
    
    return strLine.size();
}


/*
size_t SystemExec::Expect(const std::string strExpected, string &strMatchRet, list<string>& listLines, int nReadTimeout)
{
    Verify(nPid > 0, "PID not initializaed, no Child attached.", BDEXEC_NOT_EXECUTING, SystemExecException);

    char chChar;
    string strData;
    
    regex rgxMatch (strExpected);
    
    smatch smatchData;
    
    size_t nRetLen;
    
    strMatchRet = "";

    //vector<string> vecLookup = Util::getFields(strExpected, "|");
    
    listLines.clear();
    
    while (isReadReady(nReadTimeout) && (nRetLen = read(&chChar, sizeof (chChar))) > 0)
    {
        if (chChar == '\n')
        {
            listLines.push_back (strData);
            
            NOTRACE << "Addline: [" << strData << "]" << endl;
            
            strData = "";
        }
        if (chChar != '\t' && chChar < ' ')
        {
            continue;
        }
        
        strData += chChar;
        
        NOTRACE << "Received so far: [ " << strData << "]" << endl;

        
        if (regex_search  (strData, rgxMatch))
        {
            regex_search  (strData, smatchData, rgxMatch);
            
            strMatchRet = smatchData[0];
            
            NOTRACE << "Found: [" << strMatchRet << "]" << endl << endl;
            
            break;
        }
        
    
        NOTRACE << "Char [" << chChar << "], Line: [" << strMatchRet.length() << "] Size :("  << strData.length() << "): [" << strData << "]"<< endl;
    }
    
    if (strData.length() > 0)
        listLines.push_back(strData);
    
    NOTRACE << "Return: (" << listLines.size() << ") lines..." << endl << endl;
    
    return listLines.size();
}
*/


size_t SystemExec::readLine(string& strData, int nSecTimeout)
{
    Verify(nPid > 0, "PID not initializaed, no Child attached.", BDEXEC_NOT_EXECUTING, SystemExecException);

    size_t nRetLen=0;
    char chChar;

    strData = "";

    while ((nRetLen = read(&chChar, sizeof (chChar),  nSecTimeout)) > 0)
    {
        //NOTRACE << "Char: [" << chChar << "], strData: [ " << strData << "]" << endl;

        if (chChar == '\n')
        {
            //NOTRACE << "Returning: [" << strData << "]" << endl;
            
            break;
        }
        if (chChar != '\t' && chChar < ' ')
        {
            continue;
        }
        
        strData += chChar;
    }
    
    //NOTRACE << "nRetLen: [" << nRetLen << "]" << endl;
    
    return strData.length();
}






