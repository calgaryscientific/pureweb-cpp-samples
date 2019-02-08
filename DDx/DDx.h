/// Copyright 2013-2019 Calgary Scientific Inc. (operating under the brand name of PureWeb)
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

#include "CSI/PureWeb/StateManager/StateManager.h"
#include "CSI/PureWeb/StateManager/CollaborationManager.h"
#include "DDxView.h"
#include "PGView.h"
#include "DDxCineView.h"
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
    void OnRotateDDxViewBkColors(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless& response);
	void OnSessionStorageBroadcast(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless& response);
    void OnAttachStorageListener(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless& response);
	void OnDetachStorageListener(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless& response);
	void OnQuerySessionStorageKeys(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless& response);
	void OnQuerySessionsWithKey(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless& response);
	void OnSessionListenerValueChanged(SessionStorageChangedEventArgs args);	
	void OnNewKey(ISessionStorageManager& sessionStorage, SessionStorageChangedEventArgs& args);
    void OnSessionStorageSetKeyForceResponse(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless& response);	

    static const int m_viewCount = 4;
    static int m_colorCount;
    
    DDxView m_views[m_viewCount];
    PGView m_pgView;
	DDxCineView m_cineView;
    DDxOwnershipView m_ownershipView;
    CSI::Threading::Signal m_stop;
    CountedPtr<PingResponder> m_pingResponder;
    CountedPtr<StateManager> m_stateManager;
    CountedPtr<StateManagerServer> m_server;
    CountedPtr<CSI::Net::IPAddress> m_address;
    int m_port;
};
