#include "main.h"

// Base64 character set
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int base64_char_index(char c) {
    char *pos = strchr(base64_table, c);
    return (pos ? pos - base64_table : -1);
}

void base64_decode(const char *input, unsigned char *output, size_t *output_len) {
    size_t input_len = strlen(input);
    size_t i = 0, k = 0;

    for (i = 0; i < input_len; i += 4) {
        unsigned int indices[4];

        // Convert Base64 characters to indices in the table
        for (int x = 0; x < 4; x++) {
            indices[x] = base64_char_index(input[i + x]);
        }

        // Decode every 4 characters into 3 bytes
        output[k++] = (indices[0] << 2) | (indices[1] >> 4);
        if (indices[2] != -1) output[k++] = (indices[1] << 4) | (indices[2] >> 2);
        if (indices[3] != -1) output[k++] = (indices[2] << 6) | indices[3];
    }

    *output_len = k; 
}
