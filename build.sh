clang++ -Wall -Wextra -Wpedantic --std=c++23 --target=wasm32 -O3 -flto -nostdlib -c --include-directory=include -o listgen.o listgen.cpp
clang++ -Wall -Wextra -Wpedantic --std=c++23 --target=wasm32 -O3 -flto -nostdlib -c --include-directory=include -o string.o string.cpp
wasm-ld --no-entry --lto-O3 -o listgen.wasm listgen.o string.o
