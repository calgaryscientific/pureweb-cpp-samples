//
// Copyright (c) 2010 Calgary Scientific Inc., all right reserved.
//

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
    CountedPtr<ServicePing> m_servicePing;
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

     void PingMessageHandler(Typeless const& message);

 public:
     ServicePing(StateManager *stm, PingResponseHandler hnd);
     virtual ~ServicePing();

     void SendPing();
};

#endif // SERVICEPINGRESPONDER_H
