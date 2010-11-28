#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "tokenizer.hpp"


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
  if (argc < 1)
    return 1;

  const char *src = (char *) mmap_file(argv[0]);

  token_list tokens = tokenizer(src);

  printf("Parsed elements: %i", tokens.size()); 

  return 0;
}
