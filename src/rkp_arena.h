#ifndef RKP_ARENA_H
#define RKP_ARENA_H

#include <stdlib.h>
#include <string.h>

struct s_Block;

typedef struct s_Arena {
	size_t bytes_used;
	size_t bytes_allocd;

	void *next_free;
	size_t next_free_size;
	size_t last_block_size;
	struct s_Block *last_block;
	struct s_Block *first_block;
} Arena;

typedef struct s_Block {
	void *data;
	size_t data_size;
	struct s_Block *next;
} Block;

int initArena(Arena *arena);
int termArena(Arena *arena);
int resetArena(Arena *arena);
void *palloc(Arena *arena, size_t size, size_t alignment);
void *pzalloc(Arena *arena, size_t size, size_t alignment);
void *pGrowAlloc(void *ptr, size_t old_size, size_t new_size, Arena *arena);
char *pNewStr(char *str, Arena *arena);
size_t getBytesUsed(Arena *arena);
size_t getBytesAllocd(Arena *arena);
void printArenaInfo(Arena *arena);

#endif // RKP_ARENA_H
