/*
 *   Copyright 2014-2017 by Aleix Pol Gonzalez <aleixpol@blue-systems.com>
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

#ifndef TIMETRACKER_H
#define TIMETRACKER_H

#include <QObject>
#include <QVariantMap>
#include <QDateTime>
#include <QVector>
#include <QScopedPointer>

class ObjectHistory;
/**
 * This debugging class is meant to provide an overview of how the objects change
 * over time and hopefully provide the information required to detect buggy initialization.
 *
 * To use it, you'll pass the object you want to track to the constructor and the TimeTracker
 * will use Qt introspection to read the properties and check what values they have and how they
 * change.
 *
 * To analyze the results one can read the generated json file /tmp/debug-$application-$USER, as soon
 * as the process has quit.
 */

class ObjectTimeTracker : public QObject
{
Q_OBJECT
public:
    explicit ObjectTimeTracker(QObject* object);
    ~ObjectTimeTracker() override;
    void init();

private Q_SLOTS:
    void sync();
    void propertyChanged();
    void objectDestroyed();

private:
    QObject* m_object;
    QScopedPointer<ObjectHistory> m_history;
};

#endif // TIMETRACKER_H
