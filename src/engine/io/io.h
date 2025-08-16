#ifndef IO_H
#define IO_H

#include <stdlib.h>
#include <stdbool.h>

// 20Mb, arbitrary, could probably be larger
#define IO_READ_CHUNK_SIZE 2097152
#define IO_READ_ERROR_GENERAL "Error reading file: %s. errno: %d\n"
#define IO_READ_ERROR_MEMORY "Not enough free memory to read file: %s\n"

typedef struct file
{
	char* data;
	size_t len;
	bool is_valid;
} File;

File io_file_read(const char* path);

#endif