# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.3.0] - 2021-03-16

### ADDED

- Added Source/Destination as attributes of Link. Now Network is declared as a friend class within Link class.
- Added macros inside Simulator.hpp in order to obtain the Source/Destination of a link inside a route.
    - LINK_IN_ROUTE_SRC(route, link)
    - LINK_IN_ROUTE_DST(route, link)
- Macros added to the bitrate object in the algorithm implementation:
    - REQ_REACH(pos) Get the optical reach located in the *pos* on the JSON file 
    - REQ_MODULATION(pos) Get the modulation format located in the *pos* on the JSON file
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
