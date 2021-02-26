# Flex Net Sim

## Download

Download the latest release from the [release list](https://gitlab.com/DaniloBorquez/flex-net-sim/-/releases). 

If you want to try the actual developed version (a little bit updated, but coulb be unstable), you can clone this repository through the git clone command.

```
git clone git@gitlab.com:DaniloBorquez/flex-net-sim.git
```

## Installation

To compile the library, you will need:
- [CMake](https://cmake.org)

Thanks to the next libraries used:
- [JSON for modern C++](https://github.com/nlohmann/json)
- [Catch2](https://github.com/catchorg/Catch2)

### Unix systems (Mac and linux)

Create a build directory in the project root tree and run cmake there:
```
 $ mkdir build
 $ cd build
 $ cmake ..
 $ cmake --build .
 $ sudo cmake --install .
```

### Windows systems 

Create a build directory in the project root tree and run cmake there. Open a cmd as adminstrator (type cmd on windows start, right click on it and select **run as admin...**):
```
 $ mkdir build
 $ cd build
 $ cmake .. 
 $ cmake --build . --config Release
 $ cmake --install .
```

If you do not have Visual Studio installed on your operative system, but you have MinGW, you can change the previous **cmake ..** (line that generates the Makefile) with:
```
 $ mkdir build
 $ cd build
 $ cmake .. -G "MinGW Makefiles"
 $ cmake --build .
 $ cmake --install .
```

## Using  library

After installation, the use of the library is easy. You just have to include the header **<fnsim/simulator.h>** in your code (as in [examples](https://gitlab.com/DaniloBorquez/flex-net-sim/-/tree/master/examples)), and the compile it with the library flag:
```
 $ g++ <your cpp files> -lfnsim
```
Obviously, Windows is a little more complicated. You must include in this line the c++11 standard and the folder where the library was installed. Typically in **C:/Program Files (x86)/flexible-networks-simulator/**.

```
 $ g++ -std=c++11 <your cpp files> -I"C:/Program Files (x86)/flexible-networks-simulator/include/" -L"C:/Program Files (x86)/flexible-networks-simulator/lib/" -lfnsim
```

## Documentation

The documentation can be found in the next [link](http://daniloborquez.gitlab.io/flex-net-sim).

## Thanks

I really appreciate the help of the following people:
- [Álvaro Robert](https://gitlab.com/robstrings97)
- [Felipe Falcón](https://gitlab.com/ffalcon)
- [Gonzalo España](https://gitlab.com/GonzaloEspana)
- [Diana Mariño](https://gitlab.com/DianaMarino)