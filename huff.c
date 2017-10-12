
/*
	Project: Huffman compressor
	File: huff.c
	Description: structure-packed main module, implicit implementation for speed
	Written by pongozolin, isajisai (2017)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII_SIZE			128
#define MAX_PAIRS			64

typedef struct node {
	struct node *left, *right;
	unsigned weight;
	char data;
} Node; 

typedef struct heap {
	unsigned size;
	Node *array[ASCII_SIZE];
} MinHeap;

typedef struct pair {
	int arr[ASCII_SIZE/4];
	int length;
	char data;
} Pair;

void switch_Nodes(Node **a, Node **b) {
	Node *t = *a;
	*a = *b; 
	*b = t;
}

int is_leaf(Node* root) {
    return !(root->left) && !(root->right) ;
}

MinHeap* make_MinHeap() {
	MinHeap *to_return = (MinHeap *)calloc(1, sizeof(MinHeap));
	to_return->size = 0;
	return to_return;
}

void shift_up_2(MinHeap *heap) {
	int i = 0;
	while (i < heap->size) {
		(heap->array)[i] = (heap->array)[i+2];
		(heap->array)[i+1] = (heap->array)[i+3];
		++i;
	}
}

void add_to_heap(Node *to_add, MinHeap *heap) {
	int pos = ((heap)->size)++;
	((heap)->array)[pos] = to_add;
	if (((heap)->size) > 2) {
		while (((heap)->array)[pos-1]->weight > ((heap)->array)[pos]->weight)  {
			switch_Nodes(&((heap)->array[pos-1]), &((heap)->array[pos]));
			if (--pos == 0) break;
		}
	}
	return;
}

Node* combine_Nodes(Node *lighter_Node, Node *heavier_Node) {
	Node *new_Node = (Node *)calloc(1, sizeof(Node));

	new_Node->left = lighter_Node; 
	new_Node->right = heavier_Node;
	new_Node->weight = lighter_Node->weight + heavier_Node->weight;
	return new_Node;
}

void huff_iteration(MinHeap *heap) {
	add_to_heap(combine_Nodes((heap->array)[0], (heap->array)[1]), heap);
	shift_up_2(heap);
	heap->size -= 2;
}

Node* build_hufftree(MinHeap *heap) {
	while (heap->size > 1) huff_iteration(heap);
	return (heap->array)[0];
}

void print_arr(int arr[], int count) {
	int i = 0;
	while (i < count) printf("%i", arr[i++]);
}

void print_codes(Node* root, int arr[], int top) {
    if (root->left) { arr[top] = 0; print_codes(root->left, arr, top + 1); }
    if (root->right) { arr[top] = 1; print_codes(root->right, arr, top + 1); }
    if (is_leaf(root)) { printf("%c: ", root->data); print_arr(arr, top); }
}


void encode(FILE *in_file, FILE *out_file, Pair *pairs) {
	int i_count, ch;

	for (;;) {
		ch = fgetc(in_file);
		if (ch == EOF) break;
		print_arr(pairs[ch].arr, pairs[ch].length);
	}
	fclose(out_file);
}

void build_pairings(Node* root, int arr[], int top, Pair *pairs) {
	if (root->left) { arr[top] = 0; build_pairings(root->left, arr, top + 1, pairs); }
    if (root->right) { arr[top] = 1; build_pairings(root->right, arr, top + 1, pairs); }
    if (is_leaf(root)) { 
    	char index = root->data;
    	for (int i = 0; i < top; i++) { ((pairs[index]).arr)[i] = arr[i]; }
    	(pairs[index]).length = top; (pairs[index]).data = root->data;
    }
}

MinHeap* scan_file(FILE *in_file) {
	Node *dictionary = (Node *)calloc(ASCII_SIZE+MAX_PAIRS, sizeof(Node));
	MinHeap *heap = (MinHeap *)calloc(1, sizeof(MinHeap));
	int ch;

	for (;;) {
		ch = fgetc(in_file);
		if (ch == EOF) break;
		++dictionary[ch].weight;
	}

	for (ch = 0; ch < ASCII_SIZE; ch++) {
		if (dictionary[ch].weight == 0) continue;
		dictionary[ch].data = ch;
		add_to_heap(&(dictionary[ch]), heap);
	}
	return heap;
} 

int main(int argc, char *argv[]) {
	if (argc != 3) { printf("Usage: %s <input file> <output file>\n", argv[0]); return 0; }
	
	FILE in_file = fopen(argv[1], "r");
	FILE *in_file2 = fopen(argv[1], "r");
	FILE *out_file = fopen(argv[2], "w+");
	int arr[ASCII_SIZE];

	// PROCEDURE 
	MinHeap *DATA_HEAP = scan_file(in_file);
	Pair *pairs = (Pair *)calloc(ASCII_SIZE, sizeof(Pair));
	build_pairings(build_hufftree(DATA_HEAP), arr, 0, pairs);
	
	// ENCODING
	encode(in_file2, out_file, pairs);

	free(DATA_HEAP);
}