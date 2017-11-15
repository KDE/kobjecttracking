# KObjectTracking

Tools to debug the state of your QObject and see how they interact with the world

## Introduction

This module provides some tools to see how the properties change at run-time and tries to suggest ideas on how to improve your application.

## Features

At the moment there's the following features

### Watching

It will keep track of all the properties in the object and report the ones that don't change despite a notify.

### Time Tracking

It will generate a file in `/tmp/debug-<appname>-<user>.json` that will contain a list of the tracked objects with their initial state and all of the property changes over time including their timestamp.

## Usage

### QML

```
import org.kde.ObjectTracking 1.0

Item {
    ObjectDebug.watch: true
    ObjectDebug.timeTracking: true
    ObjectDebug.inherit: true
}
```

### C++

### Tests
