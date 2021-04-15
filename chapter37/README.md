# DAEMONS

## Overview
- `daemon process` 存活时间很长，通常在系统启动时被创建直到运行系统被关。在后台运行且不拥有`controlling terminal`，这样可以确保kernel永远不会为`daemon process`生成job-control和terminal-related的signal(`SIGINT` `SIGTSTP` `SIGHUP`)

## Creating a Daemon

## Guidelines for Writing Daemons

## Using SIGHUP to Reinitialize a Daemon

## Logging Messages and Errors Using syslog

### Overview

### The syslog API

### The /etc/syslog.conf File