//
// Copyright (c) 2011 Calgary Scientific Inc., all rights reserved.
//

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
