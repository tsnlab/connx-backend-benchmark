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

    "xsmm")
        if [ -z "$PKG_CONFIG_PATH" ]; then
            echo "Warning: PKG_CONFIG_PATH not set"
        fi

        CFLAGS="$(pkg-config --cflags --libs libxsmm) -lblas"

        echo '* XSMM: -O0'
        gcc -O0 src/main.c src/xsmm.c $CFLAGS && ./a.out

        echo '* XSMM: -O3'
        gcc -O3 src/main.c src/xsmm.c $CFLAGS && ./a.out

        rm -f a.out
        ;;

    "cpu")
        echo '* CPU: -O0'
        gcc -O0 src/main.c src/cpu.c && ./a.out

        echo '* CPU: -O3'
        gcc -O3 src/main.c src/cpu.c && ./a.out

        rm -f a.out
        ;;
    "zynq")
        # setting environment
        VITIS_HOME="${XILINX_VITIS:-/opt/Xilinx/Vitis/2021.2}"
        source "${VITIS_HOME}/settings64.sh"
        path=$(dirname "$(readlink -e "$0")")
        cd "$path" || true
        BASEDIR=`pwd`
        target="${2:-ps}"
        option="${3:-O0}"

        # create platform, domain, and application
        xsct <<-EOF
        setws
        platform create -name {design_1_wrapper} -hw {design_1_wrapper.xsa} -out .;platform write
        domain create -name {freertos10_xilinx_ps7_cortexa9_0} -display-name {freertos10_xilinx_ps7_cortexa9_0} -os {freertos10_xilinx} -proc {ps7_cortexa9_0} -runtime {cpp} -arch {32-bit} -support-app {freertos_hello_world}
        platform write
        platform active {design_1_wrapper}
        domain active {zynq_fsbl}
        domain active {freertos10_xilinx_ps7_cortexa9_0}
        bsp setlib -name xilffs
        platform generate
        app create -name test -platform design_1_wrapper -domain {freertos10_xilinx_ps7_cortexa9_0} -sysproj {test_system} -template {Empty Application(C)}
EOF

        if [ $target = "ps" ]; then
            cp src/cpu.c src/main.c test/src
        else
            cp src/npu.c src/main.c test/src
        fi

        # increase stack and heap memory size
        sed -i 's/0x2000/0x1E8480/g' test/src/lscript.ld
        if [ $option = "O0" ]; then
            xsct <<-EOF
            setws
            app config -name test compiler-optimization "None (-O0)"
EOF
        else
            xsct <<-EOF
            setws
            app config -name test compiler-optimization "Optimize most (-O3)"
EOF
        fi

        # application config, and build
        # reset board, and upload code to board
        if [ $target = "npu" ]; then
            xsct << EOF
            setws
            app config -name test define-compiler-symbols "__npu__"
EOF
        fi

        xsct <<-EOF
        setws
        app config -name test define-compiler-symbols "__zynq__"
        app build -name test
        connect
        source design_1_wrapper/hw/ps7_init.tcl
        targets -set -nocase -filter {name =~ "Arm*#0"}
        stop
        ps7_init
        rst -processor
        dow test/Debug/test.elf
        source design_1_wrapper/hw/ps7_init.tcl
        targets -set -nocase -filter {name =~ "Arm*#0"}
        stop
        ps7_init
        rst -processor
        dow test/Debug/test.elf
        con
        disconnect
EOF
        rm -rf .metadata .Xil design_1_wrapper test test_system .analytics IDE.log
esac
