#pragma once
#include "stdafx.h"

#include <string>
#include <windows.h>
#include <iostream>
#include "AppWindow.h"
#include "ComponentBase.h"
#include <webview2.h>
#include <tlhelp32.h>

class ScenarioEmptyWorkingSet : public ComponentBase
{
public:
    ScenarioEmptyWorkingSet(AppWindow* appWindow);
    ~ScenarioEmptyWorkingSet() override;

private:
    void EmptyWebViewWorkingSet(UINT32 processId);
    // Function to find and empty the working set of all child processes of a given parent process ID
    void EmptyChildProcesses(UINT32 parentProcessId);
    // Function to empty the working set of all processes with the specified executable name
    void EmptyProcessesByExecutableName(const wchar_t* executableName);
    EventRegistrationToken m_webResourceRequestedToken = {};
    EventRegistrationToken m_navigationCompletedToken = {};

    AppWindow* m_appWindow = nullptr;
    wil::com_ptr<ICoreWebView2> m_webView2;
    wil::com_ptr<ICoreWebView2_2> m_webView2_2;
    wil::com_ptr<ICoreWebView2_26> m_webView2_26;

    wil::com_ptr<ICoreWebView2_22> m_webView2_22;
};