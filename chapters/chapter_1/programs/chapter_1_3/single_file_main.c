#include <stdio.h>
#include <time.h>

/* =====================================================================
 * FUNCTION: slow_add (The Pointer Method)
 * * FATE IN UNOPTIMIZED (-O0): 
 * Translates literally. The CPU must cross the motherboard to read and 
 * write to the RAM address of `dest` 1,000,000,000 times.
 * * FATE IN OPTIMIZED (-O1): 
 * The compiler is HANDCUFFED by "Pointer Aliasing". Because `dest` is a 
 * pointer, the compiler cannot prove that another thread or hardware 
 * device isn't watching this exact RAM address. Therefore, it refuses to 
 * optimize it. It is legally forced to execute all 1 billion RAM writes.
 * ===================================================================== */
void slow_add(int* dest, int iterations)
{
    for (size_t i = 0; i < iterations; i++)
    {
        // FORCED RAM ACCESS: 1 read, 1 write per iteration
        *dest += 1;
    }
}

/* =====================================================================
 * FUNCTION: fast_add (The Local Variable Method)
 * * FATE IN UNOPTIMIZED (-O0): 
 * The "Debugger Rule". The compiler forces `temp` to live in RAM (on the 
 * call stack) so a debugger can read it. It loses its CPU Register 
 * advantage. It actually runs SLOWER than slow_add here because it is 
 * doing the exact same slow RAM loop, but with extra assembly instructions 
 * to manage the `temp` variable.
 * * FATE IN OPTIMIZED (-O1): 
 * The compiler is UNLEASHED. It sees `temp` is isolated and moves it 
 * directly into the ultra-fast CPU Register. The entire loop executes 
 * inside the CPU core in a fraction of a millisecond. The motherboard 
 * is only crossed EXACTLY ONCE at the very end.
 * ===================================================================== */
void fast_add(int* dest, int iterations)
{
    int temp = 0; // -O1 puts this in a CPU Register
    
    for (size_t i = 0; i < iterations; i++)
    {
        temp += 1; // Math happens purely inside the CPU
    }
    
    *dest = temp;  // Single RAM write at the end
}

int main()
{
    int num = 0;
    int target_iterations = 1000000000;

    /* -----------------------------------------------------------------
     * THE MICROBENCHMARKING TRAP (-O0 Anomaly)
     * Why did fast_add lose to slow_add in the unoptimized run?
     * 1. Both were forced to use RAM, making them equally slow.
     * 2. fast_add had more instruction bloat.
     * 3. Hardware Physics: slow_add ran first on a cold CPU, triggering 
     * maximum Turbo Boost. By the time fast_add ran, the silicon was 
     * hotter, forcing the CPU to slightly down-clock its speed to 
     * prevent overheating.
     * * RULE: Never trust benchmark timings on unoptimized code. 
     * You are measuring hardware noise, not algorithmic efficiency.
     * ----------------------------------------------------------------- */

    clock_t start_time1 = clock();
    slow_add(&num, target_iterations);
    clock_t end_time1 = clock();

    double time_taken1 = ((double)(end_time1 - start_time1)) / CLOCKS_PER_SEC;

    num = 0; // Reset for a fair test

    clock_t start_time2 = clock();
    fast_add(&num, target_iterations);
    clock_t end_time2 = clock();

    double time_taken2 = ((double)(end_time2 - start_time2)) / CLOCKS_PER_SEC;

    printf("Slow add (RAM bound): %lf seconds\n", time_taken1);
    printf("Fast add (CPU bound): %lf seconds\n", time_taken2);

    return 0;
}