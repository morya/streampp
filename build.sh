#!/bin/bash

set -e

echo "Building Screen Streamer..."

# 创建构建目录
mkdir -p build
cd build

# 配置CMake
echo "Configuring CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DCMAKE_PREFIX_PATH=/usr/local/Qt-6.10.0/lib/cmake

# 构建
echo "Building..."
cmake --build . --config Release --parallel $(nproc)

echo "Build completed successfully!"
echo "Executable: build/ScreenStreamer"