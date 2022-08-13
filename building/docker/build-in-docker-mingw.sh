mkdir -p /build
rsync -a /wxocto /build/
cd /build/wxocto/
cmake -DCMAKE_TOOLCHAIN_FILE=building/cmake/mingw-w64-x86_64.cmake -B /build/wxocto/build -S . -DCMAKE_BUILD_TYPE=Release -DMINGW=1
cmake --build /build/wxocto/build --target wxocto -- -j4
cp /build/wxocto/build/wxocto.exe /out/