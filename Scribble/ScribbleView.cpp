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

// ScribbleView.cpp : implementation of the CScribbleView class
//

#include "stdafx.h"
#include "Scribble.h"

#include "ScribbleDoc.h"
#include "ScribbleView.h"

using namespace CSI;
using namespace CSI::PureWeb;
using namespace CSI::PureWeb::Server;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CScribbleView

IMPLEMENT_DYNCREATE(CScribbleView, CView)

BEGIN_MESSAGE_MAP(CScribbleView, CView)
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()

// CScribbleView construction/destruction

CScribbleView::CScribbleView()
{
    m_startpt = -1;
    m_endpt = -1;
    m_cColor = COLORREF(RGB(0,0,0));
    CScribbleApp::StateManager().ViewManager().RegisterView("ScribbleView", this);
    
    ViewImageFormat viewImageFormat;
    viewImageFormat.PixelFormat = PixelFormat::Bgr24;
    viewImageFormat.ScanLineOrder = ScanLineOrder::TopDown;
    viewImageFormat.Alignment = 4;
    CScribbleApp::StateManager().ViewManager().SetViewImageFormat("ScribbleView", viewImageFormat);

    CScribbleApp::StateManager().CommandManager().AddUiHandler("Clear", Bind(this, &CScribbleView::OnExecuteClear));
    CScribbleApp::StateManager().XmlStateManager().AddValueChangedHandler("ScribbleColor", Bind(this, &CScribbleView::OnScribbleColorChanged));
    m_hdcOffscreen = CreateCompatibleDC(NULL);
    m_dcOffscreen.Attach(m_hdcOffscreen);   

    TypelessStateLock lock(CScribbleApp::StateManager().LockAppState());
    lock.Element("ScribbleColor").SetValue("Black");
}

CScribbleView::~CScribbleView()
{
    CScribbleApp::StateManager().ViewManager().UnregisterView("ScribbleView");
   
    if (m_hBitmap)
    {
        SelectObject(m_hdcOffscreen, m_hbitMapOld);
        DeleteObject(m_hBitmap);
    }
}

BOOL CScribbleView::PreCreateWindow(CREATESTRUCT& cs)
{
    return CView::PreCreateWindow(cs);
}

// CScribbleView drawing

void CScribbleView::OnDraw(CDC* /*pDC*/)
{
    CScribbleDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    CClientDC dc(this);
    CRect rect;
    GetClientRect(&rect);
    dc.BitBlt(rect.TopLeft().x,rect.TopLeft().y,rect.Width(),rect.Height(),&m_dcOffscreen,rect.TopLeft().x,rect.TopLeft().y,SRCCOPY);
}


// CScribbleView diagnostics

#ifdef _DEBUG
void CScribbleView::AssertValid() const
{
    CView::AssertValid();
}

void CScribbleView::Dump(CDumpContext& dc) const
{

    CView::Dump(dc);
}

CScribbleDoc* CScribbleView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScribbleDoc)));
    return (CScribbleDoc*)m_pDocument;
}
#endif //_DEBUG


// CScribbleView message handlers

void CScribbleView::OnSize(UINT nType, int cx, int cy)
{
    __super::OnSize(nType, cx, cy);

    SetClientSize(Size(cx,cy));
}

void CScribbleView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_startpt.x = point.x;
    m_startpt.y = point.y;
    CScribbleApp::StateManager().ViewManager().SetViewInteracting("ScribbleView", true);
}

void CScribbleView::OnMouseMove(UINT nFlags, CPoint point)
{
    m_endpt.x = point.x;
    m_endpt.y = point.y;
 
    if (m_startpt.x != -1)
    {
        CPen pen( PS_SOLID, 4, m_cColor);

        CPen* pOldPen = m_dcOffscreen.SelectObject( &pen );

        m_dcOffscreen.MoveTo(m_startpt.x, m_startpt.y);
        m_dcOffscreen.LineTo(m_endpt.x, m_endpt.y);
        m_startpt.x = m_endpt.x;
        m_startpt.y = m_endpt.y;

        CScribbleApp::StateManager().ViewManager().RenderViewDeferred("ScribbleView");
        Invalidate(FALSE);
    }

    CView::OnMouseMove(nFlags, point);
}

void CScribbleView::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_startpt = -1;
    CView::OnLButtonUp(nFlags, point);
    CScribbleApp::StateManager().ViewManager().SetViewInteracting("ScribbleView", false);
}

void CScribbleView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    CBrush br(RGB(255,255,255));
    m_dcOffscreen.SelectObject(&br);
    CRect rect(0,0,m_Width,m_Height);
    CBrush *pbr = &br;
    m_dcOffscreen.FillRect(&rect,pbr);
    m_startpt = -1;

    CScribbleApp::StateManager().ViewManager().RenderViewDeferred("ScribbleView");
    Invalidate();
    CView::OnLButtonDblClk(nFlags, point);
}

void CScribbleView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
    CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

    if (GetActualSize().IsEmpty())
    {
        CRect rect;
        GetClientRect(&rect);
        SetClientSize(Size(rect.Width(), rect.Height()));
    }
}

// PureWeb::IRenderedView
void CScribbleView::SetClientSize(Size clientSize)
{
    HBITMAP hLastBitmap = NULL;
    int lastWidth;
    int lastHeight;

    if (m_Width != clientSize.Width || m_Height != clientSize.Height)
    {
        if (m_hBitmap != NULL)
        {
            hLastBitmap = m_hBitmap;
            lastWidth = m_Width;
            lastHeight = m_Height;

            SelectObject(m_hdcOffscreen, m_hbitMapOld);
            m_hbitMapOld = NULL;
            m_hBitmap = NULL;
            m_pPixelBits = NULL;
        }

        m_Height = clientSize.Height;
        m_Width = clientSize.Width;
    }

    if (m_hBitmap == NULL)
    {
        BITMAPINFO bmi;
        memset(&bmi, 0, sizeof(bmi));
        bmi.bmiHeader.biSize        = sizeof(bmi);
        bmi.bmiHeader.biWidth       = m_Width;
        bmi.bmiHeader.biHeight      = -m_Height;   // top down
        bmi.bmiHeader.biPlanes      = 1;
        bmi.bmiHeader.biBitCount    = 24;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biSizeImage   = 0;

        m_hBitmap = CreateDIBSection(m_hdcOffscreen, &bmi, DIB_RGB_COLORS, &m_pPixelBits, NULL, 0);
        m_hbitMapOld = (HBITMAP)SelectObject(m_hdcOffscreen, m_hBitmap);
    }

    CBrush br(RGB(255,255,255));
    m_dcOffscreen.SelectObject(&br);
    CRect rect(0,0,m_Width,m_Height);
    CBrush *pbr = &br;
    m_dcOffscreen.FillRect(&rect,pbr);

    if (hLastBitmap)
    {
        HDC tempHDC = CreateCompatibleDC(m_hdcOffscreen);
        HBITMAP hOrigBitmap = (HBITMAP)SelectObject(tempHDC, hLastBitmap);
        BitBlt(m_hdcOffscreen, 0, 0, lastWidth, lastHeight, tempHDC, 0, 0, SRCCOPY);
        SelectObject(tempHDC, hOrigBitmap);
        DeleteObject(hLastBitmap);
    }

    CScribbleApp::StateManager().ViewManager().RenderViewDeferred("ScribbleView");
    CView::OnSize(SIZE_RESTORED,  clientSize.Width, clientSize.Height);
}

Size CScribbleView::GetActualSize()
{
    return Size(m_Width, m_Height);
}

void CScribbleView::RenderView(PureWeb::Server::RenderTarget target)
{
    Image image = target.RenderTargetImage();
    ByteArray::Copy(m_pPixelBits, image.ImageBytes(), 0, image.ImageBytes().Count());
}

void CScribbleView::PostKeyEvent(const Ui::PureWebKeyboardEventArgs& keyEvent)
{
    Ui::KeyboardEventType::Enum keyEventType = keyEvent.EventType;

    CScribbleApp::StateManager().XmlStateManager().SetValueAs<Ui::KeyboardEventType::Enum>("/ScribbleView/KeyEvent/Type", keyEventType);
    CScribbleApp::StateManager().XmlStateManager().SetValueAs<int>("/ScribbleView/KeyEvent/CharacterCode", keyEvent.CharacterCode);
    CScribbleApp::StateManager().XmlStateManager().SetValueAs<Ui::KeyCode::Enum>("/ScribbleView/KeyEvent/KeyCode", keyEvent.KeyCode);

    bool isAltDown = 0 != (keyEvent.Modifiers & Ui::Modifiers::Alternate);

    WPARAM wParam = (int)keyEvent.KeyCode;
    LPARAM lParam = isAltDown ? (1 << 29) : 0; // "context code";
    UINT message;

    if (isAltDown || keyEvent.KeyCode == Ui::KeyCode::F10)
    {
        message = keyEventType == Ui::KeyboardEventType::KeyDown ? WM_SYSKEYDOWN : WM_SYSKEYUP;
    }
    else
    {
        message = keyEventType == Ui::KeyboardEventType::KeyDown ? WM_KEYDOWN : WM_KEYUP;
    }

    if (message != 0)
    {
        ::PostMessage(m_hWnd,message, wParam, lParam);
    }
}

void CScribbleView::PostMouseEvent(const Ui::PureWebMouseEventArgs& mouseEvent)
{
    Ui::MouseEventType::Enum mouseEventType = mouseEvent.EventType;

    WPARAM wParam = 0;
    LPARAM lParam = 0;
    UINT message = 0;

    // wParam
    if (0 != (mouseEvent.Modifiers & Ui::Modifiers::Shift))     wParam |= MK_SHIFT;
    if (0 != (mouseEvent.Modifiers & Ui::Modifiers::Control))   wParam |= MK_CONTROL;
    if (0 != (mouseEvent.Buttons & Ui::MouseButtons::Left))     wParam |= MK_LBUTTON;
    if (0 != (mouseEvent.Buttons & Ui::MouseButtons::Right))    wParam |= MK_RBUTTON;
    if (0 != (mouseEvent.Buttons & Ui::MouseButtons::Middle))   wParam |= MK_MBUTTON;
    if (0 != (mouseEvent.Buttons & Ui::MouseButtons::XButton1)) wParam |= MK_XBUTTON1;
    if (0 != (mouseEvent.Buttons & Ui::MouseButtons::XButton2)) wParam |= MK_XBUTTON2;

    // for negative X coordinates we need to mask the first 16 bits otherwise the
    // following "OR" bitwise operation will not be correct
    lParam = (int) mouseEvent.X & 0x0000ffff;  
    lParam |= ((int)mouseEvent.Y) << 16;

    switch (mouseEventType)
    {
    case Ui::MouseEventType::MouseEnter:
        //OnMouseEnter(EventArgs.Empty);
        return;

    case Ui::MouseEventType::MouseLeave:
        //OnMouseLeave(EventArgs.Empty);
        return;

    case Ui::MouseEventType::MouseMove:
        message = WM_MOUSEMOVE;
        break;

    case Ui::MouseEventType::MouseDown:
        if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::Left))
        {
            message = WM_LBUTTONDOWN;
        }
        else if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::Right))
        {
            message = WM_RBUTTONDOWN;
        }
        else if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::Middle))
        {
            message = WM_MBUTTONDOWN;
        }
        else if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::XButton1))
        {
            message = WM_XBUTTONDOWN;
            wParam |= XBUTTON1 << 16; 
        }
        else if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::XButton2))
        {
            message = WM_XBUTTONDOWN;
            wParam |= XBUTTON2 << 16;
        }
        break;

    case Ui::MouseEventType::MouseUp:
        if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::Left))
        {
            message = WM_LBUTTONUP;
        }
        else if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::Right))
        {
            message = WM_RBUTTONUP;
        }
        else if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::Middle))
        {
            message = WM_MBUTTONUP;
        }
        else if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::XButton1))
        {
            message = WM_XBUTTONUP;
            wParam |= XBUTTON1 << 16;
        }
        else if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::XButton2))
        {
            message = WM_XBUTTONUP;
            wParam |= XBUTTON2 << 16;
        }
        break;

    case Ui::MouseEventType::MouseDoubleClick:
        if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::Left))
        {
            message = WM_LBUTTONDBLCLK;
        }
        else if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::Right))
        {
            message = WM_RBUTTONDBLCLK;
        }
        else if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::Middle))
        {
            message = WM_MBUTTONDBLCLK;
        }
        else if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::XButton1))
        {
            message = WM_XBUTTONDBLCLK;
            wParam |= XBUTTON1 << 16;
        }
        else if (0 != (mouseEvent.ChangedButton & Ui::MouseButtons::XButton2))
        {
            message = WM_XBUTTONDBLCLK;
            wParam |= XBUTTON2 << 16;
        }
        break;

    case Ui::MouseEventType::MouseWheel:
        message = WM_MOUSEWHEEL;
        wParam |= ((int)mouseEvent.Delta) << 16;
        break;

    default:
        return;
    }

    if (message != 0)
    {
        ::PostMessage(m_hWnd,message, wParam, lParam);
    }
}

void CScribbleView::OnExecuteClear(Guid sessionId, Typeless command, Typeless responses)
{
    CBrush br(RGB(255,255,255));
    m_dcOffscreen.SelectObject(&br);
    CRect rect(0,0,m_Width,m_Height);
    CBrush *pbr = &br;
    m_dcOffscreen.FillRect(&rect,pbr);
    m_startpt = -1;

    CScribbleApp::StateManager().ViewManager().RenderViewDeferred("ScribbleView");
    Invalidate();
}

void CScribbleView::OnScribbleColorChanged(ValueChangedEventArgs args)
{
    String color = args.NewValue().As<String>().ToLower(Locale::Invariant());
    NamedColors c = black;
    if (!(c.TryParse(color, c)))
    {
        TypelessStateLock lock(CScribbleApp::StateManager().LockAppState());
        lock.Element("ScribbleColor").SetValue("black");
    }
    m_cColor = c;
}

