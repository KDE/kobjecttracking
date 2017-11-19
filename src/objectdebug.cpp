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

#include "objectdebug.h"
#include "objecttimetracker.h"
#include "objectwatcher.h"

#include <QChildEvent>
#include <QDebug>


class ObjectDecorator
{
    const char* propName = "kobjecttracking_objectdebug";
    QHash<QObject*, ObjectDebug*> m_debugs;

public:
    void setDebugForObject(QObject* object, ObjectDebug* dbg)
    {
        Q_ASSERT(m_debugs.value(object) == nullptr);
        m_debugs[object] = dbg;
    }

    ObjectDebug* debugForObject(QObject* object)
    {
//         return dynamic_cast<ObjectDebug*>(object->property(propName).value<QObject*>());
        return m_debugs.value(object);
    }

    void forgetObject(QObject* o) {
        if (o)
            setDebugForObject(o, nullptr);
    }
};

Q_GLOBAL_STATIC(ObjectDecorator, ourDecorator)

ObjectDebug::ObjectDebug(QObject* object)
    : m_object(object)
{
    ourDecorator->setDebugForObject(object, this);
    connect(object, &QObject::destroyed, this, &QObject::deleteLater);
}

ObjectDebug::~ObjectDebug()
{
    ourDecorator->forgetObject(m_object);

    qDeleteAll(m_children);
    delete m_timeTracker;
    delete m_watcher;
}

void ObjectDebug::setTimeTracker(bool timeTracker)
{
    if (timeTracker == bool(m_timeTracker))
        return;

    if (!m_timeTracker) {
        m_timeTracker = QPointer<ObjectTimeTracker>{ new ObjectTimeTracker(m_object) };
    } else {
        delete m_timeTracker;
    }
    Q_EMIT timeTrackerChanged(m_timeTracker);
}

void ObjectDebug::setWatch(bool watch)
{
    if (watch == bool(m_watcher))
        return;

    if (!m_watcher) {
        m_watcher = QPointer<ObjectWatcher>(new ObjectWatcher(m_object));
    } else {
        delete m_watcher;
    }
    Q_EMIT watchChanged(m_watcher);
}

bool ObjectDebug::timeTracker() const
{
    return m_timeTracker;
}

bool ObjectDebug::watch() const
{
    return m_watcher;
}

bool ObjectDebug::inherit() const
{
    return m_inherit;
}

void ObjectDebug::setInherit(bool inherit)
{
    if (inherit == m_inherit)
        return;

    m_inherit = inherit;
    Q_ASSERT(m_object);
    if (inherit) {
        for(auto o: m_object->children())
            addChild(o);

        m_object->installEventFilter(this);
    } else {
        qDeleteAll(m_children);
        m_children.clear();

        m_object->removeEventFilter(this);
    }
}

void ObjectDebug::childDestroyed(QObject* object)
{
    auto od = dynamic_cast<ObjectDebug*>(object);
    if (!od) {
        return;
    }

    auto removed = m_children.remove(od);
    Q_ASSERT(removed);
}

void ObjectDebug::addChild(QObject* child)
{
    auto od = ObjectDebug::qmlAttachedProperties(child);
    od->setTimeTracker(timeTracker());
    od->setWatch(watch());
    od->setInherit(true);
    connect(this, &ObjectDebug::timeTrackerChanged, od, &ObjectDebug::setTimeTracker);
    connect(this, &ObjectDebug::watchChanged, od, &ObjectDebug::setWatch);
    connect(od, &QObject::destroyed, this, &ObjectDebug::childDestroyed);
    connect(od, &ObjectDebug::independence, this, &ObjectDebug::childDestroyed);
    m_children << od;
}

bool ObjectDebug::eventFilter(QObject* watched, QEvent* event)
{
    Q_ASSERT(m_inherit);
    Q_ASSERT(watched == m_object);

    if (event->type() == QEvent::ChildAdded) {
        QChildEvent* childEvent = static_cast<QChildEvent*>(event);

        addChild(childEvent->child());
    }
    return QObject::eventFilter(watched, event);
}

ObjectDebug* ObjectDebug::qmlAttachedProperties(QObject* object)
{
    ObjectDebug* od = ourDecorator->debugForObject(object);
    if (!od) {
        od = new ObjectDebug(object);
    } else {
        Q_EMIT od->independence(od);
        ourDecorator->setDebugForObject(object, od);
    }
    return od;
}
