#include "../kernel/hardware/acpi.h"
#include <assert.h>
#include <string.h>
static void fix(unsigned char *p,unsigned n,unsigned at){unsigned i;unsigned char sum=0;p[at]=0;for(i=0;i<n;i++)sum=(unsigned char)(sum+p[i]);p[at]=(unsigned char)(0u-sum);}
int main(void){
    unsigned char rsdp[36]={0},sdt[40]={0};struct aion_acpi_rsdp root;struct aion_acpi_sdt table;
    memcpy(rsdp,"RSD PTR ",8);rsdp[15]=2;rsdp[20]=36;rsdp[24]=0x78;rsdp[25]=0x56;rsdp[26]=0x34;rsdp[27]=0x12;
    fix(rsdp,20,8);fix(rsdp,36,32);
    assert(aion_acpi_parse_rsdp(rsdp,sizeof(rsdp),&root)==0);
    assert(root.use_xsdt==1 && root.root_address==0x12345678u);
    rsdp[10]^=1;assert(aion_acpi_parse_rsdp(rsdp,36,&root)==AION_ACPI_ECORRUPT);rsdp[10]^=1;
    assert(aion_acpi_parse_rsdp(rsdp,20,&root)==AION_ACPI_EINVAL);
    memcpy(sdt,"APIC",4);sdt[4]=40;sdt[8]=1;fix(sdt,40,9);
    assert(aion_acpi_parse_sdt(sdt,40,&table)==0);
    assert(table.length==40 && table.signature[0]=='A');
    sdt[39]^=1;assert(aion_acpi_parse_sdt(sdt,40,&table)==AION_ACPI_ECORRUPT);
    assert(aion_acpi_parse_sdt(sdt,10,&table)==AION_ACPI_EINVAL);
    return 0;
}
