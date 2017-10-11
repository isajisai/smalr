
/*
	Project: Huffman compressor
	File: huff.c
	Description: structure-packed main module, implicit implementation for speed
	Written by pongozolin, isajisai (2017)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huff.h"

int main(int argc, char *argv[]) {
	FILE *in_file;

	if (argc != 2) {
		printf("Usage: %s <input file>\n", argv[0]);
		return 0;
	} else {
		in_file = fopen(argv[1], "r");
	}

	// Check to see if dictionaries are made and work.
	MinHeap *DATA_HEAP = scan_file(in_file);
	while(DATA_HEAP->size > 20) huff_iteration(&DATA_HEAP);
	naive_print(DATA_HEAP);
	printf("%c\n", ((DATA_HEAP->array)[0])->data);;
	return 0;
}