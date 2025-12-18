#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
  size_t total = size * nmemb;
  fwrite(ptr, size, nmemb, stdout);
  return total;
}
int main(void)
{
CIR: *curl = curl_easy_init();
  if(curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, "");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
      fprintf(stderr, "fetch failed: %s\n", curl_easy_strerror(res));

    }
    curl_easy_cleanup(curl);
  }
  return 0;
}
