#ifndef __logger_h__
#define __logger_h__

#include <stdio.h>

/*! CPP guard */
#ifdef __cplusplus
extern "C" {
#endif

typedef void(uart_init_t)(void);

#if (DEBUG_LOG > 0)
#define FERROR(...)                                          \
	{                                                    \
		printf("%s:%d ERROR: ", __FILE__, __LINE__); \
		printf(__VA_ARGS__);                         \
		printf("\r\n");                              \
	}
#else
#define FERROR(...)
#endif

#if (DEBUG_LOG > 1)
#define FWARNING(...)                                          \
	{                                                      \
		printf("%s:%d WARNING: ", __FILE__, __LINE__); \
		printf(__VA_ARGS__);                           \
		printf("\r\n");                                \
	}
#else
#define FWARNING(...)
#endif

#if (DEBUG_LOG > 2)
#define FINFO(...)                                          \
	{                                                   \
		printf("%s:%d INFO: ", __FILE__, __LINE__); \
		printf(__VA_ARGS__);                        \
		printf("\r\n");                             \
	}
#else
#define FINFO(...)
#endif

void retarget_init(UART_HandleTypeDef *huart, uart_init_t *init);

#ifdef __cplusplus
}
#endif /* End of CPP guard */

#endif //__logger_h__
