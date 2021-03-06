#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "tokenizer.hpp"
#include "parser.hpp"

static
void error(int err, const char *format, ...)
{
  va_list ap;

va_start(ap, format);

  vfprintf(stderr, format, ap);
  fprintf(stderr, "\n");

va_end(ap);

  exit(err);
}


void* mmap_file(const char* path)
{
  struct stat   finfo;
  int           fd;

  if ((fd = open(path, O_RDONLY)) == -1)
    return NULL;

  if (fstat(fd, &finfo) == -1)
    return NULL;

  return mmap(0, finfo.st_size, PROT_READ, MAP_PRIVATE, fd, 0); 
}

int main(int argc, char* argv[])
{
  if (argc < 2)
    error(1, "Not enought arguments");

  const char *src = (char *) mmap_file(argv[1]);

  if (src == NULL)
    error(1, "Unable to open file: %s", argv[1]);

  token_list tokens = tokenizer(src);

  dump_tokens(tokens);

  parser_bolt parser(tokens);

  try {
    parser.run();
  } catch (exception &err) {
    std::string txt = err.what_long();

    error(2, "Error: %s", txt.c_str());
  } catch (const char *msg) {
    auto foo = parser.state_get().get();
    printf("Error loc: %.*s\n", 20, foo->value);
    error(2, "Error: %s", msg);
  }

  return 0;
}

