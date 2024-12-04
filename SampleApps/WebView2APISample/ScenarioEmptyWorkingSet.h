#pragma once
#include "stdafx.h"

#include <string>
#include <windows.h>
#include <iostream>
#include "AppWindow.h"
#include "ComponentBase.h"

class ScenarioEmptyWorkingSet : public ComponentBase
{
public:
    ScenarioEmptyWorkingSet(AppWindow* appWindow);
    ~ScenarioEmptyWorkingSet() override;

private:
    EventRegistrationToken m_webResourceRequestedToken = {};
    EventRegistrationToken m_navigationCompletedToken = {};

    AppWindow* m_appWindow = nullptr;

    wil::com_ptr<ICoreWebView2_22> m_webView2_22;
};