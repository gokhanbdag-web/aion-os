#include "ipc.h"
static struct aion_ipc_mailbox *lookup(struct aion_ipc_bus *bus, uint64_t owner) {
    size_t i;
    for (i=0;i<AION_IPC_MAX_MAILBOXES;i++) if (bus->mailboxes[i].owner==owner) return &bus->mailboxes[i];
    return 0;
}
int aion_ipc_init(struct aion_ipc_bus *bus) {
    size_t i;
    if (!bus) return AION_IPC_EINVAL;
    for (i=0;i<AION_IPC_MAX_MAILBOXES;i++) {
        struct aion_ipc_mailbox *m=&bus->mailboxes[i];
        size_t j,k;
        m->owner=0; m->head=0; m->count=0;
        for (j=0;j<AION_IPC_QUEUE_DEPTH;j++) {
            m->queue[j].sender=0; m->queue[j].length=0;
            for (k=0;k<AION_IPC_PAYLOAD_MAX;k++) m->queue[j].data[k]=0;
        }
    }
    return AION_IPC_OK;
}
int aion_ipc_open(struct aion_ipc_bus *bus, uint64_t owner) {
    size_t i;
    if (!bus || !owner) return AION_IPC_EINVAL;
    if (lookup(bus,owner)) return AION_IPC_EINVAL;
    for (i=0;i<AION_IPC_MAX_MAILBOXES;i++) if (!bus->mailboxes[i].owner) {
        bus->mailboxes[i].owner=owner;
        bus->mailboxes[i].head=0; bus->mailboxes[i].count=0;
        return AION_IPC_OK;
    }
    return AION_IPC_ENOSPC;
}
int aion_ipc_send(struct aion_ipc_bus *bus, const struct aion_security_context *security, uint64_t sender, uint64_t recipient, const void *data, size_t length) {
    struct aion_ipc_mailbox *m;
    struct aion_ipc_message *msg;
    size_t i;
    if (!bus || !sender || !recipient || !data || !length || length>AION_IPC_PAYLOAD_MAX) return AION_IPC_EINVAL;
    if (!aion_security_can_access(security,AION_PRIV_USER,AION_CAP_WRITE)) return AION_IPC_EPERM;
    m=lookup(bus,recipient);
    if (!m) return AION_IPC_ENOENT;
    if (m->count==AION_IPC_QUEUE_DEPTH) return AION_IPC_EAGAIN;
    msg=&m->queue[(m->head+m->count)%AION_IPC_QUEUE_DEPTH];
    msg->sender=sender; msg->length=length;
    for (i=0;i<length;i++) msg->data[i]=((const uint8_t *)data)[i];
    m->count++;
    return AION_IPC_OK;
}
int aion_ipc_receive(struct aion_ipc_bus *bus, const struct aion_security_context *security, uint64_t recipient, struct aion_ipc_message *out) {
    struct aion_ipc_mailbox *m;
    size_t i;
    if (!bus || !recipient || !out) return AION_IPC_EINVAL;
    if (!aion_security_can_access(security,AION_PRIV_USER,AION_CAP_READ)) return AION_IPC_EPERM;
    m=lookup(bus,recipient);
    if (!m) return AION_IPC_ENOENT;
    if (!m->count) return AION_IPC_EAGAIN;
    *out=m->queue[m->head];
    m->queue[m->head].sender=0; m->queue[m->head].length=0;
    for (i=0;i<AION_IPC_PAYLOAD_MAX;i++) m->queue[m->head].data[i]=0;
    m->head=(m->head+1)%AION_IPC_QUEUE_DEPTH;
    m->count--;
    return AION_IPC_OK;
}
int aion_ipc_close(struct aion_ipc_bus *bus, uint64_t owner) {
    struct aion_ipc_mailbox *m;
    size_t i,j;
    if (!bus || !owner) return AION_IPC_EINVAL;
    m=lookup(bus,owner);
    if (!m) return AION_IPC_ENOENT;
    m->owner=0; m->head=0; m->count=0;
    for (i=0;i<AION_IPC_QUEUE_DEPTH;i++) {
        m->queue[i].sender=0; m->queue[i].length=0;
        for (j=0;j<AION_IPC_PAYLOAD_MAX;j++) m->queue[i].data[j]=0;
    }
    return AION_IPC_OK;
}
