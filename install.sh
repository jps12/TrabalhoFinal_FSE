#!/bin/bash

cd ./central-server

npm install -g webpack                    # Install webpack globally
npm install mqtt                          # Install MQTT library
cd node_modules/mqtt
npm install .                             # Install dev deps at current dir
npm install -g webpack-cli

echo yes | webpack ./mqtt.js --output-library mqtt

cd ..
