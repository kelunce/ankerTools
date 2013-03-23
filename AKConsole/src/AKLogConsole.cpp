#include <time.h>
#include <string>
#include "../include/AKLogConsole.h"

using namespace aktool;


CAKLogConsole::CAKLogConsole(void)
{
    // 获取配置文件路径
    char szPath[MAX_PATH] = {0};
    GetModuleFileName(NULL,szPath,sizeof(szPath));
    char *p=strrchr(szPath, '\\');
    *p ='\0';
    strcat(szPath,"\\lcm.conf");

    // 加载控制台开关
    int nConsole = GetPrivateProfileInt( "Debug", "Console", 0, szPath);

    // 加载启动调试配置
    int nDebug = GetPrivateProfileInt( "Debug", "DebugOnce", 0, szPath);

    // 加载分配控制配置
    int nAlloc = GetPrivateProfileInt( "Debug", "Alloc", 0, szPath);

    // 校正控制台配置
    if(nAlloc != 0) nConsole = 1;

    // 加载日志设置
    char szFileName[MAX_PATH] = {0};
    int nLog = GetPrivateProfileInt("Log","Open",0,szPath);
    DWORD dwLen = GetPrivateProfileString("Log","FileName","",szFileName,sizeof(szFileName),szPath);

    // 校正日志
    if(dwLen == 0) nLog = 0;

    // 调试
    if(nDebug != 0)
        DebugBreak();

    // 分配控制台
    if(nAlloc != 0)
    {
        m_bAlloc = (bool)AllocConsole();
    }

    // 控制台句柄
    if(nConsole != 0)
    {
        m_conHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if(m_conHandle == INVALID_HANDLE_VALUE)
        {        
            exit(EXIT_FAILURE);
        }

        CONSOLE_SCREEN_BUFFER_INFO sbInfo;
        if(!GetConsoleScreenBufferInfo(m_conHandle, &sbInfo))
        {        
            exit(EXIT_FAILURE);
        }
        // 保存当前控制台信息
        m_stdConsoleAttributes = sbInfo.wAttributes;
    }    

    // 日志
    m_bLog = false;
    if(nLog != 0)
    {
        if(m_File.is_open())
            m_File.close();

        // 生成带时间格式文件名
        time_t now;
        time(&now);
        struct tm tm_now;
        localtime_s( &tm_now, &now);
        char szName[MAX_PATH] = {0};
        strftime(szName, MAX_PATH, "%Y-%m-%d", &tm_now);
        strcat_s(szName,MAX_PATH,szFileName);

        // 打开文件
        m_bLog = true;
        locale::global(locale(""));
        m_File.open(szName ,(ios::out|ios::app));
        locale::global(locale("C"));
    }
}

CAKLogConsole::~CAKLogConsole(void)
{
    if(m_conHandle != INVALID_HANDLE_VALUE)
    {
        SetConsoleTextAttribute(m_conHandle, m_stdConsoleAttributes);
    }

    if(m_bAlloc)
    {
        FreeConsole();
    }

    if(m_bLog)
    {
        if(m_File.is_open())
            m_File.close();
    }
}

void CAKLogConsole::Log(char * message, LogLevel level)
{
    // white color will be used as a default color for writing to console
    // other specific messages (WARNING, ERROR) will have specific colors
    ConsoleColor color = WHITE;

    string strLog = message;
    strLog += "\n";
    string strLogPrex;

    if(level == en_SUCCESS)
    {
        color = GREEN;
    }
    else if(level == en_WARNING)
    {
        color = YELLOW;
        strLogPrex = "[-] ";
        strLog = strLogPrex + strLog;
    }
    else if(level == en_ERR || level == en_CRITICAL_ERROR)
    {
        color = RED;
        strLogPrex = "[ERROR] ";
        strLog = strLogPrex + strLog;
    }
    else if(level == en_DEBUG)
    {
        color = WHITE;
        strLogPrex = "[DEBUG] ";
        strLog = strLogPrex + strLog;
    }

    WriteColored(strLog.c_str(), color);
}

void CAKLogConsole::WriteColored(const char * message, ConsoleColor color)
{
    // 输入日志
    if (m_bLog && m_File.is_open())
    {
        // 生成带时间格式日志
        time_t now;
        time(&now);
        struct tm tm_now;
        localtime_s( &tm_now, &now);

        char timestr[32];
        strftime(timestr, 32, "[%Y-%m-%d %H:%M:%S]: ", &tm_now);

        m_File << timestr<<message << std::endl;

        m_File.flush();
    }

    // 没有控制台
    if (m_conHandle == INVALID_HANDLE_VALUE)
    {
        return ;
    }

    // default is "no color" - std console text attributes
    unsigned short wTextAttribute = m_stdConsoleAttributes;

    switch(color)
    {
    case WHITE:
        // WHITE = RGB + intensity
        wTextAttribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case GREEN:
        wTextAttribute = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;
    case YELLOW:
        wTextAttribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;
    case RED:
        wTextAttribute = FOREGROUND_RED | FOREGROUND_INTENSITY;
        break;
    }

    SetConsoleTextAttribute(m_conHandle, wTextAttribute);
    //std::cout << message << std::endl;

    DWORD dwOut;
    WriteConsole(m_conHandle,message,strlen(message),&dwOut,NULL);

    // return console to "standard state" - not really necessary, but if unexpected failure occurrs, console might stay in a "colored" state
    SetConsoleTextAttribute(m_conHandle, m_stdConsoleAttributes);
}

void CAKLogConsole::LogLastError( char * strMessage,char * strFile,int nLine )
{
    char pInfo[1024];
    sprintf_s(pInfo,sizeof(pInfo),"%s %d:%d %s",strFile,nLine ,GetLastError(),strMessage);
    Log(pInfo);
}

void CAKLogConsole::LogFormat( const char *szFormat, ... )
{
    static char StrDebug[2048];
    ZeroMemory(StrDebug,sizeof(StrDebug));
    va_list va;
    va_start(va, szFormat);
    vsprintf_s(StrDebug,sizeof(StrDebug),szFormat, (va_list)va);
    va_end(va);
    Log(StrDebug);
}

bool CAKLogConsole::ReadLog( char * strFile,unsigned char byLine,char *szBuff,unsigned short wBuffSize )
{    
    ifstream file(strFile);
    if(!file.is_open())
        return false;

    bool bReadAll = false;
    if(byLine == 0) bReadAll = true;

    string strContent;
    int nCurrLen = 0;
    while (getline(file,strContent))
    {
        int nLen = strContent.length();
        if(nCurrLen + nLen >= wBuffSize)
            nLen = wBuffSize - nCurrLen;

        CopyMemory((void*)(szBuff + nCurrLen),strContent.c_str(),nLen);
        nCurrLen += nLen;

        if(nCurrLen >= wBuffSize - 1)
            break;

        if(!bReadAll)
        {
            byLine -= 1;
            if(byLine == 0)
                break;
        }
    }   
    szBuff[nCurrLen] = '\0';

    //file.getline(szMsg,1024);
    return true;
}

void CAKLogConsole::DumpMemory( char * strFile,void * pBuffer, unsigned short wSize )
{
    if((pBuffer == 0) || (wSize == 0)) return ;

    ofstream File;
    string strLog;

    locale::global(locale(""));
    File.open(strFile ,(ios::out|ios::app));
    locale::global(locale("C"));  

    if(!File.is_open()) return ;

    unsigned char *pCharBuff = (unsigned char *)pBuffer;    
    char szLine[512]={0};
    int nLineLen = 0;
    const int nLen = 20;
    for (int i = 0 ;i < wSize; ++i)
    {
        char szByte[6];
        sprintf(szByte,"0x%02x ",int(*(pCharBuff + i)));
        strcat(szLine,szByte);
        nLineLen++;
        if(nLineLen == nLen)
        {
            strLog = szLine;
            File<<strLog.c_str()<<std::endl;
            nLineLen = 0;
            ZeroMemory(szLine,sizeof(szLine));
        }
    }

    if( 0 != (wSize % nLen) )
    {
        strLog = szLine;
        File<<strLog.c_str()<<std::endl;
    }
}

bool CAKLogConsole::ReadDumpMemory( char * strFile,unsigned char byLine,void *pBuffer, unsigned short wSize )
{
    char *szContent = new TCHAR[wSize*10];
    int nRead = 0;
    if(!ReadLog(strFile,byLine,szContent,wSize*10))
        return false;

    unsigned char *pCharBuff = (unsigned char *)pBuffer;    

    for (int i = 0; i < wSize; ++i)
    {
        int nValue = 0;
        sscanf(szContent + nRead,"0x%02x ",&nValue);
        pCharBuff[i] = nValue;
        nRead += 5;
    }
    delete [] szContent;
    return true;
}

CAKLogConsole & CAKLogConsole::GetAKLogInstance()
{
	static CAKLogConsole g_Console;
	return g_Console;
}
