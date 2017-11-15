# KObjectTracking

Tools to debug the state of your QObject to see how they interact with the world

## Introduction

This module provides some tools to see how the properties change at run-time and tries to detect ideas on how to improve your application with them

## Usage

### QML

```
import org.kde.ObjectTracking 1.0

Item {
    ObjectDebug.watch: true
    ObjectDebug.timeTracking: true
}
```

### C++

### Tests
