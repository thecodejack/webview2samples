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
    DWORD delayMilliseconds = 9000;
    // Inform the user about the operation
    std::cout << "Waiting for " << delayMilliseconds / 1000 << " seconds before emptying the working set..." << std::endl;

    // Wait for the specified delay
    Sleep(delayMilliseconds);

    // Get a handle to the current process
    HANDLE hProcess = GetCurrentProcess();

    // Attempt to empty the working set
    if (EmptyWorkingSet(hProcess)) {
        std::cout << "Working set emptied successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to empty the working set. Error: " << GetLastError() << std::endl;
    }
}

ScenarioEmptyWorkingSet::~ScenarioEmptyWorkingSet()
{
    CHECK_FAILURE(
        m_appWindow->GetWebView()->remove_WebResourceRequested(m_webResourceRequestedToken));
}
