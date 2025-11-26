# System Programming Lab 11/12 - Multiprocessing and Multithreading



**Brief:**

The program mandelmovie.c is a modified version of mandel.c that creates 50 Mandelbrot set images. These images can be turned into an animation of a Mandebrot set zooming into a self-similar pattern. Using fork(), the program utilizes up to 50 child processes to efficiently create the 50 images. The program also implements threads (pthread) to make the image creation process faster. By implementing multi-processing and multi-threading, the program can have a very efficient runtime.



**Implementation and Modifications (Multiprocessing):**

Firstly, the program uses getopt() to read an argument '-n' where the user can specify the number of children (from 1 to 50) that the program should implement to execute specified set of instructions. An example of a command a user may enter is "./movie -n 15", so the program implements 15 child processes. If the user enters no argument, the default is 1 child process.

Based on the number of child processes, the work is divided amongst them. Each child process is given a batch for the number of images it will execute (number of child processes / number of images). Thus, the more child processes, the less workload each child process executes, making the runtime faster.

The ycenter position of each image is kept the same while the xcenter position, yscale, and xscale variables are changed slightly to give a sense of movement in the animation. To do this, the difference in the starting and ending position of the Mandelbrot is divided by the number of images, which becomes a shift variable. After each image creation, the image variables are changed by adding/subtractig this shift.



**Implementation and Modifications (Multithreading):**

The program uses getopt() to read an argument '-t' where the user can specify the number of threads (from 1 to 20) that the program should implement to split the work of creating one image. This can be combined with the 'n' argument to set both the number of child processes and threads at the same time. If the user enters no argument, the default is 1 thread.

Based on the number of threads, the work of creating one image is divided amongst them. Each thread is given a section of the image (start and end at calculated rows) to write the pixels. For example, if the user specifies there to be 3 threads, the image is split into three sections for each thread to work on; one does the top section, middle section, and bottom section. 

A struct named bitmap holds necessary data for the image creation, and another struct named thread\_data holds the bitmap struct and other information needed for the specific thread's execution (since pthread function only takes one argument and in this case we need to input many arguments). The function compute\_image() was modified to become the thread controller, creating, joining, and freeing the threads once execution has finished for the single image. A function named compute\_helper() was created for each thread to execute. To avoid global variable, bitmap structure is implemented in its own file (bitmap.c), where there are variable retrieval, allocation, deallocation, and image writing functions. The main program mandelmovie.c includes the bitmap header file (bitmap.h) to use those functions.





The graph of the program runtime vs. the number of child processes is linked below:

https://github.com/MSOE-CPE2600/multiprocessing-turney-mumtazz1/blob/d67c1919728d026e29c672957473eb523deb4deb/lab11graph.png



**Graph Interpretation:**

The graph's trend clearly shows the benefits of multiprocessing. As the number of child processes increases, the runtime decreases rapidly. There is a large difference in the runtime of 5 child processes compared to the runtime of 2 child processes, representing the most significant speedup. As the number of child processes increases after 5, the performance gain slows down, demonstrating the PC's core limit (for example, my laptop has 12 cores). To conclude, the best and optimal range for the number of child processes is between 5 to 10. Having more child processes than this is not very effective in speedup.



The table of the program runtime when using different combinations of the number of threads and child processes is linked below:

https://github.com/MSOE-CPE2600/multiprocessing-turney-mumtazz1/blob/044a0f4985b95021ee133bb827ab268a03aa531e/lab12table.png



**Table Interpretation**

As expected, the runtime decreases as the number of thread workers increases, and this is the same case as the number child processes increases. The baseline runtime is 97.5 seconds, where there is no parallel processing happening. If we look at purely threads, the runtime decreases to 15.2 seconds. If we look at purely processes, the runtime decreases to 13.0 seconds. The combination of the two result in much better runtime results, even reaching as low as 8.5 seconds. The runtime results clearly show that having an excess amount of child processes and threads is not the most efficient, since the runtime result for 20 thread workers and 20 child processes was 10.4 seconds, which is longer than the 8.5 seconds performed by 10 thread workers and 15 child processes. This is due to overhead, where the computer spends more time pausing and loading the threads than actually executing the work.



**Result**

From the table, it can be concluded that multiprocessing impacted the runtime results more than multithreading. This is because in multiprocessing, the entire workload is being parallelized, from the start to end of the 50 images, while in multithreading, only a part of this workload is parallelized, the creation of one image. Child processes are separate jobs and they do not need to fight over the same memory address. This way, the computer can treat each one independently and each one can run at full speed. In multithreading, the main program is dependent on the slowest thread, and has to wait for all thread workers to finish before continuing, and saving the image also takes extra time, which multithreading does not parallelize but multiprocessing does.



The most optimal combination was 10 thread and 15 child processes, resulting in a runtime of just 8.5 seconds. The CPU remains busy the entire time, making sure no time is wasted waiting for a process to finish. While some processes were paused waiting for slow operations like writing the image to disk, other processes were immediately available to utilize the CPU for calculations. No other combination resulted in a runtime this short.



The program is an excellent demonstration of the uses of multiprocessing and confirms that parallel execution is the most optimal for performance on multicore hardware.

