//
// Copyright (c) 2011 Calgary Scientific Inc., all rights reserved.
//

#include "CSI/PureWeb/StateManager/StateManager.h"

#define _DDx                    "/DDx"
#define _DDx_GRID               _DDx "/Grid"
#define _DDx_GRID_ON            _DDx_GRID "/On"
#define _DDx_GRID_MARGIN        _DDx_GRID "/Margin"
#define _DDx_GRID_LINESPACING   _DDx_GRID "/LineSpacing"
#define _DDx_GRID_LINEWIDTH     _DDx_GRID "/LineWidth"


class DDxView : public CSI::PureWeb::Server::IRenderedView
{
public:
    DDxView();

    virtual void PostKeyEvent(const CSI::PureWeb::Ui::PureWebKeyboardEventArgs& keyEvent);
    virtual void PostMouseEvent(const CSI::PureWeb::Ui::PureWebMouseEventArgs& mouseEvent);
    virtual void SetClientSize(CSI::PureWeb::Size clientSize);
    virtual CSI::PureWeb::Size GetActualSize();
    virtual void RenderView(CSI::PureWeb::Server::RenderTarget target);

    void RenderDeferred();
    CSI::String ViewName;
    
private:
    int m_colorIndex;
    CSI::PureWeb::Size m_clientSize;
};
