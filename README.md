# Flex Net Sim

**Flex Net Sim** is a C++ simulation library where you can design allocation algorithms to the flexible grid optical networks allocation problem. The library simulates the arrival and departure of several connections following a Poisson distribution. The researcher only needs to code the algorithm without worrying about the simulator code.

You can see our pre-print in [arxiv](https://arxiv.org/abs/2105.02762)

If you use our library, and you enjoy it, please cite us :heart_eyes:!
```
@misc{borquez2021flex,
      title={Flex Net Sim: A Lightly Manual}, 
      author={Felipe Falcón and Gonzalo España and Danilo Bórquez-Paredes},
      year={2021},
      eprint={2105.02762},
      archivePrefix={arXiv},
      primaryClass={cs.NI}
}
```

[[_TOC_]]

## Features

- Elastic optical networks support
- Support for different bitrates through a JSON file
- Support for different networks through a JSON file
- Support for multiple routes through a JSON file
- Support the creation of your own allocation algorithm
- Customize connection arrive/departure ratios
- Support to create your own statistics
- Customize the number of connection arrives that you want to simulate

## Download

Download the latest release from the [release list](https://gitlab.com/DaniloBorquez/flex-net-sim/-/releases). 

If you want to try the actual developed version (a little bit updated, but coul be unstable), you can clone this repository through the git clone command.

```
git clone git@gitlab.com:DaniloBorquez/flex-net-sim.git
```

## Installation
You can directly download the one file header <a href="https://daniloborquez.gitlab.io/flex-net-sim/simulator.hpp" download>HERE</a>

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