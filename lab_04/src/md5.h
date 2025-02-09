#ifndef __MD5_H__
#define __MD5_H__

#include <stdio.h>
#include <stdint.h>

#define MAX_BUF_SIZE 1024
#define RESULT_SIZE 16

void md5sum(FILE *file, uint8_t result[]);

#endif // __MD5_H__