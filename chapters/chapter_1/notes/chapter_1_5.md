### **CHAPTER 1.5: CACHES MATTER**

An important takeaway from this simple example is that a system spends a lot of time moving information from one place to another. The machine instructions in the hello program are originally stored on disk. When the program is loaded, they are copied to main memory. As the processor runs the program, instructions are copied from main memory into the processor itself. Similarly, the data string `hello, world\n`, originally on disk, is copied to main memory and then copied from main memory to the display device. From a programmer's perspective, much of this copying is overhead that slows down the "real work" of the program. Thus, a major goal for system designers is to make these copy operations run as fast as possible.

Because of physical laws, larger storage devices are slower than smaller storage devices and faster devices are more and faster devices are more expensive to build than their slower counterparts. For example, the disk drive on a typical system might be 1,000 times larger than the main memory, but it might take the processor 10,000,000 times longer to read a word from the disk than from the main memory. Similarly a typical register file stores only a few hundred bytes of information, as opposed to billions stored on the RAM but the CPU can access the register almost instantaneously but still has to take a lot of time to access data from the main memory -> CPU tightly packed, RAM farther away.

To deal with this processor-memory gap, system designers include smaller, faster storage called CACHE MEMORIES (OR SIMPLY CACHES) that serve as temporary staging areas for information that the processor is likely to need in the near future. An L1 cache on the processor chip holds tens of thousands of bytes and can be accessed nearly as fast as the register file. A larger L2 cache with hundreds of thousands to millions of bytes might take a bit longer to be accessed by the CPU than the L1 cache, but still is much faster than the main memory. Newer and more powerful systems have three levels of cache: L1, L2 and L3.

The idea behind caching is trying to get the best of both worlds - Both a very large memory and a very fast one by exploiting locality - The idea that if a program references a memory location, it is very likely to reference a nearby memory location in the near future (SPATIAL LOCALITY). Another one is the idea that if a memory location is referenced once, it is likely to be referenced again (TEMPORAL LOCALITY). By setting up caches to hold data that are likely to be accessed in the near future, we can perform memory operations using the fast caches. -> Holding data by exploiting locality works on ACCESS GRANULARITY OF THE CACHE SYSTEMS.

**MEMORY HIERARCHY.** _(Diagram text transcribed)_ Smaller, faster, costlier ->

- L1 Cache (Registers hold words retrieved from L1)
    
- L2 Cache (L1 cache holds cache lines retrieved from L2 cache)
    
- L3 Cache (L2 cache holds cache lines retrieved from L3 cache)
    
- Main memory (L3 hold cache lines retrieved from main memory)
    
- Local secondary storage (Main memory holds disk blocks from local disk)
    
- Remote secondary storage Larger, slower, cheaper <-