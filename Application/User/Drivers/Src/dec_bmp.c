#include "dec_bmp.h"

int bytes_to_read = 0;

static int __create_32_bit_int(uint8_t* _buffer)
{
	int value = 0;

	value |= *(_buffer);
	_buffer++;
	value |= *(_buffer) << 8;
	_buffer++;
	value |= *(_buffer) << 16;
	_buffer++;
	value |= *(_buffer) << 24;

	return value;
}

int bmp_get_image_size(FIL* _fptr)
{
	if (_fptr == NULL) return -1;

    return f_size(_fptr);
}

void read_bmp_file_header(FIL* _fptr, BITMAP_FILE_HEADER* _bfh)
{
	uint8_t buffer[14];

	memset(buffer, 0x00, sizeof(buffer));

	UINT btr = 14, br = 0;

	f_read(_fptr, buffer, btr, &br);

	_bfh -> file_type 			= ((buffer[0] << 8) | buffer[1]);
	_bfh -> file_size 			= __create_32_bit_int(&buffer[2]);
	_bfh -> reserved1 			= ((buffer[7] << 8) | buffer[6]);
	_bfh -> reserved2 			= ((buffer[9] << 8) | buffer[8]);
	_bfh -> pixel_data_offset 	= __create_32_bit_int(&buffer[10]);

	#if BMP_DEBUG_LOGS == 1

	printf("---------------------------------------------------------------------------------------------------------------------------------------\n");

	printf("BITMAP FILE HEADER:\n\n");

	for(int i=0; i<14; i++)
	{
		printf("0x%X ", buffer[i]);
	}
	printf("\n");

	for(int i=0; i<14; i++)
	{
		printf("%c ", buffer[i]);
	}
	printf("\n");

	printf("---------------------------------------------------------------------------------------------------------------------------------------\n");

	printf("FILE TYPE: %c%c\n", 		buffer[0], buffer[1]);
	printf("FILE SIZE: %d\n", 			_bfh->file_size);
	printf("PIXEL DATA OFFSET: %d\n", 	_bfh->pixel_data_offset);

	printf("---------------------------------------------------------------------------------------------------------------------------------------\n");
	
	#endif

	f_lseek(_fptr, 0);
}

void read_bmp_info_header(FIL* _fptr, BITMAP_INFO_HEADER* _bih)
{
	uint8_t buffer[54];

	memset(buffer, 0x00, sizeof(buffer));

	UINT btr = 54, br = 0;

	f_read(_fptr, buffer, btr, &br);

	_bih -> header_size  		= 	__create_32_bit_int(&buffer[14]);
	_bih -> image_width  		= 	__create_32_bit_int(&buffer[18]);
	_bih -> image_height 		= 	__create_32_bit_int(&buffer[22]);
	_bih -> planes		   		= 	((buffer[27] << 8) | buffer[26]);
	_bih -> bits_per_pixel 		= 	((buffer[29] << 8) | buffer[28]);
	_bih -> compression 		= 	__create_32_bit_int(&buffer[30]);
	_bih -> image_size 			= 	__create_32_bit_int(&buffer[34]);
	_bih -> x_pixels_per_meter  = 	__create_32_bit_int(&buffer[38]);
	_bih -> y_pixels_per_meter  = 	__create_32_bit_int(&buffer[42]);
	_bih -> total_colours 		= 	__create_32_bit_int(&buffer[46]);
	_bih -> important_colours   = 	__create_32_bit_int(&buffer[50]);

	#if BMP_DEBUG_LOGS == 1

	printf("---------------------------------------------------------------------------------------------------------------------------------------\n");

	printf("BITMAP INFO HEADER:\n\n");
	printf("HEADER SIZE: %d\n", 		_bih -> header_size);
	printf("IMAGE WIDTH: %d\n", 		_bih -> image_width);
	printf("IMAGE HEIGHT: %d\n", 		_bih -> image_height);
	printf("PLANES: %u\n", 				_bih -> planes);
	printf("BITS PER PIXEL: %u\n", 		_bih -> bits_per_pixel);
	printf("COMPRESSION: %d\n", 		_bih -> compression);
	printf("IMAGE SIZE: %d\n", 			_bih -> image_size);
	printf("X PIXELS PER METER: %d\n", 	_bih -> x_pixels_per_meter);
	printf("Y PIXELS PER METER: %d\n", 	_bih -> y_pixels_per_meter);
	printf("TOTAL COLOURS: %d\n", 		_bih -> total_colours);
	printf("IMPORTANT COLOURS: %d\n", 	_bih -> important_colours);

	printf("---------------------------------------------------------------------------------------------------------------------------------------\n");

	#endif
}

void read_bmp_pixel_data(FIL* _fptr, BITMAP_FILE_HEADER* _bfh, uint8_t* _buffer)
{
	f_lseek(_fptr, 0);

	f_lseek(_fptr, _bfh -> pixel_data_offset);

	UINT btr = _bfh->file_size - _bfh->pixel_data_offset, br = 0;

	f_read(_fptr, _buffer, btr, &br);
}

void read_bmp24(FIL* _fptr, BITMAP_FILE_HEADER* _bfh, BITMAP_INFO_HEADER* _bih, uint8_t* _src_buffer, uint16_t* _dest_buffer)
{
	read_bmp_file_header(_fptr, _bfh);

	read_bmp_info_header(_fptr, _bih);

	int btr = _bfh->file_size - _bfh->pixel_data_offset;

	memset(_src_buffer, 0, btr);

	read_bmp_pixel_data(_fptr, _bfh, (uint8_t*)_src_buffer);

	f_close(_fptr);

	int idx = 0;

	unsigned char r, g , b;
	unsigned short rgb;

	for(int i=0; i<btr; i+=3)
	{
		b = _src_buffer[i];
		g = _src_buffer[i + 1];
		r = _src_buffer[i + 2];

		rgb = ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);			//RGB888 To RGB565

		_dest_buffer[idx] = rgb;

		idx++;
	}
}
