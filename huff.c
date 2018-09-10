
/*
	Project: Huffman compressor
	File: huff.c
	Description: structure-packed main module, implicit implementation for speed
	Author: isajisai (2017)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ASCII_SIZE	128

struct node {
	struct node *left, *right;
	unsigned int weight;
	char data;
}; 

struct heap {
	unsigned int size;
	struct node *array[ASCII_SIZE];
};

struct pair {
	int arr[ASCII_SIZE/4];
	int length;
	char data;
};

void switch_nodes(struct node **a, struct node **b) {
	struct node *t = *a;
	*a = *b; 
	*b = t;
}

struct heap* make_heap() {
	struct heap *to_return = calloc(1, sizeof(struct heap));
	to_return->size = 0;
	return to_return;
}

void shift_up_2(struct heap *heap) {
	int i = 0;
	while (i < heap->size) {
		(heap->array)[i] = (heap->array)[i+2];
		(heap->array)[i+1] = (heap->array)[i+3]; 
		i++;
	}
	heap->size -= 2;
}

void add_to_heap(struct node *to_add, struct heap *heap) {
	int pos = heap->size++;
	heap->array[pos] = to_add;
	if (heap->size > 2) {
		while ((heap->array[pos-1])->weight > (heap->array[pos])->weight) {
			switch_nodes(&(heap->array[pos-1]), &(heap->array[pos]));
			if (--pos == 0) 
				break;
		}
	}
}

struct node* combine_nodes(struct node *lighter_node, struct node *heavier_node) {
	struct node *new_node = calloc(1, sizeof(struct node));
	new_node->left = lighter_node; 
	new_node->right = heavier_node;
	new_node->weight = lighter_node->weight + heavier_node->weight;
	return new_node;
}

struct node* build_hufftree(struct heap *heap) {
	while (heap->size > 1) {
		add_to_heap(combine_nodes(heap->array[0], heap->array[1]), heap);
        	shift_up_2(heap);
	}
	return heap->array[0];
}

void encode(FILE *in_file, FILE *out_file, struct pair *pairs) {
	int i, ch;
	int curr_size = 0;
	unsigned int buffer = 0;

	for (;;) {
		ch = fgetc(in_file);
		if (ch == EOF) 
			break; 
		i = 0;
		while (i++ < pairs[ch].length) {
			buffer <<= 1;
			buffer += (pairs[ch].arr)[i];
			curr_size++;
			if (curr_size == 8) {
				fwrite(&buffer, 1, 1, out_file);
				curr_size = 0;
				buffer = 0;
			}
		}
	}

	while (curr_size < 8) {
		buffer <<= 1;
		curr_size++;
	}

	rewind(in_file);
	fwrite(&buffer, 1, 1, out_file);
	fclose(out_file);
}

void build_pairings(struct node* root, int arr[], int top, struct pair *pairs) {
	if (root->left) { 
		arr[top] = 0; 
		build_pairings(root->left, arr, top + 1, pairs); 
	}
    	if (root->right) { 
    		arr[top] = 1; 
    		build_pairings(root->right, arr, top + 1, pairs); 
    	}
    	if (!(root->left) && !(root->right)) { 
    		char index = root->data;
    		for (int i = 0; i < top; i++) 
    			pairs[index].arr[i] = arr[i]; 
    		pairs[index].length = top; 
    		pairs[index].data = root->data;
    }
}

struct heap* scan_file(FILE *in_file) {
	struct node *dictionary = calloc(ASCII_SIZE, sizeof(struct node));
	struct heap *heap = calloc(1, sizeof(struct heap));
	int ch;

	for(;;) {
		ch = fgetc(in_file);
		if (ch == EOF) 
			break;
		dictionary[ch].weight++;
	}

	for (ch = 0; ch < ASCII_SIZE; ch++) {
		if (dictionary[ch].weight == 0) 
			continue;
		dictionary[ch].data = ch;
		add_to_heap(&(dictionary[ch]), heap);
	}

	rewind(in_file);
	return heap;
} 

int main(int argc, char *argv[]) {
	double c = clock();

	// Reading
	if (argc != 2) { 
		printf("Usage: %s <input file>\n", argv[0]); 
		return 0; 
	}
	FILE *in_file = fopen(argv[1], "r");
	FILE *out_file = fopen("out", "wb");
	int arr[ASCII_SIZE];

	// Preprocessing data 
	printf("Reading file...\n");
	struct heap *_main_heap = scan_file(in_file);
	struct pair *pairs = calloc(ASCII_SIZE, sizeof(struct pair));
	build_pairings(build_hufftree(_main_heap), arr, 0, pairs);
	
	// Encoding
	printf("Compressing...\n");
	encode(in_file, out_file, pairs);

	// Gathering info formetrics
	FILE *read_out = fopen("out", "r");
	fseek(in_file, 0L, SEEK_END); 
	fseek(read_out, 0L, SEEK_END);
	int before = ftell(in_file); 
	int after = ftell(read_out);

	// Reporting metrics
	double curr_time = (clock() - c) / CLOCKS_PER_SEC;
	double efficiency = 100 - (((double) after / (double) before) * 100);
	printf("Compressed %i bytes into %i bytes in %f seconds \n", before, after, curr_time);
	printf("Achieved %f %% compression.\n", efficiency);
	free(_main_heap);
	free(pairs);
	return 0;
}
