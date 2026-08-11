# HEICConverter

A command-line tool to convert HEIC/HEIF image files to JPEG format.

## Features

- Convert single HEIC files to JPEG
- Batch convert multiple HEIC files
- Adjustable JPEG quality
- Cross-platform support (Linux, macOS, Windows)

## Building

### Requirements

- CMake 3.10+
- C++17 compiler
- ImageMagick/Magick++
- vcpkg (optional, for dependency management)

### Build Instructions

#### Linux/macOS

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

#### Windows

Set up vcpkg and run:

```batch
build-local.bat
```

## Usage

### Convert a single file

```bash
./HEICConverter -i input.heic -o output.jpg -q 90
```

### Convert all HEIC files in a directory

```bash
./HEICConverter -i /path/to/directory -r -q 85
```

### Options

- `-i, --input <file>` - Input HEIC file or directory
- `-o, --output <file>` - Output JPEG file (single file mode only)
- `-q, --quality <1-100>` - JPEG quality (default: 85)
- `-r, --recursive` - Enable batch mode for directories
- `-h, --help` - Show help message

## License

MIT License
