//
// Copyright (c) 2011 Calgary Scientific Inc., all rights reserved.
//

#include "CSI/PureWeb/StateManager/StateManager.h"

#define _DDx                        "/DDx"
#define _DDx_ASYNCIMAGEGENERATION   _DDx "/AsyncImageGeneration"
#define _DDx_RAPIDIMAGEGENERATION   _DDx "/RapidImageGeneration"
#define _DDx_USEDEFERREDRENDERING   _DDx "/UseDeferredRendering"
#define _DDx_USECLIENTSIZE          _DDx "/UseClientSize"
#define _DDx_USECLIENTSCALING       _DDx "/UseClientScaling"
#define _DDx_USETILES               _DDx "/UseTiles"
#define _DDx_SHOWMOUSEPOS           _DDx "/ShowMousePos"

class PGView : public CSI::PureWeb::Server::IRenderedView
{
public:
    PGView();
    ~PGView();

    virtual void PostKeyEvent(const CSI::PureWeb::Ui::PureWebKeyboardEventArgs& keyEvent);
    virtual void PostMouseEvent(const CSI::PureWeb::Ui::PureWebMouseEventArgs& mouseEvent);
    virtual void SetClientSize(CSI::PureWeb::Size clientSize);
    virtual CSI::PureWeb::Size GetActualSize();
    virtual void RenderView(CSI::PureWeb::Server::RenderTarget target);

    void Initialize();
    void Uninitialize();
    void Render();
    CSI::String ViewName;
    
private:

    CSI::Threading::PrecisionTimer m_timer;
    void TimerCallback(CSI::Threading::PrecisionTimer&, CSI::TimeSpan, void*);
    void TimerCallbackUI();

    void OnViewRendered(CSI::PureWeb::Server::IViewManager& sender, CSI::PureWeb::Server::ViewRenderedEventArgs& args);
    void AdvanceRendering();

    bool m_useClientSize;
    void OnUseClientSizeChanged(CSI::ValueChangedEventArgs const& args);

    bool m_useDeferredRendering;
    void OnUseDeferredRenderingChanged(CSI::ValueChangedEventArgs const& args);

    bool m_asyncImageGeneration;
    void OnAsyncImageGenerationChanged(CSI::ValueChangedEventArgs const& args);

    bool m_rapidImageGeneration;
    void OnRapidImageGenerationChanged(CSI::ValueChangedEventArgs const& args);

    void OnScreenshotRequested(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless& response);

    CSI::PureWeb::Size m_clientSize;
    CSI::PureWeb::Size m_imageSize;
    CSI::Collections::List<CSI::PureWeb::Image> m_imageList;
    int m_currentImage; 

    bool m_showMousePos;
    void OnShowMousePosChanged(CSI::ValueChangedEventArgs const& args);

    int m_mouseX;
    int m_mouseY;
};
