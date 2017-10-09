
/*
	Project: Huffman compressor
	File: huff.c
	Description: structure-packed main module, implicit implementation for speed
	Written by pongozolin, isajisai (2017)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII_SIZE	128

typedef struct node {
	struct node *left;
	struct node *right;
	unsigned int weight;
} Node; 

Node* scan_file(FILE *in_file) {
	Node *dictionary = (Node *)calloc(ASCII_SIZE, sizeof(Node));
	int ch;

	for(;;) {
		ch = fgetc(in_file);
		if(ch == EOF) break;
		++dictionary[ch].weight;
	}
	return dictionary;
}

int main(int argc, char *argv[]) {
	FILE *in_file;

	if(argc != 2) {
		printf("Usage: %s <input file>\n", argv[0]);
		return 0;
	} else {
		in_file = fopen(argv[1], "r");
	}

	// Check to see if dictionaries are made and work.
	Node *table = scan_file(in_file);
	int count = 0;

	while(count < ASCII_SIZE) {
		printf("%i: The count for %c \tis: %i\n", count, count, table[count].weight);
		count++;
	}
}