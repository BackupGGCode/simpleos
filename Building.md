# Cross Compiler #

To compile SimpleOS you must first create a GCC cross complier
  * More information on the [OSDev wiki](http://wiki.osdev.org/GCC_Cross-Compiler)
  * The following has been tested on RHEL 6 x86\_64 with Binutils 2.20 and GCC 4.4

```
cd /usr/local/src

wget ftp://ftp.gnu.org/gnu/gcc/gcc-4.8.2/gcc-4.8.2.tar.gz
wget http://ftp.gmplib.org/gmp/gmp-5.1.3.tar.bz2
wget http://www.mpfr.org/mpfr-current/mpfr-3.1.2.tar.bz2
wget http://multiprecision.org/mpc/download/mpc-1.0.1.tar.gz

tar -xf gcc-4.8.2.tar.gz
tar -xf gmp-5.1.3.tar.bz2
tar -xf mpfr-3.1.2.tar.bz2
tar -xf mpc-1.0.1.tar.gz

mv gmp-5.1.3 gcc-4.8.2/gmp
mv mpfr-3.1.2 gcc-4.8.2/mpfr
mv mpc-1.0.1 gcc-4.8.2/mpc

cd gcc-4.8.2

./configure --prefix=/usr/local --enable-languages=c,c++ --program-suffix=-cross --without-headers --disable-nls --disable-multilib --with-arch_32=i686 --build=x86_64-redhat-linux 

make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

which gcc-cross
gcc-cross -v
```

Once you have a working cross compiler you can use the project Makefile to build a floppy disk or ISO image.