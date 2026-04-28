***
# CHAPTER 1.7: THE OPERATING SYSTEM MANAGES THE HARDWARE

Back to the hello example. When the shell loaded and ran the hello program, and when the hello program printed its message, neither program accessed the hardware directly. Rather, they relied on the services provided by the operating system. It is reasonable to think of the Operating system as a layer of software interposed between the application program (our hello program, the shell and every other software running on your system) and the hardware as shown in the figure below:

[Application Programs | Operating System] -> Software
[Processor | Main memory | I/O devices] -> Hardware

One question that I asked when reading this was: IF THE OS IS SANDWITCHED BETWEEN THE APPLICATION PROGRAMS AND THE HARDWARE, DOES THAT MEAN EVERYTHING PASSES THROUGH THE OS BEFORE THE CPU? - THE ANSWER IS NO.

So the OS is the only entity that has "the keys to the kingdom" (direct access to the hardware). Your program, on the other hand lives in a sandbox. It can do math on the CPU as fast as it wants, but the moment it needs to touch "real" "real world" - disk, screen or network, it has to stop and ask for permission.

**2. THE TWO WORLDS: USER MODE VS. KERNEL MODE**
The CPU has a physical bit in one of its control registers called THE MODE BIT. This bit determines what the CPU is allowed to do.
- USER MODE (APPLICATION PROGRAM): The mode bit is "restricted" (set, I mean). The CPU can only execute a safe subset of instructions. If the program tries to talk to hardware directly, the CPU triggers A GENERAL PROTECTION FAULT and kills the program.
- KERNEL MODE (THE OS): The mode bit is set to "Privileged". The OS can do anything.

**2. SWITCHING MODES**
Since an application program (like MS word) is in user mode, it physically cannot talk to the disk. To get around this, it uses a system call (syscall).
- THE REQUEST: Your C code calls write()
- THE PROGRAM: Your program executes a special assembly instruction (like SYSCALL). This instruction does two things simultaneously:
1. THE TRAP: Your program executes a special assembly instruction
- It flips the Mode Bit from User to Kernel.
- It jumps the CPU's Program counter to a specific, pre-defined address in the operating system's code.
3. THE EXECUTION: The OS takes over. It checks if you have permission to write to that file. If yes, it talks to the hardware and handles the disk I/O.
4. THE RETURN: Once the OS is done, it executes another special instruction (SYCRET) that flips the Mode Bit back to User and jumps back to the exact next line in the program.

**3. THE CONTEXT SWITCH: THE ILLUSION OF MULTITASKING**
If a program is running, the OS doesn't just sit there and wait for the program to finish. Every few milliseconds, a Hardware Timer (a physical chip on the motherboard) sends an electrical signal to the CPU.
- This "Timer Interrupt" forces the CPU to stop running your program and jump into the OS.
- The OS saves your registers (the "Context") to memory.
- It then loads the registers of another program (like your browser) - It loads the registers from memory cause it already saved them to memory.
- It switches the CPU back to User Mode and lets the browser run for a bit.
This happens so fast (thousands of times per second) that you get the illusion that every program is running at the same time.

So the answer to the question is that: Every single instruction inside the CPU's instruction set Architecture is categorised into Privileged and Safe. The safe ones are the ones anyone can use, the Privileged are reserved for the kernel (Ring 0). So in the FETCH -> DECODE -> EXECUTE CYCLE. During the Decode Cycle, The CPU compares the instruction's privilege requirement with the current execution mode, if they're the same execution takes place, if not, the CPU swaps directly to kernel mode and the OS sends a signal, it then kills the Program.

The operating system has two primary purposes: (1) To protect the hardware from misuse by runaway applications and (2) To provide applications with simple and uniform mechanisms for manipulating complicated and often wildly different low-level hardware devices. The OS achieves this by using three fundamental abstractions: Processes, Virtual memory and files.

**Abstractions provided by the OS.**
Files are abstractions for I/O devices, to the program, everything is a stream of bytes. Virtual memory is an abstraction for RAM + disk. It creates the illusion of infinite memory by using RAM as cache for disk storage. It gives your program a private map of memory making it look like your program owns the memory.
Processes (Abstract everything). It's essentially the program's sandbox, where it believes it owns the hardware.

***

# CHAPTER 1.7.1: PROCESSES

When a program such as hello runs on a modern system, the operating system provides the illusion that the program is the only one running on the system. The program appears to have exclusive use of both the processor, main memory, and I/O devices. The processor appears to execute the instructions in the program, one after the other without interruption. And the code and data of the program appear to be the only objects in the system's memory. These illusions are provided by the notion of a process (PROCESS).

A process is an operating system's abstraction for a running program - The sandbox of the program - Where the program runs with the illusion of Virtual Memory and Files and also Exclusive use of Processor.
Multiple processes can run concurrently on the same system and each process appear to have exclusive use of the hardware but by concurrently we mean that the instructions of the programs/processes are interleaved with the instructions of another process - CONTEXT SWITCHING.

Traditional systems could only execute one program/process at a time, these are called UNIPROCESSOR SYSTEMS while newer MULTI-CORE PROCESSOR SYSTEMS can execute several programs simultaneously - meaning each CPU core works on a separate program/process. In either case, a single CPU can appear to execute multiple processes concurrently by having the processor switch among them, this is done by the Operating System through CONTEXT SWITCHING.

A CONTEXT is essentially all the state information that a process needs to run. This state information includes information such as THE CURRENT VALUES OF THE PC, THE REGISTER FILE, AND THE CONTENTS OF MAIN MEMORY.

Let's use our hello program as example: The shell process and the hello process. Initially, the shell process is running alone, waiting for input on the command line. When we ask it to run the hello program, the shell carries out our request by invoking a special function known as a system call that passes control to the OS. The OS saves the shell's context, creates a new process and its context, and then passes control to it (this control might also be suspended in reality). After hello terminates, the OS passes control back to the shell by loading its context from memory.

NOTE: The Operating System is not a process that needs to be scheduled, the question that arrived for this is that: IS THE OS ALSO A PROCESS THAT NEEDS TO BE CONTEXT SWITCHED FOR IT TO HAVE CONTROL? NO.
The OS doesn't need to be context switched in the same way as a process, instead it uses a physical hardware clock built into the motherboard which I explained in chapter 1.7.

***

# CHAPTER 1.7.2: THREADS

**PART 1: ARCHITECTURE OF A THREAD**
To understand threads, you must completely separate the concept of "memory" from the concept of "execution".
A Process is essentially just a memory container (a virtual warehouse). It holds your compiled code, your global variables, and your heap. A process does not actually do anything.
A Thread is the fundamental unit of execution, it's the active "worker" inside the process (memory container) that actually steps through the code and performs math on the CPU. Every program starts with one main thread but can spawn thousands more inside the exact same memory container.

**THE THREAD BLUEPRINT**
To execute C code, a thread requires its own hardware state. The OS guarantees every single thread possesses its own private, isolated copy of three things:
1. THE PROGRAM COUNTER (PC): A physical hardware register pointing to the exact memory address of the C instructions this specific thread is currently executing.
2. THE CPU REGISTERS: The hardware scratchpad used to perform immediate and logic operations.
3. THE STACK: A strictly private block of memory used to store its local variables. Thread A cannot read Thread B stack.
- SHARED MEMORY:
Because all threads spawned by a program live inside the exact same Process container, they perfectly share the rest of the architecture - THE CODE, THE GLOBALS & THE HEAP.

**CONTEXT SWITCHING MECHANISM**
Modern Operating Systems do not schedule processes. The kernel scheduler only sees a massive, flat queue of threads (called task_structs in Linux) waiting for CPU time. If we're working with a uniprocessor system, there's only one physical hardware Program counter, the CPU can only execute one thread at a time. CONTEXT SWITCHING COMES INTO PLAY TO CREATE THE ILLUSION OF MULTIPLE THREADS RUNNING AT ONCE.

**THREAD SWITCH VS PROCESS SWITCH**
When the kernel scheduler picks the next thread in the queue, the CPU either has to perform a "heavy switch" or a "light switch". This is the exact difference between A THREAD CONTEXT SWITCH AND A PROCESS CONTEXT SWITCH.

1. THE THREAD CONTEXT SWITCH:
- THE SCENARIO: The kernel swaps from Thread 1 of a program to Thread 2 of the same program
- THE ACTION: The kernel notices that they live in the same memory container.
The kernel leaves the Virtual memory map completely alone. It simply swaps out the math register and the Program counter.
Same memory container means leaving the cache lines alone - Fast.

2. THE PROCESS CONTEXT SWITCH:
- THE SCENARIO: The kernel swaps thread 1 of a program to the main thread of another program. The kernel notices that this new thread belongs to a new memory map.
Inside the CPU is a register called CR3 that points to the current virtual memory map. The CPU must overwrite this register to point to the new program's memory map.
THE CACHE PENALTY: Overwriting the CR3 register triggers a hardware flush of critical cache called TLB (TRANSLATION LOOKASIDE BUFFER). The CPU instantly throws away all of its memory address translations, overwriting it with new ones; expensive. And also it has to pull new data from the program's memory space to overwrite its cache lines.

**CONCURRENCY VS PARALLELISM**
- CONCURRENCY (UNIPROCESSOR): On a single core machine, context switching creates the illusion of simultaneous execution CONTEXT SWITCHING.
- PARALLELISM (MULTIPROCESSOR): On modern systems with multiple cores, there are multiple physical hardware program counters. The OS can assign multiple different PCs to multiple different threads and each CPU can do context switching.
