/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <iomanip>
#include "MyLog.h"

Log *Log::ms_log = NULL;

Log::Log(std::string fileName)
: m_logout(fileName.c_str(), std::ios::out | std::ios::app)
{

}

Log& Log::instance()
{
    //    if(NULL == ms_log)
    //    {
    //        static Log ms_log = new Log("./log.txt");
    //        if(!ms_log->m_logout.good())
    //        {
    //            std::cout << "log file \"./log.txt\"" << std::endl;
    //        }
    //    }
    //    return *ms_log;
    static Log slog("./log.txt");
    if(!slog.m_logout.good())
    {
        std::cout << "log file \"./log.txt\"" << std::endl;
    }
    return slog;
}

void Log::release()
{
    if(NULL != ms_log)
    {
        delete ms_log;
        ms_log = NULL;
    }
}

std::string Log::getTypeStr(int level)
{
    std::string str = "";
    switch(level)
    {
        case INFO_LEVEL:
            return "[info]";
            break;
        case ERR_LEVEL:
            return "[error]";
            break;
        default:
            return "[log]";
            break;
    }
}

void Log::showMat(const std::string &matName, const Mat &test)
{
    if(!test.empty())
    {
        if(!m_logout.good())
        {
            std::cout << "log file \"./log.txt\"" << std::endl;
        }
        else
        {
            log(INFO_LV, matName + ":");
            if(test.channels() == 4)
            {
                MatConstIterator_<Vec4b> colorit, colorend;
                int count = 0;
                for(colorit = test.begin<Vec4b>(), colorend = test.end<Vec4b>();colorit != colorend;++colorit)
                {
#ifdef DEBUG
                    printf("(%3u, %3u, %3u, %3u) ", (*colorit)[0], (*colorit)[1], (*colorit)[2], (*colorit)[3]);
#endif
                    m_logout << "(" << std::setw(3) << (unsigned int) (*colorit)[0] << "," << std::setw(3) << (unsigned int) (*colorit)[1] << "," << std::setw(3) << (unsigned int) (*colorit)[2] << "," << std::setw(3) << (unsigned int) (*colorit)[3] << ")";
                    if(++count % test.cols == 0)
                    {
                        m_logout << "\n";
#ifdef DEBUG
                        printf("\n");
#endif
                    }
                }
            }
            else
            {
                MatConstIterator_<Vec3b> colorit, colorend;
                int count = 0;
                for(colorit = test.begin<Vec3b>(), colorend = test.end<Vec3b>();colorit != colorend;++colorit)
                {
#ifdef DEBUG
                    printf("(%3u, %3u, %3u) ", (*colorit)[0], (*colorit)[1], (*colorit)[2]);
#endif
                    m_logout << "(" << std::setw(3) << (unsigned int) (*colorit)[0] << "," << std::setw(3) << (unsigned int) (*colorit)[1] << "," << std::setw(3) << (unsigned int) (*colorit)[2] << ") ";
                    if(++count % test.cols == 0)
                    {
                        m_logout << "\n";
#ifdef DEBUG
                        printf("\n");
#endif
                    }
                }
            }
        }
    }
}