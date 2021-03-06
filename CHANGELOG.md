# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.2.0] - 2021-03-06

### ADDED

- You can get the Bitrate value from a bitrate object
- Unit tests in BitRate class
- Exceptions in BitRate getters.
- One big header file added. If you want, no needs to compile the library. Just include the one big header file into your project.

### FIXED

- JSON BitRate method. Now the method get the values from jason in an ordered way.

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
