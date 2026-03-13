# AGENTS.md - Screen Streamer Development Guide

## Build Commands

### Basic Build
```bash
# Create build directory and configure
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/path/to/Qt/6.10.0/lib/cmake

# Build the project
cmake --build . --config Release --parallel $(nproc)

# Alternative using build script
./build.sh
```

### Development Build
```bash
# Debug build with symbols
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/path/to/Qt/6.10.0/lib/cmake
cmake --build . --config Debug

# Clean build
rm -rf build && mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### Running Tests
```bash
# Build and run all tests
cd build
cmake --build . --target test
ctest --output-on-failure

# Run specific test
ctest -R "TestName" --output-on-failure
```

### Code Quality Checks
```bash
# Format code (if clang-format is configured)
find src -name "*.cpp" -o -name "*.h" | xargs clang-format -i

# Static analysis (if available)
cd build
cmake --build . --target analyze
```

## Code Style Guidelines

### File Organization
- **Header files**: `.h` extension, placed in appropriate module directories
- **Source files**: `.cpp` extension, mirror header file structure
- **Include guards**: Use `#ifndef FILENAME_H` / `#define FILENAME_H` pattern
- **Qt headers**: Use `<QClassName>` format (no `.h` extension)

### Naming Conventions
- **Classes**: `PascalCase` (e.g., `ConfigManager`, `MainToolbar`)
- **Methods/Functions**: `camelCase` (e.g., `initialize()`, `updateCameraList()`)
- **Variables**: `camelCase` with descriptive names (e.g., `m_cameraCapture`, `m_isStreaming`)
- **Constants**: `UPPER_SNAKE_CASE` (e.g., `MAX_FRAME_RATE`)
- **Private members**: Prefix with `m_` (e.g., `m_screenCapture`)
- **Static members**: Prefix with `s_` (e.g., `s_instance`)

### Import/Include Order
1. System headers (C/C++ standard library)
2. Qt framework headers
3. FFmpeg headers (with `extern "C"` wrapper)
4. Project headers (relative paths)

Example:
```cpp
#include <vector>
#include <string>

#include <QApplication>
#include <QWidget>

extern "C" {
#include <libavcodec/avcodec.h>
}

#include "core/config/ConfigManager.h"
#include "ui/MainToolbar.h"
```

### Class Structure
```cpp
#ifndef CLASSNAME_H
#define CLASSNAME_H

#include <QObject>
#include <QString>

class ClassName : public QObject {
    Q_OBJECT
public:
    explicit ClassName(QObject* parent = nullptr);
    ~ClassName();
    
    // Public methods
    bool initialize();
    void shutdown();
    
signals:
    void statusChanged(bool isActive);
    void errorOccurred(const QString& error);
    
public slots:
    void onEvent(const QString& data);
    
private:
    // Private methods
    void helperFunction();
    
    // Member variables
    bool m_isInitialized = false;
    QString m_currentState;
};

#endif // CLASSNAME_H
```

### Error Handling
- Use `bool` return types for operations that can fail
- Emit `errorOccurred(const QString&)` signals for asynchronous errors
- Use `qWarning()`, `qDebug()`, `qCritical()` for logging
- Document error conditions in method comments

### Memory Management
- Use `QScopedPointer` or `std::unique_ptr` for owned pointers
- Use raw pointers for non-owned references
- Follow Qt parent-child relationship for QObject-derived classes
- Ensure proper cleanup in destructors

### FFmpeg Integration
- Wrap FFmpeg includes with `extern "C" { ... }`
- Check FFmpeg function return values
- Properly allocate/free FFmpeg structures
- Handle FFmpeg errors with appropriate logging

### UI Development
- Use Qt Widgets (not QML) for this project
- Follow dark theme styling from `resources/styles/dark.qss`
- Use signals/slots for UI interactions
- Keep UI logic separate from business logic

### Configuration Management
- Use `ConfigManager` singleton for persistent settings
- Store configuration in JSON format
- Use default values for new configuration items
- Handle configuration errors gracefully

## Development Workflow

1. **Before making changes**: Run existing tests to ensure they pass
2. **Implement feature**: Follow code style guidelines
3. **Add tests**: Create unit tests for new functionality
4. **Verify build**: Ensure project builds without warnings
5. **Test functionality**: Manually test the feature
6. **Update documentation**: Update README.md if needed

## Environment Setup

### Required Dependencies
- Qt 6.10.0 (with MinGW 13.2.0 on Windows)
- FFmpeg 6.x LTS (dynamic linking)
- CMake 3.25+
- C++17 compatible compiler

### Environment Variables
```bash
# Windows (adjust paths as needed)
export FFMPEG_DIR="C:/ffmpeg"
export Qt6_DIR="C:/Qt/6.10.0/mingw_64/lib/cmake/Qt6"

# Linux/macOS
export FFMPEG_DIR="/usr/local/ffmpeg"
export Qt6_DIR="/usr/local/Qt-6.10.0/lib/cmake/Qt6"
```

## Testing Guidelines

### Unit Tests
- Place tests in `tests/` directory
- Use Qt Test framework for Qt components
- Test both success and failure cases
- Mock external dependencies when needed

### Integration Tests
- Test module interactions
- Verify FFmpeg encoding/decoding
- Test network streaming functionality
- Validate configuration persistence

### Performance Testing
- Measure frame capture/encoding latency
- Test memory usage under load
- Verify 60 FPS stability
- Benchmark hardware encoder performance

## Commit Guidelines

- Use descriptive commit messages
- Reference issue numbers when applicable
- Keep commits focused on single changes
- Ensure code builds and tests pass before committing

## Troubleshooting

### Common Issues
- **FFmpeg not found**: Set `FFMPEG_DIR` environment variable
- **Qt modules missing**: Verify Qt installation includes required components
- **Build errors**: Clean build directory and reconfigure
- **Runtime crashes**: Check FFmpeg DLL availability on Windows

### Debugging Tips
- Enable Qt debug output with `QT_LOGGING_RULES="*.debug=true"`
- Use `qDebug() << "Debug message";` for tracing
- Check FFmpeg error codes with `av_strerror()`
- Validate configuration file format and permissions