# 🖥️ Computer Systems – Complete Notes

> [!info] About These Notes
> Transcribed from handwritten notes covering OS abstractions, processes, threads, memory, networks, and performance.

---

## 📑 Table of Contents

- [[#Chapter 1.6 – Storage Devices Form A Hierarchy]]
- [[#Chapter 1.7 – The Operating System Manages The Hardware]]
  - [[#Chapter 1.7.1 – Processes]]
  - [[#Chapter 1.7.2 – Threads]]
  - [[#Chapter 1.7.3 – Virtual Memory]]
  - [[#Chapter 1.7.4 – Files]]
- [[#Chapter 1.8 – Systems Communicate With Other Systems Using Networks]]
- [[#Chapter 1.9.1 – Amdahl's Law]]
- [[#Chapter 1.9.2 – Concurrency And Parallelism]]

---

## Chapter 1.6 – Storage Devices Form A Hierarchy

> [!abstract] Core Idea
> Every storage device in a computer system forms a **memory hierarchy**. As we move from the top to the bottom of the hierarchy, devices become **slower, cheaper, and less costly per byte**.

The register file occupies the **top level** of the hierarchy (Level 0), then cache levels L1–L3, then main memory (Level 4), and so on.

### The Main Idea – Caching

> [!tip] Key Principle
> **Storage at one level serves as a cache for storage at the next lower level.**
> So **Level k** serves as a **cache for Level k+1**.

- Level k essentially acts as **temporary storage** for data at level k+1.
- This determines **temporal and spatial locality**, which we've explained elsewhere.
- Thus, the **register file** is a cache for the **L1 cache**.
- **L1 and L2** are caches for **L3** respectively.
- **L2 and L3** are caches for **Level L4** (main memory).

This entire system works on **Access Granularity** — the logic works **only because we can move data in chunks**, never in single bytes.

### Cache Layout (Per Core)

| Level | Description |
|-------|-------------|
| L1 Cache | Private to each CPU core |
| L2 Cache | Each core has its own L2 cache |
| L3 / Shared Memory | All physical cores on the chip share this + RAM |

### The L5 Cache Analogy

The **L5 cache** is a cache for main memory, which is a cache for the disk. On some networked systems with distributed file systems, the **local disk serves as a cache for data stored on remote disks** (like your Google Drive).

> [!note]
> Just as programmers can exploit knowledge of different caches to improve performance, programmers can exploit their understanding of the **entire memory hierarchy** → See **Chapter 6**.

---

## Chapter 1.7 – The Operating System Manages The Hardware

When the shell loaded and ran the hello program, and when hello printed its message, neither program accessed the hardware directly. They **relied on the services provided by the operating system**.

> [!quote] Mental Model
> Think of the OS as a **layer of software interposed** between the application program and the hardware.

```
┌─────────────────────────────┐
│     Application Programs    │  ← Software
│     Operating System        │
├─────────────────────────────┤
│  Processor | RAM | I/O Devs │  ← Hardware
└─────────────────────────────┘
```

> [!question] Common Question
> *If the OS is sandwiched between the Application Programs and the Hardware, does that mean everything passes through the OS before the CPU?*
> **The Answer is NO.**

The OS is the only entity that has **"the keys to the kingdom"** (direct access to hardware). Your program lives in a **sandbox** — it can do math on the CPU as fast as it wants, but the moment it needs to touch the "real world" (disk, screen, network), it must **stop and ask for permission**.

### OS Primary Purposes

The OS has two primary purposes:
1. **To protect the hardware** from misuse by running applications.
2. **To provide applications** with simple and uniform mechanisms for manipulating I/O — complicated and often wildly different low-level hardware devices.

The OS achieves this using three fundamental abstractions:

```
Virtual Memory  ──┐
                  ├──► { Processor → Process }
Files           ──┘     { Main Memory + Files → Virtual Memory }
                        { I/O Devices → Files }
```

---

### 1. The Two Worlds: User Mode vs. Kernel Mode

The CPU has a physical control register called **The Mode Bit**. This bit determines what the CPU is allowed to do.

> [!warning] User Mode (Application Programs)
> The mode bit is **"restricted"**. The CPU can only execute a **safe subset** of instructions. If the program tries to talk to hardware directly, the CPU triggers a **General Protection Fault** and kills the program.

> [!success] Kernel Mode (The OS)
> The mode bit is set to **"Privileged"**. The OS can do **anything**.

### 2. Switching Modes (System Calls)

Since an application program (like MS Word) is in user mode, it **physically cannot talk to the disk**. To get around this, it uses a **system call (syscall)**.

**The sequence:**
1. **The Request:** Your C code calls `write()`
2. **The Trap:** Your program executes a special assembly instruction (like `SYSCALL`). This does two things simultaneously:
   - It **flips the Mode Bit** from User → Kernel
   - It **jumps the CPU's Program Counter** to a specific, pre-defined address in the OS's code
3. **The Execution:** The OS takes over. It checks if you have permission to write to that file. If yes, it talks to the hardware and handles the disk I/O.
4. **The Return:** Once the OS is done, it executes another special instruction (`SYSRET`) that flips the Mode Bit back to **User** and jumps back to the exact next line in the program.

---

### Chapter 1.7.1 – Processes

> [!abstract] Definition
> A **process** is an operating system's abstraction for a running program — the sandbox of the program — where the program lives with the illusion of Virtual Memory, Files, and exclusive use of the Processor.

When a program such as `hello` runs, the OS provides the illusion that:
- It is **the only program running** on the system
- It has **exclusive use** of the processor, main memory, and I/O devices
- Its code and data are **the only objects** in the system's memory

These illusions are provided by the notion of a **process**.

Multiple processes can run concurrently on the same hardware, but by *concurrently* we mean that the **instructions of processes are interleaved** with each other.

#### Context Switching – The Illusion of Multitasking

> [!example] How It Works
> If a program is running, the OS doesn't just sit there and wait for it to finish. Every few milliseconds, a **hardware timer** (a physical chip on the motherboard) sends an electrical signal to the CPU.

**The sequence:**
- This **"Timer Interrupt"** forces the CPU to stop running your program and jump into the OS.
- The OS **saves your registers** (the "Context") to memory.
- It then **loads the registers** of another program (like your browser) from memory.
- It **switches the CPU back to User Mode** and lets the browser run for a bit.
- This happens so fast (thousands of times per second) that you get the **illusion that every program is running at the same time**.

#### What Is A Context?

A **context** is essentially all the state information that a process needs to run. This includes:
- The current values of the **Program Counter (PC)**
- The **Register File**
- The contents of **Main Memory**

#### Traditional vs. Multi-Core Systems

| System Type | Behavior |
|------------|----------|
| **Uniprocessor Systems** | Can only execute one program/process at a time |
| **Multi-Core Processor Systems** | Can execute several programs simultaneously — each CPU core works on a separate program/process |

In either case, a single CPU can *appear* to execute multiple processes concurrently by having the processor **switch among them** — this is done by the OS through **Context Switching**.

#### Hello World – Context Switch Example

1. Shell process is running alone, waiting for input
2. User asks to run the hello program
3. Shell invokes a **system call**, passing control to the OS
4. OS **saves the shell's context**, creates a new process and its context
5. OS passes control to the hello process
6. After hello terminates, the OS **passes control back to the shell** by loading its context from memory

> [!note] Important
> **Is the OS also a process that needs to be context switched?** **No.**
> The OS doesn't need to be context switched like a process. Instead, it uses a **physical hardware clock** built into the motherboard (the timer interrupt explained above).

#### Every Single CPU Instruction – The Privilege Check

Every single instruction inside the CPU's execution set architecture is categorized into **Privileged** and **Safe**.
- The **safe ones** can be executed in User Mode (Ring 3)
- The **privileged ones** are reserved for the kernel (Ring 0)

During the **Fetch → Decode → Execute Cycle**, at the **Decode** stage, the CPU compares the instruction's privilege requirement with the current execution mode. If they match, execution takes place. If not, the **CPU jumps directly to kernel mode** and the OS sends a signal — killing the program.

---

### Chapter 1.7.2 – Threads

#### Part 1: Architecture of a Thread

> [!important] Key Separation
> To understand threads, you must **completely separate** the concept of **"memory"** from the concept of **"execution"**.

**A Process** is essentially just a **memory container** (a virtual warehouse). It holds your compiled code, your global variables, and your heap. **A process does not actually do anything.**

**A Thread** is the **fundamental unit of execution** — it's the active "worker" inside the process (memory container) that actually steps through the code and performs math on the CPU.

> Every program starts with **one main thread** but can spawn **thousands more** inside the exact same memory container.

#### The Thread Blueprint

To execute C code, a thread requires its own **hardware state**. The OS guarantees every thread possesses its own private, isolated copy of **three things**:

1. **The Program Counter (PC):** A physical hardware register pointing to the exact memory address of the C instruction this specific thread is currently executing.
2. **The CPU Registers:** The hardware scratchpad used to perform immediate and logic operations.
3. **The Stack:** A strictly private block of memory used to store its local variables. *Thread A cannot read Thread B's stack.*

#### Shared Memory

Because all threads spawned by a program live inside the **exact same Process Container**, they perfectly share the rest of the architecture:
- **The Code**
- **The Globals**
- **The Heap**

#### Context Switching Mechanism

Modern Operating Systems do **not** schedule processes. The kernel scheduler only sees a **massive, flat queue of threads** (called `task_structs` in Linux) waiting for CPU time.

#### Thread Switch vs. Process Switch

When the kernel scheduler picks the next thread in the queue, the CPU either performs a **"heavy switch"** or a **"light switch"**.

**1. The Thread Context Switch (Light – Same Program)**
- **Scenario:** The kernel swaps from Thread 1 of Program A to Thread 2 of **the same program**.
- **Action:** The kernel notices they live in the **same memory container**. It leaves the Virtual Memory map completely alone. It simply **swaps out the math registers and the Program Counter**. Same memory container means **leaving the cache lines alone** → Fast!

**2. The Process Context Switch (Heavy – Different Program)**
- **Scenario:** The kernel swaps Thread 1 of a program to the main thread of **another program**. The kernel notices this new thread belongs to a **new memory map**.
- **The CR3 Register:** Inside the CPU is a register called `CR3` that points to the current virtual memory map. The CPU must **overwrite this register** to point to the new program's memory map.
- **The Cache Penalty:** Overwriting `CR3` triggers a hardware flush of a critical cache called the **TLB (Translation Lookaside Buffer)**. The CPU instantly throws away **all of its memory address translations**, overwriting with new ones — **expensive**. It also has to pull new data from the program's memory space to override its cache lines.

#### Hyperthreading

**Hyperthreading** is Intel's proprietary marketing name for a hardware architecture called **Simultaneous Multithreading (SMT)**. A single CPU core can execute instructions from two threads at once.

#### Concurrency vs. Parallelism

| Concept | Definition |
|---------|-----------|
| **Concurrency (Uniprocessor)** | On a single core machine, context switching creates the *illusion* of simultaneous execution. Two tasks make progress but are **never truly executing at the same nanosecond**. |
| **Parallelism (Multiprocessor)** | With multiple cores, there are multiple physical hardware program counters. The OS can assign different PCs to different threads. Each CPU can do context switching. Truly **executing at the same time**. |

> [!warning] Key Distinction
> **Parallelism cannot always achieve Concurrency** — some tasks cannot be made parallel.
> You can have Concurrency without Parallelism (uniprocessor), but Parallelism implies Concurrency.

---

#### Instruction Level Parallelism

CPU cores can execute instructions simultaneously using several techniques:

**1. Pipelining (Assembly Line)**

Using a laundry analogy:
- **Sequential (No Pipelining):** Washer → Dryer → Folder. Wait for load 1 to finish before starting load 2. Dryer and Folder sit **completely idle** while Washer runs.
- **Pipelined:** While load 1 is in the Dryer, load 2 is in the Washer, load 3 is being Folded — all **simultaneously**. This is exactly what the CPU does with instructions.

**2. Superscalar Execution**

Modern processors get close to **one instruction per cycle**, but can actually execute **2–4 IPC (Instructions Per Cycle)**. This is called **Superscalar Architecture**.

**3. SIMD – Single Instruction, Multiple Data**

Use **large registers** to hold data, then use **one instruction** to perform operations on that large amount of data simultaneously.

---

### Chapter 1.7.3 – Virtual Memory

> [!abstract] Definition
> **Virtual Memory** is an abstraction that provides each process with the **illusion** that it has **exclusive use of the main memory**.

Each process has the same uniform view of memory, known as its **Virtual Address Space**. The virtual address space for Linux processes is:

#### Virtual Address Space Layout (Bottom → Top)

| Region | Description |
|--------|-------------|
| **Program Code & Data** | Code begins at the same fixed address for all processes, followed by data locations corresponding to global C variables. Initialized directly from the binary executable. |
| **Heap** | Follows the code/data areas. Expands and contracts **dynamically at runtime** as a result of calls to `malloc` and `free`. |
| **Shared Libraries** | Near the middle of the address space. Holds code and data for shared libraries such as the C standard library and the math library. |
| **Stack** | At the top of the **user's** virtual address space. The compiler uses this to implement function calls. Expands and contracts dynamically as stack frames are created and popped. |
| **Kernel Virtual Memory** | The **top region** — reserved for the kernel. Application programs are **not allowed** to read or write this area or call kernel functions directly. They must invoke the kernel via **Mode Bit / syscall**. |

#### The Page Table Trick

> [!question] Does Every Process Have A Copy of the Kernel Code?
> **No.** The OS Kernel is **loaded into Physical RAM exactly once** when the computer first boots up. It **never gets duplicated**.

Every single process gets its own **private Page Table**. The OS builds the Page Table for each new process and splits it into two parts:

**User Mode (The Bottom):**
- The OS finds empty, unused blocks of physical RAM and maps the bottom of the virtual address space to those unique blocks.
- Process A's user space → Physical RAM Block 20
- Process B's user space → Physical RAM Block 50

**Kernel Space (The Top):**
- For the top section, the OS **hard-codes** the page table to point to the **exact same physical address** for every single process.
- Two major reasons:

| Reason | Explanation |
|--------|-------------|
| **Saving RAM** | The Linux kernel is massive. Duplicating it into physical RAM for every running program would exhaust memory almost instantly. |
| **Speed** | The `CR3` register doesn't need to get flushed because the kernel's address is literally already in `CR3`. The CPU just flips the Mode Bit to kernel mode and jumps — **no TLB flush needed**. |

#### Multiple Processes Accessing the Kernel Simultaneously

> [!question] What happens in a multiprocessor system where multiple threads try to access the kernel at the same time?

If both processes are just **reading** data → everything is fine.

If they both try to **write** at the exact same nanosecond → **everything gets corrupted**.

This was fixed in **Phases**:

**Phase 1: The Big Kernel Lock (BKL) – Brute Force**
- A software padlock on the kernel.
- If Core 0 executed a system call and entered the kernel, it **locked the door**.
- If Core 1 tried to execute a system call, it hit the locked door and was **forced to wait** for Core 0 to finish.
- **The Flaw:** If Core 0 was inside the kernel doing heavy I/O, Core 1 was forced to wait even for completely unrelated operations. The multiprocessor system degraded into a **uniprocessor system**.

**Phase 2: Fine Grained Locking**
- Kernel developers spent years systematically ripping out the BKL and replacing it with **Fine Grained Locks**.
- Instead of locking the whole kernel, the OS now puts **thousands of tiny, specific locks** on individual data structures.
- Lock for network, lock for memory allocator, etc.
- Now if Core 0 locks the memory allocator, Core 1 can **still access the network queue**.

**How Does A CPU Actually Wait? (Spinlock)**

When a core hits a locked resource, the OS decides: **Context Switch or Stay Idle?**

The kernel uses the most aggressive, bare-metal synchronization: **The Spinlock**.

```c
while (lock_is_held) { /* do nothing */ }
```

Core 1 sits there checking the lock address **millions of times per second**. The absolute **nanosecond** Core 0 releases the lock, Core 1 grabs it and takes over again.

---

### Chapter 1.7.4 – Files

> [!abstract] Definition
> **A file is essentially a sequence of bytes** — nothing more, nothing less.

Every I/O device — including disks, keyboards, displays, and even **networks** — is modeled as a file for our programs. All input and output in the system is performed by **reading and writing files**, using a small set of system calls known as **Unix I/O**.

> [!tip] Why This Is Powerful
> This simple and elegant notion of a file provides application programs with a **uniform view of all varied I/O devices**. Application programmers who manipulate the contents of a disk file are blissfully unaware of the specific disk technology — they just write to the "file" and the OS handles the underlying hardware translation.

---

## Chapter 1.8 – Systems Communicate With Other Systems Using Networks

Up to this point, we have treated a system as an isolated collection of hardware and software. In practice, modern systems are often **linked to other systems by networks**.

From the point of view of an individual system, the **network can be viewed as just another I/O device**. When the system copies a sequence of bytes from main memory to the network adapter, the data flows across the network to another machine.

With the advent of global networks such as the **Internet**, copying information from one machine to another has become one of the **most important uses of a computer system**.

### What Is A Protocol?

> [!abstract] Definition
> At the bare metal, a protocol is simply an **agreed-upon struct and a switch statement shared by two completely blind machines**.

When your **Network Interface Card (NIC)** receives electrical pulses over a fiber optic cable, it translates those pulses into raw bytes and drops them onto the heap. The CPU then faces a massive problem:

> *What exactly is that sequence of bytes? What do I read it as?*

**The hardware is blind. It has no idea.** A protocol is the **strict, mathematically defined rulebook** that tells:
- The **receiving machine** exactly how to read that block of memory
- The **sending machine** exactly how to package data before sending

A protocol typically has **three rules/components**:

**1. Syntax (The Memory Layout)**
The physical format of the bytes. Example layout:
- `Byte 0`: Message Type (Enum: 1 for text, 2 for command)
- `Bytes 1–4`: Payload Length (how many bytes to read next)
- `Bytes 6+`: The actual data

In C code, a message sent over a network is the **exact struct** — you know precisely how to read the message.

**2. Semantics (The Execution Logic)**
The **meaning** behind the bytes. If the Message type is 1, do something; if it's a command, do something else. This is essentially what separates commands from raw text.

**3. Sequence (The State Machine)**
The **legal order of operations**. Two machines cannot just start sending bytes:
- Rule 1: The Client must send a "Hello" byte **(SYN)**
- Rule 2: The Server must respond with an acknowledgement byte **(ACK)**
- Rule 3: Only then can the Client send the payload. If the server receives a payload without a "Hello" first, the protocol is violated.

### Telnet

Telnet is a simple **mutual contract** between the Client (receiving machine) and the Server (sending machine). It dictates that any byte arriving on that specific network connection must strictly be parsed as either **standard ASCII text** or a **predefined control signal**.

At its core, Telnet is essentially a **bidirectional pipe for unformatted, 8-bit ASCII text**. But from a low-level perspective, there is **In-Band Signalling** — Telnet uses the same channel for both data and control.

**The Raw Text Stream:**
If you type `ls -l`, Telnet packages those exact ASCII bytes and sends them over the network. The remote server reads them, executes the command, and streams the output back.

**The Control Sequences:**
If you press `Ctrl+C` while running a program remotely, your OS intercepts the keystroke and generates a raw interrupt byte (usually `0x03`). Telnet takes it and sends it over the wire. The remote Telnet server recognizes this as a strict command to send a **SIGINT (Signal Interrupt)** directly to the OS kernel to kill the running process.

> This is **In-Band Signalling** — mixing data (text) and control signals in the **exact same channel**.

---

## Chapter 1.9.1 – Amdahl's Law

> [!abstract] Core Concept
> **Gene Amdahl** made a simple but insightful observation: when we speed up **one part** of a system, the effect on the **overall system performance** depends on both **how significant this part was** and **how much it sped up**.

### The Formula

Consider a system where executing an application requires time **T_old**.
Suppose some part requires a fraction **α** of this time, and we improve it by a factor **k**.

$$T_{new} = (1-\alpha) \cdot T_{old} + \frac{\alpha \cdot T_{old}}{k} = T_{old}\left[(1-\alpha) + \frac{\alpha}{k}\right]$$

**Speedup S = T_old / T_new:**

$$S = \frac{1}{(1-\alpha) + \frac{\alpha}{k}}$$

### Example

A part of the system initially consumed **60% of the time** (α = 0.6) and is sped up by a factor of **3** (k = 3):

$$S = \frac{1}{(1-0.6) + \frac{0.6}{3}} = \frac{1}{0.4 + 0.2} = 1.67\times$$

Even though we significantly improved a **major part** of the system, our net speedup was only **1.67×**.

### The Insight

> [!important] Amdahl's Law Insight
> **Even though a component of a system is critical to the execution of a program, optimizing that part alone does not equal a significant/critical increase in system performance.**
> To achieve an overall faster machine, **the parts left unoptimized should be kept to the barest minimum.**

### Problem 1 – Truck Driver Analogy

**Setup:** A truck driver carries potatoes from Boise, Idaho to Minneapolis, Minnesota — **2,500 km** total. Average speed: **100 km/hr**. Total time: **25 hrs**.

Montana abolishes its speed limit — **1,500 km** of the trip. Truck can travel at **150 km/hr**.

**Solution:**
- Montana = 1,500 km → **α = 1500/2500 = 0.6** (60% of trip)
- Montana at 150 km/hr → takes **10 hrs** instead of 15 hrs → **k = 1.5**

$$S = \frac{1}{(1-0.6) + \frac{0.6}{1.5}} = \frac{1}{0.4 + 0.4} = \frac{1}{0.8} = 1.25\times$$

**Part B:** How fast must we travel through Montana to achieve an **overall speedup of 1.67×?**

$$1.67 = \frac{1}{0.4 + \frac{0.6}{k}}$$

Solving: **k ≈ 3** → Must travel through Montana in **5 hours**.

### Problem 2 – Software Performance

**Setup:** The marketing department promised customers a **2× performance improvement**. Only **80%** of the system can be improved (α = 0.8). How much must this part be improved?

$$2 = \frac{1}{(1-0.8) + \frac{0.8}{k}} = \frac{1}{0.2 + \frac{0.8}{k}}$$

Solving: **k ≈ 2.67**

### Special Case: k = ∞

What if we could optimize a part to take **zero time** (k = ∞)?

$$S = \frac{1}{(1-0.6) + \frac{0.6}{\infty}} = \frac{1}{0.4} = 2.5\times$$

> Still only **2.5× speedup** even though we reduced 60% of the work to zero — showing the ceiling imposed by the remaining 40%.

---

## Chapter 1.9.2 – Concurrency And Parallelism

> [!quote] Historical Drivers
> Throughout the history of digital computers, two demands have been constant forces in driving improvements:
> **We Want Them To Do More, And We Want Them To Run Faster.**
> Both factors improve when the processor does more things at once.

### Concurrency

> [!abstract] Definition
> **Concurrency** is about **dealing with multiple things at once**. In a purely concurrent system, two tasks are making progress at the same time but they are **never executing simultaneously at the exact same nanosecond**.

### Parallelism

> [!abstract] Definition
> **Parallelism** is about **doing multiple things at once** — truly executing at the **exact same time**. Requires multiple CPU cores or processors.

### Processors Cannot Always Achieve Concurrency

True parallelism requires multiple CPU cores or processors. Some tasks however **cannot be made parallel** — Parallelism Cannot Always Achieve Concurrency.

> You can have Concurrency on a single core (via context switching), but Parallelism on a multiprocessor system implies true simultaneous execution.

### And The Process Design of Multiprocessor Systems

A **Multiprocessor System** places multiple database processing engines running concurrently under the control of a single OS kernel. This is the physical realization of Parallelism.

---

*Notes end here. Tags: #OS #ComputerSystems #Processes #Threads #VirtualMemory #Networks #AmdahlsLaw #Parallelism*
