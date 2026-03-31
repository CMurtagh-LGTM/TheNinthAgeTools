mkdir -p build_wasm

if [ ! -d "webidl2wit" ]; then
    # git clone https://github.com/CMurtagh-LGTM/webidl2wit.git --depth=1
    cp -r ~/Documents/packages/webidl2wit/ .
    cd webidl2wit
    cargo build
    cd ..
fi

if [ ! -d "wit-bindgen" ]; then
    cp -r ~/Documents/packages/wit-bindgen/ .
    cd wit-bindgen
    cargo build
    cd ..
fi

cmake -B build_wasm -G Ninja -DCMAKE_TOOLCHAIN_FILE=../wasi-sdk/share/cmake/wasi-sdk-p2.cmake .
cmake --build build_wasm

# ln -s $PWD/build_wasm/compile_commands.json compile_commands.json
