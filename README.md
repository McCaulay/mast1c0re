# mast1c0re

## Overview
The "mast1c0re" project provides the necessary framework for developing payloads that can be executed on the PlayStation 4 or PlayStation 5 through a PlayStation 2 game save file by escaping the PlayStation emulator.

For additional information on the inner workings of the vulnerabilities used within this project, see the assosicated blog post "[mast1c0re: Part 3 – Escaping the emulator](https://mccaulay.co.uk/mast1c0re-part-3-escaping-the-emulator/)" and the original blog post "[mast1c0re: Hacking the PS4 / PS5 through the PS2 Emulator - Part 1 - Escape](https://cturt.github.io/mast1c0re.html)" by [CTurt](https://twitter.com/cturte).

## Installation

**NOTE:** It is recommended you use a linux distribution such as Ubuntu, as Windows support has not been tested.

To start developing using this mast1c0re repository, the following dependencies are required:

* [ps2sdk](https://github.com/ps2dev/ps2sdk)
* [okrager](https://github.com/McCaulay/okrager)

Clone the mast1c0re repository and add the path to the ENV variable `$MAST1C0RE`:

~~~
git clone git@github.com/McCaulay/mast1c0re.git /opt/mast1c0re/
echo 'export MAST1C0RE=/opt/mast1c0re/' >> ~/.bashrc
source ~/.bashrc
~~~

## Project Compilation
All provided sample projects have been provided with a `build.sh` script which builds the project for the target systems and firmware versions.

Note that the firmware version "0.00" means that the game save file should be compatible with all firmware versions.

Compile a target sample project by changing directory to that project, then execute the `build.sh` script:

~~~
cd samples/ps-lightbar
./build.sh
~~~

## Next Steps
### Improvements
* Upload and load config file "config-emu-ps4.txt" (https://www.psdevwiki.com/ps4/Talk:PS2_Classics_Emulator_Compatibility_List)
* Emulator load lua file
* Arbitary native code execution (CTurtE's mast1c0re Part 2)
* Kernel Exploits
* EBOOT v1.0 support
* Additional firmware version support
* Load PS2 games from USB
* Load PS2 games from HDD
* Make system calls firmware indepedent
* Additional PS2 game save exploits for other games. (Change "okrager" binary for another game)

### Project Ideas
* ps-psh - Primitive shell command server allowing you to telnet to the PS4/PS5
  * pwd - Print the current working directory
  * cd - Change directory
  * ls - List files in current working directory
  * cp - Copy a file
  * mv - Move a file or directory / rename
  * rm - Delete a file
  * download - Download a file from the remote console to the local system
  * upload - Upload a file from the local system to the remote console
  * play - Mount and play a PS2 ISO file
  * notification - Show a PS notification
  * root - Kernel exploit (Firmware dependent)
* ps-ftp - FTP server
* ps-debug-settings - Enable debug settings using a kernel exploit (Firmware dependent)
* ps-hen - Homebrew enabler using a kernel exploit (Firmware dependent)
* ps-cnc - Remote command and control server supporting TCP commands similar to ps-psh
* Desktop GUI app to manage PS2 games - Using the "ps-cnc", it could upload, manage, mount and play PS2 games stored on HDD

## References
* <https://cturt.github.io/mast1c0re.html>
* <https://psi-rockin.github.io/ps2tek/>
* <https://ps2dev.github.io/ps2sdk/index.html>
* <https://www.psdevwiki.com/ps4/PS2_Emulation>
* <https://www.psdevwiki.com/ps4/PS2_Classics_Emulator_Compatibility_List>
* <https://www.psdevwiki.com/ps4/Talk:PS2_Classics_Emulator_Compatibility_List>
* <https://www.psdevwiki.com/ps4/Syscalls>
* <http://fxr.watson.org/fxr/source/sys/syscall.h?v=FREEBSD-9-1>
* <https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf>
* <https://github.com/CTurt/PS4-SDK>
* <https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain>