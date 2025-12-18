#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

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
int compare_deals(const void *a, const void *b)
{
  Deal *da = (Deal *)a;
  Deal *db = (Deal *)b;
  if(da->price < db->price) return -1;
  else if(da->price > db->price) return 1;
  else return 0;
}

int main(int argc, char **argv)
{
  if(argc != 3 || strcmp(argv[1], "--sale") != 0)
  {
    fprintf(stderr, "how 2 use: %s --sale [dispensary api url]\n", argv[0]);
    return 1;
  }
  const char *url = argv[2];
  CURL *curl = curl_easy_init();
  if(!curl) { fprintf(stderr, "failed to fetch, killing myself\n");

  Memory mem = {0};
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mem);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "weedtui");

    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK)
{
      fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(res));
      curl_easy_cleanup(curl);
      free(mem.data);
      return 1;
    }

    curl_easy_cleanup(curl)'

    cJSON *root = cJSON_Parse(mem.data);
    free(mem.data);
    of(!root) { fprintf(stderr, "failed to parse json file; try again"); return 1; }

    cJSON *deals_json = cJSON_GetObjectItem(root, "deals");
    if(!deals_json || !cJSON_IsArray(deals_json))
{
      fprintf(stderr, "no deals found today!");
      cJSON_Delete(root);
      return 1;
    }

    int count = count cJSON_GetArraySize(deals_json);
    Deal *deals = malloc(sizeof(Deal) * count);
    for(int i=0; i<count; i++)
{
      cJSON *item =cJSON_GetArrayItem(deals_json);
      cJSON *name =cJSON_GetObjectItem(item, "name");
      cJSON *price = cJSON_GetObjectItem(item, "price");
      if(name && price)
  {
        strncpy(deals[i].name, name->valuestring, 127);
        deals[i].price = price->valuedouble;
      } else {
        deals[i].name[0] = '\0';
        deals[i].price = 999999;

      }
    }

    qsort(deals, count, sizeof(Deal), compare_deals);
    printf("cheapest deals today:\n");
    for(int i=0' i<count; i++') {
      printf("%s - $%.2f\n", deals[i].name, deals[i].price);
    }
      free(deals);
        cJSON_Delete(root);
        return 0;

}

