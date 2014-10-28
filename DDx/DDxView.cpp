//
// Copyright (c) 2011 Calgary Scientific Inc., all rights reserved.
//

#include "Canvas.h"
#include "DDxView.h"

using namespace CSI;
using namespace CSI::PureWeb;
using namespace CSI::PureWeb::Server;
using namespace CSI::PureWeb::Ui;

static Log::Logger logger = Log::GetLogger<DDxView>(); 

DDxView::DDxView()
{
    static int i = 0;
    
    m_colorIndex = i++;
}

void DDxView::SetClientSize(Size clientSize)
{
    m_clientSize = clientSize;
}

Size DDxView::GetActualSize()
{
    return m_clientSize;
}

void DDxView::RenderView(RenderTarget target)
{    
    PureWebColor bgColor;
    Image image = target.RenderTargetImage();
    
    switch(m_colorIndex % 6)
    {
    case 1:
        bgColor = PureWebColor::FromKnownColor(PureWebKnownColor::DarkBlue);
        break;
    case 2:
        bgColor = PureWebColor::FromKnownColor(PureWebKnownColor::DarkGreen);
        break;
    case 3:
        bgColor = PureWebColor::FromKnownColor(PureWebKnownColor::DarkMagenta);
        break;
    case 4:
        bgColor = PureWebColor::FromKnownColor(PureWebKnownColor::DarkCyan);
        break;
    case 5:
        bgColor = PureWebColor::FromKnownColor(PureWebKnownColor::DarkRed);
        break;
    default:
        bgColor = PureWebColor::FromKnownColor(PureWebKnownColor::Black);
        break;
    }
    
    Canvas canvas(image);
    canvas.Clear(bgColor);   

    bool gridOn = true;
    int margin = 0;
    int width = 4;
    int spacing = 100;

    {
        TypelessStateLock lock(StateManager::Instance()->LockAppState());
        lock.SetValueAs<PureWebColor>(Text::Format("/DDx/{0}/BackgroundColor", ViewName), bgColor);

        gridOn = lock.GetValueAs<bool>(_DDx_GRID_ON);
        margin = lock.GetValueAs<int>(_DDx_GRID_MARGIN);
        width = lock.GetValueAs<int>(_DDx_GRID_LINEWIDTH);
        spacing = lock.GetValueAs<int>(_DDx_GRID_LINESPACING);
    }

    if (gridOn)
    {
        Int32 w = image.Width();
        Int32 h = image.Height();
        PureWebColor lineColor(PureWebColor::FromKnownColor(PureWebKnownColor::DarkSlateBlue));

        for (int x = spacing; x < w; x += spacing)
        {
            canvas.DrawVLine(lineColor, width, margin, h-margin, x);
        }

        for (int y = spacing; y < h; y += spacing)
        {
            canvas.DrawHLine(lineColor, width, margin, w-margin, y);
        }
    }

}
 
DEFINE_ENUM(KeyboardEventName, KeyboardEventType::Enum)
{
    "None",             KeyboardEventType::None,
    "KeyDown",       KeyboardEventType::KeyDown,
    "KeyUp",       KeyboardEventType::KeyUp,
}
END_ENUM(KeyboardEventName);

DEFINE_ENUM(InputEventModifiers, Modifiers::Enum)
{
    "None",             Modifiers::None,
    "Shift",            Modifiers::Shift,
    "Control",          Modifiers::Control,
    "Alternate",        Modifiers::Alternate,
    "Apple",            Modifiers::Apple,
    "Windows",          Modifiers::Windows,
}
END_ENUM(InputEventModifiers);

void DDxView::PostKeyEvent(const CSI::PureWeb::Ui::PureWebKeyboardEventArgs& keyEvent)
{
    {
        TypelessStateLock lock(StateManager::Instance()->LockAppState());

        String path = Text::Format("/DDx/{0}/KeyEvent", ViewName);

        lock.SetValue(path + "/Type", KeyboardEventName::GetName(keyEvent.EventType));
        lock.SetValueAs<int>(path + "/KeyCode", keyEvent.KeyCode);
        lock.SetValueAs<int>(path + "/CharacterCode", keyEvent.CharacterCode);
        lock.SetValue(path + "/Modifiers", InputEventModifiers::GetName(keyEvent.Modifiers));
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

void DDxView::PostMouseEvent(const PureWebMouseEventArgs& mouseEvent) 
{
    {
        TypelessStateLock lock(StateManager::Instance()->LockAppState());

        String path = Text::Format("/DDx/{0}/MouseEvent", ViewName);

        lock.SetValue(path + "/Type", MouseEventName::GetName(mouseEvent.EventType));
        lock.SetValueAs<double>(path + "/X", mouseEvent.X);
        lock.SetValueAs<double>(path + "/Y", mouseEvent.Y);
        lock.SetValue(path + "/Buttons", MouseButtonName::GetName(mouseEvent.Buttons));
        lock.SetValue(path + "/ChangedButton", MouseButtonName::GetName(mouseEvent.ChangedButton));
        lock.SetValue(path + "/Modifiers", InputEventModifiers::GetName(mouseEvent.Modifiers));
    }

    if (mouseEvent.EventType == MouseEventType::MouseDoubleClick &&
        mouseEvent.Buttons == MouseButtons::Left)
    {
        ++m_colorIndex;
        RenderDeferred();
    }
        
}

void DDxView::RenderDeferred()
{        
    StateManager::Instance()->ViewManager().RenderViewDeferred(ViewName);
}

