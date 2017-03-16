
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curl/curl.h"


// callback example: https://github.com/curl/curl/blob/master/docs/examples/getinmemory.c

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}


int main(){



    struct MemoryStruct chunk;

      chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
      chunk.size = 0;    /* no data at this point */



    curl_global_init(CURL_GLOBAL_ALL);

    CURL *curl=curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "http://ici.radio-canada.ca/");
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,WriteMemoryCallback);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void*)&chunk);
    CURLcode result;
    result=curl_easy_perform(curl);

    if(result != CURLE_OK) {
       fprintf(stderr, "curl_easy_perform() failed: %s\n",
               curl_easy_strerror(result));
     }
     else {

        printf("%s | ", chunk.memory);
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;

}
