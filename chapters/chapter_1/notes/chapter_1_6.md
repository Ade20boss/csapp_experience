# CHAPTER 1.6: STORAGE DEVICES FORM A HIERARCHY

The storage devices in every computer system form an hierarchy (a memory hierarchy) similar to the figure above. As we move from the top of the hierarchy to the bottom, the devices become slower, larger, and less costly per byte. The register file occupies the top level in the hierarchy, Level 0, then the 3 levels of caching, occupying the hierarchy levels 1-3. Main memory occupies level 4, and so on.

The main idea behind memory hierarchy is that STORAGE AT ONE LEVEL SERVES AS A CACHE FOR STORAGE AT THE NEXT LOWER LEVEL, SO LEVEL k ACTS AS A CACHE FOR LEVEL k+1.

So essentially level k acts as temporary storage for data at level k+1. It determines importance through locality (spatial and temporal locality) which we've explained.
Thus, the register file is a cache for the L1 cache. Caches L1 and L2 are caches for L2 and L3 respectively.
locality works on ACCESS GRANULARITY OF THE CACHE. The L1 cache is a cache for the main memory, which is a cache for the disk. On some networked systems with distributed file systems, the local disk serves as a cache for data stored on the disks of other systems (essentially remote disks like your google drive).

Just as programmers can exploit knowledge of different caches to improve performance, programmers can exploit their understanding of the entire memory hierarchy. -> CHAPTER 6.
A critical point to make on access granularity: The entire logic and system hierarchy works only because we can move data in chunks never in single bytes.
