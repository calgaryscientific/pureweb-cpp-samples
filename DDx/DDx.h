//
// Copyright (c) 2011 Calgary Scientific Inc., all rights reserved.
//

#include "CSI/PureWeb/StateManager/StateManager.h"
#include "CSI/PureWeb/StateManager/CollaborationManager.h"
#include "DDxView.h"
#include "PGView.h"
#include "DDxOwnershipView.h"
#include "ServicePingResponder.h"

class DDx : public CSI::PureWeb::Server::ISessionDefaultColorProvider
{
public:
    void Go(int argc, char* argv[]);

private:
    void OnPureWebStartup(CSI::PureWeb::Server::StateManager&, CSI::EmptyEventArgs&);
    void OnPureWebShutdown(CSI::PureWeb::Server::StateManager&, CSI::EmptyEventArgs&);
            
    CSI::PureWeb::PureWebColor GetSessionDefaultColor(CSI::Guid sessionId);
    void OnGridStateChanged(CSI::ValueChangedEventArgs args);
    void OnTakeOwnership(CSI::Guid sessionid, const CSI::Typeless command, const CSI::Typeless& response);
    void OnProfilerStateChanged(CSI::ValueChangedEventArgs args);
    void OnSetProperty(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless& responses);
    void OnEcho(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless& response);
    void OnTestMerge(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless& response);

    static const int m_viewCount = 4;
    static int m_colorCount;
    
    DDxView m_views[m_viewCount];
    PGView m_pgView;
    DDxOwnershipView m_ownershipView;
    CSI::Threading::Signal m_stop;
    CountedPtr<PingResponder> m_pingResponder;
    CountedPtr<StateManager> m_stateManager;
    CountedPtr<StateManagerServer> m_server;
    CountedPtr<CSI::Net::IPAddress> m_address;
    int m_port;
};
