# System Programming Lab 11 Multiprocessing



**Brief:**

The program mandelmovie.c is a modified version of mandel.c that creates 50 Mandelbrot set images. These images can be turned into an animation of a Mandebrot set zooming into a self-similar pattern. Using fork(), the program utilizes up to 50 child processes to efficiently create the 50 images.



**Implementation and Modifications:**

Firstly, the program uses getopt() to read an argument '-n' where the user can specify the number of children (from 1 to 50) that the program should implement to execute specified set of instructions. An example of a command a user may enter is "./movie -n 15", so the program implements 15 child processes. If the user enters no argument, the default is 1 child process.

Based on the number of child processes, the work is divided amongst them. Each child process is given a batch for the number of images it will execute (number of children / number of images). Thus, the more child processes, the less workload each child process executes, making the runtime faster.

The ycenter position of each image is kept the same while the xcenter position, yscale, and xscale variables are changed slightly to give a sense of movement in the animation. To do this, the starting and ending position of the Mandelbrot is divided by the number of images, which becomes a shift variable. After each image, the image variables are changed by adding/subtractig this shift.



The graph of the program runtime vs. the number of child processes is linked below:

https://github.com/MSOE-CPE2600/multiprocessing-turney-mumtazz1/blob/d67c1919728d026e29c672957473eb523deb4deb/lab11graph.png 



**Graph Interpretation:**

The graph's trend clearly shows the benefits of multiprocessing. As the number of child processes increases, the runtime decreases rapidly. There is a large difference in the runtime of 5 child processes compared to the runtime of 2 child processes, representing the most significant speedup. As the number of child processes increases after 5, the performance gain slows down, demonstrating the PC's core limit (for example, my laptop has 12 cores). To conclude, the best and optimal range for the number of child processes is between 5 to 10. Having more child processes than this is not very effective in speedup.



**Result**

The program is an excellent demonstration of the uses of multiprocessing and confirms that parallel execution is the most optimal for performance on multicore hardware.

