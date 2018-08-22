#!/bin/sh

# riscv: GCC_VER="gcc-7.2.0"
# riscv: BINUTILS_VER="binutils-2.28"
# riscv: NEWLIB_VER="newlib-2.5.0.20170922"

GCC_VER="gcc-4.2.4"
BINUTILS_VER="binutils-2.16.1"
NEWLIB_VER="newlib-1.9.0"

GCC_TAR=${GCC_VER}.tar.gz
BINUTILS_TAR=${BINUTILS_VER}.tar.gz
NEWLIB_TAR=${NEWLIB_VER}.tar.gz"

wget	ftp://ftp.gnu.org/pub/gnu/gcc/$GCC_VER/$GCC_TAR
wget	ftp://ftp.gnu.org/pub/gnu/binutils/$BINUTILS_TAR
wget	ftp://sources.redhat.com/pub/newlib/$NEWLIB_TAR

tar -xvf $GCC_TAR

tar -xvf $BINUTILS_TAR

tar -xzvf $NEWLIB_TAR

rm $NEWLIB_TAR $GCC_TAR $BINUTILS_TAR
