# wcurses

**wcurses** is a library that provides partial `ncurses` functionality for Windows and acts as a wrapper for `ncurses` on Unix-like systems. It enables the creation of cross-platform console interfaces without the need to modify code for different operating systems.

## Features

- **Cross-platform compatibility**: Acts as a interface to ncurses on Unix-like systems and provides a custom implementation for Windows.
- **Partial Windows support**: Not all `ncurses` functions are available in the Windows version.
- **Windows limitations**:
  - Uses `<Windows.h>` for part of its functionality.
  - Avoid using `<Windows.h>` functions that modify terminal settings, output text, or read input, as they may cause unexpected behavior.
  - `Windows Terminal` may experience issues due to Microsoft API changes.

## Installation

### Unix-like systems (Linux, macOS)

Before proceeding, ensure that you have the `ncurses` library installed.

1. Clone the repository:
   ```sh
   git clone https://github.com/your-repo/wcurses.git
   cd wcurses
   ```

2. Use `build_gcc.sh` for automatic compilation with `CMake`:
   ```sh
   ./build_gcc.sh
   ```

3. Move the `include` folder to your project directory.

4. Move the `libwcurses.a` file from the `lib/gcc` folder to the `lib` folder in your project.

5. Compilation and usage in your project:
   ```sh
   g++ your_files.cpp -I include -L lib -lwcurses -lncurses -o your_program
   ```

### Windows

There are two available compilation options: MinGW and Visual Studio.

#### MinGW 14.2.0

1. Clone the repository:
   ```sh
   git clone https://github.com/your-repo/wcurses.git
   cd wcurses
   ```

2. Use `build_mingw_14_2_0.bat` for automatic compilation with `CMake`:
   ```sh
   build_mingw.bat
   ```

3. Move the `include` folder to your project directory.

4. Move the `libwcurses.a` file from the `lib/mingw` folder to the `lib` folder in your project.

5. Compilation and usage in your project:
   ```sh
   g++ your_files.cpp -I include -L lib -lwcurses -o your_program
   ```

#### Visual Studio 17

1. Clone the repository:
   ```sh
   git clone https://github.com/your-repo/wcurses.git
   cd wcurses
   ```

2. Use `build_vs_17.bat` to compile the library:
   ```sh
   build_vs_17.bat
   ```

3. Move the `include` folder to your project directory.

4. Move the `wcurses.lib` file from the `lib/vc17` folder to the `lib` folder in your project.

## Debug and Release Versions

- `wcurses_d` is the debug version.
- `wcurses` is the release version.

## Usage

Here is a minimal example using `wcurses`:

```cpp
#include "wcurses/wcurses.h"

int main() {
  curs::wcurses.Initscr(size);
  curs::wcurses << "Hello, wcurses!" << curs::Enld;
  curs::wcurses.Refresh();
  curs::wcurses.Getch();
  curs::wcurses.Endwin();
  return 0;
}
```

## Contributions and Support

I appreciate anyone who can help fix potential bugs or improve the library! If you find an issue or have enhancement ideas, feel free to open an issue or submit a pull request in the repository.



## References

- [Official ncurses mirror](https://github.com/mirror/ncurses)

