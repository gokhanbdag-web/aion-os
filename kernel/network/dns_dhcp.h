#ifndef AION_DNS_DHCP_H
#define AION_DNS_DHCP_H
#include <stddef.h>
#include <stdint.h>
#define AION_DNS_DHCP_OK 0
#define AION_DNS_DHCP_EINVAL (-22)
#define AION_DNS_DHCP_ECORRUPT (-74)
#define AION_DNS_MAX_NAME 255u
struct aion_dns_header {
    uint16_t id, flags, questions, answers, authorities, additionals;
};
struct aion_dhcp_offer {
    uint32_t transaction_id;
    uint8_t offered_ip[4], server_ip[4], client_mac[6];
    uint8_t message_type;
    uint8_t subnet_mask[4], router[4], dns_server[4], server_identifier[4];
    uint8_t has_subnet_mask,has_router,has_dns_server,has_server_identifier;
};
int aion_dns_parse_header(const void *data,size_t length,struct aion_dns_header *out);
int aion_dns_read_name(const void *data,size_t length,size_t offset,char *name,size_t name_capacity,size_t *next_offset);
int aion_dhcp_parse_reply(const void *data,size_t length,struct aion_dhcp_offer *out);
#endif
