#include "ScenarioEmptyWorkingSet.h"
#include <Psapi.h>
// Copyright (C) Microsoft Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "stdafx.h"
#include "AppWindow.h"
#include "CheckFailure.h"
#include <Shlwapi.h>

using namespace Microsoft::WRL;

ScenarioEmptyWorkingSet::ScenarioEmptyWorkingSet(AppWindow* appWindow)
    : m_appWindow(appWindow)
{
    // Specify the executable name of WebView2 processes
    const wchar_t* executableName = L"msedgewebview2.exe";

    // Empty the working set for all processes with this executable name
    EmptyProcessesByExecutableName(executableName);

    // Keep the program running to observe results
    Sleep(5000);
}

ScenarioEmptyWorkingSet::~ScenarioEmptyWorkingSet()
{
    CHECK_FAILURE(
        m_appWindow->GetWebView()->remove_WebResourceRequested(m_webResourceRequestedToken));
}

void ScenarioEmptyWorkingSet::EmptyChildProcesses(UINT32 parentProcessId) {
    if (parentProcessId == 0) {
        std::cerr << "Invalid parent process ID." << std::endl;
        return;
    }

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create process snapshot. Error: " << GetLastError() << std::endl;
        return;
    }

    PROCESSENTRY32 processEntry = {};
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &processEntry)) {
        do {
            if (processEntry.th32ParentProcessID == parentProcessId) {
                std::cout << "Found child process: PID = " << processEntry.th32ProcessID << std::endl;
                EmptyWebViewWorkingSet(processEntry.th32ProcessID);
            }
        } while (Process32Next(hSnapshot, &processEntry));
    }
    else {
        std::cerr << "Failed to retrieve process list. Error: " << GetLastError() << std::endl;
    }

    CloseHandle(hSnapshot);
}

void ScenarioEmptyWorkingSet::EmptyWebViewWorkingSet(UINT32 processId)
{
    std::cout << "Triggering EmptyWorkingSet: " << processId << std::endl;
    // Open a handle to the process
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA, FALSE, processId);
    if (!hProcess) {
        std::cerr << "Failed to open process. Error: " << GetLastError() << std::endl;
        return;
    }
    // Attempt to empty the working set
    if (EmptyWorkingSet(hProcess)) {
        std::cout << "Working set emptied successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to empty the working set. Error: " << GetLastError() << std::endl;
    }
}

void ScenarioEmptyWorkingSet::EmptyProcessesByExecutableName(const wchar_t* executableName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create process snapshot. Error: " << GetLastError() << std::endl;
        return;
    }

    PROCESSENTRY32 processEntry = {};
    processEntry.dwSize = sizeof(PROCESSENTRY32);


    if (Process32First(hSnapshot, &processEntry)) {
        do {
            // Compare the executable name
            if (_wcsicmp(processEntry.szExeFile, executableName) == 0) {
                std::cout << "Found process with executable name \"" << executableName
                    << "\": PID = " << processEntry.th32ProcessID << std::endl;

                // Trigger EmptyWorkingSet on the matched process
                EmptyWebViewWorkingSet(processEntry.th32ProcessID);
            }
        } while (Process32Next(hSnapshot, &processEntry)); // Continue to the next process
    }
    else {
        std::cerr << "Failed to retrieve process list. Error: " << GetLastError() << std::endl;
    }


    CloseHandle(hSnapshot);
}
