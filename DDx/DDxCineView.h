//
// Copyright (c) 2011 Calgary Scientific Inc., all rights reserved.
//

#include "CSI/PureWeb/StateManager/StateManager.h"

struct CineFrame
{
	CSI::PureWeb::Image image;
	CSI::TimeSpan deltaT;
};

class DDxCineView : public CSI::PureWeb::Server::IRenderedView
{

public:
    DDxCineView();

    virtual void PostKeyEvent(const CSI::PureWeb::Ui::PureWebKeyboardEventArgs& keyEvent) {}
    virtual void PostMouseEvent(const CSI::PureWeb::Ui::PureWebMouseEventArgs& mouseEvent);
    virtual void SetClientSize(CSI::PureWeb::Size clientSize);
    virtual CSI::PureWeb::Size GetActualSize();
    virtual void RenderView(CSI::PureWeb::Server::RenderTarget target);
	
	void OnExecuteGenerateCine(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless responses);
    void OnExecuteDestroyCine(CSI::Guid sessionId, CSI::Typeless command, CSI::Typeless responses);
    void RenderDeferred();
    CSI::String ViewName;	    
    
private:
    CSI::PureWeb::Size m_clientSize;
	CSI::DateTime m_frameTime;
    CSI::Collections::List<CineFrame> m_frameList;
    CSI::Collections::List<CineFrame>::Enumerator m_frameEnumerator;    
	
protected:
    void GenerateNextCineFrame(CSI::Guid m_cinematicId);

};
