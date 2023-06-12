# Assignment 1: Performance Analysis on a Quad-Core CPU #

原始题目见 [Handout.md](./Handout.md)

## Program 1: Parallel Fractal Generation Using Threads

> Modify the starter code to parallelize the Mandelbrot generation using two processors. Specifically, compute the top half of the image in thread 0, and the bottom half of the image in thread 1. This type of problem decomposition is referred to as spatial decomposition since different spatial regions of the image are computed by different processors.

详见代码

> Extend your code to use 2, 3, 4, 5, 6, 7, and 8 threads, partitioning the image generation work accordingly (threads should get blocks of the image). Note that the processor only has four cores but each core supports two hyper-threads, so it can execute a total of eight threads interleaved on its execution contents. In your write-up, produce a graph of speedup compared to the reference sequential implementation as a function of the number of threads used FOR VIEW 1. Is speedup linear in the number of threads used? In your writeup hypothesize why this is (or is not) the case? (you may also wish to produce a graph for VIEW 2 to help you come up with a good answer. Hint: take a careful look at the three-thread datapoint.)

|num of thread|speed up (view 1)|speed up (view 2)|
|-|-|-|
|1|1x|1x|
|2|1.87x|1.58x|
|3|1.63x|2.13x|
|4|2.41x|2.29x|
|5|2.31x|2.62x|
|6|3.04x|3.07x|
|7|3.11x|3.31x|
|8|3.71x|3.85x|

根据该表格，加速比和使用的线程数并不是线性关系。

> To confirm (or disprove) your hypothesis, measure the amount of time each thread requires to complete its work by inserting timing code at the beginning and end of workerThreadStart(). How do your measurements explain the speedup graph you previously created?

事实上，由于每个线程分配的计算量不均，会导致加速比波动。
在按照行均分为八个线程时，每个线程运算时间为

![prog1-0](./assets/prog1-0.png)

中间区域（Thread 3, 4）的计算量显然要大于两侧区域（Thread 0, 7）

这导致某些时刻，线程数增加反而导致加速比下降（2 to 3）

同时，在 VIEW 2 下线程数和加速比更接近线性关系，
这是由于 VIEW 2 下均分行分配给线程的计算量相近。

![prog1-1](./assets/prog1-1.png)

> Modify the mapping of work to threads to achieve to improve speedup to at about 7-8x on both views of the Mandelbrot set (if you're above 7x that's fine, don't sweat it). You may not use any synchronization between threads in your solution. We are expecting you to come up with a single work decomposition policy that will work well for all thread counts---hard coding a solution specific to each configuration is not allowed! (Hint: There is a very simple static assignment that will achieve this goal, and no communication/synchronization among threads is necessary.). In your writeup, describe your approach to parallelization and report the final 8-thread speedup obtained.


观察计算密度分布，再进行行的分配，让每个进程的计算量趋于相等。
关于行和计算量的关系可以静态分析，或者用插值实现

TODO

> Now run your improved code with 16 threads. Is performance noticably greater than when running with eight threads? Why or why not?

当不同线程的任务量已经趋于相等时，超过物理线程的线程数会增加非并行部分的花费，

但在并行性不够好时，增加线程超过物理线程数相当于手动将任务分成更小的部分，从而提高加速比。

## Program 2: Vectorizing Code Using SIMD Intrinsics

> Implement a vectorized version of clampedExpSerial in clampedExpVector . Your implementation should work with any combination of input array size (N) and vector width (VECTOR_WIDTH).

详见代码

> Run ./myexp -s 10000 and sweep the vector width from 2, 4, 8, to 16. Record the resulting vector utilization. You can do this by changing the #define VECTOR_WIDTH value in CS149intrin.h. Does the vector utilization increase, decrease or stay the same as VECTOR_WIDTH changes? Why?

|vector width|utilization|
|-|-|-|
|2|79.8%|
|4|72.1%|
|8|68.1%|
|16|66.3%|

随着 VECTOR_WIDTH 提升，vector utilization 不断下降

原因是当 VECTOR_WIDTH 增大，
每个 exponents 和组内最大 exponents 的差值期望增加，
故 disable 的 vector lanes 期望增加

> Extra credit: (1 point) Implement a vectorized version of arraySumSerial in arraySumVector. Your implementation may assume that VECTOR_WIDTH is a factor of the input array size N. Whereas the serial implementation has O(N) span, your implementation should have at most O(N / VECTOR_WIDTH + log2(VECTOR_WIDTH)) span. You may find the hadd and interleave operations useful.

详见代码

## Program 3: Parallel Fractal Generation Using ISPC

