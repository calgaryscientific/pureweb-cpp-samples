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
