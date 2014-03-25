//
// Copyright (c) 2011 Calgary Scientific Inc., all rights reserved.
//

#include "Canvas.h"
#include "PGView.h"

using namespace CSI;
using namespace CSI::Threading;
using namespace CSI::PureWeb;
using namespace CSI::PureWeb::Server;
using namespace CSI::PureWeb::Ui;

static Log::Logger logger = Log::GetLogger<PGView>(); 

static void InitializeImage(Image& image)
{
    const int perRowShift = 10;

    for (SizeType i = 0; i < (SizeType)image.Height(); i++)
    {
        Byte* pRow = image.Scan0() + (i * image.RowStride());

        double pixelValue = (double)((i * perRowShift) % 256);
        double perPixelIncrement = 512.0 / image.Width();
        
        for (SizeType j = 0; j < (SizeType)image.Width(); j++)
        {
            *pRow = (Byte)pixelValue;
            *(pRow + 1) = 0;
            *(pRow + 2) = 0;
                 
            pRow += image.BytesPerPixel();
            
            if (pixelValue > 255.0 || pixelValue < 0.0)
            {
                perPixelIncrement *= -1;
            }
            pixelValue += perPixelIncrement;
        }
    }
}

static void InitializeImages(Size size, Collections::List<Image>& imageList)
{
    const int imageCount = 25;

    Image masterImage(size.Width, size.Height + imageCount, PixelFormat::Bgr24);
    InitializeImage(masterImage);

    imageList.Clear();
    for (int i = 0; i < imageCount; i++)
    {
        Image image(size.Width, size.Height, PixelFormat::Bgr24);
        ByteArray::Copy(masterImage.ImageBytes(), i * masterImage.RowStride(), image.ImageBytes(), 0, image.ImageBytes().Length());

        imageList.Add(image);
    }
}

static Size DefaultSize(1400,900);

PGView::PGView()
    : m_currentImage(0),
      m_clientSize(DefaultSize),
      m_asyncImageGeneration(false),  
      m_rapidImageGeneration(false),  
      m_useDeferredRendering(true),
      m_useClientSize(true),
      m_showMousePos(true),
      m_timer(TimeSpan::FromMilliseconds(50))   // approx 20 fps
{
}

PGView::~PGView()
{
}

void PGView::Initialize()
{
    StateManager::Instance()->XmlStateManager().AddValueChangedHandler(_DDx_ASYNCIMAGEGENERATION, Bind(this, &PGView::OnAsyncImageGenerationChanged));
    StateManager::Instance()->XmlStateManager().AddValueChangedHandler(_DDx_RAPIDIMAGEGENERATION, Bind(this, &PGView::OnRapidImageGenerationChanged));
    StateManager::Instance()->XmlStateManager().AddValueChangedHandler(_DDx_USEDEFERREDRENDERING, Bind(this, &PGView::OnUseDeferredRenderingChanged));
    StateManager::Instance()->XmlStateManager().AddValueChangedHandler(_DDx_USECLIENTSIZE, Bind(this, &PGView::OnUseClientSizeChanged));
    StateManager::Instance()->XmlStateManager().AddValueChangedHandler(_DDx_SHOWMOUSEPOS, Bind(this, &PGView::OnShowMousePosChanged));
    StateManager::Instance()->CommandManager().AddUiHandler("Screenshot", Bind(this, &PGView::OnScreenshotRequested));
    {
        TypelessStateLock lock(StateManager::Instance()->LockAppState());
        
        lock.SetValueAs<bool>(_DDx_ASYNCIMAGEGENERATION, m_asyncImageGeneration);
        lock.SetValueAs<bool>(_DDx_USEDEFERREDRENDERING, m_useDeferredRendering);
        lock.SetValueAs<bool>(_DDx_USECLIENTSIZE, m_useClientSize);
        lock.SetValueAs<bool>(_DDx_SHOWMOUSEPOS, m_showMousePos);
    }

    m_timer.StartAsync(Bind(this, &PGView::TimerCallback));
    StateManager::Instance()->ViewManager().ViewRendered() += Bind(this, &PGView::OnViewRendered);
}

void PGView::Uninitialize()
{
    m_timer.Stop(false);
    while (m_timer.State() != PrecisionTimer::Stopped)
    {
        UiDispatcher::PumpMessages(TimeSpan::FromMilliseconds(100));
    }
    
    StateManager::Instance()->XmlStateManager().RemoveValueChangedHandler(_DDx_ASYNCIMAGEGENERATION, Bind(this, &PGView::OnAsyncImageGenerationChanged));
    StateManager::Instance()->XmlStateManager().RemoveValueChangedHandler(_DDx_RAPIDIMAGEGENERATION, Bind(this, &PGView::OnRapidImageGenerationChanged));
    StateManager::Instance()->XmlStateManager().RemoveValueChangedHandler(_DDx_USEDEFERREDRENDERING, Bind(this, &PGView::OnUseDeferredRenderingChanged));
    StateManager::Instance()->XmlStateManager().RemoveValueChangedHandler(_DDx_USECLIENTSIZE, Bind(this, &PGView::OnUseClientSizeChanged)); 
    StateManager::Instance()->XmlStateManager().RemoveValueChangedHandler(_DDx_SHOWMOUSEPOS, Bind(this, &PGView::OnShowMousePosChanged));

    m_clientSize = Size(0, 0);
}

void PGView::OnScreenshotRequested(CSI::Guid sessionId, CSI::Typeless typeless, CSI::Typeless& response)
{
    Image current;
    if (m_imageList.Count() > 0)
    {
        current = m_imageList[m_currentImage % m_imageList.Count()];
        ByteArray bytes = JpegEncoder::JpegCompress(current, 100);
        ContentInfo cinfo("image/jpeg", bytes);

        CSI::Guid key = StateManager::Instance()->ResourceManager().Store(cinfo);
        response["ResourceKey"] = key;
    }
    else
    {
        CSI_THROW(CSI::Exception, "Unable to generate image from empty image list.");
    }
}

void PGView::AdvanceRendering()
{
    CSI_ASSERT(UiDispatcher::IsMessageThread());

    if (m_imageList.Count() > 0)
       m_currentImage = (m_currentImage + 1) % m_imageList.Count();
    
    Render();
}

void PGView::TimerCallbackUI()
{
    if (m_asyncImageGeneration)
    {
        AdvanceRendering();
    } 
}

void PGView::TimerCallback(CSI::Threading::PrecisionTimer& timer, CSI::TimeSpan, void*)
{
    Function<void(void)> renderFunc = Bind(this, &PGView::TimerCallbackUI);
    UiDispatcher::Invoke(renderFunc);
}

void PGView::OnAsyncImageGenerationChanged(CSI::ValueChangedEventArgs const& args)
{
    m_asyncImageGeneration = args.NewValue().ConvertOr<bool>(m_asyncImageGeneration);
    logger.Info.Format("AsyncImageGeneration set to {0}", m_asyncImageGeneration ? "true" : "false");
}

void PGView::OnRapidImageGenerationChanged(CSI::ValueChangedEventArgs const& args)
{
    m_rapidImageGeneration = args.NewValue().ConvertOr<bool>(m_rapidImageGeneration);
    logger.Info.Format("RapidImageGeneration set to {0}", m_rapidImageGeneration ? "true" : "false");
}

void PGView::OnUseDeferredRenderingChanged(CSI::ValueChangedEventArgs const& args)
{
    m_useDeferredRendering = args.NewValue().ConvertOr<bool>(m_useDeferredRendering);
}

void PGView::OnUseClientSizeChanged(CSI::ValueChangedEventArgs const& args)
{
    m_useClientSize = args.NewValue().ConvertOr<bool>(m_useClientSize);
    AdvanceRendering();
}

void PGView::OnShowMousePosChanged(CSI::ValueChangedEventArgs const& args)
{
    m_showMousePos = args.NewValue().ConvertOr<bool>(m_showMousePos);
}

void PGView::SetClientSize(Size clientSize)
{
    m_clientSize = clientSize;
}

Size PGView::GetActualSize()
{
    if (m_useClientSize)
    {
        return m_clientSize; 
    }
    else
    {
        return DefaultSize;
    }
}

void PGView::RenderView(RenderTarget target)
{    
    Image current;
    Image image = target.RenderTargetImage();

    if (m_imageList.Count() > 0)
    {
        current = m_imageList[m_currentImage % m_imageList.Count()];
    }

    if (image.Size() != current.Size())
    {
        InitializeImages(image.Size(), m_imageList);
        current = m_imageList[m_currentImage % m_imageList.Count()];
    }
    
    CSI_ASSERT(current.RowStride() == image.RowStride() &&
               current.Height() == image.Height());

    current.CopyTo(image);

    if (m_showMousePos)
    {
        const int width = 10;
        Canvas canvas(image);
        PureWebColor red = PureWebColor::FromKnownColor(PureWebKnownColor::Red);
        //canvas.FillRect(red, m_mouseX-width/2, m_mouseY-width/2, m_mouseX+width/2, m_mouseY+width/2);
        canvas.FillCircle(red, m_mouseX, m_mouseY, 11);
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

void PGView::PostKeyEvent(const CSI::PureWeb::Ui::PureWebKeyboardEventArgs& keyEvent)
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

void PGView::PostMouseEvent(const PureWebMouseEventArgs& mouseEvent) 
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

    if (mouseEvent.EventType == MouseEventType::MouseMove)
    {
        //logger.Debug.Format("MouseMove {0} {1}", mouseEvent.X, mouseEvent.Y);
        m_mouseX = (int)mouseEvent.X;
        m_mouseY = (int)mouseEvent.Y;

        if (!m_asyncImageGeneration)
        {
            AdvanceRendering();
        }
    }
    else if (mouseEvent.EventType == MouseEventType::MouseDown)
    {
         StateManager::Instance()->ViewManager().SetViewInteracting(ViewName, true);
    }
    else if (mouseEvent.EventType == MouseEventType::MouseUp)
    {
         StateManager::Instance()->ViewManager().SetViewInteracting(ViewName, false);
    }
}

void PGView::Render()
{        
    if (m_useDeferredRendering)
    {
        StateManager::Instance()->ViewManager().RenderViewDeferred(ViewName);
    }
    else
    {
        StateManager::Instance()->ViewManager().RenderViewImmediate(ViewName);
    }
}

void PGView::OnViewRendered(IViewManager& sender, ViewRenderedEventArgs& args)
{
    if(m_rapidImageGeneration)
    {
        UiDispatcher::BeginInvoke(Bind(this, &PGView::AdvanceRendering));
    }
}
