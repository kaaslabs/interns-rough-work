#!/bin/bash
# Coffee Vending Machine - Java Run Script

cd "$(dirname "$0")"

# Build if not already built
if [ ! -d "bin" ] || [ ! -f "bin/CoffeeVendingMachineDemo.class" ]; then
    ./build.sh
fi

if [ -d "bin" ]; then
    echo "Running Coffee Vending Machine..."
    echo ""
    cd bin
    java CoffeeVendingMachineDemo
else
    echo "Build failed. Cannot run."
    exit 1
fi
