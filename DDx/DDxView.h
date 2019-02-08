/// Copyright 2013-2019 Calgary Scientific Inc. (operating under the brand name of PureWeb)
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

    int GetBkColorIndex();
    void SetBkColorIndex(int colorIndex);

    CSI::String ViewName;
    
private:
    int m_colorIndex;
    long m_wheelDelta;
    CSI::PureWeb::Size m_clientSize;
};
