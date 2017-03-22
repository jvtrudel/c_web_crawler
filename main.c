
#define ZF_LOG_LEVEL ZF_LOG_INFO
#include <zf_log.h>

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


// tokenizer
//  http://stackoverflow.com/questions/19479983/tokenize-a-string-in-c-with-strtok-include-digits-as-delimiters
void tokenize(char text []) {
    char *word;
   // char text[] = "Some - text, from stdin. We'll see! what happens? 4ND 1F W3 H4V3 NUM83R5?!?";
    int nbr_words = 0;

    word = strtok(text, " ,.-!?()<>\\/;:\n\"'+{}[]\t=\r");

    while (word != NULL) {
    printf("%s\n", word);
    word = strtok(NULL, " ,.-!?()<>\\/;:\n\"'+{}[]\t=\r");
    nbr_words += 1;
    }
}

int main(){



    struct MemoryStruct chunk;

      chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
      chunk.size = 0;    /* no data at this point */


    ZF_LOGI("curl initialisation");
    curl_global_init(CURL_GLOBAL_ALL);

    CURL *curl=curl_easy_init();

    ZF_LOGI("set curl options \n url: %s","https://jvtrudel.wordpress.com/");
    curl_easy_setopt(curl, CURLOPT_URL, "https://jvtrudel.wordpress.com/");
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,WriteMemoryCallback);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void*)&chunk);

    ZF_LOGI("proceed to curl request");
    CURLcode result;
    result=curl_easy_perform(curl);

    ZF_LOGI("Result post-processing");
    if(result != CURLE_OK) {
       fprintf(stderr, "curl_easy_perform() failed: %s\n",
               curl_easy_strerror(result));
     }
     else {

        //printf("%s | ", chunk.memory);
        tokenize(chunk.memory);
    }

    ZF_LOGI("curl finalization");
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;

}
