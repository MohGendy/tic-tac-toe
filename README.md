# Build Instructions

## To Build Use Next Commands

```sh
cd ./build  
make clean  
cmake .. -G "MinGW Makefiles"  
cmake --build .
```
## after building run the excutable

```sh
my_program.exe
```
## To Run Tests

```sh
runTests.exe
```
##steps to pack

```sh
cmake --build .
cmake --install . --prefix "C:\path\to\install\directory"
cpack
```

