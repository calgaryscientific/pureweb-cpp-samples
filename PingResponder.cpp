//
// Copyright (c) 2010 Calgary Scientific Inc., all right reserved.
//

#include "PingResponder.h"

DEFINE_CLASS_LOGGER(PingResponder);
#define logger _CLASS_LOGGER

PingResponder::PingResponder(void): m_pStateManager(NULL)
{
}

PingResponder::~PingResponder(void)
{
}


void PingResponder::Initialize(StateManager* pStateManager)
{
    m_pStateManager = pStateManager;
    m_pStateManager->CommandManager().AddUiHandler("DDxPing", Bind(this, &PingResponder::ProcessPing));
}


void PingResponder::Uninitialize()
{
}


void PingResponder::SessionConnected(Guid sessionId, Typeless const& command)
{
    
}

void PingResponder::SessionDisconnected(Guid sessionId, Typeless const& command)
{
    
}

void PingResponder::ProcessPing(Guid sessionId, Typeless const& command, Typeless& responses)
{
    ResponseInfo response("DDxPing", m_xmlContentAdaptor.CreateContent(command));
    responses["PingResponse"] = "DDxPong";
}

