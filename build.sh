clang++ -Wall -Wextra -Wpedantic --std=c++23 --target=wasm32 -O3 -flto -nostdlib -c -o listgen.o listgen.cpp
wasm-ld --no-entry --lto-O3 -o listgen.wasm listgen.o
