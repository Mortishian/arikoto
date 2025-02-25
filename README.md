# Arikoto (ありこと)
[![CodeQL Runner](https://github.com/AFellowSpeedrunner/arikoto/actions/workflows/codeql.yml/badge.svg)](https://github.com/AFellowSpeedrunner/arikoto/actions/workflows/codeql.yml)

Arikoto is my personal Operating System development project (which I'm hoping will be usable in the future) that is inspired by and is aiming to have a similar nature to [ToaruOS](https://github.com/klange/toaruos) (which is my favourite hobby OS out there and I may be slightly obsessed with it).

## Features/Current Stuff:
UEFI and BIOS x86_64 support with ~~multiboot2~~ Limine protocol (again).

Written in C (with partial help from artificial intelligence because obviously I'm not the best programmer in the world but I'm not fully writing the code with AI because that would be stupid and I wouldn't learn anything).

Some memory support and listing upon boot.
Simple VFS/ramdisk functionality.
Some kind of scheduler.

Full CMake build system with seperated includes and src.

## Building and Testing

### Linux

Run `mkdir build && cd build` in the Arikoto root folder. Type `cmake ..`. The project will be configured automatically.

From here, you can type `make`, `make run`, `make run-kvm`, `make run-uefi` and (my favourite) `make run-uefi-kvm`.

### macOS

However, if you are on macOS, you will need to install Linux.

If you are not building on Linux and are instead using macOS, do not expect any support for problems that are encountered.

## Ways to Contribute

While Arikoto is my personal OS project, helping to fix bugs and issues in Arikoto helps a lot. Any major changes to functionality or any new features to be added need to be discussed in GitHub Issues. 

Main conversation related to Arikoto will take place on the [Zed code editor channel](https://zed.dev/channel/Arikoto-19596) and on the [Discord server](https://discord.gg/UczSZb7s7B).

## Why CMake for an OS project?

It's expansive and flexible for me. It's something I wanted to experiment and use more, so why not use it with a personal experimental project?
