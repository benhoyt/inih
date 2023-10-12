#!/usr/bin/env bash

cd examples
g++ -Wall INIReaderExample.cpp ../cpp/INIReader.cpp ../ini.c -o INIReaderExample
