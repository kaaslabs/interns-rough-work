#!/bin/bash
# Coffee Vending Machine - Java Build Script

echo "Compiling Java files..."

cd "$(dirname "$0")/src"

# Create output directory
mkdir -p ../bin

# Compile all Java files
javac -d ../bin *.java

if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo ""
    echo "To run: cd java/bin && java CoffeeVendingMachineDemo"
else
    echo "Compilation failed!"
    exit 1
fi
