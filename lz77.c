
/*
	Project: LZ77 compressor (DEMO)
	File: lz77.c
	Description: main module
	Written by pongozolin, isajisai (2017)
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define WINDOW_SIZE    4096
#define LOOKAHEAD_SIZE 128

/* Generic data types such as "unsigned long" may differ in size on different
architectures (such as 32 bits on x86 vs 64 bits on x64), so we'll use data
types of that will always be the same size, which increases portability. */
/* For most uses, a 16-bit Offset is size-sufficient for a WINDOW_SIZE of 4 KiB,
and an 8-bit Length is size-sufficient for a LOOK_AHEAD of 16 B. */
typedef uint16_t Offset;
typedef uint8_t  Length;
typedef uint8_t  Symbol;

typedef struct
{
	Offset offset;
	Length length;
	Symbol nextsym;
} Triplet;

Symbol slider[WINDOW_SIZE + LOOKAHEAD_SIZE];

static int compress(char *, char *);
static int lz77(FILE *, FILE *);
static void find_match(Triplet *, Symbol *, unsigned, Symbol *, unsigned);

int
main(int argc, char *argv[])
{
	char outname[128];
	int i;
	int retval = 0;

	if(argc < 3) {
		printf("Usage: %s <input file> <output file>\n", argv[0]);
		return 0;
	}

	return compress(argv[1], argv[2]);
}

static int
compress(char *in, char *out)
{
	FILE *fin, *fout;
	int retval = 0;

	fin = fopen(in, "rb");
	if(fin == NULL) {
		fprintf(stderr, "Could not open input file '%s'\n", in);
		return -1;
	}

	fout = fopen(out, "wb");
	if(fout == NULL) {
		fprintf(stderr, "Could not open output file '%s'\n", out);
		fclose(fin);
		return -1;
	}

	printf("Compressing '%s' to '%s'...\n", in, out);

	retval = lz77(fin, fout);

	fclose(fin);
	fclose(fout);

	return retval;
}

static int
lz77(FILE *fin, FILE *fout)
{
	unsigned wsize = 0, lsize = 0;
	size_t read;
	int c;

	read = fread(slider + WINDOW_SIZE, 1, LOOKAHEAD_SIZE, fin);
	if(read != LOOKAHEAD_SIZE && ferror(fin)) {
		fprintf(stderr, "Error reading from the input file!\n");
		return -1;
	}

	lsize = read;

	while(lsize > 0) {
		Triplet triplet;
		unsigned tlength, slidecount;

		find_match(&triplet, slider, wsize, slider + WINDOW_SIZE, lsize);
		fprintf(fout, "%u %u '%c'\n", triplet.offset, triplet.length, triplet.nextsym >= 0x20 ? triplet.nextsym : ' ');

		tlength = triplet.length + 1;
		slidecount = tlength;

		if(wsize < WINDOW_SIZE) {
			unsigned lesser;

			lesser = WINDOW_SIZE - wsize < tlength ? WINDOW_SIZE - wsize : tlength;
			memmove(slider + wsize, slider + WINDOW_SIZE, lesser);
			memmove(slider + WINDOW_SIZE, slider + WINDOW_SIZE + lesser, LOOKAHEAD_SIZE - lesser);
			wsize += lesser;
			lsize -= lesser;
			slidecount -= lesser;
		}

		if(slidecount > 0) {
			/* slide the entire slider */
			memmove(slider, slider + slidecount, WINDOW_SIZE + LOOKAHEAD_SIZE - slidecount);
			lsize -= slidecount;
		}

		read = fread(slider + WINDOW_SIZE + lsize, 1, tlength, fin);
		if(read != tlength && ferror(fin)) {
			fprintf(stderr, "Error reading from the input file!\n");
			return -1;
		}

		lsize += read;
	}

	return 0;
}

static void
find_match(Triplet *tbuf, Symbol *window, unsigned wsize, Symbol *lookahead, unsigned lsize)
{
	unsigned matchpos, pos;
	unsigned matchlen = 0;
	int update = 0;

	/* default case: assume the lookahead buffer starts with an unseen symbol */
	tbuf->offset = 0;
	tbuf->length = 0;
	tbuf->nextsym = *lookahead;

	/* when we have no window, the default case holds true */
	if(wsize == 0) return;

	pos = wsize - 1;

	for(;;) {
		unsigned wpos = pos, lpos = 0, mpos;
		unsigned len = 0;
		int wposzero = 0;

		while(window[wpos] != lookahead[lpos]) {
			if(wpos == 0) {
				wposzero = 1;
				break;
			}

			--wpos;
		}

		if(wposzero) break; /* no more searching */

		mpos = wpos;

		while(lpos < lsize - 1 && wpos < wsize && window[wpos] == lookahead[lpos]) {
			++wpos;
			++lpos;
			++len;
		}

		if(len > matchlen) {
			update = 1;
			matchlen = len;
			matchpos = mpos;
			/* if we get a max-length match, then we need not continue searching */
			if(len == lsize - 1) break;
		}

		if(mpos == 0) break;

		pos = mpos - 1;
	}

	if(update) {
		tbuf->offset  = wsize - matchpos;
		tbuf->length  = matchlen;
		tbuf->nextsym = lookahead[matchlen];
	}
}
