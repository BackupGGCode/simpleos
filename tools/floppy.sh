#!/bin/bash

echo Creating floppy.img

dd if=/dev/zero of=floppy.img bs=1k count=1440 > /dev/null 2>&1
losetup /dev/loop0 floppy.img
mke2fs /dev/loop0 > /dev/null 2>&1

echo "(fd0) /dev/loop0" > floppy.map

mount /dev/loop0 /mnt/floppy
mkdir -p /mnt/floppy/boot/grub
cp /usr/share/grub/x86_64-redhat/stage1 /mnt/floppy/boot/grub/stage1
cp /usr/share/grub/x86_64-redhat/stage2 /mnt/floppy/boot/grub/stage2

grub --batch --device-map=floppy.map /dev/loop0 <<EOT > /dev/null 2>&1
root (fd0)
setup (fd0)
quit
EOT

rm -f floppy.map

if [ -f grub.conf ]; then
   cp grub.conf /mnt/floppy/boot/grub/grub.conf
else
	cat << EOF > /mnt/floppy/boot/grub/grub.conf
timeout 0

title   SimpleOS
root    (fd0)
kernel  /kernel
EOF
fi

umount /dev/loop0
losetup -d /dev/loop0

echo Done
