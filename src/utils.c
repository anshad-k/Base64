#include "main.h"

resp_key_t keys[] = {
    {
        .key_name = "int",
        .byte_len = sizeof(int),
        .pack_value = pack_int
    },
    {
        .key_name = "uint",
        .byte_len = sizeof(unsigned int),
        .pack_value = pack_uint
    },
    {
        .key_name = "short",
        .byte_len = sizeof(short),
        .pack_value = pack_short
    },
    {
        .key_name = "float",
        .byte_len = sizeof(float),
        .pack_value = pack_float
    },
    {
        .key_name = "double",
        .byte_len = sizeof(double),
        .pack_value = pack_double
    },
    {
        .key_name = "big_endian_double",
        .byte_len = sizeof(double),
        .pack_value = pack_big_endian_double    
    }, 
    {
        .key_name = NULL,
        .byte_len = 0,
        .pack_value = NULL
    }
};

void pack_int(const unsigned char* buffer, size_t byte_len, char* res_value) {
    int value = 0;
    memcpy(&value, buffer, byte_len);
    sprintf(res_value, "%d", value);
}

void pack_uint(const unsigned char* buffer, size_t byte_len, char* res_value) {
    unsigned int value = 0;
    memcpy(&value, buffer, byte_len);
    sprintf(res_value, "%u", value);
}

void pack_short(const unsigned char* buffer, size_t byte_len, char* res_value) {
    short value = 0;
    memcpy(&value, buffer, byte_len);
    sprintf(res_value, "%d", value);
}

void pack_float(const unsigned char* buffer, size_t byte_len, char* res_value) {
    float value = 0;
    memcpy(&value, buffer, byte_len);
    sprintf(res_value, "%f", value);
}

void pack_double(const unsigned char* buffer, size_t byte_len, char* res_value) {
    double value = 0;
    memcpy(&value, buffer, byte_len);
    sprintf(res_value, "%.15lf", value);
}

void pack_big_endian_double(const unsigned char* buffer, size_t byte_len, char* res_value) {
    double value = 0;
    char *bed_buffer = malloc(byte_len);
    memcpy(bed_buffer, buffer, byte_len);
    for(int i = 0; i < (int)(byte_len / 2); i++) {
        unsigned char tmp = bed_buffer[i];
        bed_buffer[i] = buffer[byte_len - i - 1];
        bed_buffer[byte_len - i - 1] = tmp;
    }
    memcpy(&value, bed_buffer, byte_len);
    sprintf(res_value, "%.15lf", value);
    free(bed_buffer);
}
