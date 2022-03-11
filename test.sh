if [[ $# -eq 1 && $1 == 'xnnpack' ]]; then
    echo '* CPU: -O0'
    gcc -O0 src/main.c src/xnnpack.c && ./a.out

    echo '* CPU: -O3'
    gcc -O3 src/main.c src/xnnpack.c && ./a.out

    rm -f a.out
else
    echo '* CPU: -O0'
    gcc -O0 src/main.c src/cpu.c && ./a.out

    echo '* CPU: -O3'
    gcc -O3 src/main.c src/cpu.c && ./a.out

    rm -f a.out
fi
