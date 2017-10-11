/*
	Project: Huff Utils Tester
	File: test2.c
	Description: testing huff utils (huff.h)
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

// heap methods
MinHeap* make_MinHeap() {
	MinHeap *to_return = (MinHeap *)calloc(1, sizeof(MinHeap));
	to_return->size = 0;
	return to_return;
}

void switch_Nodes(Node **a, Node **b) {
	Node *t = *a;
	*a = *b;
	*b = t;
}

void add_to_heap(Node **to_add, MinHeap **heap) {
	int pos = (*heap)->size;
	(((*heap)->array)[pos]) = *to_add;
	++((*heap)->size);
	if (((*heap)->size) > 2) {
		while (((*heap)->array)[pos-1]->weight > ((*heap)->array)[pos]->weight)  {
			switch_Nodes(&((*heap)->array[pos-1]), &((*heap)->array[pos]));
			if (--pos == 0) break;
		}
	}
	return;
}

void naive_print(MinHeap *heap, int depth) {
	int i = 0;
	while (i < depth) {
		printf("%i\n", (*((heap->array)[i])).weight);
		++i;
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

int main() {
	Node *array = (Node *)malloc(5*sizeof(Node));
	array[0].weight = 256; array[0].data = 'c';
	array[1].weight = 128; array[1].data = 'a';
	array[2].weight = 64; array[2].data = 't';
	array[3].weight = 78; array[2].data = 'h';
	array[4].weight = 12; array[2].data = 'e';

	Node *the_c = &array[0];
	Node *the_a = &array[1];
	Node *the_t = &array[2];
	Node *the_h = &array[3];
	Node *the_e = &array[4];

	MinHeap *THE_HEAP = make_MinHeap();
	add_to_heap(&the_c, &THE_HEAP);
	add_to_heap(&the_a, &THE_HEAP);
	add_to_heap(&the_t, &THE_HEAP);
	add_to_heap(&the_h, &THE_HEAP);
	add_to_heap(&the_e, &THE_HEAP);
	printf("%i\n", THE_HEAP->size);
	naive_print(THE_HEAP, 5);
	free(array);
	return 0;
}


