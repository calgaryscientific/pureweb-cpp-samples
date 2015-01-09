
// Scribble.cpp : Defines the class behaviors for the application.
//
#define WIN32_LEAN_AND_MEAN


#include "stdafx.h"
//#include "afxwinappex.h"
#include "Scribble.h"
#include "MainFrm.h"

#include "ScribbleDoc.h"
#include "ScribbleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CScribbleApp

BEGIN_MESSAGE_MAP(CScribbleApp, CWinApp)
    ON_COMMAND(ID_APP_ABOUT, &CScribbleApp::OnAppAbout)
    // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CScribbleApp construction

CScribbleApp::CScribbleApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

// The one and only CScribbleApp object

CScribbleApp theApp;

// PureWeb shutdown helper
static HWND hMainWnd = NULL;
static void OnPureWebShutdown(CSI::PureWeb::Server::StateManager&, CSI::EmptyEventArgs&)
{
    ::PostMessage(hMainWnd, WM_QUIT, 0, 0);
}

// Class to look for /PureWeb and (optional) /ServerAddress command line parameters
class CPureWebCommandLineInfo : public CCommandLineInfo
{
public:

    BOOL m_bStartPureWeb;
    CSI::String m_serverAddress;
    int m_port;

    CPureWebCommandLineInfo()
        : m_bStartPureWeb(FALSE),
          m_port(8082)
    {
    }

    virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
    {
        if (bFlag && 0 == _tcsicmp(pszParam, _T("PureWeb")))
        {
            m_bStartPureWeb = TRUE;
        }
        else if (bFlag && 0 == _tcsnicmp(pszParam, _T("Server="), 7))
        {
            std::basic_string<TCHAR> tmp = pszParam;
            tmp = tmp.substr(7);
            for (int i = 0; i < tmp.length(); i++)
            {
#ifdef UNICODE
                m_serverAddress += (CSI::Char16)tmp.c_str()[i];
#else
                m_serverAddress += tmp.c_str()[i];
#endif
            }
        } 
        else if (bFlag && 0 == _tcsnicmp(pszParam, _T("Port="), 5))
        {
            std::basic_string<TCHAR> tmp = pszParam;
            m_port = std::stoi(tmp.substr(5));
        }
    }
#ifdef _UNICODE
    virtual void ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast)
    {
        if (bFlag && 0 == _stricmp(pszParam, "PureWeb"))
        {
            m_bStartPureWeb = TRUE;
        }
        else if (bFlag && 0 == _strnicmp(pszParam, "Server=", 7))
        {
            std::string tmp = pszParam;
            m_serverAddress = tmp.substr(7).c_str();
        }
        else if (bFlag && 0 == _strnicmp(pszParam, "Port=", 5))
        {
            std::string tmp = pszParam;
            m_port = std::stoi(tmp.substr(5));
        }
    }
#endif
};


// CScribbleApp initialization

BOOL CScribbleApp::InitInstance()
{
    CWinApp::InitInstance();

    // Init CSI / PureWeb libraries
    CSI::Library::Initialize();
    CSI::Threading::UiDispatcher::InitMessageThread();    // this thread is the UI thread

    // create PureWeb object instances
    m_pServer = new CSI::PureWeb::Server::StateManagerServer();
    m_pStateManager = new CSI::PureWeb::Server::StateManager();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));
    LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CScribbleDoc),
        RUNTIME_CLASS(CMainFrame),       // main SDI frame window
        RUNTIME_CLASS(CScribbleView));
    if (!pDocTemplate)
        return FALSE;
    AddDocTemplate(pDocTemplate);

    // Parse command line for standard shell commands, DDE, file open
    CPureWebCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Dispatch commands specified on the command line.  Will return FALSE if
    // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    // now that the main window has been created, start the PureWeb server
    // if specified on command line
    if (cmdInfo.m_bStartPureWeb)
    {
        hMainWnd = m_pMainWnd->GetSafeHwnd();
        m_pStateManager->Uninitialized() += OnPureWebShutdown;

        // if no address provided for the PureWeb server then start as a managed service, otherwise
        // start as a unmanaged service
        
        if (cmdInfo.m_serverAddress.Length() == 0)
        {
            m_pServer->Start(m_pStateManager.get());
        }
        else
        {
			m_pServer->Start(m_pStateManager.get(), cmdInfo.m_serverAddress, cmdInfo.m_port);
        }
    }

    // The one and only window has been initialized, so show and update it
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();
    // call DragAcceptFiles only if there's a suffix
    //  In an SDI app, this should occur after ProcessShellCommand

    return TRUE;
}

int CScribbleApp::ExitInstance()
{
    // make sure server stops
    while (!m_pServer->Stop(CSI::TimeSpan::FromMilliseconds(250)))
    {
        // a server thread may be blocked on an operation queued to the UI thread
        PostQuitMessage(0);
        while (PumpMessage())
        {
        }
    }

    // release server and state manager objects before uninitializing CSI library
    m_pServer = NULL;
    m_pStateManager = NULL;

    // Uninitialize CSI / PureWeb libraries
    CSI::Library::Uninitialize();

    return CWinApp::ExitInstance();
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialog Data
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Implementation
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CScribbleApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

// CScribbleApp message handlers

