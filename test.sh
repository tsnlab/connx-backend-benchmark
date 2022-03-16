#!/bin/bash
set -eo pipefail

backend="${1:-cpu}"

case $backend in
    "xnnpack")
        CFLAGS="XNNPACK/build/local/libXNNPACK.a XNNPACK/build/local/cpuinfo/libcpuinfo.a XNNPACK/build/local/clog/libclog.a XNNPACK/build/local/pthreadpool/libpthreadpool.a -lpthread -I XNNPACK/include/ -I XNNPACK/build/local/pthreadpool-source/include/ -Wno-deprecated-declarations"

        echo '* XNNPACK: -O0'
        gcc -O0 src/main.c src/xnnpack.c $CFLAGS && ./a.out

        echo '* XNNPACK: -O3'
        gcc -O3 src/main.c src/xnnpack.c $CFLAGS && ./a.out

        rm -f a.out
        ;;

    "cpu")
        echo '* CPU: -O0'
        gcc -O0 src/main.c src/cpu.c && ./a.out

        echo '* CPU: -O3'
        gcc -O3 src/main.c src/cpu.c && ./a.out

        rm -f a.out
        ;;
esac
