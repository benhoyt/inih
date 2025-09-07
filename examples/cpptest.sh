#!/usr/bin/env bash

g++ -Wall INIReaderExample.cpp ../cpp/INIReader.cpp ../ini.c -o INIReaderExample
./INIReaderExample > cpptest.txt
rm INIReaderExample

g++ -Wall INIReaderExampleErrors.cpp ../cpp/INIReader.cpp ../ini.c -o INIReaderExampleErrors
./INIReaderExampleErrors > cpptesterrors.txt
rm INIReaderExampleErrors
