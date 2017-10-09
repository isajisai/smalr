
/*
	Project: Test zone
	File: exam.c
	Description: tests, tests, tests
	Written by pongozolin, isajisai (2017)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dog {
	struct dog *next_pupp;
	unsigned int num_woofs;
} Dog;

int count_chars(FILE *in_file) {
	int ch;
	int count = 0;
	while(1) {
		ch = fgetc(in_file);
		if(ch == EOF) break;
		count++;
	}
	return count;
}

int main(int argc, char **argv) {
	char *FILE_NAME = argv[1];
	FILE *in_file = fopen(FILE_NAME, "r");
	printf("%i\n", count_chars(in_file));

	Dog *pupp_zone = (Dog *)calloc(256, sizeof(Dog));
	pupp_zone[33].num_woofs = 25;
	pupp_zone[33].next_pupp = &pupp_zone[45];

	pupp_zone[45].num_woofs = 88;
	pupp_zone[45].next_pupp = &pupp_zone[33];

	Dog this_woofer = pupp_zone[33];
	printf("%i\n", this_woofer.num_woofs);
	printf("%i\n", (this_woofer.next_pupp)->num_woofs);
	printf("%i\n", ((this_woofer.next_pupp)->next_pupp)->num_woofs);
	printf("%i\n", pupp_zone[50].num_woofs);
	this_woofer.num_woofs++;
	printf("%i\n", this_woofer.num_woofs);
}