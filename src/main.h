#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define ASSERT(x) if (!(x)) { fprintf(stderr, "Assertion failed: " #x "\n"); exit(1); }

#define URL_SIZE 256
#define BUFFER_SIZE 1024

#define PROBLEM_START 11
#define PROBLEM_END 54

typedef void (*pack_value_t)(const unsigned char* buffer, size_t byte_len, char* res_value);

struct _resp_key_t{
    const char* key_name;
    size_t byte_len;
    pack_value_t pack_value;
};

typedef struct _resp_key_t resp_key_t;

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata);
void request_get(const char *url, unsigned char *buffer);
void request_post(const char *url, unsigned char *json_data);

void base64_decode(const char *input, unsigned char *output, size_t *output_len);
void unpack_json_string(char *key, char *input, char* output);
void pack_json_number(const char *key, const char *value, unsigned char *buffer);

void pack_int(const unsigned char* buffer, size_t byte_len, char* res_value);
void pack_uint(const unsigned char* buffer, size_t byte_len, char* res_value);
void pack_short(const unsigned char* buffer, size_t byte_len, char* res_value);
void pack_float(const unsigned char* buffer, size_t byte_len, char* res_value);
void pack_double(const unsigned char* buffer, size_t byte_len, char* res_value);
void pack_big_endian_double(const unsigned char* buffer, size_t byte_len, char* res_value);


#endif