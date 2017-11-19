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

#ifndef OBJECTWATCHER_H
#define OBJECTWATCHER_H

#include <QObject>
#include <QVariant>
#include <QHash>

class ObjectWatcher : public QObject
{
    Q_OBJECT
public:
    struct PropertyValue {
        PropertyValue(const QVariant &value = {}) : lastValue(value) {}
        QVariant lastValue;
        int times = 0;

        void integrateValue(QObject* object, const QMetaProperty& prop, const QVariant& value);
    };

    explicit ObjectWatcher(QObject* object);

    Q_SLOT void propertyChanged();

private:
    QObject* const m_watched;
    QHash<QByteArray, PropertyValue> m_values;
};

#endif
