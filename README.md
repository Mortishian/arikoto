# Arikoto

Arikoto is a kernel made to be similar to the Misaka kernel from ToaruOS 1.99-2.x.

TODO: Finish readme.

## How to use this?

### Dependencies

Any `make` command depends on GNU make (`gmake`) and is expected to be run using it. This usually means using `make` on most GNU/Linux distros, or `gmake` on other non-GNU systems.

It is recommended to build this project using a standard UNIX-like system, using a Clang/LLVM toolchain capable of cross compilation (the default, unless `KCC` and/or `KLD` are explicitly set).

Additionally, building an ISO with `make all` requires `xorriso`, and building a HDD/USB image with `make all-hdd` requires `sgdisk` (usually from `gdisk` or `gptfdisk` packages) and `mtools`.

### Architectural targets

The `KARCH` make variable determines the target architecture to build the kernel and image for.

The default `KARCH` is `x86_64` and will currently stay as such.

### Makefile targets

Running `make all` will compile the kernel (from the `kernel/` directory) and then generate a bootable ISO image.

Running `make all-hdd` will compile the kernel and then generate a raw image suitable to be flashed onto a USB stick or hard drive/SSD.

Running `make run` will build the kernel and a bootable ISO (equivalent to make all) and then run it using `qemu` (if installed).

Running `make run-hdd` will build the kernel and a raw HDD image (equivalent to make all-hdd) and then run it using `qemu` (if installed).

Running `run-bios` and `run-hdd-bios` targets are equivalent to their non `-bios` counterparts except that they boot `qemu` using the default SeaBIOS firmware instead of OVMF.

Running `make run-kvm` will build the kernel and a bootable ISO and then run it with KVM support using `qemu`.

## Credits

@IanSeyler - Framebuffer implementation code

@mintsuki - Original Limine C template kernel project and Limine bootloader
