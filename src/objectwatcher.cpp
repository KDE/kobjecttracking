/*
 *   Copyright 2017 by Aleix Pol Gonzalez <aleixpol@blue-systems.com>
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

#include "objectwatcher.h"

#include <QMetaProperty>
#include <QDebug>

void ObjectWatcher::PropertyValue::integrateValue(QObject* object, const QMetaProperty& prop, const QVariant& value)
{
//     if (!QByteArray(object->metaObject()->className()).contains("ListView")) return;

    if (value == lastValue && !QByteArray(prop.name()).contains("hildren")) {
        times++;
        qDebug() << "repeated value" << object << prop.name() << value << times << "times";
    } else {
        times = 0;
        lastValue = value;
    }
}

ObjectWatcher::ObjectWatcher(QObject* object)
    : m_watched(object)
{
    QMetaMethod propChange = metaObject()->method(metaObject()->indexOfSlot("propertyChanged()"));
    Q_ASSERT(propChange.isValid() && metaObject()->indexOfSlot("propertyChanged()")>=0);

    const auto mo = object->metaObject();
    for(int i = 0;  i<mo->propertyCount(); ++i) {
        const auto prop = mo->property(i);
        const auto value = prop.read(object);
        m_values.insert(prop.name(), {value});

        if (prop.hasNotifySignal()) {
            connect(object, prop.notifySignal(), this, propChange);
        }
    }

    connect(object, &QObject::destroyed, this, &QObject::deleteLater);
}

void ObjectWatcher::propertyChanged()
{
    Q_ASSERT(sender() == m_watched);

    const QMetaObject* mo = m_watched->metaObject();

    for (int i = 0, pc = mo->propertyCount(); i<pc; ++i) {
        const QMetaProperty prop = mo->property(i);
        if (prop.notifySignalIndex() == senderSignalIndex()) {
            const auto value = prop.read(m_watched);
            m_values[prop.name()].integrateValue(m_watched, prop, value);
        }
    }
}

void ObjectWatcher::watch(QObject* object)
{
    static QSet<QObject*> objects;
    if (objects.contains(object) || object->metaObject()->className() == ObjectWatcher::staticMetaObject.className())
        return;
    if (object->metaObject()->className() == QByteArray("QAction")) //it's _very_ noisy and probably a liability
        return;
    if (object->metaObject()->className() == QByteArray("Kirigami::BasicTheme")) //it's _very_ noisy and probably a liability
        return;

    objects.insert(object);
    new ObjectWatcher(object);
}
