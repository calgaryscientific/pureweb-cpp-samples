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

#include "Canvas.h"

using namespace CSI;
using namespace CSI::PureWeb;

static Log::Logger logger = Log::GetLogger<Canvas>(); 

Canvas::Canvas(Image image)
{
    CSI_ASSERT(image.BytesPerPixel() == 3);
    CSI_ASSERT(image.PixelFormat() == PixelFormat::Bgr24);

    m_image = image;
    m_pixels = image.Scan0();
    m_stride = image.RowStride();
    m_width = image.Width();
    m_height = image.Height();
}
  
void Canvas::SetPixel(PureWebColor color, int x, int y)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
    {
        return;
    }

    int index = x*3 + y*m_stride;

    m_pixels[index + 0] = color.B();
    m_pixels[index + 1] = color.G();
    m_pixels[index + 2] = color.R();
}

void Canvas::Clear(PureWebColor color)
{
    Int32 w = m_image.Width();
    Int32 h = m_image.Height();

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            SetPixel(color, x, y);
        }
    }
}
    
void Canvas::DrawHLine(PureWebColor color, int width, int x1, int x2, int y1)
{    
    for(int x = x1; x <= x2; x++)
    {
        for(int y = y1-width/2; y <= y1+width/2; y++)
        {
            SetPixel(color, x, y);
        }
    }
}
    
void Canvas::DrawVLine(PureWebColor color, int width, int y1, int y2, int x1)
{ 
    for(int y = y1; y <= y2; y++)
    {
        for(int x = x1-width/2; x <= x1+width/2; x++)
        {
            SetPixel(color, x, y);
        }
    }
}    

void Canvas::FillRect(PureWebColor color, int x1, int y1, int x2, int y2)
{ 
    for(int y = y1; y <= y2; y++)
    {
        for(int x = x1; x <= x2; x++)
        {
            SetPixel(color, x, y);
        }
    }
}

void Canvas::FillCircle(PureWebColor color, int x, int y, int radius)
{ 
    for(int dy = -radius; dy < radius; dy++)
    {
        int py = y + dy;
        int dx = (int)(Math::Sqrt(radius*radius - dy*dy) + 0.5);
        for(int px = x - dx; px < x + dx; px++)
        {
            SetPixel(color, px, py);
        }
    }
}
