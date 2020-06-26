/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
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
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef PONG_H
#define PONG_H

#include <QObject>
#include <QDebug>
#include <QDateTime>

class Pong: public QObject
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.ScreenSaver")

public slots:
    Q_SCRIPTABLE quint32 Inhibit(const QString &appname, const QString &reason) {
        qDebug() << QDateTime::currentDateTime().toString(Qt::ISODate) << objectName() << QDateTime::currentDateTime().toString(Qt::ISODate) << appname << "Inhibit" << reason;
        static int cookie = 0;
        handedOut[++cookie] = appname;
        return cookie ^ 0xf4c0ff;
    }
    Q_SCRIPTABLE quint32 Inhibit(const QString &appname, const quint32 xid, const QString &reason, const quint32 flags) {
        qDebug() << QDateTime::currentDateTime().toString(Qt::ISODate) << objectName() << QDateTime::currentDateTime().toString(Qt::ISODate) << appname << "Inhibit" << reason << xid << flags;
        static int cookie = 0;
        handedOut[++cookie] = appname;
        return cookie ^ 0xf4c0ff;
    }

    Q_SCRIPTABLE void SimulateUserActivity() {
        qDebug() << QDateTime::currentDateTime().toString(Qt::ISODate) << objectName() << "SimulateUserActivity";
    }

    Q_SCRIPTABLE void UnInhibit(quint32 cookie) {
        cookie ^= 0xf4c0ff;

        qDebug() << QDateTime::currentDateTime().toString(Qt::ISODate) << objectName() << handedOut.value(cookie) << "UnInhibit";
        handedOut.remove(cookie);
    }
    Q_SCRIPTABLE void Uninhibit(quint32 cookie) {
        cookie ^= 0xf4c0ff;

        qDebug() << QDateTime::currentDateTime().toString(Qt::ISODate) << objectName() << handedOut.value(cookie) << "Uninhibit";
        handedOut.remove(cookie);
    }

    protected:
    QMap<int, QString> handedOut;
};

class GnomeSessionPong: public Pong
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.gnome.SessionManager")
};

class GnomeScreensaverPong: public Pong
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.gnome.ScreenSaver")
};

class XdgPowerPong : public Pong
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.PowerManagement.Inhibit")
};

#endif
