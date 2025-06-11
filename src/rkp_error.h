#ifndef RKP_ERROR_H
#define RKP_ERROR_H

#include <stddef.h>
#include <inttypes.h>

typedef enum { // ultimately determined by flags but general conventions provided
	RKP_ERR_SEV_DEBUG, // stderr, log
	RKP_ERR_SEV_INFO, // stdout, log
	RKP_ERR_SEV_ERROR, // stderr, stdout, log
	RKP_ERR_SEV_FATAL // stderr, stdout, log
} e_rkp_error_severity;

typedef enum {
	RKP_ERR_TYPE_NONE,
	RKP_ERR_TYPE_ARG,
	RKP_ERR_TYPE_IO,
	RKP_ERR_TYPE_MEM,
	RKP_ERR_TYPE_MATH
} e_rkp_error_type;

typedef enum {
	RKP_ERR_CTX_EXTERNAL,
	RKP_ERR_CTX_LEXER,
	RKP_ERR_CTX_PARSER,
	RKP_ERR_CTX_TRANSPILER,
	RKP_ERR_CTX_VM,
	RKP_ERR_CTX_RUNTIME
} e_rkp_error_context;

typedef enum {
	RKP_ERR_FLAG_NONE		= 0,
	RKP_ERR_FLAG_USER_MSG	= 1 << 0,
	RKP_ERR_FLAG_LOG_ONLY	= 1 << 1,
	RKP_ERR_FLAG_DEV_ONLY	= 1 << 2,
	RKP_ERR_FLAG_CRITICAL	= 1 << 3,
	RKP_ERR_FLAG_HALT		= 1 << 4
} e_rkp_error_flags;

typedef struct s_rkp_error {
	e_rkp_error_severity 	severity;
	e_rkp_error_type 		type;
	e_rkp_error_context 	context;
	const char 				*msg;
	const char 				*file;
	size_t line;
	size_t column;
	int code;
	uint32_t flags;
} rkp_error;

int rkp_error_open_log_file(char *filename);
int rkp_error_close_log_file(void);

#endif
