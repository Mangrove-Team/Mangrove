cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="/home/antoiix/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake"
cd build
make
cp Mangrove ../
cd ../