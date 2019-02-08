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

#ifndef SERVICEPINGRESPONDER_H
#define SERVICEPINGRESPONDER_H

#include "CSI/PureWeb/StateManager/StateManager.h"
#include "CSI/Standard/CsiStandard.h"
#include "CSI/Typeless/Typeless.h"

using namespace CSI;
using namespace CSI::PureWeb::Server;

class ServicePing;

class PingResponder : public IStateManagerPlugin
{
private:
    StateManager* m_pStateManager;
    int m_pingResponsesReceivedCount; 
    ServicePing *m_servicePing;
    bool m_sendingPing;
    MonotonicTime startPing; 

    void PingMessageHandler(Int64 pingTimeMillseconds);

public:
    PingResponder();
    ~PingResponder();

    virtual void Initialize(StateManager* pStateManager);
    virtual void Uninitialize();
    virtual void SessionConnected(Guid sessionId, Typeless const& command);
    virtual void SessionDisconnected(Guid sessionId, Typeless const& command);

    void ClientPingReply(Guid sessionId, Typeless const& command, Typeless& responses);
    void TriggerServerPing(Guid sessionId, Typeless const& command, Typeless& responses);
};

typedef CSI::Function<void(Int64 pingTimeInMilliseconds)> PingResponseHandler;

class ServicePing
 {
 private:
     PingResponseHandler m_pingResponseHandler;
     StateManager *m_pStateManager;
     MonotonicTime startPing; 

 public:
     ServicePing(StateManager *stm, PingResponseHandler hnd);
     virtual ~ServicePing();

     void SendPing();
     void PingMessageHandler(Typeless const& message);
};

#endif // SERVICEPINGRESPONDER_H
