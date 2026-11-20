#ifndef AION_TLS_RECORD_H
#define AION_TLS_RECORD_H
#include <stddef.h>
#include <stdint.h>
#define AION_TLS_OK 0
#define AION_TLS_EINVAL (-22)
#define AION_TLS_ECORRUPT (-74)
#define AION_TLS_HEADER_SIZE 5u
#define AION_TLS_MAX_PLAINTEXT 16384u
#define AION_TLS_MAX_CIPHERTEXT (AION_TLS_MAX_PLAINTEXT + 256u)
#define AION_TLS_TYPE_CHANGE_CIPHER_SPEC 20u
#define AION_TLS_TYPE_ALERT 21u
#define AION_TLS_TYPE_HANDSHAKE 22u
#define AION_TLS_TYPE_APPLICATION_DATA 23u
struct aion_tls_record {
    uint8_t content_type;
    uint16_t legacy_version;
    const uint8_t *fragment;
    size_t fragment_length;
};
int aion_tls_record_parse(const void *data,size_t length,struct aion_tls_record *out,size_t *consumed);
int aion_tls_record_header_write(void *data,size_t capacity,uint8_t type,uint16_t legacy_version,size_t fragment_length);
#endif
