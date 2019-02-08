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

