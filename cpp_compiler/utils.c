#include <string.h>
#include "util.h"

#if 0

const char* get_line(const char *start, size_t len,  const char *pos)
{
  const char *begin = pos,
             *end = pos;

  for (; begin > start; begin--) {
    if (*begin == '\r' || *begin == '\n') {
      begin += 1;
      break;
    }
  }

  for (; end < (start + len); end++) {
    if (*end == '\r' || *end == '\n') {
      end -= 1;
      break;
    }
  }

  return strndup(begin, (size_t) (end - begin));
}

#endif
