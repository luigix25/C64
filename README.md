# C64 Emulator
A Very Basic C64 Emulator implemented using C++14 and SDL.

# Compiling

SDL library must be already installed 

```
make
```

# Usage

Launch

```
./main
```
Load PRG file

```
./main path/to/file.prg
```

# Things Working

* CPU Opcodes
* VIC II bank switching
* Memory bank switching

# Things Partially Implemented

* Keyboard 
	* Some keys are not mapped

* VIC II
	* Raster Interrupt
	* Char mode
	* Char MCM
	* Bitmap Mode
	* Bitmap MCM

# TODO List

* CPU code refactoring
* Memory code refactoring
* Cartridge Loading
