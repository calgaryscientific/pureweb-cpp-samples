//
// Copyright (c) 2011 Calgary Scientific Inc., all rights reserved.
//

#include "DDx.h"
#include "CSI/PureWeb/StateManager/CollaborationManager.h"
#include "CSI/PureWeb/StateManager/ProfilerManager.h"
#include "CSI/Standard/CsiProcess.h"

using namespace CSI;
using namespace CSI::PureWeb;
using namespace CSI::PureWeb::Server;
using namespace CSI::PureWeb::Diagnostics;

DEFINE_CLASS_LOGGER(DDx);
#define logger _CLASS_LOGGER

int DDx::m_colorCount = 0;

void DDx::Go(int argc, char* argv[])
{
    m_server = new StateManagerServer();
    m_stateManager = new StateManager("DDx");

    m_pingResponder = new PingResponder();
    m_stateManager->PluginManager().RegisterPlugin("DDxPingResponder", m_pingResponder.get());

    // Register shutdown handler and start
    m_stateManager->Initialized() += Bind(this, &DDx::OnPureWebStartup);
    m_stateManager->Uninitialized() += Bind(this, &DDx::OnPureWebShutdown);
    if (argc == 1)
    {
        m_server->Start(m_stateManager.get());
    }
    else
    {
        m_address = new CSI::Net::IPAddress();
        
        if (CSI::Net::IPAddress::TryParse(argv[1], *m_address))
        {
            m_server->Start(m_stateManager.get(), *m_address, 8082);
        }
        else
        {
            logger.Error.Format("Unable to parse address {0}", argv[1]);
        }
    }

    m_stop.Acquire();

    while (!m_server->Stop(TimeSpan::FromMilliseconds(250)))
    {
        // a server thread may be blocked on an operation queued to the UI thread
        logger.Debug.Format("Shutting down server {0}", DateTime::Now());
    }
}

void DDx::OnPureWebStartup(StateManager& stateManager, EmptyEventArgs&) 
{
    ProfilerManager& profilerManager = stateManager.ProfilerManager();
    profilerManager.SetUpdateIntervalMs(1000);
    CSI_ASSERT(profilerManager.UpdateIntervalMs() == 1000);
    profilerManager.StartUpdating();

    stateManager.CommandManager().AddUiHandler("TakeOwnership", Bind(this, &DDx::OnTakeOwnership));
    stateManager.CommandManager().AddUiHandler("SetProperty", Bind(this, &DDx::OnSetProperty));
    stateManager.CommandManager().AddUiHandler("Echo", Bind(this, &DDx::OnEcho));
    stateManager.CommandManager().AddUiHandler("TestMerge", Bind(this, &DDx::OnTestMerge));

    const bool useTiles = true;
    stateManager.XmlStateManager().SetValueAs<bool>(_DDx_USETILES, useTiles);

    for(int i = 0; i < m_viewCount; i++)
    {
        m_views[i].ViewName = Text::Format("/DDx/View{0}", i);
        stateManager.ViewManager().RegisterView(m_views[i].ViewName, &m_views[i]);
    }

    m_pgView.ViewName = "PGView";
    stateManager.ViewManager().RegisterView(m_pgView.ViewName, &m_pgView);
    m_pgView.Initialize();

    // Register view for ownership test
    stateManager.ViewManager().RegisterView(m_ownershipView.ViewName, &m_ownershipView);

    CollaborationManager::Instance().SetSessionDefaultColorProvider(this);
    
    {
        TypelessStateLock lock(StateManager::Instance()->LockAppState());
        
        lock.SetValueAs<bool>(_DDx_GRID_ON, true);
        lock.SetValueAs<int>(_DDx_GRID_MARGIN, 0);
        lock.SetValueAs<int>(_DDx_GRID_LINEWIDTH, 4);
        lock.SetValueAs<int>(_DDx_GRID_LINESPACING, 100);
    }

    stateManager.XmlStateManager().AddChildChangedHandler(_DDx_GRID, Bind(this, &DDx::OnGridStateChanged));
    stateManager.XmlStateManager().AddChildChangedHandler("/PureWeb/Profiler", Bind(this, &DDx::OnProfilerStateChanged));
}

void DDx::OnTestMerge(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless& response)
{
    CSI::Typeless state;
    CSI::Typeless a("A");
    a.SetOrderedChildName("B");
    state.AddNamedChild(a);

    CSI::Typeless diffNode("DiffScript");
    diffNode.SetOrderedChildName("Diff");

    int count = command["Count"].ValueOr(3000);
    for(int i = 0; i < count; i++)
    {
        CSI::String node = CSI::Text::Format("<Diff><Type>Inserted</Type><Path>/A/#{0}</Path><Value /></Diff>", i);
        CSI::Typeless child = CSI::Xml::TypelessXml::FromString(node);
        diffNode.AddOrderedChild(child);
    }
    CSI::Stopwatch sw;
    CSI::TypelessDifference::MergeInPlace(state, diffNode);
    StateManager::Instance()->XmlStateManager().SetValueAs("/OnTestMerge", sw.Elapsed().TotalMilliseconds());
}

void DDx::OnTakeOwnership(CSI::Guid sessionid, const CSI::Typeless command, const CSI::Typeless& response)
{
    CollaborationManager::Instance().SetOwnerSession(sessionid);
}

void DDx::OnPureWebShutdown(StateManager& stateManager, EmptyEventArgs&)
{
    ProfilerManager& profilerManager = stateManager.ProfilerManager();
    profilerManager.StopUpdating();

    for(int i = 0; i < m_viewCount; i++)
    {
        stateManager.ViewManager().UnregisterView(m_views[i].ViewName);
        m_views[i].SetClientSize(Size(0, 0));
    }

    m_pgView.Uninitialize();
    stateManager.ViewManager().UnregisterView(m_pgView.ViewName);

    stateManager.ViewManager().UnregisterView(m_ownershipView.ViewName);
    m_ownershipView.SetClientSize(Size(0, 0));
    
    CollaborationManager::Instance().SetSessionDefaultColorProvider(NULL);
    m_colorCount = 0;
    
    stateManager.XmlStateManager().RemoveAllValueChangedHandlers(_DDx_USETILES);

    stateManager.CommandManager().RemoveUiHandler("TakeOwnership");
    stateManager.CommandManager().RemoveUiHandler("SetProperty");
    stateManager.CommandManager().RemoveUiHandler("Echo");
    stateManager.CommandManager().RemoveUiHandler("TestMerge");

    // if running unmanaged, restart the state manager server to connect back to the server

    if (m_address.get() != NULL)
    {
        try
        {
            m_server->Start(m_stateManager.get(), *m_address, 8082);
        }
        catch (Exception e)
        {
            logger.Error.Format("An error occurred starting the StateManagerServer: " + e.Message());
            m_stop.Set();
        }
    }
    else
    {
            m_stop.Set();
    }
}
  
PureWebColor DDx::GetSessionDefaultColor(Guid sessionId)
{
    switch (m_colorCount++ % 7)
    {
    case 0:
        return PureWebColor::FromKnownColor(PureWebKnownColor::Red);
    case 1:
        return PureWebColor::FromKnownColor(PureWebKnownColor::Orange);
    case 2:
        return PureWebColor::FromKnownColor(PureWebKnownColor::Yellow);
    case 3:
        return PureWebColor::FromKnownColor(PureWebKnownColor::Green);
    case 4:
        return PureWebColor::FromKnownColor(PureWebKnownColor::Blue);
    case 5:
        return PureWebColor::FromKnownColor(PureWebKnownColor::Indigo);
    case 6:
        return PureWebColor::FromKnownColor(PureWebKnownColor::Violet);
    default:
        return PureWebColor::FromKnownColor(PureWebKnownColor::White);
    }
}

void DDx::OnGridStateChanged(ValueChangedEventArgs args)
{    
    for(int i = 0; i < m_viewCount; i++)
    {
        m_views[i].RenderDeferred();
    }
}

void DDx::OnProfilerStateChanged(ValueChangedEventArgs args)
{
    if(!args.Path().Contains("LastUpdated"))
        return;

    {
        TypelessStateLock lock(StateManager::Instance()->LockAppState());
        
        CSI::Xml::TypelessXmlWriterOptions options;
        options.SetIndentation(0);
        options.SetIndentationPerLevel(0);
        options.SetLineTerminator("");
        logger.Info.Format("Profiler info updated: {0}", CSI::Xml::TypelessXml::ToString(lock.Element("/PureWeb/Profiler"), false, options));
    }
}

void DDx::OnSetProperty(Guid sessionId, Typeless command, Typeless& responses)
{    
    String path = command["Path"];
    String value = command["Value"];
    
    {
        TypelessStateLock lock(StateManager::Instance()->LockAppState());

        lock.SetValue(path, value);
    }
        
    logger.Debug.Format("SetProperty {0} {1}", path, value);
}


void DDx::OnEcho(Guid sessionId, Typeless command, Typeless& response)
{
    String key = command["Key"];
    String contentType = command["Type"];
    String contentText;

    if (contentType.CompareNoCase("Text") == 0)
    {
        contentText = command["Content"];
    }
    else if (contentType.CompareNoCase("Character") == 0)
    {
		// Reading in character as string as Typeless does not support Char properly, and just SByte
        contentText = command["Content"];
    }
    else if (contentType.CompareNoCase("DateTime") == 0)
    {
        DateTime content = command["Content"].As<DateTime>();
        Typeless convertBack = Typeless(content);
        contentText = convertBack;
    }
    else if (contentType.CompareNoCase("Byte") == 0)
    {
        Byte content = command["Content"].As<Byte>();
        Typeless convertBack = Typeless(content);
        contentText = convertBack;
    }
    else if (contentType.CompareNoCase("Integer") == 0)
    {
        Int32 content = command["Content"].As<Int32>();
        Typeless convertBack = Typeless(content);
        contentText = convertBack;
    }
    else if (contentType.CompareNoCase("Unsigned Integer") == 0)
    {
        UInt32 content = command["Content"].As<UInt32>();
        Typeless convertBack = Typeless(content);
        contentText = convertBack;
    }
    else if (contentType.CompareNoCase("Long") == 0)
    {
        Int64 content = command["Content"].As<Int64>();
        Typeless convertBack = Typeless(content);
        contentText = convertBack;
    }
    else if (contentType.CompareNoCase("Unsigned Long") == 0)
    {
        UInt64 content = command["Content"].As<UInt64>();
        Typeless convertBack = Typeless(content);
        contentText = convertBack;
    }
    else if (contentType.CompareNoCase("Float") == 0)
    {
        float content = command["Content"].As<float>();
        Typeless convertBack = Typeless(content);
        contentText = convertBack;
    }
    else if (contentType.CompareNoCase("Double") == 0)
    {
        double content = command["Content"].As<double>();
        Typeless convertBack = Typeless(content);
        contentText = convertBack;
    }
    else if (contentType.CompareNoCase("Decimal") == 0)
    {
        double content = command["Content"].As<double>();
        Typeless convertBack = Typeless(content);
        contentText = convertBack;
    }
    else if (contentType.CompareNoCase("Boolean") == 0)
    {
        bool content = command["Content"].As<bool>();
        Typeless convertBack = Typeless(content);
        contentText = convertBack;
    }

    TypelessStateLock lock(StateManager::Instance()->LockAppState());
    CSI::String path = Text::Format("/DDx/Echo/{0}", key);
    lock.SetValue(path, contentText);

    response["Key"] = key;
    response["Content"] = contentText;
}
