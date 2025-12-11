mkdir -p build_wasm

cmake -B build_wasm -G Ninja -DCMAKE_TOOLCHAIN_FILE=../wasi-sdk/share/cmake/wasi-sdk.cmake .
cmake --build build_wasm

# ln -s $PWD/build_wasm/compile_commands.json compile_commands.json
wasm2wat build_wasm/listgen.wasm > build_wasm/listgen.wat
