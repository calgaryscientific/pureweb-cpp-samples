#ifndef PUREWEB_H
#define PUREWEB_H

#pragma warning( disable : 4100 )
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600 
#endif

#undef min
#undef max

#include "CSI/Standard/CsiStandard.h"
#include "CSI/Standard/CsiThreading.h"
#include "CSI/PureWeb/StateManager/StateManager.h"

#include <QApplication>

#ifdef new
#undef new
#endif

class PureWebCommon
{
public:
    static CSI::PureWeb::Server::StateManager& StateManager()
    {
        return *CSI::PureWeb::Server::StateManager::Instance();
    }

    static void OnPureWebShutdown(CSI::PureWeb::Server::StateManagerServer& server, CSI::EmptyEventArgs& args)
    {
        QApplication::exit();
    }
};


#endif

