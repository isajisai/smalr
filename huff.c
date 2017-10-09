
/*
	Project: Huffman compressor
	File: huff.c
	Description: main module
	Written by pongozolin, isajisai (2017)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE	256
#define ASCII_MAX		255

typedef struct Node {
	Node* next;
	unsigned int weight;
	char item;
}