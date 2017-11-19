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

#include "objecttracking.h"
#include "objectdebug.h"

ObjectTracking::ObjectTracking(QObject* parent)
    : QObject(parent)
{}

ObjectTracking * ObjectTracking::self()
{
    static ObjectTracking self;
    return &self;
}

ObjectTracking::~ObjectTracking()
{
    qDeleteAll(m_used);
}

void ObjectTracking::track(QObject* object, ObjectTracking::Options options, ObjectTracking::Depth depth)
{
    auto od = ObjectDebug::qmlAttachedProperties(object);
    od->setWatch(options & Watch);
    od->setTimeTracker(options & Track);
    od->setInherit(depth == Inherit);
}
