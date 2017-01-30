#!/bin/sh
nodecmd=`which node || which nodejs`
$nodecmd test/check_compatibility.js

