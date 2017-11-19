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

#ifndef OBJECTTRACKING_H
#define OBJECTTRACKING_H

#include <QObject>
#include <QSet>

#include "kobjecttracking_export.h"

class ObjectDebug;

class KOBJECTTRACKING_EXPORT ObjectTracking : public QObject
{
Q_OBJECT
public:
    ObjectTracking(QObject* parent = nullptr);
    ~ObjectTracking() override;

    /** If it's easier to use it as a singleton, it's also possible */
    static ObjectTracking* self();

    enum Option {
        Watch = 1,
        Track = 2
    };
    Q_ENUM(Option);
    Q_DECLARE_FLAGS(Options, Option)

    enum Depth {
        Alone,
        Inherit
    };
    Q_ENUM(Depth);

    void track(QObject* object, Options options, Depth depth);

private:
    QSet<QPointer<ObjectDebug>> m_used;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ObjectTracking::Options)

#endif
