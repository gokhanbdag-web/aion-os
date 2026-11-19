#include "../kernel/ipc/ipc.h"
#include <assert.h>
int main(void) {
    static struct aion_ipc_bus bus;
    struct aion_security_context reader,writer,none;
    struct aion_ipc_message msg;
    uint8_t payload[2]={0x41,0x42};
    size_t i;
    assert(aion_ipc_init(&bus)==0);
    assert(aion_security_context_init(&reader,AION_PRIV_USER,AION_CAP_READ)==0);
    assert(aion_security_context_init(&writer,AION_PRIV_USER,AION_CAP_WRITE)==0);
    assert(aion_security_context_init(&none,AION_PRIV_USER,0)==0);
    assert(aion_ipc_open(&bus,11)==0);
    assert(aion_ipc_open(&bus,11)==AION_IPC_EINVAL);
    assert(aion_ipc_send(&bus,&none,10,11,payload,2)==AION_IPC_EPERM);
    assert(aion_ipc_send(&bus,&writer,10,11,payload,2)==0);
    assert(aion_ipc_receive(&bus,&none,11,&msg)==AION_IPC_EPERM);
    assert(aion_ipc_receive(&bus,&reader,11,&msg)==0);
    assert(msg.sender==10 && msg.length==2 && msg.data[0]==0x41 && msg.data[1]==0x42);
    assert(aion_ipc_receive(&bus,&reader,11,&msg)==AION_IPC_EAGAIN);
    for (i=0;i<AION_IPC_QUEUE_DEPTH;i++) assert(aion_ipc_send(&bus,&writer,10,11,payload,2)==0);
    assert(aion_ipc_send(&bus,&writer,10,11,payload,2)==AION_IPC_EAGAIN);
    for (i=0;i<AION_IPC_QUEUE_DEPTH;i++) assert(aion_ipc_receive(&bus,&reader,11,&msg)==0);
    assert(aion_ipc_send(&bus,&writer,10,11,payload,0)==AION_IPC_EINVAL);
    assert(aion_ipc_send(&bus,&writer,10,11,payload,AION_IPC_PAYLOAD_MAX+1)==AION_IPC_EINVAL);
    assert(aion_ipc_send(&bus,&writer,10,12,payload,2)==AION_IPC_ENOENT);
    assert(aion_ipc_close(&bus,11)==0);
    assert(aion_ipc_receive(&bus,&reader,11,&msg)==AION_IPC_ENOENT);
    assert(aion_ipc_open(&bus,11)==0);
    assert(aion_ipc_receive(&bus,&reader,11,&msg)==AION_IPC_EAGAIN);
    assert(aion_ipc_init(0)==AION_IPC_EINVAL);
    return 0;
}
