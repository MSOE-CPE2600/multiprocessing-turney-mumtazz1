/***********************************************************************
 * @file mandelmovie.c
 * @brief Source file which implements image creation for movie
 * Course: CPE2600
 * Assignment: Lab 12 - Multithreading
 * Author: Zoya Mumtaz
 * Date: 11/20/2025
 * Note: compile with
 *     $ make
 *     run with
 *     $ ./movie
 */
/// 
//  mandel.c
//  Based on example code found here:
//  https://users.cs.fiu.edu/~cpoellab/teaching/cop4610_fall22/project3.html
//
//  Converted to use jpg instead of BMP and other minor changes
//  
///

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <pthread.h>
#include "jpegrw.h"
#include "bitmap.h"

#define NUM_IMAGES 50 //number of image needed, 50 iterations to be done

//declare struct to hold image coordinate variables for one thread
struct thread_data {
	struct bitmap *bm;
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	int max;

	int starting_row;
	int ending_row;
};

// local routines
static int iterations_at_point( double x, double y, int max );
static void compute_image(struct bitmap *bm, double xmin, double xmax,
									double ymin, double ymax, int max );
static void* compute_helper(void *arg);
static void show_help();


int main( int argc, char *argv[] )
{
	char c;
    int num_child = 1; //default 1 child
	int thread_count = 1; //default threads is 1

	// These are the default configuration values used
	// if no command line arguments are given.
	//char *outfile = "mandel.jpg";
	double xcenter = 0.0;
	double ycenter = 0;
	double xscale = 4;
	double yscale = 4;
	int    image_width = 1000;
	int    image_height = 1000;
	int    max = 1000;

    double initial_scale = 2.5; //initial scale zoom
    double end_scale = 0.07; //final image scale
    double total_scale = initial_scale - end_scale; //total scale to be done

    double scale_size = total_scale / (NUM_IMAGES - 1); //scale step for each image

    double x_start = -0.6; //starting x position
    double x_end = -1.77; //ending x position
    double x_shift = (x_end - x_start)/(NUM_IMAGES - 1); //x shift step for each image

	//the y shift was not needed for this program
    double y_start = 0.0; //starting y position
    double y_end = 0.0; //ending x position
    double y_shift = (y_end - y_start)/(NUM_IMAGES - 1); //y shift step for each image

	// For each command line argument given,
	// override the appropriate configuration value.

	while((c = getopt(argc,argv,"t:n:h"))!=-1) {
		switch(c) 
		{
            case 'n':
                num_child = atoi(optarg);
                break;    
			case 'h':
			    show_help();
				return 0;
				break;
			case 't':
			    thread_count = atoi(optarg);
				break;	
			default:
			    return 0;		  
		}
	}

    if (num_child < 1 || num_child > NUM_IMAGES){
        printf("Error: Number of children must be between 1 and 50.\n");
        return 1;
    }

	if (thread_count < 1 || thread_count > 20){
        printf("Error: Number of threads must be between 1 and 20.\n");
        return 1;
    }


    int batch = NUM_IMAGES / num_child; //divide processes with num of children
    for (int i = 0; i < num_child; i++){ //each child iterates
        int start = i * batch; //the child's starting image
        int end = start + batch; //the child's ending image

        if (i == num_child - 1){
            end = NUM_IMAGES; //handle the last 50th image
        }

        pid_t pid = fork();
        if (pid == 0){
            for(int v = start; v < end; v++){
                // Calculate y and x scale and the center x and y positions
                double current_scale = initial_scale - (v * scale_size);
                xscale = current_scale;
                yscale = current_scale;
                xcenter = x_start + (v * x_shift);
                ycenter = y_start + (v * y_shift);

                //create filename
                char curr_filename[64];
                //using sprintf to print to buffer
                sprintf(curr_filename, "mandel%d.jpg", v);

                // Display the configuration of the image.
                printf("mandel: x=%lf y=%lf xscale=%lf yscale=%1f max=%d outfile=%s PID=%d\n",xcenter,ycenter,xscale,yscale,max,curr_filename, getpid());

                // Create a raw image of the appropriate size.
                imgRawImage* img = initRawImage(image_width,image_height);

                // Fill it with a black
                setImageCOLOR(img,0);

				// Create the BitMap Struct
				struct bitmap *bm = bitmap_create(image_width, image_height, max, img, thread_count);

                // Compute the Mandelbrot image
                compute_image(bm,xcenter-xscale/2,xcenter+xscale/2,ycenter-yscale/2,ycenter+yscale/2,max);

                // Save the image in the stated file.
                storeJpegImageFile(img,curr_filename);

                // free the malloc
			    free_bitmap(bm);
                freeRawImage(img);
            }

            exit(0);
        }
    }

    for (int i = 0; i < num_child; i++){
        wait(NULL);
    }

	return 0;
}


/*
Return the number of iterations at point x, y
in the Mandelbrot space, up to a maximum of max.
*/

int iterations_at_point( double x, double y, int max )
{
	double x0 = x;
	double y0 = y;

	int iter = 0;

	while( (x*x + y*y <= 4) && iter < max ) {

		double xt = x*x - y*y + x0;
		double yt = 2*x*y + y0;

		x = xt;
		y = yt;

		iter++;
	}

	return iter;
}


/*
* Helper function for compute_image(), goes into the pthread_create() argument
* contains instructions that the thread will execute
* takes in void * and returns void *
*/
void *compute_helper(void *arg) {
	struct thread_data *data = (struct thread_data *)arg; //typecast the arg
	struct bitmap *bm = data->bm;

	int width = bitmap_width(bm);
	int height = bitmap_height(bm);

	// For every pixel in the image...

	// Determine the point in x,y space for that pixel.
	for(int i = data->starting_row; i < data->ending_row; i++){

		for(int j = 0; j < width ; j++){
			double x = data->xmin + j * (data->xmax - data->xmin) / width;
			double y = data->ymin + i * (data->ymax - data->ymin) / height;

			// Compute the iterations at that point.
			int iters = iterations_at_point(x, y, data->max);

			// Set the pixel in the bitmap.
			bitmap_set(bm, i, j, iters);

		}
	}
	return NULL;
}

/*
Compute an entire Mandelbrot image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
*/

void compute_image(struct bitmap *bm, double xmin, double xmax, double ymin, double ymax, int max )
{
	int threads = bitmap_threads(bm);
	int height = bitmap_height(bm);


	//allocate memory (dynamic arrays) for managing each thread
	pthread_t *pthreads = malloc(threads * sizeof(pthread_t));
	struct thread_data *data_arr = malloc(threads * sizeof(struct thread_data));

	//create each thread
	for (int i = 0; i < threads; i++){
		//general data
		data_arr[i].bm = bm;
		data_arr[i].xmin = xmin;
		data_arr[i].xmax = xmax;
		data_arr[i].ymin = ymin;
		data_arr[i].ymax = ymax;
		data_arr[i].max = max;

		//calculate the starting and ending row for the thread
		data_arr[i].starting_row = i * (height/threads); 
		data_arr[i].ending_row = (i + 1) * (height/threads);

		//edge case for the last thread and any remainder rows of the image
		if (i == threads - 1){
			data_arr[i].ending_row = height;
		}

		//create the thread
		pthread_create(&pthreads[i], NULL, compute_helper, &data_arr[i]);

	}

	for (int i = 0; i < threads; i++){
		//pthread_join will pause until pthreads[i] is done
		//making sure that each thread has finished befeore continuing
		pthread_join(pthreads[i], NULL);
	}

	//deallocating
	free(pthreads);
	free(data_arr);
	
}



// Show help message
void show_help()
{
	printf("Use: ./movie [option]\n");
	printf("-n <number of child processes>\n");
	printf("Example: ./movie -n 15\n");
	printf("-t <number of threads>\n");
	printf("Example: ./movie -t 5\n");
	printf("Note: both processes and threads can be set.\n");
	printf("Example: ./movie -n 10 -t 3\n");
	printf("-h: this help message.\n");

}
