# System Programming Lab 11 Multiprocessing



Brief:

The program mandelmovie.c is a modified version of mandel.c that creates 50 images. These images can be turned into an animation of a Mandebrot set zooming in to a self-similar pattern. Using fork(), the program uses up to 50 child processes to quickly create the 50 images.



Implementation and Modifications:

Firstly, the program takes in an argument '-n' so the user can specify the number of children (from 1 to 50) that the program should implement to execute specified set of instructions. An example of a command a user may enter is "./movie -n 15", so the program implements 15 child processes. If the user enters no argument, the default is 1 child process. 

Based on the number of child processes, the work is divided amongst them. Each child process is given a batch for the number of images it will execute (number of children / number of images). Thus, the more child processes, the less workload each child process executes, making the runtime faster. 

The ycenter position of each image is kept the same while the xcenter position, yscale, and xscale variables are changed slightly to give a sense of movement in the animation. To do this, the starting and ending position of the Mandelbrot is divided by the number of images, which becomes a shift variable. After each image, the image variables are changed by adding/subtractig this shift.

