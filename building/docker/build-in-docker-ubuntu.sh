mkdir -p /build
rsync -a /wxocto /build/
cd /build/wxocto/
cmake -B /build/wxocto/build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build /build/wxocto/build --target wxocto -- -j4
cp /build/wxocto/build/wxocto /out/