//
// Copyright (c) 2011 Calgary Scientific Inc., all rights reserved.
//

#include "CSI/PureWeb/StateManager/StateManager.h"


class DDxOwnershipView : public CSI::PureWeb::Server::IRenderedView
{
public:
    DDxOwnershipView();

    virtual void PostKeyEvent(const CSI::PureWeb::Ui::PureWebKeyboardEventArgs& keyEvent) {}
    virtual void PostMouseEvent(const CSI::PureWeb::Ui::PureWebMouseEventArgs& mouseEvent);
    virtual void SetClientSize(CSI::PureWeb::Size clientSize);
    virtual CSI::PureWeb::Size GetActualSize();
    virtual void RenderView(CSI::PureWeb::Server::RenderTarget target);

    void RenderDeferred();
    CSI::String ViewName;
    
private:
    CSI::PureWeb::Size m_clientSize;
};
