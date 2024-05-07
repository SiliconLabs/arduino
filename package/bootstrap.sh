#!/bin/bash
# Bootstraps the development environment by cloning the Arduino Core-API and linking into the 'core/silabs/api' folder

echo "Setting up the development environment..."
git submodule update --init --recursive
rm -rf ../cores/silabs/api
ln -s ../../extra/core-api/api ../cores/silabs/api
echo "Development environment setup finished"
