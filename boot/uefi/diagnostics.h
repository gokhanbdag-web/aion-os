#ifndef AION_DIAGNOSTICS_H
#define AION_DIAGNOSTICS_H
#include <stdint.h>
void aion_diag_write(const char *message);
void aion_diag_event(const char *component, const char *event);
void aion_diag_hex(uint64_t value);
#endif
