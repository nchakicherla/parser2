#include "rkp_arena.h"

#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdalign.h>

#define MEMORY_HOG_FACTOR 8
#define DEF_BLOCK_SIZE 4096

static Block *newInitBlock(size_t block_size) {
	
	Block *block =  malloc(sizeof(Block));
	if(!block) {
		fprintf(stderr, "block alloc failed! exiting...\n");
		exit(1);
	}

	block->data = malloc(block_size);
	if(!block->data) {
		fprintf(stderr, "block data alloc failed! exiting...\n");
		exit(2);
	}

	block->data_size = block_size;
	block->next = NULL;
	
	return block;
}

int initArena(Arena *arena) {
	size_t block_size = DEF_BLOCK_SIZE;

	arena->first_block = newInitBlock(block_size);

	arena->bytes_used = 0;
	arena->bytes_allocd = sizeof(Arena) + sizeof(Block) + block_size;
	arena->next_free = arena->first_block->data;
	arena->next_free_size = arena->first_block->data_size;
	arena->last_block_size = block_size;
	arena->last_block = arena->first_block;
	return 0;
}

int termArena(Arena *arena) {

	Block *curr = arena->first_block;
	Block *next = NULL;

	while(curr) {
		next = curr->next;
		free(curr->data);
		free(curr);
		curr = next;
	}
	return 0;
}

int resetArena(Arena *arena) {

	Block *curr = arena->first_block;
	Block *next = NULL;

	while(curr) {
		next = curr->next;
		free(curr->data);
		free(curr);
		curr = next;
	}

	arena->first_block = newInitBlock(arena->last_block_size);
	arena->last_block = arena->first_block;

	arena->next_free = arena->first_block->data;
	arena->next_free_size = arena->first_block->data_size;

	arena->bytes_used = 0;
	arena->bytes_allocd = sizeof(Arena) + sizeof(Block) + arena->last_block_size;
	return 0;
}

void *palloc(Arena *arena, size_t size, size_t alignment) {
	
	// bump up per alignment
	size_t current = (size_t)arena->next_free;
	size_t aligned = (current + alignment - 1) & ~(alignment - 1);
	size_t padding = aligned - current;

	if(arena->next_free_size < size + padding) {
		Block *last_block = arena->last_block;
		Block *new_block = NULL;

		size_t new_block_size = arena->last_block_size;

		while(new_block_size < size * MEMORY_HOG_FACTOR) {
			new_block_size = new_block_size * 2;
		}

		new_block = newInitBlock(new_block_size);
		if(!new_block) {
			return NULL;
		}
		last_block->next = new_block;
		last_block = new_block;

		arena->last_block = new_block;
		arena->last_block_size = new_block_size;
		arena->next_free = (char *)last_block->data + size;
		arena->next_free_size = new_block_size - size;

		arena->bytes_used += size;
		arena->bytes_allocd += sizeof(Block) + new_block_size;

		return last_block->data;
	}

	void *output = (void *)aligned;
	arena->next_free = (void *)(aligned + size);
	arena->next_free_size  = arena->next_free_size - (size + padding);

	arena->bytes_used += size;

	return output;
}

void *pzalloc(Arena *arena, size_t size, size_t alignment) {
	void* output = palloc(arena, size, alignment);
	for(size_t i = 0; i < size; i++) {
		((char *)output)[i] = '\0';
	}
	return output;
}

void *pGrowAlloc(void *ptr, size_t old_size, size_t new_size, Arena *arena) {
	void *output_ptr = palloc(arena, new_size, 32); // use default alignment of 32
	memcpy(output_ptr, ptr, old_size);
	return output_ptr;
}

char *pNewStr(char *str, Arena *arena) {
	
	char *output = NULL;
	size_t len = strlen(str);

	output = palloc(arena, len + 1, alignof(char));

	for(size_t i = 0; i < len; i++) {
		output[i] = str[i];
	}
	output[len] = '\0';
	return output;
}

size_t getBytesUsed(Arena *arena) {
	return arena->bytes_used;
}

size_t getBytesAllocd(Arena *arena) {
	return arena->bytes_allocd;
}

void printArenaInfo(Arena *arena) {
	printf("\nMEMPOOL INFO - \n");
	printf("\tUSED: %zu, (%f MB)\n", getBytesUsed(arena), (double)getBytesUsed(arena) / (1024 * 1024));
	printf("\tALLOCD: %zu, (%f MB)\n", getBytesAllocd(arena), (double)getBytesAllocd(arena) / (1024 * 1024));
	return;
}
