//
// Copyright (c) 2011 Calgary Scientific Inc., all rights reserved.
//

#include "Canvas.h"
#include "DDxOwnershipView.h"

using namespace CSI;
using namespace CSI::PureWeb;
using namespace CSI::PureWeb::Server;
using namespace CSI::PureWeb::Ui;

static Log::Logger logger = Log::GetLogger<DDxOwnershipView>(); 

DDxOwnershipView::DDxOwnershipView()
{
    ViewName = "DDx_OwnershipView";
}

void DDxOwnershipView::SetClientSize(Size clientSize)
{
    m_clientSize = clientSize;
}

Size DDxOwnershipView::GetActualSize()
{
    if (m_clientSize.IsEmpty())
        return Size(512,512);

    return m_clientSize; 
}

void DDxOwnershipView::RenderView(RenderTarget target)
{   
    Image image = target.RenderTargetImage();
    Canvas c(image);
    c.Clear(PureWebColor::FromKnownColor(PureWebKnownColor::DarkBlue));
    for(int i  = 0; i < image.Height(); i = i + 20)
    {
        c.DrawHLine(PureWebColor::FromKnownColor(PureWebKnownColor::AntiqueWhite), 2, 0, image.Width(), i);
    }
}

DEFINE_ENUM(MouseEventName, MouseEventType::Enum)
{
    "None",             MouseEventType::None,
    "MouseEnter",       MouseEventType::MouseEnter,
    "MouseLeave",       MouseEventType::MouseLeave,
    "MouseUp",          MouseEventType::MouseUp,
    "MouseDown",        MouseEventType::MouseDown,
    "MouseMove",        MouseEventType::MouseMove,
    "MouseWheel",       MouseEventType::MouseWheel,
    "MouseDoubleClick", MouseEventType::MouseDoubleClick,
}
END_ENUM(MouseEventName);

DEFINE_ENUM(MouseButtonName, MouseButtons::Enum)
{
    "None",             MouseButtons::None,
    "Left",             MouseButtons::Left,
    "Right",            MouseButtons::Right,
    "Middle",           MouseButtons::Middle,
    "XButton1",         MouseButtons::XButton1,
    "XButton2",         MouseButtons::XButton2,
}
END_ENUM(MouseButtonName);

DEFINE_ENUM(MouseModifiers, Modifiers::Enum)
{
    "None",             Modifiers::None,
    "Shift",            Modifiers::Shift,
    "Control",          Modifiers::Control,
    "Alternate",        Modifiers::Alternate,
    "Apple",            Modifiers::Apple,
    "Windows",          Modifiers::Windows,
}
END_ENUM(MouseModifiers);

void DDxOwnershipView::PostMouseEvent(const PureWebMouseEventArgs& mouseEvent) 
{
    
        
}

void DDxOwnershipView::RenderDeferred()
{        
    StateManager::Instance()->ViewManager().RenderViewDeferred(ViewName);
}

