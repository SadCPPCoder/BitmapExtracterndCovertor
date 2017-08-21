/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyLog.h
 * Author: zhangbob
 *
 * Created on 2017年6月20日, 上午10:07
 */

#ifndef MYLOG_H
#define MYLOG_H

#include <fstream>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <sstream>

using namespace std;
using namespace cv;

#define CLOG (__LINE__), (__FUNCTION__)
#define ERR_LV CLOG, (ERR_LEVEL)
#define INFO_LV CLOG, (INFO_LEVEL)
#define LOG Log::instance().log

//#define DEBUG
#define INFO_LEVEL  (0)
#define ERR_LEVEL   (1)
#define LOG_LEVEL   (ERR_LEVEL)

class Log
{
public:
    Log(std::string fileName);
    template<class T>
    void show(T t);
    template<class ...Args>
    void log(const int line, const string& function, int level, Args... args);
    void showMat(const std::string &matName, const Mat &test);
    std::string getTypeStr(int level);
public:
    static Log& instance();
    static void release();
private:
    static Log *ms_log;
    std::ofstream m_logout;
    std::ostringstream m_oSS;
};

template<class T>
void Log::show(T t)
{
    m_oSS << t;
}

template<class ...Args>
void Log::log(const int line, const string& function, int level, Args... args)
{
    m_oSS.str("");
    m_oSS << "[" << __DATE__ << " " << __TIME__ << "-" << line << " line-" << function << "]" << getTypeStr(level) << " ";
    
    int arr[] = {(show(args), 0)...};
    m_oSS << std::endl;
    
    if(level >= LOG_LEVEL)
    {
        std::cout << m_oSS.str();
    }
    
    if(!m_logout.good())
    {
        std::cout << "log file \"./log.txt\"" << std::endl;
    }
    else
    {
        m_logout << m_oSS.str();
    }
}

#endif /* MYLOG_H */

