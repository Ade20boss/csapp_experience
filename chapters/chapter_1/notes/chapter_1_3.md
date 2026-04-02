# 🖥️ Understanding How Compilation Systems Work

> For simple programs such as `hello.c`, we can rely on the compilation system to produce correct and efficient machine code. However, there are important reasons why programmers need to understand how compilation systems work.

---

## 🔄 The Compilation Pipeline

![[compilation_pipeline.svg]]

> The compiler is not just a translator — it **architects how your program's memory will be laid out at runtime**. This is why understanding it matters beyond just writing code.

---

## 📌 Why Programmers Need to Understand Compilation Systems

### 1. ⚡ Optimizing Program Performance

Modern compilers are sophisticated and they usually produce good code. As programmers, we don't necessarily need to understand the inner workings of compilers — but it is imperative that we have a **basic understanding** of these compilers in order to make good coding decisions.

**Key questions to consider:**
- Is a `switch` statement always more efficient than an `if-else` sequence?
- How much overhead is incurred by a function call?
- Is a `while` loop more efficient than a `for` loop?
- Why does our loop run faster if we save into a **local variable** instead of an assignment passed as reference?

> 📖 **Chapter 3** introduces x86-64.  
> 📖 **Chapter 5** helps us understand how to optimize C code by making simple transformations.  
> 📖 **Chapter 6** is about memory and how we can use this knowledge to run code more efficiently.

---

### 2. 🔗 Understanding Link Time Errors

Some of the most puzzling errors in systems software development are associated with the **linker**. Understanding the compilation system helps us understand how the linker works and the errors one might encounter.

**Key questions:**
- What does it mean when the linker reports that it cannot resolve a reference?
- What is the difference between a **static variable** and a **global one**?
- What happens if you define two global variables in different C files with the same name?
- Why does it matter the order we list libraries on the command line?

> 📖 **Chapter 7**

---

### 3. 🔐 Avoiding Security Holes

**Buffer Overflow vulnerabilities** have accounted for many security holes. These vulnerabilities happen because only few programmers understand the need to **restrict the quantity and forms of data** they accept from untrusted sources.

---

## 💥 What Is a Buffer Overflow?

Essentially, a buffer overflow happens when the **data stored on a container** (or meant to be stored) **exceeds the container** (e.g., a variable). When this happens, the **adjacent memory block gets overwritten** by the remainder.

> ⚠️ This on itself isn't necessarily a vulnerability — an attacker doesn't automatically take over the system just by causing an overflow.

So, you don't necessarily need to understand how compilers work to understand buffer overflows. What you need to understand is **how information is stored or represented in computers**.

Now, moving past just understanding and actually *using* this knowledge to take over the system — you need to understand the **compilation system**, because it is the compiler that **architects the memory of your program**.

---

## 🪝 Basic Stack Hijack

When a function is called, the compiler creates a **stack frame** for it *(basically a sandbox for the function)* and then pushes **two things** to this stack:

1. The **local variables**
2. The **return address**

![[buffer_overflow.svg]]

Since the return address tells the CPU where to jump back to after function execution, all the attacker needs to do is:

> Overflow the local variables container by the **exact number of bytes** of the address of their malicious program → the CPU **blindly starts the malicious program** after function execution.

---

## 🛡️ Defensive Compiler Methods

The second reason you might want to learn about the compilation system is to learn about **defensive compiler methods** like **Stack Canaries**.

![[stack_canary.svg]]

- The compiler inserts a **secret random value** (the "canary") between the local variables and the return address.
- Before the function returns, it **checks if the canary is still intact**.
- If an attacker overflows the buffer, the canary gets corrupted → the program **aborts immediately**, before the hijacked return address is ever used.
