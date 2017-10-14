
/*
	Project: Huffman compressor
	File: huff.c
	Description: structure-packed main module, implicit implementation for speed
	Written by pongozolin, isajisai (2017)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ASCII_SIZE			128

typedef struct NODE 
{
	struct NODE *left, *right;
	unsigned weight;
	char data;
} NODE; 

typedef struct heap 
{
	unsigned size;
	NODE *array[ASCII_SIZE];
} MIN_HEAP;

typedef struct PAIR 
{
	int arr[ASCII_SIZE/4];
	int length;
	char data;
} PAIR;

void switch_NODEs(NODE **a, NODE **b) 
{
	NODE *t = *a;
	*a = *b; 
	*b = t;
}

MIN_HEAP* make_MIN_HEAP() 
{
	MIN_HEAP *to_return = calloc(1, sizeof(MIN_HEAP));
	to_return->size = 0;
	return to_return;
}

void shift_up_2(MIN_HEAP *heap) 
{
	int i = 0;
	while (i < heap->size) 
	{
		(heap->array)[i] = (heap->array)[i+2];
		(heap->array)[i+1] = (heap->array)[i+3]; 
		i++;
	}
}

void add_to_heap(NODE *to_add, MIN_HEAP *heap) 
{
	int pos = (heap->size)++;
	(heap->array)[pos] = to_add;
	if (heap->size > 2) 
	{
		while ((heap->array)[pos-1]->weight > (heap->array)[pos]->weight)  
		{
			switch_NODEs(&(heap->array[pos-1]), &(heap->array[pos]));
			if (--pos == 0) 
				break;
		}
	}
}

NODE* combine_NODEs(NODE *lighter_NODE, NODE *heavier_NODE) 
{
	NODE *new_NODE = calloc(1, sizeof(NODE));
	new_NODE->left = lighter_NODE; 
	new_NODE->right = heavier_NODE;
	new_NODE->weight = lighter_NODE->weight + heavier_NODE->weight;
	return new_NODE;
}

void huff_iteration(MIN_HEAP *heap) 
{
	add_to_heap(combine_NODEs((heap->array)[0], (heap->array)[1]), heap);
	shift_up_2(heap);
	heap->size -= 2;
}

NODE* build_hufftree(MIN_HEAP *heap) 
{
	while (heap->size > 1) 
		huff_iteration(heap);
	return (heap->array)[0];
}

void encode(FILE *in_file, FILE *out_file, PAIR *pairs) 
{
	int i, ch;
	int curr_size = 0;
	unsigned buffer = 0;

	for (;;) 
	{
		ch = fgetc(in_file);
		if (ch == EOF) 
			break; 
		i = 0;
		while (i++ < (pairs[ch]).length) 
		{
			buffer <<= 1;
			buffer += (pairs[ch].arr)[i];
			curr_size++;
			if (curr_size == 8) 
			{
				fwrite(&buffer, 1, 1, out_file);
				curr_size = 0;
				buffer = 0;
			}
		}
	}
	while (curr_size < 8) 
	{
		buffer <<= 1;
		curr_size++;
	}
	rewind(in_file);
	fwrite(&buffer, 1, 1, out_file);
	fclose(out_file);
}

void build_PAIRings(NODE* root, int arr[], int top, PAIR *pairs) 
{
	if (root->left) 	
	{ 
	arr[top] = 0; 
	build_PAIRings(root->left, arr, top + 1, pairs); 
	}
    if (root->right) 
    { 
    	arr[top] = 1; 
    	build_PAIRings(root->right, arr, top + 1, pairs); 
    }
    if (!(root->left) && !(root->right)) 
    { 
    	char index = root->data;
    	for (int i = 0; i < top; i++) 
    		(pairs[index].arr)[i] = arr[i]; 
    	(pairs[index]).length = top; 
    	(pairs[index]).data = root->data;
    }
}

MIN_HEAP* scan_file(FILE *in_file) 
{
	NODE *dictionary = calloc(ASCII_SIZE, sizeof(NODE));
	MIN_HEAP *heap = calloc(1, sizeof(MIN_HEAP));
	int ch;

	for (;;) 
	{
		ch = fgetc(in_file);
		if (ch == EOF) 
			break;
		dictionary[ch].weight++;
	}
	for (ch = 0; ch < ASCII_SIZE; ch++) 
	{
		if (dictionary[ch].weight == 0) 
			continue;
		dictionary[ch].data = ch;
		add_to_heap(&(dictionary[ch]), heap);
	}
	rewind(in_file);
	return heap;
} 

int main(int argc, char *argv[]) 
{
	double c = clock();
	if (argc != 2) 
	{ 
		printf("Usage: %s <input file>\n", argv[0]); 
		return 0; 
	}
	FILE *in_file = fopen(argv[1], "r");
	FILE *out_file = fopen("out", "wb");
	int arr[ASCII_SIZE];

	// PROCEDURE 
	printf("Reading file...\n");
	MIN_HEAP *DATA_HEAP = scan_file(in_file);
	PAIR *pairs = (PAIR *)calloc(ASCII_SIZE, sizeof(PAIR));
	build_PAIRings(build_hufftree(DATA_HEAP), arr, 0, pairs);
	
	// ENCODING
	printf("Compressing...\n");
	encode(in_file, out_file, pairs);

	// CLEANUP AND PROCESSING
	FILE *read_out = fopen("out", "r");
	fseek(in_file, 0L, SEEK_END); fseek(read_out, 0L, SEEK_END);
	int before = ftell(in_file); 
	int after = ftell(read_out);
	free(DATA_HEAP);
	free(pairs);
	double curr_time = (clock() - c) / CLOCKS_PER_SEC;
	double efficiency = 100 - (((double) after / (double) before) * 100);
	printf("Compressed %i bytes into %i bytes in %f seconds \n", before, after, curr_time);
	printf("Achieved %f %% compression.\n", efficiency);
}
