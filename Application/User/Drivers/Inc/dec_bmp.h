/*
    Author: Devashish Lahariya
    Date: 26/07/2023

    Description:
    This is a sinmple to use Library for reading and decoding Bitmap (.bmp) image files. Currently supported bit depths are 16-Bit.

    This Software is provided as it is, use at your own risk, the author is not responsible for ANY Damages caused.
*/

#ifndef DEC_BMP_H
#define DEC_BMP_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ff.h"

#define BMP_DEBUG_LOGS 0

typedef struct
{
	uint16_t file_type;
	int file_size;
	uint16_t reserved1;
	uint16_t reserved2;
	int pixel_data_offset;
}BITMAP_FILE_HEADER;

typedef struct
{
	int header_size;
	int image_width;
	int image_height;
	uint16_t planes;
	uint16_t bits_per_pixel;
	int compression;
	int image_size;
	int x_pixels_per_meter;
	int y_pixels_per_meter;
	int total_colours;
	int important_colours;
}BITMAP_INFO_HEADER;

int bmp_get_image_size(FIL*);
void read_bmp_file_header(FIL*, BITMAP_FILE_HEADER*);
void read_bmp_info_header(FIL*, BITMAP_INFO_HEADER*);
void read_bmp_pixel_data(FIL*, BITMAP_FILE_HEADER*, uint8_t*);
void read_bmp24(FIL* _fptr, BITMAP_FILE_HEADER* _bfh, BITMAP_INFO_HEADER* _bih, uint8_t* _src_buffer, uint16_t* _dest_buffer);

#endif
