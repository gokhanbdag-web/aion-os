#include "pci.h"
static uint32_t cfg(aion_pci_config_read_fn rd,void *ctx,uint8_t b,uint8_t d,uint8_t f,uint8_t off){return rd(ctx,b,d,f,off);}
int aion_pci_discover(aion_pci_config_read_fn rd,void *ctx,struct aion_pci_inventory *out){
    uint8_t visited[256]={0},pending[256]={0};
    unsigned bus,dev,fn;int changed=1;
    if(!rd || !out)return AION_PCI_EINVAL;
    out->count=0;pending[0]=1;
    while(changed){
        changed=0;
        for(bus=0;bus<256;bus++){
            if(!pending[bus] || visited[bus])continue;
            visited[bus]=1;changed=1;
            for(dev=0;dev<32;dev++){
                uint32_t id=cfg(rd,ctx,(uint8_t)bus,(uint8_t)dev,0,0);
                uint32_t hdr;unsigned functions;
                if((id&0xffffu)==0xffffu)continue;
                hdr=cfg(rd,ctx,(uint8_t)bus,(uint8_t)dev,0,0x0c);
                functions=(hdr&0x00800000u)?8u:1u;
                for(fn=0;fn<functions;fn++){
                    uint32_t ident=fn?cfg(rd,ctx,(uint8_t)bus,(uint8_t)dev,(uint8_t)fn,0):id;
                    uint32_t klass,header;
                    struct aion_pci_device *item;
                    if((ident&0xffffu)==0xffffu)continue;
                    if(out->count==AION_PCI_MAX_DEVICES)return AION_PCI_ENOSPC;
                    klass=cfg(rd,ctx,(uint8_t)bus,(uint8_t)dev,(uint8_t)fn,0x08);
                    header=cfg(rd,ctx,(uint8_t)bus,(uint8_t)dev,(uint8_t)fn,0x0c);
                    item=&out->devices[out->count++];
                    item->bus=(uint8_t)bus;item->device=(uint8_t)dev;item->function=(uint8_t)fn;
                    item->vendor_id=(uint16_t)ident;item->device_id=(uint16_t)(ident>>16);
                    item->class_code=(uint8_t)(klass>>24);item->subclass=(uint8_t)(klass>>16);
                    item->prog_if=(uint8_t)(klass>>8);item->header_type=(uint8_t)(header>>16);
                    if(item->class_code==0x06 && item->subclass==0x04 && (item->header_type&0x7fu)==1u){
                        uint32_t buses=cfg(rd,ctx,(uint8_t)bus,(uint8_t)dev,(uint8_t)fn,0x18);
                        unsigned secondary=(buses>>8)&0xffu;
                        unsigned subordinate=(buses>>16)&0xffu;
                        unsigned b;
                        if(secondary!=0 && secondary<=subordinate)
                            for(b=secondary;b<=subordinate;b++)pending[b]=1;
                    }
                }
            }
        }
    }
    return AION_PCI_OK;
}
