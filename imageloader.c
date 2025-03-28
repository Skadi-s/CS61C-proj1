/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Error opening file %s\n", filename);
		return NULL;
	}
	Image *image = malloc(sizeof(Image));
	if (image == NULL) {
		fprintf(stderr, "Error allocating memory for image\n");
		fclose(fp);
		return NULL;
	}
	char format[3];
	if (fscanf(fp, "%2s", format) != 1 || strcmp(format, "P3") != 0) {
		fprintf(stderr, "Invalid PPM format\n");
		free(image);
		fclose(fp);
		return NULL;
	}
	int maxval;
	if (fscanf(fp, "%u %u %d", &image->cols, &image->rows, &maxval) != 3) {
		fprintf(stderr, "Error reading image dimensions or maxval\n");
		free(image);
		fclose(fp);
		return NULL;
	}
	image->image = malloc(image->rows * sizeof(Color *));
	if (image->image == NULL) {
		fprintf(stderr, "Error allocating memory for image rows\n");
		free(image);
		fclose(fp);
		return NULL;
	}
	for (uint32_t i = 0; i < image->rows; i++) {
		image->image[i] = malloc(image->cols * sizeof(Color));
		if (image->image[i] == NULL) {
			fprintf(stderr, "Error allocating memory for image columns\n");
			for (uint32_t j = 0; j < i; j++) {
				free(image->image[j]);
			}
			free(image->image);
			free(image);
			fclose(fp);
			return NULL;
		}
		for (uint32_t j = 0; j < image->cols; j++) {
			if (fscanf(fp, "%hhu %hhu %hhu", &image->image[i][j].R, &image->image[i][j].G, &image->image[i][j].B) != 3) {
				fprintf(stderr, "Error reading pixel data\n");
				for (uint32_t k = 0; k <= i; k++) {
					free(image->image[k]);
				}
				free(image->image);
				free(image);
				fclose(fp);
				return NULL;
			}
		}
	}
	fclose(fp);
	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	if (image == NULL || image->image == NULL) {
		fprintf(stderr, "Invalid image data\n");
		return;
	}
	printf("P3\n");
	printf("%u %u\n", image->cols, image->rows);
	printf("255\n");
	for (uint32_t i = 0; i < image->rows; i++) {
		for (uint32_t j = 0; j < image->cols; j++) {
			printf("%hhu %hhu %hhu ", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	if (image == NULL) {
		return;
	}
	for (uint32_t i = 0; i < image->rows; i++) {
		free(image->image[i]);
	}
	free(image->image);
	free(image);
}