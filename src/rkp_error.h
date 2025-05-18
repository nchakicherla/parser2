#ifndef RKP_ERROR_H
#define RKP_ERROR_H

#include <stddef.h>

typedef enum {
	RKP_ERR_TYPE_NONE,
	RKP_ERR_TYPE_ARG,
	RKP_ERR_TYPE_IO,
	RKP_ERR_TYPE_MEM,
	RKP_ERR_TYPE_MATH
} RKPErrorType;

typedef enum {
	RKP_ERR_CTX_EXTERNAL,
	RKP_ERR_CTX_LEXER,
	RKP_ERR_CTX_PARSER,
	RKP_ERR_CTX_TRANSPILER,
	RKP_ERR_CTX_VM,
	RKP_ERR_CTX_RUNTIME
} RKPErrorContext;

typedef enum {
	RKP_SEV_DEBUG,
	RKP_SEV_INFO,
	RKP_SEV_ERROR,
	RKP_SEV_FATAL
} RKPSeverity;

typedef struct {
	RKPSeverity severity;
	RKPErrorType type;
	RKPErrorContext context;
	const char *msg;
	const char *file;
	size_t line;
	size_t column;
	int code;
} Error;

void rkp_error_test(void);

#endif
