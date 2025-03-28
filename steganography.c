/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	if (image == NULL || image->image == NULL) {
		fprintf(stderr, "Invalid image data\n");
		return NULL;
	}
	if (row < 0 || row >= image->rows || col < 0 || col >= image->cols) {
		fprintf(stderr, "Invalid pixel coordinates\n");
		return NULL;
	}
	Color *pixel = malloc(sizeof(Color));
	if (pixel == NULL) {
		fprintf(stderr, "Error allocating memory for pixel\n");
		return NULL;
	}
	uint8_t b = image->image[row][col].B;
	pixel->R = (b & 1) ? 255 : 0;
	pixel->G = (b & 1) ? 255 : 0;
	pixel->B = (b & 1) ? 255 : 0;
	return pixel;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	if (image == NULL || image->image == NULL) {
		return NULL;
	}
	Image *newImage = malloc(sizeof(Image));
	if (newImage == NULL) {
		return NULL;
	}
	newImage->rows = image->rows;
	newImage->cols = image->cols;
	newImage->image = malloc(newImage->rows * sizeof(Color *));
	if (newImage->image == NULL) {
		free(newImage);
		return NULL;
	}
	for (uint32_t i = 0; i < newImage->rows; i++) {
		newImage->image[i] = malloc(newImage->cols * sizeof(Color));
		if (newImage->image[i] == NULL) {
			for (uint32_t j = 0; j < i; j++) {
				free(newImage->image[j]);
			}
			free(newImage->image);
			free(newImage);
			return NULL;
		}
		for (uint32_t j = 0; j < newImage->cols; j++) {
			Color *pixel = evaluateOnePixel(image, i, j);
			if (pixel == NULL) {
				for (uint32_t k = 0; k <= i; k++) {
					free(newImage->image[k]);
				}
				free(newImage->image);
				free(newImage);
				return NULL;
			}
			newImage->image[i][j].R = pixel->R;
			newImage->image[i][j].G = pixel->G;
			newImage->image[i][j].B = pixel->B;
			free(pixel);
		}
	}
	return newImage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return -1;
	}

	Image *image = readData(argv[1]);
	if (image == NULL) {
		fprintf(stderr, "Error reading image data\n");
		return -1;
	}

	Image *newImage = steganography(image);
	if (newImage == NULL) {
		fprintf(stderr, "Error creating steganography image\n");
		freeImage(image);
		return -1;
	}

	writeData(newImage);
	freeImage(image);
	freeImage(newImage);

	return 0;
}
