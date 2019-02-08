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

#include "ServicePingResponder.h"

DEFINE_CLASS_LOGGER(PingResponder);
#define logger _CLASS_LOGGER

PingResponder::PingResponder(): 
    m_pStateManager(NULL), 
    m_pingResponsesReceivedCount(0),
    m_servicePing(NULL)
{
}

PingResponder::~PingResponder()
{
    if (m_servicePing != NULL)
    {
        m_pStateManager->SystemMessageDispatcher().UnregisterSystemMessageHandler("DDx-Pong", Bind(m_servicePing, &ServicePing::PingMessageHandler));
        delete m_servicePing;
    }
}

void PingResponder::Initialize(StateManager* pStateManager)
{
    m_pStateManager = pStateManager;
    m_pStateManager->CommandManager().AddUiHandler("DDxRoundtripPing", Bind(this, &PingResponder::ClientPingReply));
    m_pStateManager->CommandManager().AddUiHandler("DDxServiceServerPing", Bind(this, &PingResponder::TriggerServerPing));

    if (m_servicePing == NULL)
    {
        m_servicePing = new ServicePing(m_pStateManager, Bind(this, &PingResponder::PingMessageHandler));
        m_pStateManager->SystemMessageDispatcher().RegisterSystemMessageHandler("DDx-Pong", Bind(m_servicePing, &ServicePing::PingMessageHandler));
    }
}


void PingResponder::Uninitialize()
{
    m_pStateManager->CommandManager().RemoveUiHandler("DDxRoundtripPing");
    m_pStateManager->CommandManager().RemoveUiHandler("DDxServiceServerPing");

    // make sure that last instance of m_servicePing is deleted before the app exits to prevent 
    // mysterious exceptions in CSI.Standard when the app does exit (PWEB-4013) - probably due
    // to static initializtaion/teardown and attempting to access objects that no longer exist
    // in the ServicePing destructor.

    m_servicePing = NULL;
}


void PingResponder::SessionConnected(Guid sessionId, Typeless const& command)
{
}

void PingResponder::SessionDisconnected(Guid sessionId, Typeless const& command)
{
}

void PingResponder::ClientPingReply(Guid sessionId, Typeless const& command, Typeless &responses)
{
    responses["DDxClientPingResponse"] = "";
}


void PingResponder::TriggerServerPing(Guid sessionId, Typeless const& command, Typeless& responses)
{
    startPing = MonotonicTime::Now();
    m_servicePing->SendPing();
}

void PingResponder::PingMessageHandler(Int64 pingTimeInMilliseconds)
{
    m_pingResponsesReceivedCount++;
    m_pStateManager->XmlStateManager().SetValueAs<int>("DDxServiceServerPingResponseCount", m_pingResponsesReceivedCount); 
    m_pStateManager->XmlStateManager().SetValueAs<Int64>("DDxServiceServerPingResponse", pingTimeInMilliseconds);
}

ServicePing::ServicePing(StateManager *stm, PingResponseHandler hnd) :
    m_pStateManager(stm),
    m_pingResponseHandler(hnd)
{
}

ServicePing::~ServicePing()
{
}

void ServicePing::SendPing()
{
    startPing = MonotonicTime::Now();
    Typeless message("EmptyPingMessage");
    m_pStateManager->SystemMessageDispatcher().QueueSystemMessage("DDx-Ping", message);
}

void ServicePing::PingMessageHandler(Typeless const& message)
{
    m_pingResponseHandler(MonotonicTime::Now().TotalMilliseconds() - startPing.TotalMilliseconds());
}