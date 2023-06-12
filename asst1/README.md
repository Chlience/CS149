# Assignment 1: Performance Analysis on a Quad-Core CPU #

原始题目见 [Handout.md](./Handout.md)

> Is speedup linear in the number of threads used? In your writeup hypothesize why this is (or is not) the case? (you may also wish to produce a graph for VIEW 2 to help you come up with a good answer. Hint: take a careful look at the three-thread datapoint.)
> To confirm (or disprove) your hypothesis, measure the amount of time each thread requires to complete its work by inserting timing code at the beginning and end of workerThreadStart(). How do your measurements explain the speedup graph you previously created?

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

事实上，由于每个线程分配的计算量不均，会导致加速比波动。
在按照行均分为八个线程时，每个线程运算时间为

![prog1-0](./assets/prog1-0.png)

中间区域（Thread 3, 4）的计算量显然要大于两侧区域（Thread 0, 7）

这导致某些时刻，线程数增加反而导致加速比下降（2 to 3）

同时，在 VIEW 2 下线程数和加速比更接近线性关系，
这是由于 VIEW 2 下均分行分配给线程的计算量相近。

![prog1-1](./assets/prog1-1.png)

> Now run your improved code with 16 threads. Is performance noticably greater than when running with eight threads? Why or why not?

