#include "rkp_error.h"

#include <stdio.h>
#include <stdlib.h>

FILE *log_output_file = NULL;
char *log_output_filename = NULL;

int rkp_error_set_log_file(char *filename) {
	if(log_output_file) {
		fprintf(stderr, "couldn't open log_output_file: \'%s\'. log_output_file not closed...\n", filename);
		return 1;
	}
	log_output_file = fopen(filename, "w");
	if(!log_output_file) {
		fprintf(stderr, "couldn't open log_output_file: \'%s\'. fopen failed...\n", filename);
		fprintf(stderr, "exiting...\n");
		exit(1);
	}
	printf("log_output_file successfully set: \'%s\'\n", filename);
	log_output_filename = filename;
	return 0;
}

int rkp_error_close_log_file(void) {
	if(!log_output_file) {
		fprintf(stderr, "couldn't close log_output_file: not currently set\n");
		return 1;
	}
	fclose(log_output_file);
	log_output_file = NULL;
	log_output_filename = NULL;
	printf("log_output_file successfully closed: \'%s\'\n", log_output_filename);
	return 0;
}
