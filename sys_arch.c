// sys_arch.c
#include "lwip/sys.h"
#include "pico/stdlib.h"
#include "hardware/sync.h"

static uint32_t critical_section;

u32_t sys_now(void) {
    return to_ms_since_boot(get_absolute_time());
}

sys_prot_t sys_arch_protect(void) {
    critical_section = save_and_disable_interrupts();
    return critical_section;
}

void sys_arch_unprotect(sys_prot_t pval) {
    restore_interrupts(pval);
}