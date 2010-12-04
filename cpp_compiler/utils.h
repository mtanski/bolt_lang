#ifndef __UTILS_H__
#define __UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* stringify macro */
#define __STRINGIFY(str) #str
#define STRINGIFY(str) __STRINGIFY(str)

const char* get_line(const char *start, size_t len,  const char *pos);

#ifdef __cplusplus
}
#endif

#endif /* __UTILS_H__ */
