#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

size_t get_file_size(const char *filename) {
  struct stat st;
  stat(filename, &st);
  return st.st_size;
}

const int match(const char* line, const size_t length, const char* pattern) {
  // no pattern or all matched
  if (pattern == NULL || pattern[0] == 0)
    return 0;
  // no more string to match
  if (length <= 0)
    return 0;
  if (pattern[0] == line[0] || pattern[0] == '.')
    return match(line + 1, length - 1, pattern + 1);
  return pattern[0] - line[0];
}

const char* find_line_start(const char *buffer, const size_t length) {
  const char *ptr = buffer + (length - 1);
  while (ptr >= buffer) {
    if (*ptr == '\n')
      return ptr + 1;
    ptr--;
  }
  return buffer;
}

const int find_line_size(const char *buffer, const size_t length) {
  int size = 0;
  while (size < length && buffer[size] != '\n') {
    size++;
  }
  return size;
}

const char* bisect_start(const char* data, const size_t data_size, const char *pattern) {
  if (data_size <= 1)
    return NULL;

  size_t mid = data_size / 2;
  // rewind to line start
  const char *line_start = find_line_start(data, mid);
  // compute corrected mid offset
  mid = line_start - data;
  // find line end
  const int line_size = find_line_size(line_start, data_size - mid);
  // compare line with pattern
  const int compare = match(line_start, line_size, pattern);
  printf("bisect_start(%p, %ld, %s) mid: %ld, line: %.50s (%d)\n", data, data_size, pattern, mid, line_start, compare);

  if (compare > 0) { // line is smaller than pattern, search in next lines
    return bisect_start(line_start + line_size + 1, data_size - mid - line_size -1, pattern);
  } else { // line is higher or equal to pattern, search in previous lines
    const char* start = bisect_start(data, mid, pattern);
    // if there is no match in previous lines but current line is matching, it's the start !
    if (compare == 0 && start == NULL)
      start = line_start;
    return start;
  }
}

const char* bisect_end(const char* data, const size_t data_size, const char *pattern) {
  if (data_size <= 1)
    return NULL;

  size_t mid = data_size / 2;
  // rewind to line start
  const char *line_start = find_line_start(data, mid);
  // compute corrected mid offset
  mid = line_start - data;
  // find line end
  const int line_size = find_line_size(line_start, data_size - mid);
  // compare line with pattern
  const int compare = match(line_start, line_size, pattern);
  printf("bisect_end(%p, %ld, %s) mid: %ld, line: %.50s (%d)\n", data, data_size, pattern, mid, line_start, compare);

  if (compare < 0) { // line is higher than pattern, search in previous lines
    return bisect_end(data, mid, pattern);
  } else { // line is lower or equal to pattern, search in next lines
    const char* end = bisect_end(line_start + line_size + 1, data_size - mid - line_size - 1, pattern);
    // if there is no match in next lines but current line is matching, it's the end !
    if (compare == 0 && end == NULL)
      end = line_start + line_size + 1;
    return end;
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: bisect <file> <pattern>\n");
    return 1;
  }
  const char *filename = argv[1];
  const char *pattern = argv[2];
  size_t file_size = get_file_size(filename);
  printf("bisecting %s (%.3g MB)\n", filename, file_size / 1024. / 1024.);
  int fd = open(filename, O_RDONLY, 0);
  if (fd == -1) {
    fprintf(stderr, "Can't open file\n");
    return 1;
  }
  char *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
  if (data == NULL) {
    fprintf(stderr, "Can't mmap file into memory\n");
    return 1;
  }
  const char *start = bisect_start(data, file_size, pattern);
  printf("Starts at %ld: %.50s\n", start - data, start);
  const char *end = bisect_end(start, file_size - (start - data), pattern);
  printf("Ends at %ld: %.50s\n", end - data, end);
  write(1, start, end - start);
  munmap(data, file_size);
  close(fd);
  return 0;
}