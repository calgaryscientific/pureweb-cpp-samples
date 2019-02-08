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

class Canvas
{
public:
    Canvas(CSI::PureWeb::Image image);

    void Clear(CSI::PureWeb::PureWebColor color);
    void DrawHLine(CSI::PureWeb::PureWebColor color, int width, int x1, int x2, int y);
    void DrawVLine(CSI::PureWeb::PureWebColor color, int width, int y1, int y2, int x);
    void FillRect(CSI::PureWeb::PureWebColor color, int x1, int y1, int x2, int y2);
    void FillCircle(CSI::PureWeb::PureWebColor color, int x, int y, int radius);
    void SetPixel(CSI::PureWeb::PureWebColor color, int x, int y);

private:
    CSI::PureWeb::Image m_image;
    CSI::Byte* m_pixels;
    CSI::Int32 m_stride;
    CSI::Int32 m_width;
    CSI::Int32 m_height;
};
