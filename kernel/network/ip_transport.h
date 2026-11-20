#ifndef AION_IP_TRANSPORT_H
#define AION_IP_TRANSPORT_H
#include <stddef.h>
#include <stdint.h>
#define AION_NET_OK 0
#define AION_NET_EINVAL (-22)
#define AION_NET_ECORRUPT (-74)
#define AION_IPPROTO_TCP 6u
#define AION_IPPROTO_UDP 17u
struct aion_ipv4_packet {
    uint8_t source[4], destination[4], protocol, ttl;
    uint16_t identification, fragment_offset;
    uint8_t more_fragments;
    const uint8_t *payload;
    size_t payload_length;
};
struct aion_udp_segment {
    uint16_t source_port, destination_port, checksum;
    const uint8_t *payload;
    size_t payload_length;
};
struct aion_tcp_segment {
    uint16_t source_port, destination_port, window, checksum;
    uint32_t sequence, acknowledgment;
    uint8_t flags;
    const uint8_t *payload;
    size_t payload_length;
};
int aion_ipv4_parse(const void *data,size_t length,struct aion_ipv4_packet *out);
int aion_udp_parse(const void *data,size_t length,struct aion_udp_segment *out);
int aion_tcp_parse(const void *data,size_t length,struct aion_tcp_segment *out);
#endif
