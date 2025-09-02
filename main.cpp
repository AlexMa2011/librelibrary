#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>

bool KillProcessByName(const wchar_t* filename)
{
    bool processFound = false;
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32W pEntry;
    pEntry.dwSize = sizeof(pEntry);

    if (Process32FirstW(hSnapShot, &pEntry))
    {
        while (Process32NextW(hSnapShot, &pEntry))
        {
            if (!wcscmp(pEntry.szExeFile, filename))
            {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pEntry.th32ProcessID);
                if (hProcess != NULL)
                {
                    if (TerminateProcess(hProcess, 9))
                    {
                        std::wcout << L"已结束进程: " << filename << std::endl;
                        processFound = true;
                    }
                    else
                    {
                        std::wcerr << L"无法结束进程: " << filename << L", 错误代码: " << GetLastError() << std::endl;
                    }
                    CloseHandle(hProcess);
                }
                else
                {
                    std::wcerr << L"无法打开进程: " << filename << L", 错误代码: " << GetLastError() << std::endl;
                }
            }
        }
    }
    CloseHandle(hSnapShot);
    return processFound;
}

int main()
{
    SetConsoleOutputCP(65001); // 设置控制台输出为 UTF-8 编码

    bool processProtectKilled = KillProcessByName(L"ProcessProtect.exe");
    bool studentKilled = KillProcessByName(L"Student.exe");

    if (!processProtectKilled && !studentKilled)
    {
        std::wcout << L"未找到 ProcessProtect.exe 和 Student.exe 进程。" << std::endl;
    }

    system("pause"); // 暂停，等待用户按键
    return 0;
}
