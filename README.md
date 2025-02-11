# Arikoto

Arikoto is my personal Operating System development project (which I'm hoping will be usable in the future) that is inspired by and is aiming to have a similar nature to [ToaruOS](https://github.com/klange/toaruos) (which is my favourite hobby OS out there and I may be slightly obsessed with it).

## Features/Current Stuff:
UEFI and BIOS x86_64 support with ~~multiboot2~~ Limine protocol (again).

Written in C (with partial help from artificial intelligence because obviously I'm not the best programmer in the world but I'm not fully writing the code with AI because that would be stupid and I wouldn't learn anything).

Some memory support and listing upon boot.
Simple VFS/ramdisk functionality.

Full CMake build system with seperated includes and src.

## Building and Testing

Run `mkdir build && cd build` in the Arikoto root folder. Type `cmake ..`. The project will be configured automatically.

From here, you can type `make`, `make run`, `make run-kvm`, `make run-uefi` and (my favourite) `make run-uefi-kvm`.

## Why CMake for an OS project?

It's expansive and flexible for me. It's something I wanted to experiment and use more, so why not use it with a personal experimental project?
