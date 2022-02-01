// ProcessMonitor.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include "ProcessMonitor.h"
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <functional>
#include <psapi.h>
/// <summary>
// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1
/// </summary>
std::string ProcessMonitor::GetProcessName(DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    // Get a handle to the process.
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    // Get the process name.
    if (hProcess != NULL) {
        HMODULE hMod;
        DWORD cbNeeded;
        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
            GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
    }
    CloseHandle(hProcess);
    std::wstring name = szProcessName;
    return std::string(name.begin(), name.end());
}
/// <summary>
/// 
/// </summary>
std::string ProcessMonitor::GetActiveWindowTitle()
{
    //LPSTR wnd_title;
    char wnd_title[256];
    char wnd_path[256];
    // get handle of currently active window
    HWND hwnd = GetForegroundWindow();
    // get title text of currently active window
    GetWindowTextA(hwnd, wnd_title, sizeof(wnd_title));
    // get exe path of currently active window
    GetWindowModuleFileNameA(hwnd, wnd_path, sizeof(wnd_path));
    DWORD dwProcId = 0;
    GetWindowThreadProcessId(hwnd, &dwProcId);
    std::string title = wnd_title;
    std::string path = GetProcessName(dwProcId);
    // return path + " | " + title;
    return path;
}
/// <summary>
/// 
/// </summary>
std::string ProcessMonitor::OnWindowChange(std::string activeWindow, const std::function<void(std::string)>& callback)
{
    std::string newActiveWindow = GetActiveWindowTitle();
    if (newActiveWindow != activeWindow) {
        activeWindow = newActiveWindow;
        callback(activeWindow);
    }
    return activeWindow;
}

int main()
{
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
