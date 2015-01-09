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

#include <QApplication>

#include "mainwindow.h"
#include "pureweb.h"
#include "QtMessageTickler.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    CSI::CountedPtr<CSI::PureWeb::Server::StateManagerServer> server;
    CSI::CountedPtr<CSI::PureWeb::Server::StateManager> stateManager;

    // Init CSI / PureWeb libraries
    CSI::Library::Initialize();
    CSI::Threading::UiDispatcher::InitMessageThread();    // this thread is the UI thread

    // Create PureWeb object instances
    server = new CSI::PureWeb::Server::StateManagerServer();
    stateManager = new CSI::PureWeb::Server::StateManager();

    if (argc > 1 && QString(argv[1]).contains("PureWeb", Qt::CaseInsensitive))
    {
        stateManager->PluginManager().RegisterPlugin("QtMessageTickler", new QtMessageTickler());


	QString serverAddress;
	QString port = "8082";

	if( argc > 2 && QString(argv[2]).contains("Server", Qt::CaseInsensitive) )
	  {
	    serverAddress = QString(argv[2]).split("=")[1];
	  }

	if( argc > 3 && QString(argv[3]).contains("Port", Qt::CaseInsensitive) )
	  {
	    port = QString(argv[3]).split("=")[1];
	  }

	if( serverAddress.isEmpty() ) {
	  qDebug() << "serverAddress is empty";
	  server->Start(stateManager.get());
	} else {
	  qDebug() << "serverAddress is not empty " << serverAddress << " , " << port;
	  CSI::String sAddress = "127.0.0.1"; //serverAddress.toStdString().c_str();
	  int sPort = port.toInt();
	  server->Start(stateManager.get(),sAddress,8085);
	}

        server->ShutdownRequested() += PureWebCommon::OnPureWebShutdown;
    }

    QApplication app(argc, argv);
    MainWindow window;

#if defined(Q_OS_SYMBIAN)
    window.showMaximized();
#else
    window.show();
#endif
    return app.exec();
}

