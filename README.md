# Wolf3DC - Wolfenstein 3D Borland C IDE Emulator

**A modern IDE frontend fogr compiling Wolfenstein 3D source code using DJGPP/Open Watcom compilers.**

## Overview

Wolf3DC emulates the classic Borland C++ IDE experience while providing a bridge to modern DOS development tools. It supports the original Wolfenstein 3D `.PRJ` (Borland project) format and compiles C code using industry-standard compilers like DJGPP.

This project is designed specifically for developers who want to **compile and modify the original Wolfenstein 3D source code** from the id-Software repository.

## Features

### 🎮 **Wolfenstein 3D Focused**
- Native support for `.PRJ` (Borland C Project) files
- Handles `.C`, `.H`, and `.ASM` source files
- DOS memory models: real mode, protected mode (with CWSDPMI)
- Optimization levels: O0, O1, O2, O3, Os

### 🛠️ **Compiler Integration**
- **DJGPP** (primary) - GCC for DOS with 32-bit protected mode
- **Open Watcom** - Classic DOS compiler alternative
- Auto-detection of available compilers
- Full support for linking object files

### 🖥️ **Borland IDE-Style Interface**
- Menu-driven TUI (Text User Interface)
- Project management (create, open, save)
- File manager for adding/removing source files
- Compile with one command
- Run compiled executables directly
- Settings management

## System Requirements

- **Linux/macOS/Windows** (host system)
- CMake 3.10+
- C++17 compatible compiler
- **DJGPP** OR **Open Watcom** (for DOS cross-compilation)

### Installing DJGPP

**Ubuntu/Debian:**
```bash
sudo apt-get install djgpp-binutils djgpp-gcc djgpp-base
```

**macOS (Homebrew):**
```bash
brew install djgpp
```

**Windows:**
Download from [DJGPP Project](http://www.delorie.com/djgpp/)

## Building Wolf3DC

```bash
mkdir build
cd build
cmake ..
make
```

Executable: `./bin/w3d_ide` (Linux/macOS) or `bin\\w3d_ide.exe` (Windows)

## Usage

### Running the IDE

```bash
./bin/w3d_ide
```

You'll see the Borland-style menu:

```
╔════════════════════════════════════════════════════════════════╗
║     WOLF3DC - Wolfenstein 3D Borland C IDE Emulator v1.0      ║
║                    DOS Compiler Frontend                        ║
╚════════════════════════════════════════════════════════════════╝

Main Menu:

  [N] New Project
  [O] Open Project
  [C] Compile
  [R] Run
  [S] Settings
  [F] File Manager
  [H] Help
  [Q] Quit

────────────────────────────────────────────────────────────────
Command:
```

### Example: Compiling Wolfenstein 3D

1. **Create a new project** or **open** an existing `.PRJ` file from id-Software's repository
2. **Add source files** (`.C` and `.ASM` files)
3. **Compile** - Wolf3DC will automatically:
   - Compile all `.C` files with DJGPP
   - Assemble `.ASM` files with NASM
   - Link everything into a DOS executable
4. **Run** the resulting `.EXE`

## Project Format (.PRJ)

Borland C project files are simple text-based:

```ini
;
; Wolfenstein 3D Borland C Project
;

[PRJ]
ProjectName=WOLF3D
Output=WOLF3D.EXE

[FILES]
WL_MAIN.C
WL_GAME.C
WL_DRAW.C
WL_PLAY.C
WL_MENU.C
WL_AGENT.C
WL_ACT1.C
WL_ACT2.C
WL_STATE.C
WL_INTER.C
WL_SCALE.C
WL_TEXT.C
WL_DEBUG.C
ID_MM.C
ID_CA.C
ID_IN.C
ID_VH.C
ID_VL.C
ID_PM.C
ID_SD.C
WL_DR_A.ASM
C0.ASM

[OPTIONS]
Optimization=O2
MemoryModel=large
DebugInfo=true
Platform=dos_protected
```

Wolf3DC automatically parses and generates these files.

## Supported File Types

| Extension | Purpose |
|-----------|----------|
| `.C` | C source files |
| `.H` | C header files (included automatically) |
| `.ASM` | x86 assembly files (NASM format) |
| `.OBJ` | Object files (compiled intermediates) |
| `.PRJ` | Project configuration |
| `.EXE` | Final DOS executable |

## Architecture

```
Wolf3DC IDE Interface (TUI)
        ↓
Project Manager (.PRJ parsing/generation)
        ↓
Compiler Pipeline (DJGPP/Watcom wrapper)
        ↓
C Compiler (djgpp-gcc)
        ↓
Assembler (NASM)
        ↓
Linker (djgpp-ld with CWSDPMI)
        ↓
DOS .EXE (Protected Mode)
```

## Compiler Options

### Memory Models
- `real` - 16-bit real mode (64KB segments)
- `protected` - 32-bit protected mode with CWSDPMI extender (recommended)
- `huge` - Extended memory

### Optimization Levels
- `O0` - No optimization (fastest compile)
- `O1` - Basic optimization
- `O2` - Recommended for W3D
- `O3` - Aggressive optimization
- `Os` - Size optimization

## Troubleshooting

### "No suitable DOS C compiler found"

Ensure DJGPP or Open Watcom is installed and in your PATH.

### Compilation fails with linker errors

- Check that all source files are properly added to the project
- Ensure `.ASM` files are in NASM format, not Intel/AT&T format
- Verify memory model settings match your source code

### Generated .EXE won't run on real DOS

- Make sure "protected mode" is selected (requires CWSDPMI.EXE)
- For real DOS 86 compatibility, switch to "real mode" (16-bit)

## Development

Wolf3DC is organized into modular components:

- **`ide_interface.cpp`** - Main UI loop and menu handling
- **`project_manager.cpp`** - Project file parsing and management
- **`compiler_pipeline.cpp`** - Compiler detection and execution
- **`ui.cpp`** - Terminal formatting and utilities

## References

- [Wolfenstein 3D Source Code](https://github.com/id-Software/wolf3d)
- [DJGPP Documentation](http://www.delorie.com/djgpp/)
- [Open Watcom Compiler](https://github.com/open-watcom/open-watcom-v2)
- [CWSDPMI DOS Extender](http://www.delorie.com/djgpp/v2misc/cwsdpmi/)

## License

MIT License - See LICENSE file for details

Note: Wolfenstein 3D source code is under id-Software's license terms.

## Author

Wolf3DC Development Team
