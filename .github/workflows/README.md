# GitHub Actions Workflows

## Build and Release Workflow

### Overview
This workflow automatically builds the Screen Streamer application on Windows 10 x64 with MinGW and creates release packages when tags are pushed.

### Triggers
- **Push to main/develop branches**: Builds the application and uploads artifacts
- **Push tags (v*)**: Builds, creates release package, and publishes to GitHub Releases
- **Pull requests**: Builds to verify changes

### Environment
- **OS**: Windows 10/11 x64 (latest)
- **Compiler**: MinGW 13.2.0 (via MSYS2)
- **Qt**: 6.10.0 (MinGW 64-bit)
- **FFmpeg**: 6.x LTS (shared libraries)
- **CMake**: 3.28.0+
- **Build System**: Ninja

### Steps
1. **Setup MSYS2/MinGW**: Installs MinGW toolchain and build tools
2. **Install Qt 6.10.0**: Downloads and configures Qt with MinGW support
3. **Install FFmpeg 6.x**: Downloads FFmpeg shared libraries
4. **Configure CMake**: Sets up build with proper paths
5. **Build Project**: Compiles with parallel jobs
6. **Collect Artifacts**: Gathers executable and DLLs
7. **Deploy Qt Dependencies** (tag only): Runs windeployqt
8. **Create Release Package** (tag only): Creates ZIP archive
9. **Upload to GitHub Releases** (tag only): Publishes release

### Release Package Contents
When a tag is pushed (e.g., `v1.0.0`), the workflow creates:
- `ScreenStreamer-v1.0.0-windows-x64.zip` containing:
  - `ScreenStreamer.exe` (main executable)
  - Required Qt DLLs and plugins
  - FFmpeg DLLs (avcodec, avformat, avutil, swscale)
  - Resources and documentation

### Manual Trigger
You can manually trigger the workflow from the GitHub Actions tab:
1. Go to **Actions** → **Build and Release**
2. Click **Run workflow**
3. Select branch and click **Run workflow**

### Artifact Retention
- **Build artifacts**: Retained for 7 days
- **Release assets**: Permanent (stored with GitHub release)

### Customization
To modify the workflow:
1. Edit `.github/workflows/build.yml`
2. Key variables in the `env` section:
   - `QT_VERSION`: Qt version (default: 6.10.0)
   - `FFMPEG_VERSION`: FFmpeg version (default: 6.1)
   - `BUILD_TYPE`: Build configuration (default: Release)

### Notes
- The workflow uses `windeployqt` to automatically copy Qt dependencies
- FFmpeg DLLs are copied from the installed location
- For installer creation (InnoSetup), additional setup is required on the runner