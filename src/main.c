#include "main.h"

extern resp_key_t keys[];

int main() {
    const char base_url[] = "https://hackattic.com/challenges/help_me_unpack";
    const char *access_token = getenv("ACCESS_TOKEN");
    ASSERT(access_token != NULL);

    char problem_url[URL_SIZE] = {0};
    char solution_url[URL_SIZE] = {0};
    unsigned char buffer[BUFFER_SIZE] = {0};
    unsigned char decoded_output[BUFFER_SIZE] = {0};
    char value[64] = {0};

    unsigned char *p_decoded_output = (unsigned char *)decoded_output;
    size_t decoded_len = 0;

    snprintf(problem_url, sizeof(problem_url), "%s/problem?access_token=%s", base_url, access_token);
    snprintf(solution_url, sizeof(solution_url), "%s/solve?access_token=%s", base_url, access_token);

    request_get(problem_url, buffer);

    printf("get resp: %s\n", buffer);
    unpack_json_string("bytes", (char *)buffer, (char *)decoded_output);
    strcpy((char *)buffer, (char *)decoded_output);
    memset(decoded_output, 0, sizeof(decoded_output));

    printf("base64 parsed data: %s\n", buffer);
    base64_decode((const char *)buffer, decoded_output, &decoded_len);
    printf("decoded data: ");
    for(int i = 0; i < (int)decoded_len; i++) {
        printf("%02x", decoded_output[i]);
    }
    printf("\n");

    memset(buffer, 0, sizeof(buffer));

    strcat((char *)buffer, "{");
    for(int i = 0; keys[i].key_name != NULL; i++) {
        keys[i].pack_value(p_decoded_output, keys[i].byte_len, value);
        p_decoded_output += 4 * (keys[i].byte_len / 4 + (int)(keys[i].byte_len % 4 != 0));
        pack_json_number(keys[i].key_name, value, (unsigned char*)buffer);
        if(keys[i + 1].key_name != NULL) {
            strcat((char *)buffer, ", ");
        }
    }
#ifdef PLAYGROUND
    strcat((char *)solution_url, "&playground=1");
#endif
    strcat((char *)buffer, "}");

    printf("\npost data: %s\n", buffer);

    request_post(solution_url, buffer);

    return 0;
}

void request_get(const char *url, unsigned char *buffer) {
    CURL *curl = curl_easy_init();
    ASSERT(curl != NULL);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

    CURLcode res = curl_easy_perform(curl);
    ASSERT(res == CURLE_OK);

    curl_easy_cleanup(curl);

}

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    unsigned char *buffer = (unsigned char *) userdata;
    strncat((char *)buffer, ptr, total_size);
    return total_size;
}

void request_post(const char *url, unsigned char *json_data) {
    CURL *curl = curl_easy_init();
    ASSERT(curl != NULL);

    char post_resp[BUFFER_SIZE / 4] = {0};

    curl_easy_setopt(curl, CURLOPT_URL, url);
    
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, post_resp);


    CURLcode res = curl_easy_perform(curl);
    ASSERT(res == CURLE_OK);

    printf("post resp: %s\n", post_resp);

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
}

void unpack_json_string(char *key, char *input, char* output) {
    char *start = strstr(input, key);
    *output = '\0';
    if(start == NULL) {
        return;
    }
    start += strlen(key) + 2;
    char *val_start = strchr(start, '\"');
    if(val_start == NULL) {
        return;
    }
    val_start++;
    char *val_end = strchr(val_start, '\"');
    if(val_end == NULL) {
        return;
    }
    strncpy(output, val_start, val_end - val_start);
}

void pack_json_number(const char *key, const char *value, unsigned char *buffer) {
    strcat((char *)buffer, "\"");
    strcat((char *)buffer, key);
    strcat((char *)buffer, "\": ");
    strcat((char *)buffer, value);
}