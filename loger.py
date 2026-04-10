import serial
import time
import datetime

# ================= 配置区 =================
COM_PORT = 'COM20'       # 请修改为你的蓝牙虚拟串口号 (Mac/Linux 通常是 /dev/tty.xxx)
BAUD_RATE = 9600        # 请修改为你的单片机波特率
LOG_FILE = 'log.txt' # 保存日志的文件名
# ==========================================

def start_logging():
    try:
        # 打开串口
        ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)
        print(f"成功连接到 {COM_PORT}，波特率 {BAUD_RATE}")
        print(f"日志将实时保存到 {LOG_FILE}。按 Ctrl+C 停止记录。")

        # 以追加模式(a)打开文件
        with open(LOG_FILE, "a", encoding="utf-8") as f:
            while True:
                if ser.in_waiting > 0:
                    # 读取一行数据并解码
                    # 注意：如果单片机发的是GBK编码的中文，将 utf-8 改为 gbk
                    # errors='replace' 可以防止因为蓝牙丢包导致的解码崩溃
                    raw_data = ser.readline()
                    text = raw_data.decode('utf-8', errors='replace').strip()
                    
                    if text: # 如果数据不为空
                        # 获取当前时间戳
                        now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
                        
                        # 拼接日志格式
                        log_line = f"[{now}] {text}\n"
                        
                        # 打印在电脑屏幕上
                        print(log_line, end="")
                        
                        # 写入文件并立即刷新缓冲区，防止断电或程序崩溃导致丢失
                        f.write(log_line)
                        f.flush() 

    except serial.SerialException as e:
        print(f"串口错误: {e}")
        print("请检查COM口是否被其他软件占用，或者蓝牙是否断开。")
    except KeyboardInterrupt:
        print("\n已停止记录日志。")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

if __name__ == '__main__':
    start_logging()