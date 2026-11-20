#ifndef AION_ETHERNET_H
#define AION_ETHERNET_H
#include <stddef.h>
#include <stdint.h>
#define AION_ETHERNET_OK 0
#define AION_ETHERNET_EINVAL (-22)
#define AION_ETHERNET_ECORRUPT (-74)
#define AION_ETHERNET_HEADER_SIZE 14u
#define AION_ETHERNET_VLAN_HEADER_SIZE 18u
#define AION_ETHERNET_MAX_FRAME 1522u
#define AION_ETHERTYPE_VLAN 0x8100u
#define AION_ETHERTYPE_QINQ 0x88a8u
struct aion_ethernet_frame {
    uint8_t destination[6];
    uint8_t source[6];
    uint16_t ethertype;
    uint16_t vlan_id;
    uint8_t priority;
    uint8_t tagged;
    const uint8_t *payload;
    size_t payload_length;
};
int aion_ethernet_parse(const void *buffer,size_t length,struct aion_ethernet_frame *out);
int aion_ethernet_build(void *buffer,size_t capacity,const uint8_t destination[6],const uint8_t source[6],uint16_t ethertype,const void *payload,size_t payload_length,size_t *written);
#endif
