## Development environment for AVR butterfly

This environment i based on <https://github.com/Tmplt/atmega169p-devenv> and
modified to accommodate the new lab equipment in D0003E and debugging.

This directory contains all necessary headers and a Makefile to program on the Atmel AVR Butterfly.
Each `.c`-file placed into this directory is compiled by its own into an `.o`-file.
The main executable is then created by linking these object files.

All warnings are enabled, except those for quirks used in abundance by the included header files.
These extra warnings can be omitted by commenting out the value of `EXTRAFLAGS` in the makefile, but they are highly recommended.

`include/avr` was copied from the latest release of AVR Libc
<http://www.nongnu.org/avr-libc/>. All other header files, and necessary compilation
steps were copied from Atmel Studio 7.0.

### How to run
To program the butterfly you need to run `make` with root privileges or create a udev-rule
(see <https://github.com/Tmplt/atmega169p-devenv>). To start a debugging session
run `make debug` with root privileges.

### Debugging quirks
To step through non function calls in the beginning of main you need to set a break
point at the first line of main. Note that `break main` does not do this.


### Dependencies
The Makefile depends on `avr-gcc`, `avr-objcopy`, `avr-objdump`, `avr-size`, `avrdude`, `avr-gdb`, `avarice`
and possibly the `avr-libc` headers.
These can be installed on Arch Linux via the `avr-libc`, `avr-gcc`, `avr-binutils`, `avr-gdb`, `avarice`(AUR) and `avrdude` packages (or via your distro's equivalent).
