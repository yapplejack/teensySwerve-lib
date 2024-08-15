#!/bin/bash
./gradlew :spotlessApply
./gradlew publish
cp -rf build/repos/releases/teensylib teensy/repos/