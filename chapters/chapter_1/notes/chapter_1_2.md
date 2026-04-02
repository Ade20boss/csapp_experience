### 1.2 Programs are Translated by Other Programs into Different Forms

The `hello.c` program begins life as a high-level C program because it can be read and understood by human beings in that form. However, in order to run `hello.c` on the system, the individual C statements must be translated by other programs into a sequence of low-level machine-language instructions. These instructions are then packaged in a form called an **Executable Object Program** and then stored as a binary disk file. Object programs are also referred to as **executable object files**.

On a Unix system, the translation from source file to object file is performed by a **Compiler Driver**.

_(Note diagram flow: Source file `hello.c` -> Pre-processor `cpp` -> Modified source `hello.i` -> Compiler `cc1` -> Assembly program `hello.s` -> Assembler `as` -> Relocatable object `hello.o` -> Linker `ld` (with `printf.o`) -> Executable `hello`)_

So the running is typically done like this: `linux> gcc -o hello hello.c` Here, the `gcc` compiler driver reads the source file `hello.c` and translates it into an executable object file `hello`. The translation happens in four phases (preprocessor, compiler, assembler, and linker) and are collectively known as **The Compilation System**.

**Distinction Between the Compiler Driver and the Compiler:**

- The compiler driver is the overarching manager program that orchestrates the entire build process. When you invoke a command like `gcc` or `clang` on the terminal, you are executing the compiler driver.
    
    - **Function:** It parses command-line arguments and flags to determine exactly what needs to be built.
        
    - **Orchestration:** It sequentially invokes the four specialized tools of the compilation (preprocessor, compiler, assembler, linker).
        
    - **File Management:** It routes the output of one tool as the input to the next (e.g., passing the `.i` preprocessed file to the compiler) and automatically deletes these intermediate temporary files when the build is complete.
        
- **The Compiler:** The compiler is a highly specialized, single-purpose program invoked by the compiler driver. In the GNU toolchain, the actual C compiler is typically named `cc1`.
    
    - **Function:** Lexical analysis, syntax parsing, semantic analysis, and hardware specific optimizations. It strictly accepts preprocessed source code and outputs assembly code.
        

**The Compilation System Phases:**

1. **Preprocessor Phase:** The preprocessor (`cpp`) modifies the original C program according to directives that begin with the '#' character. For example, the `#include <stdio.h>` command directly tells the preprocessor to read the contents of the system header file `stdio.h` and insert it directly into the program text. The result is another C program, typically with the `.i` suffix.
    
2. **Compilation Phase:** The compiler (`cc1`) translates the text file (modified source code) `hello.i` into the text file `hello.s`, which contains an assembly-language program. Each line of the assembly program describes basically Machine Language in Textual form. Assembly language is the universal middleman between software and computer hardware.
    
    - _The Hardware Only Speaks One Language:_ The processor is physically wired to understand one specific set of electrical instructions. The compilers translate, so essentially Assembly language is those electrical instructions in textual form.
        
3. **Assembly Phase:** Next, the assembler (`as`) translates `hello.s` into machine-language instructions, packages them in a form known as relocatable object program, and stores the result. The Assembler is essentially just a giant lookup table, its entire job is to read the `hello.s` file, see the ASCII text and convert them to binary.
    
4. **Linker Phase:** Our hello program calls the `printf` function, which is part of the standard C library provided by every C compiler. That is why we added the preprocessor directive `#include <stdio.h>` which does include `printf` but note that it doesn't contain the actual implementation of `printf`, it only contains a declaration. The actual implementation resides in a separate precompiled object file called `printf.o`, which must somehow be merged with our `hello.o` program. The linker (`ld`) handles this merging. The result is the `hello` file, which is an executable object file which is ready to be loaded into memory and executed by the system.
    

**File Types Deep Dive (C files vs. Headers):**

- At the hardware level, there's absolutely no difference between each one of these files. So the normal C program (`.c`), the header files (`.h`), and the system header files are also all just plain text files containing ASCII characters. The compiler itself doesn't actually care what the file extension is; you could technically name your file `hello.txt` and tell the compiler to build it. It will build it as long as its valid C syntax.
    
- The difference between these files lies entirely in design conventions and how the preprocessor searches for them.
    
- **The C Program (`.c` file):** This is where you write the actual executable logic. It contains the function definitions, logic, memory allocation, i.e., the actual stuff that runs.
    
- **The Header File (`.h` file):** This is essentially a promise. It doesn't contain the implementations, just declarations — "this function exists, it takes these parameters, it returns this type". When you split a project across multiple `.c` files, headers are how they talk to each other. File A can `#include "file_b.h"` and now knows what functions file B offers, without needing to see how they work.
    

**Normal Header Files vs. System Header Files:** The difference here dictates exactly where the preprocessor looks on your hard drive when it encounters an `#include` directive.

- **Normal Header File (User-Defined):** These are the headers you write for your own projects.
    
    - **Syntax:** You use double quotes: `#include "file_b.h"`
        
    - **Preprocessor Behavior:** The preprocessor searches the current, local directory of your project first. If it doesn't find it there, it moves on to check other directories specified in the compiler flags.
        
- **System Header File:** These are provided by the compiler toolchain, the C standard library or the OS (e.g. `stdio.h`, `stdlib.h`). They provide interfaces that allow you to interact with the OS.
    
    - **Syntax:** You use angle brackets: `#include <stdio.h>`
        
    - **Preprocessor Behavior:** The preprocessor ignores the local directory entirely. It immediately looks in the standard system directories.