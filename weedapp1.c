#include <stdio.h>


typedef struct
{
  char name[128];
  double price;
} Deal;
typedef struct
{
  char *data;
  size_t size;
} Memory;
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
  size_t total = size * nmemb;
  Memory *mem = (Memory *)userdata;
  char *tmp = realloc(mem->data, mem->size + total + 1);
  if(!tmp) return 0;
  mem->data = tmp;
  memcpy(&(mem->data[mem->size]), ptr, total);
  mem->size += total;
  mem->data[mem->size] = '\0';
  return total;
}

