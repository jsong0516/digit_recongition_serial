/*
 * PROJ1-1: YOUR TASK A CODE HERE
 *
 * You MUST implement the calc_min_dist() function in this file.
 *
 * You do not need to implement/use the swap(), flip_horizontal(), transpose(), or rotate_ccw_90()
 * functions, but you may find them useful. Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include "digit_rec.h"
#include "utils.h"
#include "limits.h"

#define MAX_ROTATION 3 // 90, 180, 270

void flip_horizontal(unsigned char *arr, int width);
void flip_vertical(unsigned char *arr, int width);
void swap(unsigned char *x, unsigned char *y);
void rotate_ccw_90(unsigned char *arr, int width) ;
void transpose(unsigned char *arr, int width);

/* Swaps the values pointed to by the pointers X and Y. */
void swap(unsigned char *x, unsigned char *y) {
    unsigned char temp = *x;
    *x = *y;
    *y = temp;
}


void flip_vertical(unsigned char *arr, int width) {
    unsigned char temp;
    for(int y = 0; y < width; y++)
	for(int x = 0; x < width / 2 ; x++)
		swap(&arr[y*width + x], &arr[y*width + width - 1 - x]);
}


/* Flips the elements of a square array ARR across the y-axis. */
void flip_horizontal(unsigned char *arr, int width) {
	transpose(arr, width);
	flip_vertical(arr, width);
	transpose(arr, width);
}

/* Transposes the square array ARR. */
void transpose(unsigned char *arr, int width) {
    for(int y = 0; y < width; y++)
	for(int x = y + 1; x < width ; x++)
		swap(&arr[y*width + x], &arr[x*width + y]);
}

/* Rotates the square array ARR by 90 degrees counterclockwise. */
void rotate_ccw_90(unsigned char *arr, int width) {
    transpose(arr, width);
    flip_vertical(arr, width);
}

/* Returns the squared Euclidean distance between TEMPLATE and IMAGE. The size of IMAGE
 * is I_WIDTH * I_HEIGHT, while TEMPLATE is square with side length T_WIDTH. The template
 * image should be flipped, rotated, and translated across IMAGE.
 */


void finding_corner(unsigned char *image, int i_width, int i_height, int *new_x, int *new_y){
	unsigned char bg = *image; //assume the background color is white for now and the very first pixel
	int x_t = -1; // assume width and height cannot be negative value
	int y_t = -1;
	//finding y direction
	for (int y = 0; y < i_height; y++){
		for (int x = 0; x < i_width; x++)
			if(image[y*i_width + x] != bg ){
				y_t = y;
				break;
			} 
		if(y_t != -1)
			break;
	}
	//finding x direction
	for (int x = 0; x < i_width; x++){
		for (int y = 0; y < i_height; y++)
			if(image[x+ y*i_width] != bg ){
				x_t = x;
				break;
			} 
		if(x_t != -1)
			break;
	}
	//finding x 
	if(y_t == -1 && x_t == -1){
		printf("\nNo digit is found in the image. Exit with 0\n"); // no translated
		exit(0);
	}
	*new_x = x_t;
	*new_y = y_t;
}

int calc_min_dist_helper(unsigned char *image, int i_width, int i_height,int x_start, int y_start, unsigned char *template, int t_width){
	int distance = 0;	
	int x_t = 0;
	int y_t = 0;
	int x_val = 0, y_val = 0;
	
	for(int save_y_start = y_start; save_y_start < t_width; save_y_start++){
		for(int save_x_start = x_start; save_x_start < t_width; save_x_start++){
			x_val = image[save_y_start*i_width + save_x_start];
			y_val = template[y_t*t_width + x_t];
			distance += (x_val - y_val) * (x_val - y_val);
			x_t++;
		}
		y_t++;
		x_t = 0;
	}
	return distance;
}


unsigned int calc_min_dist(unsigned char *image, int i_width, int i_height, unsigned char *template, int t_width) {
    unsigned int min_dist = UINT_MAX;
    unsigned int euclid_dist;
    int x = 0, y = 0;
    if(i_width <= 0 || i_height <= 0) //impossible to match
	return UINT_MAX;
    if(image == NULL){
	printf("Image passed the null object, exit with error");
	exit(0);
    }
    if(i_width != t_width || i_width != i_height) { // means it is translated, since we assume it is same font
	finding_corner(image, i_width, i_height, &x, &y);
	//printf("New x and y : %d %d\n", x,y);
    }
    
    //normal check
    euclid_dist = calc_min_dist_helper(image, i_width, i_height, x, y, template, t_width);
    min_dist = (min_dist < euclid_dist) ? min_dist : euclid_dist; 


    // test rotation
    for(int i = 0; i < MAX_ROTATION ; i++)
    {
    	rotate_ccw_90(template, t_width);
	euclid_dist = calc_min_dist_helper(image, i_width, i_height, x, y, template, t_width);
    }
    rotate_ccw_90(template, t_width);
    min_dist = (min_dist < euclid_dist) ? min_dist : euclid_dist; 

    // test flip - horizontal
    flip_horizontal(template, t_width);
    euclid_dist = calc_min_dist_helper(image, i_width, i_height, x, y, template, t_width);
    min_dist = (min_dist < euclid_dist) ? min_dist : euclid_dist; 
    flip_horizontal(template, t_width);

    // test flip - vertical
    flip_vertical(template, t_width);
    euclid_dist = calc_min_dist_helper(image, i_width, i_height, x, y, template, t_width);
    min_dist = (min_dist < euclid_dist) ? min_dist : euclid_dist; 
    flip_vertical(template, t_width);
	
    printf("Distance ... %d\n", min_dist);
    return min_dist;

}

