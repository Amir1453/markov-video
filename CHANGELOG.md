# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Added

- Added -latex-compiler-options flag which allows users to input their own flags.
- Added CHANGELOG.md
- Created class MarkovProcessor.
- Added get_timestamp().
- Added file_extension option -fe to specify video extension.

### Changed

- Abstracted std::system calls to execute_command.
- Changed release name to markov-video-v*.*.* in GitHub workflow. 
- Separated functions from the main function.
- Replaced if statements with switch case enums.
- Improved README.md.
- Changed default build directory to build_{timestamp}.
- Added DEFAULT_VIDEO_EXTENSION to constants. 

### Fixed

- Minor formatting issues.
- Removed unnecessary includes and added needed ones.
- Fixed const& primitives to be copied.
- Removed redundant backslashes from command strings.
- Switched Markov class definition to snake_case.

## [0.1.1] - 2024-08-17

### Changed

- Made iterations mandatory only if -G or -V is set.
- Moved program help options order.

## [0.1.0] - 2024-08-16

### Added

- Initial release.
