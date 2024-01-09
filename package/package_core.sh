#!/bin/bash
# Packages the core into a ZIP file excluding unnecessary files and folders
# and calculates the SHA256 checksum for the output zip file
# Commander: tar -czf Commander_osx_1v14p5b1276.tgz Commander_osx_1v14p5b1276

CORE_VERSION=1.0.0
EXCLUDE_LIST=''silabs-arduino/.git/**\*' 'silabs-arduino/package/**\*' 'silabs-arduino/test/**\*' 'silabs-arduino/.DS_Store/**\*''
OUTPUT_FILE_NAME=silabs_arduino_core-$CORE_VERSION.zip

cd ..
mkdir /tmp/silabs-arduino
cp -r * /tmp/silabs-arduino

rm -rf /tmp/silabs-arduino/.git
rm -rf /tmp/silabs-arduino/package
rm -rf /tmp/silabs-arduino/test
rm /tmp/silabs-arduino/installed.json

( cd /tmp; zip -r $OUTPUT_FILE_NAME silabs-arduino -x $EXCLUDE_LIST )
mv /tmp/$OUTPUT_FILE_NAME .
shasum -a 256 $OUTPUT_FILE_NAME
wc -c $OUTPUT_FILE_NAME
rm -rf /tmp/silabs-arduino
