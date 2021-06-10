# barcode-scanner
## Build
mkdir build

Specify DCMake_BUILD_TYPE as 'Debug' or 'Release'

e.g.

cmake -DCMAKE_BUILD_TYPE=Debug ..

make

## Sample Applications

identifySample and finalSample can be run using webcam at index 0. No arguments required

decodeSample requires 2 image path location parameters