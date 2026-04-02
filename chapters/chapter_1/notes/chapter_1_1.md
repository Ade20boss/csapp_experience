### Chapter 1: A Tour of Computer Systems

**General Overview Notes**
* A computer system essentially consists of hardware and systems software that work together to run application programs.
* Specific implementations of systems change over time, but the underlying concepts do not. 
* Main focus is to understand how these components work and how they appear to the programmer.
* **Performance of these programs:**
    * Avoid strange numerical errors caused by the way computers represent numbers.
    * Optimizing C code using clever tricks that exploit designs of modern processors and memory systems.
    * How the computer works - Avoid security buffer overflows.
    * Recognize and correct linking errors.
    * Create Unix shell, dynamic storage allocation package, web server.
    * Concurrency.

---

### 1.1 Tracing the Lifetime of the `hello.c` Program

From the time it is created by the programmer, until it runs on a system, prints its message and terminates.

**Code:**
```c
#include <stdio.h>
int main() {
    printf("hello, world\n");
    return 0;
}
```

**Information is Bits + Context**
* Each image of ASCII is text. 
* The `hello.c` program begins life as a source program (or source file) that the programmer creates with an editor and saves in a text file called `hello.c`. 
* The source code is a sequence of bits, each with a value of 0 or 1, typically organized in bytes. Each byte represents some text character in the source program. Most modern computer systems rep. text characters using ASCII (American Standard Code for Information Interchange) standard.
* The `hello.c` program is stored in a file as a sequence of bytes. Each byte has an integer value that corresponds to some ASCII character. For example, the first byte in the `hello.c` program "#" has an integer value 35, the second byte has an integer value 105, which corresponds to the character "i", and so on. Each text line is terminated by the invisible newline character `\n`, which is represented by the integer value 10. 
* Files such as `hello.c` that consist exclusively of ASCII characters are known as **text files**. All other files are known as **binary files**.
* The representation of `hello.c` illustrates a fundamental idea: All information in a system, including disk files, programs stored in memory, user data stored in memory, and the data transferred across a network is represented as a bunch of bits (bytes). 
* The only thing that distinguishes different data objects is the context in which we view them. For example, in different contexts, the same sequence of bytes might represent an integer, float, string, or even machine instruction.

**Why was C successful?**
* C was developed from the beginning as the system programming language for the Unix operating system - Most of the Unix kernel and all of its supporting tools and libraries were written in C. As Unix became popular, many people were exposed to C and since Unix was almost entirely written in C, it could be easily ported to new machines.
* C is a small, simple language. Since the design was controlled by a single person, rather than a committee, and the result was a clean, consistent design, C was easy to learn.
* C was designed for a practical purpose.
* C is the language of choice for system-level programming and there is a huge installed base of application-level programs as well. 
* However, it is not perfect for all programmers and all situations. C pointers are a common source of confusion and programming errors. It also lacks explicit support for useful abstractions such as classes, objects and exceptions.
