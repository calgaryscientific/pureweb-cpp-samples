/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include "scribblearea.h"
#include "pureweb.h"

using namespace CSI;
using namespace CSI::PureWeb;
using namespace CSI::PureWeb::Server;


ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent),
      myPenWidth(2)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbling = false;

    // Register PureWeb view
    ViewRegistrationOptions vo;
    vo.PreferHWAcceleratedEncoder = true;
    vo.RenderForCollaborators = false;    
    PureWebCommon::StateManager().ViewManager().RegisterView("ScribbleView", this, vo);

    // Set view image format
    ViewImageFormat viewImageFormat;
    viewImageFormat.PixelFormat = PixelFormat::Rgb24;
    viewImageFormat.ScanLineOrder = ScanLineOrder::TopDown;
    viewImageFormat.Alignment = 4;
    PureWebCommon::StateManager().ViewManager().SetViewImageFormat("ScribbleView", viewImageFormat);

    // Bind events from client.
    PureWebCommon::StateManager().CommandManager().AddUiHandler("Clear", Bind(this, &ScribbleArea::OnExecuteClear));

    // ScribbleAppCpp is using different value for color
    PureWebCommon::StateManager().XmlStateManager().AddValueChangedHandler("ScribbleColor", Bind(this, &ScribbleArea::OnScribbleColorChanged));

}


ScribbleArea::~ScribbleArea()
{
    PureWebCommon::StateManager().ViewManager().UnregisterView("ScribbleView");
}


CSI::PureWeb::Size ScribbleArea::GetActualSize()
{
    return CSI::PureWeb::Size(width(),height());
}


void ScribbleArea::PostMouseEvent(const CSI::PureWeb::Ui::PureWebMouseEventArgs& mouseEvent)
{
    QEvent::Type action = QEvent::None;
    Qt::MouseButton button;
    Qt::MouseButtons buttons;
    Qt::KeyboardModifiers keys = 0;

    switch(mouseEvent.EventType)
    {
        case CSI::PureWeb::Ui::MouseEventType::MouseDown:
            action = QEvent::MouseButtonPress;
            break;
        case CSI::PureWeb::Ui::MouseEventType::MouseUp:
            action = QEvent::MouseButtonRelease;
            break;
        case CSI::PureWeb::Ui::MouseEventType::MouseMove:
            action = QEvent::MouseMove;
            break;
        default:
            return;
            break;
    }

    switch (mouseEvent.ChangedButton)
    {
        case CSI::PureWeb::Ui::MouseButtons::Left:
            button = Qt::LeftButton;
            break;
        case CSI::PureWeb::Ui::MouseButtons::Right:
            button = Qt::RightButton;
            break;
        default:
            button = Qt::NoButton;
            break;
    }

    if (0 != (mouseEvent.Modifiers & Ui::Modifiers::Shift))     keys |= Qt::ShiftModifier;
    if (0 != (mouseEvent.Modifiers & Ui::Modifiers::Control))   keys |= Qt::ControlModifier;
    if (0 != (mouseEvent.Buttons & Ui::MouseButtons::Left))     buttons |= Qt::LeftButton;
    if (0 != (mouseEvent.Buttons & Ui::MouseButtons::Right))    buttons |= Qt::RightButton;
    if (0 != (mouseEvent.Buttons & Ui::MouseButtons::Middle))   buttons |= Qt::MidButton;
    if (0 != (mouseEvent.Buttons & Ui::MouseButtons::XButton1)) buttons |= Qt::XButton1;
    if (0 != (mouseEvent.Buttons & Ui::MouseButtons::XButton2)) buttons |= Qt::XButton2;



    QMouseEvent * m = new QMouseEvent(action, QPoint(mouseEvent.X,mouseEvent.Y), button, buttons, keys);
    QCoreApplication::postEvent(this, m);

}


void ScribbleArea::RenderView(CSI::PureWeb::Server::RenderTarget renderTarget)
{
    // Copy scribble image into PureWeb image.
    // Note: Can only direct copy byte array if image format is QImage::Format_RGB888.

    ByteArray imageBytes = renderTarget.RenderTargetImage().ImageBytes();
    ByteArray::Copy((void*)this->image.bits(), imageBytes, CSI::SizeType(0), imageBytes.Count());
}


void ScribbleArea::SetClientSize(CSI::PureWeb::Size clientSize)
{
    //QResizeEvent * e = new QResizeEvent(QSize(clientSize.Width,clientSize.Height),size());
    //QCoreApplication::postEvent(this->parentWidget(),e);

    QSize newSize = QSize(clientSize.Width, clientSize.Height);

    if (size() != newSize)
    {
        this->parentWidget()->resize(newSize);
        resize(newSize);
        update();
    }
}


void ScribbleArea::OnExecuteClear(Guid sessionId, Typeless command, Typeless responses)
{
    clearImage();
}


void ScribbleArea::OnScribbleColorChanged(ValueChangedEventArgs args)
{
    QString colorName (args.NewValue().As<String>().ToLower(Locale::Invariant()).ToAscii().begin());

    QColor color(colorName);

    if (!color.isValid())
    {
        color = QColor("black");
        TypelessStateLock lock(PureWebCommon::StateManager().LockAppState());
        lock.Element("ScribbleColor").SetValue("black");
    }
    setPenColor(color);
}


void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}


void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}


void ScribbleArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}



void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
        PureWebCommon::StateManager().ViewManager().SetViewInteracting("ScribbleView", true);
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
    {
        drawLineTo(event->pos());
    }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
        PureWebCommon::StateManager().ViewManager().SetViewInteracting("ScribbleView", false);
    }
}


void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);

    PureWebCommon::StateManager().ViewManager().RenderViewDeferred("ScribbleView");

}


void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    int newWidth = qMax(10, event->size().width());
    int newHeight = qMax(10, event->size().height());
    resizeImage(&image, QSize(newWidth, newHeight));

    update();

    QWidget::resizeEvent(event);
}


void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}


void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    // Note: Must use Format_RGB888 for PureWeb 24bit color mode.
    QImage newImage(newSize, QImage::Format_RGB888);

    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}







