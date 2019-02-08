// Copyright 2013-2019 Calgary Scientific Inc. (operating under the brand name of PureWeb)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Scribble.h : main header file for the Scribble application
//
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "PureWeb.h"


// CScribbleApp:
// See Scribble.cpp for the implementation of this class
//

class CScribbleApp : public CWinApp
{
private:

    // since the CWinApp class controls the lifecycle of 
    // the application, we also let it manage the lifecycle
    // of the the PureWeb StateManager class instances
    
    // a counted pointer to the state manager
    CSI::CountedPtr<CSI::PureWeb::Server::StateManagerServer> m_pServer;

    // the state manager itself
    CSI::CountedPtr<CSI::PureWeb::Server::StateManager> m_pStateManager;


public:
    CScribbleApp();

    static CSI::PureWeb::Server::StateManager& StateManager()
    {
        return *CSI::PureWeb::Server::StateManager::Instance();
    }
    
// Overrides
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

// Implementation
    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()
};

extern CScribbleApp theApp;
