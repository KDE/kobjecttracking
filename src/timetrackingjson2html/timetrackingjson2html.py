#
#  Copyright 2014-2017 by Aleix Pol Gonzalez <aleixpol@blue-systems.com>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Library General Public License as
#  published by the Free Software Foundation; either version 2, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details
#
#  You should have received a copy of the GNU Library General Public
#  License along with this program; if not, write to the
#  Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

import argparse
import json
import random, string
import sys

header="""<html>
  <head>
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
      google.charts.load('current', {'packages':['timeline']});
      google.charts.setOnLoadCallback(drawChart);
      function drawChart() {
        var container = document.getElementById('timeline');
        var chart = new google.visualization.Timeline(container);
        var dataTable = new google.visualization.DataTable();
        dataTable.addColumn({ type: 'string', id: 'Object-Property' });
        dataTable.addColumn({ type: 'string', id: 'Value' });
        dataTable.addColumn({ type: 'number', id: 'Start' });
        dataTable.addColumn({ type: 'number', id: 'End' });
"""



footer="""
        var options = {}
        chart.draw(dataTable, options);
      }

    </script>
  </head>
  <body>
    <div id="timeline" style="height: 100%; width: 100%"></div>
  </body>
</html>
"""

names = set()
def uniqueObjectName(name):
    global names
    orig = name
    while name in names:
        name = orig
        name += ("-" + ''.join(random.SystemRandom().choice(string.ascii_uppercase + string.digits) for _ in range(5)))
    names |= set({name})
    return name

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("input", help="input text file. /tmp/debug-$appname-$USER.json")
    args = parser.parse_args()

    values = []
    with open(args.input) as json_data:
        data = json.load(json_data)
        for v in data:
            name = uniqueObjectName(v['name'])
            events = v['events']
            initial = v["initial"]
            construction = events[0]['time']
            destruction  = events[-1]['time']
            lastTime = {}
            unchangedProperties = initial.copy()

            values.append([name, "lifetime", construction, destruction])
            for e in events:
                propName = e['name'] if name in e else None
                if not propName:
                    continue

                event = lastTime.get(propName, {'value': str(initial[propName]), 'time': construction})

                values.append([name +"-"+ propName, event["value"], event['time'], e['time']])

                if propName in unchangedProperties:
                    del unchangedProperties[propName]
                lastTime[propName] = e

            for (propName, e) in lastTime.items():
                values.append([name +"-"+ propName, e["value"], e['time'], destruction])

            #too boring...
            #for (v, i) in unchangedProperties.items():
                #values += ("            [\"%s\", new Date(%u),  new Date(%u) ],\n" % (name +"-"+ v, construction, destruction))


    values = values[:-2]
    print(header)
    print("        dataTable.addRows(\n", json.dumps(values, indent=4), "        );")
    print(footer)
