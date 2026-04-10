/**
  **************************** Copyright ******************************
  *
  *       (C) Copyright 2025, China, IFR Laboratory,DreamTeam.
  *                         All Rights Reserved
  *
  * FileName 	: ifr_log.h
  * Version		: v1.0
  * Author		: csr
  * Date		: 2025-11-10
  * Description	: 基于 ifr_usart 的非阻塞日志系统。支持队列发送、分级打印和宏定义一键启停。
  *********************************************************************
  */
#ifndef __IFR_LOG_H
#define __IFR_LOG_H

#include "ifr_usart.h"
#include <stdio.h>
#include <stdarg.h>

#define IFR_LOG_ENABLE          1
#define IFR_LOG_MAX_LEN         256 
#define IFR_LOG_USE_TIMESTAMP   1
#include <string.h>
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))

class IFR_Log_ClassDef {
private:
    IFR_USART_ClassDef* Log_usart;

public:
    IFR_Log_ClassDef() : Log_usart(NULL) {}

    void Init(IFR_USART_ClassDef* usart_obj);

    void Printf(const char* prefix, const char* format, ...);
};

// 声明外部全局单例
extern IFR_Log_ClassDef IFR_Logger;

#if IFR_LOG_ENABLE == 1
    /**
      * @brief 日志打印宏 (使用方式和 printf 完全一样)
      * LOG_INFO: 普通信息
      * LOG_WARN: 警告信息
      * LOG_ERR:  错误信息
      */
    #define LOG_INFO(format, ...) 	IFR_Logger.Printf("[INFO]", "[%s:%d] " format, __FILENAME__, __LINE__, ##__VA_ARGS__)
    #define LOG_WARN(format, ...)   IFR_Logger.Printf("[WARN]", "[%s:%d] " format, __FILENAME__, __LINE__, ##__VA_ARGS__)
    #define LOG_ERR(format, ...)    IFR_Logger.Printf("[ERR]", "[%s:%d] " format, __FILENAME__, __LINE__, ##__VA_ARGS__)
    #define LOG_DEBUG(format, ...)  IFR_Logger.Printf("[DBG]", "[%s:%d] " format, __FILENAME__, __LINE__, ##__VA_ARGS__)
#else
    // 当关闭日志时，编译器会将这些宏替换为空，完全不占用 CPU 和内存
    #define LOG_INFO(format, ...)   ((void)0)
    #define LOG_WARN(format, ...)   ((void)0)
    #define LOG_ERR(format, ...)    ((void)0)
    #define LOG_DEBUG(format, ...)  ((void)0)
#endif

#endif // __IFR_LOG_H
