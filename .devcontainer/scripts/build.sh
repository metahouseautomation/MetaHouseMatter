#!/bin/bash

cd /opt/esp/idf;
. ./export.sh;
cd ..;

cd /opt/esp;
git clone --depth 1 -b release/v1.2 --single-branch https://github.com/espressif/esp-matter.git;
cd esp-matter;
git submodule update --init --depth 1;
./connectedhomeip/connectedhomeip/scripts/checkout_submodules.py --platform esp32 --shallow;
./install.sh;
. ./export.sh;
cd ..;

set -e;

export IDF_CCACHE_ENABLE=1