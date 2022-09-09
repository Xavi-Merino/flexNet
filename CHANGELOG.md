# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.6.0] - 2021-12-22
### ADDED
- A callback function after each disconnection.

### FIXED

- Example 001 fixed. Reset slots in each route.
  
## [0.5.3] - 2022-05-19

### FIXED
 
- Changed the Agresti-Coull confidence interval's previously hardcoded solution only working for 95% confidence.

## [0.5.2] - 2022-03-21

### FIXED

- Changed the path of .json files (NSF Network and its routes) to the proper ones inside the second example's main file.

## [0.5.1] - 2022-03-15

### FIXED

- There was a typo in JSON network files, and the related class: lenght -> length
- Changing vector initialization error in examples. 

## [0.5.0] - 2021-12-22

### ADDED

- Confidence intervals: Wilson, Wald, and Agresti-Coull
- Added seed setters for the Src/Dst seeds' inside Simulator's class. 
- Test cases inside test_simulator.cpp for the corresponding Src/Dst seed setters.

### FIXED

- There was a typo in JSON network file, and the related class: lenght -> length
- Minor bugs

## [0.4.1] - 2021-07-28

### CHANGED

- The doxygen documentation of Link class was changed for a better compresion.
- Documentation of Simulator class:
  - Extended/changed comments on the class and methods description (.hpp).
- Documentation of Event class:
  - Extended/changed all comments on the class and methods description (.hpp).
- Documentation of Bit Rate class:
  - Extended/changed comments on the class and methods description (.hpp).
    
### ADDED

- Test case in test_simulator.cpp:
  - Added test case for testing UniformVariable class' non-void constructor and getNextValue method.
  - Was added the test for the setters methods for each variable.
  - Added constructor case.
- Test case in test_controller.cpp
    -Added test for checking no error throwing on Connection's addLink method.
- Test case in test_bitrate.cpp:
  - Added test for all of the BitRate's class getter methods.


## [0.4.0] - 2021-06-29

### ADDED

- New Feature: Three metrics for the Network, in network class.
  - Average Neighborhood
  - Normal Average Neighborhood
  - Nodal Variance

### CHANGED

- Documentation of Network/Controller class:
  - Extended/changed all comments on the class and methods description (.hpp).
  - Added comments on a method (isSlotUsed) to describe it clearer (.cpp).
  - Standarized some variables names: changed all similar variables names into one (i.e., from link and linkPos to linkPos), changed the variables in spanish into english and changed all variable names to camelCase (both .hpp and .cpp).

### DELETED

- nodeCounter variable.

## [0.3.0] - 2021-03-16

### ADDED

- Added Source/Destination as attributes of Link. Now Network is declared as a friend class within Link class.
- Added macros inside Simulator.hpp in order to obtain the Source/Destination of a link inside a route.
  - LINK_IN_ROUTE_SRC(route, link)
  - LINK_IN_ROUTE_DST(route, link)
- Macros added to the bitrate object in the algorithm implementation:
  - REQ*REACH(pos) Get the optical reach located in the \_pos* on the JSON file
  - REQ*MODULATION(pos) Get the modulation format located in the \_pos* on the JSON file
  - REQ_BITRATE_STR Get the bitrate value as string
  - REQ_BITRATE Get the bitrate value as double
- Adding getter to the simulation time with a method in simulator object
- Adding getter to the blocking probability with a method in simulator object

## [0.2.0] - 2021-03-06

### ADDED

- You can get the Bitrate value from a bitrate object
- Unit tests in BitRate class
- Exceptions in BitRate getters.
- One big header file added. If you want, no needs to compile the library. Just include the one big header file into your project.

### FIXED

- JSON BitRate method. Now the method get the values from json in an ordered way.

## [0.1.1] - 2021-03-02

### CHANGED

- Setting parameters of simulation must be done BEFORE the calling to init method. If you do it after, then an exception is thrown.

## [0.1.0] - 2021-02-26

### Added

- Elastic optical networks support
- Support for different bitrates through a JSON file
- Support for different networks through a JSON file
- Support for multiple routes through a JSON file
- Support the creation of your own allocation algorithm
- Customize connection arrive/departure ratios
- Support to create your own statistics
- Customize the number of connection arrives that you want to simulate

[0.1.0]: https://gitlab.com/DaniloBorquez/flex-net-sim/-/releases/v0.1.0
[0.1.1]: https://gitlab.com/DaniloBorquez/flex-net-sim/-/compare/v0.1.0...v0.1.1
[0.2.0]: https://gitlab.com/DaniloBorquez/flex-net-sim/-/compare/v0.1.1...v0.2.0
[0.3.0]: https://gitlab.com/DaniloBorquez/flex-net-sim/-/compare/v0.2.0...v0.3.0
[0.4.0]: https://gitlab.com/DaniloBorquez/flex-net-sim/-/compare/v0.3.0...v0.4.0
[0.4.1]: https://gitlab.com/DaniloBorquez/flex-net-sim/-/compare/v0.4.0...v0.4.1
[0.5.0]: https://gitlab.com/DaniloBorquez/flex-net-sim/-/compare/v0.4.1...v0.5.0
[0.5.1]: https://gitlab.com/DaniloBorquez/flex-net-sim/-/compare/v0.5.0...v0.5.1
[0.5.2]: https://gitlab.com/DaniloBorquez/flex-net-sim/-/compare/v0.5.1...v0.5.2
[0.5.3]: https://gitlab.com/DaniloBorquez/flex-net-sim/-/compare/v0.5.2...v0.5.3
[0.6.0]: https://gitlab.com/DaniloBorquez/flex-net-sim/-/compare/v0.5.3...v0.6.0
