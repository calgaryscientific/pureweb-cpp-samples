//
// Copyright (c) 2010 Calgary Scientific Inc., all right reserved.
//

#ifndef PINGRESPONDER_H
#define PINGRESPONDER_H

#include "CSI/PureWeb/StateManager/StateManager.h"
#include "CSI/Standard/CsiStandard.h"
#include "CSI/Typeless/Typeless.h"

using namespace CSI;
using namespace CSI::PureWeb::Server;

class PingResponder : public IStateManagerPlugin
{
private:
    StateManager* m_pStateManager;
   
    XmlContentAdaptor m_xmlContentAdaptor;
public:
    PingResponder(void);
    ~PingResponder(void);

    virtual void Initialize(StateManager* pStateManager);
    virtual void Uninitialize();
    virtual void SessionConnected(Guid sessionId, Typeless const& command);
    virtual void SessionDisconnected(Guid sessionId, Typeless const& command);

    void ProcessPing(Guid sessionId, Typeless const& command, Typeless& responses);
};

#endif // PINGRESPONDER_H
