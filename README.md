# Tic-Tac-Toe

A simple Tic-Tac-Toe game written in pure C for Windows.

## Build

### Prerequisites

- **Windows OS**
- **GCC Compiler** (MinGW-w64 recommended, or MSYS2)
- **windres** (comes with MinGW-w64)
- **Terminal** (PowerShell, CMD, or Git Bash)

### Compile

1. Compile the resource file
```bash
windres icon.rc -o icon.o
```

2. Compile the application
```bash
gcc -Wall -Wextra -std=c11 -mwindows src/main.c src/game.c src/ai.c icon.o -o tic_tac_toe.exe -lgdi32 -luser32
```

### Run

```bash
./tic_tac_toe.exe
```

## Credits

App icon: [Tic tac toe icons created by Magnific - Flaticon](https://www.flaticon.com/free-icons/tic-tac-toe)