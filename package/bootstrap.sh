#!/bin/bash
# Bootstraps the development environment by cloning the Arduino Core-API and linking into the 'core/gecko/api' folder

echo "Setting up the development environment..."
git submodule update --init --recursive
rm -rf ../cores/gecko/api
ln -s ../../extra/core-api/api ../cores/gecko/api
echo "Development environment setup finished"
