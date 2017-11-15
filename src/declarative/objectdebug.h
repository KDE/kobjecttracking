/*
 *   Copyright 2014 by Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef OBJECTDEBUG_H
#define OBJECTDEBUG_H

#include <QObject>
#include <QPointer>

#include <qqml.h>

class ObjectTimeTracker;
class ObjectWatcher;

class ObjectDebug : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool watch READ watch WRITE setWatch)
    Q_PROPERTY(bool timeTracker READ timeTracker WRITE setTimeTracker)
public:
    ObjectDebug(QObject* object);

    void setWatch(bool watch);
    bool watch() const;

    void setTimeTracker(bool timeTracker);
    bool timeTracker() const;

    static ObjectDebug *qmlAttachedProperties(QObject *object);

private:
    QPointer<ObjectTimeTracker> m_timeTracker;
    QPointer<ObjectWatcher> m_watcher;
};

QML_DECLARE_TYPEINFO(ObjectDebug, QML_HAS_ATTACHED_PROPERTIES)

#endif
