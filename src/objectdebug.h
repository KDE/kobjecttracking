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
#include <QSet>

#include "kobjecttracking_export.h"

class ObjectTimeTracker;
class ObjectWatcher;

class KOBJECTTRACKING_EXPORT ObjectDebug : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool watch READ watch WRITE setWatch NOTIFY watchChanged)
    Q_PROPERTY(bool timeTracker READ timeTracker WRITE setTimeTracker NOTIFY timeTrackerChanged)
    Q_PROPERTY(bool inherit READ inherit WRITE setInherit)
public:
    ObjectDebug(QObject* object);
    ~ObjectDebug() override;

    void setWatch(bool watch);
    bool watch() const;

    void setTimeTracker(bool timeTracker);
    bool timeTracker() const;

    void setInherit(bool inherit);
    bool inherit() const;

    static ObjectDebug *qmlAttachedProperties(QObject *object);

    bool eventFilter(QObject* watched, QEvent* event) override;

Q_SIGNALS:
    void watchChanged(bool watch);
    void timeTrackerChanged(bool timeTracker);
    void independence(ObjectDebug* od);

private:
    void addChild(QObject* child);
    void childDestroyed(QObject* object);

    QPointer<QObject> m_object;
    QPointer<ObjectTimeTracker> m_timeTracker;
    QPointer<ObjectWatcher> m_watcher;
    QSet<ObjectDebug*> m_children;
    bool m_inherit = false;
};

#endif
