#pragma once

#ifndef AKLog
#define AKLog aktool::CAKLogConsole::GetAKLogInstance()
#endif

#include <windows.h>
#include <iostream>
#include <fstream>


namespace aktool
{
	using namespace std;

	// 日志类,暂无重定位输出输入功能
	class CAKLogConsole
	{
		// 类型定义
	public:
		typedef enum _LogLevel
		{
			en_HEADER,
			en_DEBUG,
			en_INFO,
			en_SUCCESS,
			en_WARNING,
			en_ERR,
			en_CRITICAL_ERROR,
		} LogLevel;

		// 控制台文字颜色
		typedef enum _ConsoleColor
		{
			GREEN,
			RED,
			YELLOW,
			WHITE,
		} ConsoleColor;

		// 构造与析构
	public:

		CAKLogConsole(void);

		~CAKLogConsole(void);

		// 对外接口
	public:
		// 输出
		void Log(char * message, LogLevel level = en_WARNING);

		// 输出GetLastError
		void LogLastError( char * strMessage,char * strFile ,int nLine);

		// 输出按指定格式
		void LogFormat(const char *szFormat, ...);

		// 控制台输出 
		void WriteColored(const char * message, ConsoleColor color);

		// 读日志
		bool ReadLog(char * strFile,unsigned char byLine,char *szBuff,unsigned short wBuffSize);

		// 打印内存
		void DumpMemory(char * strFile,void * pBuffer, unsigned short wSize);

		// 读取内存数据
		bool ReadDumpMemory(char * strFile,unsigned char byLine,void *pBuffer, unsigned short wSize);

		// 获取本类实例
		static CAKLogConsole &GetAKLogInstance();

	private:
		// 控制台句柄
		HANDLE  m_conHandle;

		// 控制台属性
		USHORT  m_stdConsoleAttributes;

		// 是否分配了控制台
		bool    m_bAlloc;

		// 是否写日志
		bool    m_bLog;

		// 日志流
		ofstream m_File;
	};
}

#ifndef _BULID_AKCONSOLE_LIB
    #ifdef _DEBUG
        #pragma comment(lib,"AKConsole_D.lib")
    #else
        #pragma comment(lib,"AKConsole.lib")
    #endif
#endif