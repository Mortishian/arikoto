# Arikoto

Arikoto is my personal Operating System development project (which I'm hoping will be usable in the future) that is inspired by and is aiming to have a similar nature to [ToaruOS](https://github.com/klange/toaruos) (which is my favourite hobby OS out there and I may be slightly obsessed with it).

## Features/Current Stuff:
UEFI and BIOS x86_64 support with ~~multiboot2~~ Limine protocol (again).

Written in C (with partial help from artificial intelligence because obviously I'm not the best programmer in the world but I'm not fully writing the code with AI because that would be stupid and I wouldn't learn anything).

Some memory support and listing upon boot.
Simple VFS functionality.

## Testing
Run `make run-uefi-kvm` or `run-kvm`.
