
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
