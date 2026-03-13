#!/bin/bash

set -e

echo "Building Screen Streamer..."

# 检测操作系统
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    # Windows (MSYS2/MinGW)
    CMAKE_GENERATOR="Ninja"
    CMAKE_PREFIX_PATH="C:/Qt/6.10.0/mingw_64/lib/cmake"
    PARALLEL_JOBS=$(nproc)
    FFMPEG_DIR="C:/ffmpeg"
else
    # Linux/macOS
    CMAKE_GENERATOR="Unix Makefiles"
    CMAKE_PREFIX_PATH="/usr/local/Qt-6.10.0/lib/cmake"
    PARALLEL_JOBS=$(nproc)
    FFMPEG_DIR="/usr/local/ffmpeg"
fi

# 创建构建目录
mkdir -p build
cd build

# 配置CMake
echo "Configuring CMake..."
echo "Generator: $CMAKE_GENERATOR"
echo "Qt path: $CMAKE_PREFIX_PATH"
echo "FFmpeg dir: $FFMPEG_DIR"

cmake .. -G "$CMAKE_GENERATOR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH" \
    -DFFMPEG_DIR="$FFMPEG_DIR"

# 构建
echo "Building with $PARALLEL_JOBS parallel jobs..."
cmake --build . --config Release --parallel $PARALLEL_JOBS

echo "Build completed successfully!"
echo "Executable: build/ScreenStreamer"