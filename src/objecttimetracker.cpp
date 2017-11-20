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

#include "objecttimetracker.h"
#include <QDateTime>
#include <qmetaobject.h>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>
#include <QChildEvent>
#include <QCoreApplication>

#include "objectwatcher.h"

class ObjectHistory
{
public:
    QString name;
    QJsonObject initial;
    QVector<QJsonObject> events;
};

Q_GLOBAL_STATIC_WITH_ARGS(const qint64, s_beginning, (QDateTime::currentDateTime().toMSecsSinceEpoch()))

struct TimeTrackerWriter : QObject {
    Q_OBJECT
public:
    TimeTrackerWriter() {
        QObject::connect(QCoreApplication::instance(), &QCoreApplication::destroyed, this, &TimeTrackerWriter::print);
    }

    void print() const {
        QJsonArray array;

        Q_FOREACH(const ObjectHistory& history, m_data) {
            array.append(QJsonObject {
                { QStringLiteral("name"), history.name },
                { QStringLiteral("events"), serializeEvents(history.events) },
                { QStringLiteral("initial"), QJsonValue::fromVariant(history.initial) }
            });
        }
        Q_ASSERT(array.count() == m_data.count());
        QJsonDocument doc;
        doc.setArray(array);

        QFile f(QStringLiteral("/tmp/debug-") + QCoreApplication::instance()->applicationName() + QLatin1Char('-') + QString::fromUtf8(qgetenv("USER")) + QStringLiteral(".json"));
        bool b = f.open(QFile::WriteOnly);
        Q_ASSERT(b);
        f.write(doc.toJson());
    }

    void feed(QObject* obj, const ObjectHistory& tracker)
    {
        m_data[obj] = tracker;
    }

    QHash<QObject*, ObjectHistory> m_data;

private:
    QJsonArray serializeEvents(const QVector<QJsonObject>& events) const {
        QJsonArray ret;
        for(const auto& ev: events) {
            ret.append(ev);
        }
        Q_ASSERT(ret.count() == events.count());
        return ret;
    }
};
Q_GLOBAL_STATIC(TimeTrackerWriter, s_writer)

static QString variantToString(const QVariant &v)
{
    QString val;
    QDebug d(&val);
    d << v;
    return val;
}

ObjectTimeTracker::ObjectTimeTracker(QObject* o)
    : QObject()
    , m_object(o)
    , m_history(new ObjectHistory)
{
    *s_beginning * 1; // ensure it's initialized

    QTimer* t = new QTimer(this);
    t->setInterval(2000);
    t->setSingleShot(false);
    connect(t, &QTimer::timeout, this, &ObjectTimeTracker::sync);
    t->start();

    connect(o, &QObject::destroyed, this, &ObjectTimeTracker::objectDestroyed);

    auto mo = m_object->metaObject();
    m_history->name = QString::fromUtf8(mo->className());
    m_history->events.append(QJsonObject {
        { QStringLiteral("time"), QDateTime::currentDateTime().toMSecsSinceEpoch() - *s_beginning },
        { QStringLiteral("type"), QStringLiteral("constructor") },
        { QStringLiteral("name"), {} },
        { QStringLiteral("value"), m_object->objectName() }
    });

    QMetaMethod propChange = metaObject()->method(metaObject()->indexOfSlot("propertyChanged()"));
    Q_ASSERT(propChange.isValid() && metaObject()->indexOfSlot("propertyChanged()")>=0);

    for (int i = 0, pc = mo->propertyCount(); i<pc; ++i) {
        QMetaProperty prop = mo->property(i);
        m_history->initial[QLatin1String(prop.name())] = variantToString(prop.read(o));

        if (prop.hasNotifySignal())
            connect(o, prop.notifySignal(), this, propChange);
    }
}

ObjectTimeTracker::~ObjectTimeTracker()
{
    sync();
}

void ObjectTimeTracker::objectDestroyed()
{
    m_history->events.append(QJsonObject {
        { QStringLiteral("time"), QDateTime::currentDateTime().toMSecsSinceEpoch() - *s_beginning },
        { QStringLiteral("type"), QStringLiteral("destroyed") }
    });
    deleteLater();
}

void ObjectTimeTracker::sync()
{
    s_writer->feed(m_object, *m_history);
}

void ObjectTimeTracker::propertyChanged()
{
    Q_ASSERT(sender() == m_object);

    const QMetaObject* mo = m_object->metaObject();

    for (int i = 0, pc = mo->propertyCount(); i<pc; ++i) {
        const QMetaProperty prop = mo->property(i);
        if (prop.notifySignalIndex() == senderSignalIndex()) {

            m_history->events.append(QJsonObject {
                { QStringLiteral("time"), QDateTime::currentDateTime().toMSecsSinceEpoch() - *s_beginning },
                { QStringLiteral("type"), QStringLiteral("property") },
                { QStringLiteral("name"), QString::fromLatin1(prop.name()) },
                { QStringLiteral("value"), variantToString(prop.read(m_object)) }
            });
        }
    }
}

#include "objecttimetracker.moc"
