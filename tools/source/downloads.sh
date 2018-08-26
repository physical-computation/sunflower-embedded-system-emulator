#!/bin/sh

GCC_VER="gcc-7.2.0"
BINUTILS_VER="binutils-2.29.1"
NEWLIB_VER="newlib-2.5.0.20170922"

GCC_TAR=${GCC_VER}.tar.gz
BINUTILS_TAR=${BINUTILS_VER}.tar.gz
NEWLIB_TAR=${NEWLIB_VER}.tar.gz

wget	ftp://ftp.gnu.org/pub/gnu/gcc/$GCC_VER/$GCC_TAR
wget	ftp://ftp.gnu.org/pub/gnu/binutils/$BINUTILS_TAR
wget	ftp://sources.redhat.com/pub/newlib/$NEWLIB_TAR

tar -xvf $GCC_TAR
tar -xvf $BINUTILS_TAR
tar -xzvf $NEWLIB_TAR

rm $NEWLIB_TAR $GCC_TAR $BINUTILS_TAR
