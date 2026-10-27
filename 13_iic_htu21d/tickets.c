#include <windows.h>
#include <stdio.h>

#define WINDOW_COUNT 3          // 窗口数量
int g_tickets = 50;             // 总票数
HANDLE hMutex;                  // 互斥量句柄
const char* LOG_PATH = "tickets_log.txt";

// 售票窗口线程
DWORD WINAPI SellThread(LPVOID lpParam) {
    int id = *(int*)lpParam;
    DWORD written;
    char record[64];

    while (TRUE) {
        // --- [互斥开始] 获取钥匙 ---
        WaitForSingleObject(hMutex, INFINITE);

        if (g_tickets > 0) {
            int current = g_tickets--;
            printf("窗口 %d 售出第 %d 张票\n", id, current);

            // 1. 使用课件中的 CreateFile API
            HANDLE hFile = CreateFileA(
                LOG_PATH, 
                FILE_APPEND_DATA,           // 追加模式
                FILE_SHARE_READ,            // 共享读
                NULL, 
                OPEN_ALWAYS,                // 不存在则创建
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, // 重点：直通磁盘标志
                NULL
            );

            if (hFile != INVALID_HANDLE_VALUE) {
                sprintf_s(record, "Window %d sold Ticket %d\r\n", id, current);
                // 2. 移动指针并写入文件
                SetFilePointer(hFile, 0, NULL, FILE_END);
                WriteFile(hFile, record, (DWORD)strlen(record), &written, NULL);
                // 3. 关闭句柄
                CloseHandle(hFile);
            }

            // --- [互斥结束] 释放钥匙 ---
            ReleaseMutex(hMutex);
            Sleep(10); // 模拟出票时间，并给其他窗口竞争机会
        } else {
            ReleaseMutex(hMutex); // 票卖完了也要记得解锁
            break;
        }
    }
    return 0;
}

int main() {
    // 初始化：清理旧日志并创建互斥量
    DeleteFileA(LOG_PATH);
    hMutex = CreateMutex(NULL, FALSE, NULL);

    HANDLE hThreads[WINDOW_COUNT];
    int ids[WINDOW_COUNT];

    // 创建多个线程
    for (int i = 0; i < WINDOW_COUNT; i++) {
        ids[i] = i + 1;
        hThreads[i] = CreateThread(NULL, 0, SellThread, &ids[i], 0, NULL);
    }

    // 等待所有窗口售票结束
    WaitForMultipleObjects(WINDOW_COUNT, hThreads, TRUE, INFINITE);

    for (int i = 0; i < WINDOW_COUNT; i++) CloseHandle(hThreads[i]);
    CloseHandle(hMutex);

    printf("售票任务圆满完成！记录已保存至 %s\n", LOG_PATH);
    return 0;
}
