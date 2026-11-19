#ifndef AION_IPC_H
#define AION_IPC_H
#include <stddef.h>
#include <stdint.h>
#include "../security/privilege.h"
#define AION_IPC_MAX_MAILBOXES 32u
#define AION_IPC_QUEUE_DEPTH 8u
#define AION_IPC_PAYLOAD_MAX 64u
#define AION_IPC_OK 0
#define AION_IPC_EINVAL (-22)
#define AION_IPC_EPERM (-1)
#define AION_IPC_ENOENT (-2)
#define AION_IPC_ENOSPC (-28)
#define AION_IPC_EAGAIN (-11)
struct aion_ipc_message {
    uint64_t sender;
    size_t length;
    uint8_t data[AION_IPC_PAYLOAD_MAX];
};
struct aion_ipc_mailbox {
    uint64_t owner;
    size_t head;
    size_t count;
    struct aion_ipc_message queue[AION_IPC_QUEUE_DEPTH];
};
struct aion_ipc_bus {
    struct aion_ipc_mailbox mailboxes[AION_IPC_MAX_MAILBOXES];
};
int aion_ipc_init(struct aion_ipc_bus *bus);
int aion_ipc_open(struct aion_ipc_bus *bus, uint64_t owner);
int aion_ipc_send(struct aion_ipc_bus *bus, const struct aion_security_context *security, uint64_t sender, uint64_t recipient, const void *data, size_t length);
int aion_ipc_receive(struct aion_ipc_bus *bus, const struct aion_security_context *security, uint64_t recipient, struct aion_ipc_message *out);
int aion_ipc_close(struct aion_ipc_bus *bus, uint64_t owner);
#endif
