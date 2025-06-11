#include "rkp_error.h"

#include <stdio.h>
#include <stdlib.h>

FILE *log_file_ptr = NULL;
char *log_file_name = NULL;

int rkp_error_open_log_file(char *filename) {
	if(log_file_ptr) {
		fprintf(stderr, "couldn't open log_file_ptr: \'%s\'. existing log file not closed...\n", filename);
		return 1;
	}
	log_file_ptr = fopen(filename, "w");
	if(!log_file_ptr) {
		fprintf(stderr, "couldn't open log_file_ptr: \'%s\'. fopen failed...\n", filename);
		return 2;
	}
	printf("log_file_ptr successfully set: \'%s\'@%p\n", filename, (void *)log_file_ptr);
	log_file_name = filename; // passed filename must be null-terminated
	return 0;
}

int rkp_error_close_log_file(void) {
	if(!log_file_ptr) {
		fprintf(stderr, "couldn't close log_file_ptr: not currently set\n");
		return 1;
	}
	fclose(log_file_ptr);
	log_file_ptr = NULL;
	log_file_name = NULL;
	printf("log_file_ptr successfully closed: \'%s\'\n", log_file_name);
	return 0;
}
