cmake_minimum_required(VERSION 3.16)
project(arikoto LANGUAGES C ASM)

# Settings
set(IMAGENAME "arikoto")
set(QEMUFLAGS -m 2G)

# Add kernel subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/kernel ${CMAKE_BINARY_DIR}/kernel)

# Prevent recloning directories everytime we build
add_custom_target(limine-setup
    COMMAND test -d limine || (git clone https://github.com/limine-bootloader/limine.git --branch=v9.x-binary --depth=1 && make -C limine)
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(ovmf-setup
    COMMAND mkdir -p ovmf
    COMMAND test -f ovmf/ovmf-code-x86_64.fd || curl -Lo ovmf/ovmf-code-x86_64.fd https://github.com/osdev0/edk2-ovmf-nightly/releases/latest/download/ovmf-code-x86_64.fd
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# ISO creation
add_custom_target(iso
    DEPENDS kernel limine-setup
    COMMAND ${CMAKE_COMMAND} -E remove_directory iso_root
    COMMAND ${CMAKE_COMMAND} -E make_directory iso_root/boot iso_root/boot/limine iso_root/EFI/BOOT
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/kernel/arikoto iso_root/boot/
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/limine.conf
        ${CMAKE_SOURCE_DIR}/limine/limine-bios.sys
        ${CMAKE_SOURCE_DIR}/limine/limine-bios-cd.bin
        ${CMAKE_SOURCE_DIR}/limine/limine-uefi-cd.bin
        iso_root/boot/limine/
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/limine/BOOTX64.EFI
        ${CMAKE_SOURCE_DIR}/limine/BOOTIA32.EFI
        iso_root/EFI/BOOT/
    COMMAND xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus
        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin
        -efi-boot-part --efi-boot-image --protective-msdos-label
        iso_root -o ${IMAGENAME}.iso
    COMMAND ${CMAKE_SOURCE_DIR}/limine/limine bios-install ${IMAGENAME}.iso
    COMMAND ${CMAKE_COMMAND} -E remove_directory iso_root
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# QEMU targets
add_custom_target(run
    DEPENDS iso
    COMMAND qemu-system-x86_64 -M q35 -cdrom ${IMAGENAME}.iso -boot d ${QEMUFLAGS}
)

add_custom_target(run-kvm
    DEPENDS iso
    COMMAND qemu-system-x86_64 -M q35 -cdrom ${IMAGENAME}.iso -boot d -enable-kvm ${QEMUFLAGS}
)

add_custom_target(run-uefi
    DEPENDS iso ovmf-setup
    COMMAND qemu-system-x86_64 -M q35
        -drive if=pflash,unit=0,format=raw,file=${CMAKE_SOURCE_DIR}/ovmf/ovmf-code-x86_64.fd,readonly=on
        -cdrom ${IMAGENAME}.iso -boot d ${QEMUFLAGS}
)

add_custom_target(run-uefi-kvm
    DEPENDS iso ovmf-setup
    COMMAND qemu-system-x86_64 -M q35
        -drive if=pflash,unit=0,format=raw,file=${CMAKE_SOURCE_DIR}/ovmf/ovmf-code-x86_64.fd,readonly=on
        -cdrom ${IMAGENAME}.iso -boot d -enable-kvm ${QEMUFLAGS}
)
