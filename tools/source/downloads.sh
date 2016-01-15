#!/bin/sh

wget	ftp://ftp.gnu.org/pub/gnu/gcc/gcc-4.2.4/gcc-4.2.4.tar.bz2
wget	ftp://ftp.gnu.org/pub/gnu/binutils/binutils-2.16.1.tar.bz2
wget	ftp://sources.redhat.com/pub/newlib/newlib-1.9.0.tar.gz

bunzip2 gcc-4.2.4.tar.bz2
tar -xvf gcc-4.2.4.tar

bunzip2 binutils-2.16.1.tar.bz2
tar -xvf binutils-2.16.1.tar

tar -xzvf newlib-1.9.0.tar.gz
