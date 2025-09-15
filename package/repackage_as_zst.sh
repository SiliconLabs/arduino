#!/bin/bash

# Repackages various archive files as zstd archives

# Check if filename is provided
if [ -z "$1" ]; then
    echo "No filename provided. Usage: ./repackage_as_zst.sh filename"
    exit 1
fi

# Extract the filename without the extension
filename=$(basename -- "$1")
extension="${filename##*.}"
filename="${filename%.*}"

# Create a temporary directory
mkdir -p temp >/dev/null 2>&1

# Extract the archive to the temporary directory
if [ "$extension" == "gz" ] || [ "$extension" == "bz" ] || [ "$extension" == "xz" ]; then
    tar -xf "$1" -C temp
elif [ "$extension" == "zip" ]; then
    unzip "$1" -d temp
else
    echo "Unsupported file extension. Only tar.gz, tar.bz, tar.xz, and zip archives are supported."
    exit 1
fi

# Create a new archive with zstd compression
tar --zstd -cf "${filename}.zst" -C temp .

# Remove the temporary directory
rm -r temp

echo "Done. Created ${filename}.zst"
