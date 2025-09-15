#!/bin/bash
# Packages the core into a ZIP/ZST file excluding unnecessary files and folders
# and calculates the file size and the SHA256 checksum of the output file

CORE_VERSION=3.0.0
EXCLUDE_LIST_ZIP=''silabs-arduino/.git/**\*' 'silabs-arduino/package/**\*' 'silabs-arduino/test/**\*' 'silabs-arduino/.DS_Store/**\*' 'silabs-arduino/.github/**\*' 'silabs-arduino/extra/core-api/**\*''
EXCLUDE_LIST_ZST="--exclude=silabs-arduino/.git --exclude=silabs-arduino/package --exclude=silabs-arduino/test --exclude=silabs-arduino/.DS_Store --exclude=silabs-arduino/.github --exclude=silabs-arduino/extra/core-api"
OUTPUT_FILE_NAME=silabs_arduino_core-$CORE_VERSION

echo "Silicon Labs Arduino Core packager"

# Copy the Core-API files to the cores/gecko/api folder
echo "Copying Core-API files..."
rm -rf ../cores/gecko/api
cp -r ../extra/core-api/api ../cores/gecko/api

cd ..
rm -rf /tmp/silabs-arduino
mkdir /tmp/silabs-arduino
echo "Copying core files..."
cp -r * /tmp/silabs-arduino

# Restore the original Core-API link
( cd package; sh bootstrap.sh )

echo "Filtering core files..."
rm -rf /tmp/silabs-arduino/.git
rm -rf /tmp/silabs-arduino/package
rm -rf /tmp/silabs-arduino/test
rm -rf /tmp/silabs-arduino/extra/core-api
rm /tmp/silabs-arduino/installed.json

if [ "$1" != "--skip-zip" ]; then
    echo "Creating ZIP archive..."
    ( cd /tmp; zip -r $OUTPUT_FILE_NAME.zip silabs-arduino -x $EXCLUDE_LIST_ZIP )
    mv /tmp/$OUTPUT_FILE_NAME.zip .
    shasum -a 256 $OUTPUT_FILE_NAME.zip
    wc -c $OUTPUT_FILE_NAME.zip
fi

echo "Creating ZST archive..."
( cd /tmp; tar $EXCLUDE_LIST_ZST --zstd -cvf $OUTPUT_FILE_NAME.zst silabs-arduino )
mv /tmp/$OUTPUT_FILE_NAME.zst .
shasum -a 256 $OUTPUT_FILE_NAME.zst
wc -c $OUTPUT_FILE_NAME.zst

rm -rf /tmp/silabs-arduino
