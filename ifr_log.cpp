/**
  **************************** Copyright ******************************
  *
  *       (C) Copyright 2025, China, IFR Laboratory,DreamTeam.
  *                         All Rights Reserved
  *
  * FileName 	: ifr_log.cpp
  * Version		: v1.0
  * Author		: csr
  * Date		: 2025-11-10
  * Description	: ifr_log 库的具体实现。利用 vsnprintf 格式化字符串并推入 usart 的 DMA 队列。
  *********************************************************************
  */
#include "ifr_log.h"
#include <string.h>

// 实例化全局日志对象
IFR_Log_ClassDef IFR_Logger;

/**
  * @概述	日志初始化，注册底层通讯的 USART 对象
  * @参数1	usart_obj: 已经初始化的 IFR_USART_ClassDef 对象指针
  * @返回值 void
  */
void IFR_Log_ClassDef::Init(IFR_USART_ClassDef* usart_obj) 
{
    if (usart_obj != NULL) {
        this->Log_usart = usart_obj;
    }
}

/**
  * @概述	日志底层格式化发送函数
  * @参数1	prefix: 日志等级前缀 (如 "[INFO]")
  * @参数2	format: 格式化字符串 (如 "Temp: %d")
  * @参数3  ... : 可变参数
  * @返回值 void
  */
void IFR_Log_ClassDef::Printf(const char* prefix, const char* format, ...) 
{
    if (this->Log_usart == NULL) return;

    char log_buf[IFR_LOG_MAX_LEN];
    uint16_t current_len = 0;

    // 压入时间戳和前缀
#if IFR_LOG_USE_TIMESTAMP == 1
    // 获取毫秒级时间戳：例如 [0012.345] [INFO] 
    uint32_t tick = HAL_GetTick();
    current_len = snprintf(log_buf, sizeof(log_buf), "[%04lu.%03lu] %s ", tick / 1000, tick % 1000, prefix);
#else
    // 不加时间戳：例如 [INFO] 
    current_len = snprintf(log_buf, sizeof(log_buf), "%s ", prefix);
#endif
    if (current_len < sizeof(log_buf)) {
        va_list args;
        va_start(args, format);
        int formatted_len = vsnprintf(&log_buf[current_len], sizeof(log_buf) - current_len, format, args);
        va_end(args);

        if (formatted_len > 0) {
            current_len += formatted_len;
        }
    }

    // 强制添加回车换行符 "\r\n"
    // 预留2个字节空间给 \r\n
    if (current_len > sizeof(log_buf) - 3) {
        current_len = sizeof(log_buf) - 3;
    }
    log_buf[current_len++] = '\r';
    log_buf[current_len++] = '\n';
    log_buf[current_len]   = '\0';

    bool enqueued = this->Log_usart->ifr_usart_entxqueue((uint8_t*)log_buf, current_len);
    if (enqueued) {
        this->Log_usart->ifr_usart_transmit_tx_queue();
    }
}