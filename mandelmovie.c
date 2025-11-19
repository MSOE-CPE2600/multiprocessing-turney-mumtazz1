/***********************************************************************
 * @file mandel.c
 * @brief Source file which implements image creation for movie
 * Course: CPE2600
 * Assignment: Lab 11 - Multiprocessing
 * Author: Zoya Mumtaz
 * Date: 11/11/2025
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
#include "jpegrw.h"

#define NUM_IMAGES 50 //number of image needed, 50 iterations to be done

// local routines
static int iteration_to_color( int i, int max );
static int iterations_at_point( double x, double y, int max );
static void compute_image( imgRawImage *img, double xmin, double xmax,
									double ymin, double ymax, int max );
static void show_help();


int main( int argc, char *argv[] )
{
	char c;
    int num_child = 1; //default 1 child

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

	while((c = getopt(argc,argv,"n:h"))!=-1) {
		switch(c) 
		{
            case 'n':
                num_child = atoi(optarg);
                break;    
			case 'h':
			    show_help();
				return 0;
				break;
			default:
			    return 0;		  
		}
	}

    if (num_child < 1 || num_child > NUM_IMAGES){
        printf("Error: Number of children must be between 1 and 50.\n");
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

                // Compute the Mandelbrot image
                compute_image(img,xcenter-xscale/2,xcenter+xscale/2,ycenter-yscale/2,ycenter+yscale/2,max);

                // Save the image in the stated file.
                storeJpegImageFile(img,curr_filename);

                // free the mallocs
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
Compute an entire Mandelbrot image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
*/

void compute_image(imgRawImage* img, double xmin, double xmax, double ymin, double ymax, int max )
{
	int i,j;

	int width = img->width;
	int height = img->height;

	// For every pixel in the image...

	for(j=0;j<height;j++) {

		for(i=0;i<width;i++) {

			// Determine the point in x,y space for that pixel.
			double x = xmin + i*(xmax-xmin)/width;
			double y = ymin + j*(ymax-ymin)/height;

			// Compute the iterations at that point.
			int iters = iterations_at_point(x,y,max);

			// Set the pixel in the bitmap.
			setPixelCOLOR(img,i,j,iteration_to_color(iters,max));
		}
	}
}


/*
Convert a iteration number to a color.
Here, we just scale to gray with a maximum of imax.
Modify this function to make more interesting colors.
*/
int iteration_to_color( int iters, int max )
{
	int color = 0xFFFFFF*iters/(double)max;
	return color;
}


// Show help message
void show_help()
{
	printf("Use: ./movie [option]\n");
	printf("-n <number of child processes>\n");
	printf("Example: ./movie -n 15\n");
	printf("-h: this help message.\n");

}
