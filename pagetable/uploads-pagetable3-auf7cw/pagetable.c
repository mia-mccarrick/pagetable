#define PTE_SIZE 8 
#define _POSIX_C_SOURCE 200809L
#include "mlpt.h"
#include "config.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

size_t ptbr;
size_t pages_space = 1 << POBITS;;
size_t vpn_bits_per_level = POBITS - 3; ;
size_t page_num_mask = (1 << (POBITS - 3)) - 1;
size_t valid_bit_mask = -2; 

size_t kth_vpn(size_t va, int k) { 
    size_t vpn = va >> POBITS;
    size_t kth_vpn_shift = (vpn_bits_per_level * ((LEVELS - 1) - k));
    size_t kth_vpn = ((vpn >> kth_vpn_shift) & page_num_mask) * PTE_SIZE;
    return kth_vpn;
}

void newpage(void** index) {
    posix_memalign(index, pages_space, pages_space);
    memset(*index, 0, pages_space);
}

size_t translate(size_t va){
    if (va > ((~0) >> (64 - (vpn_bits_per_level) * LEVELS + POBITS))) {
        return ~0;
    }
    if (!ptbr) {
        return ~0; 
    }
    size_t page_offset = va & (pages_space - 1);
    size_t* pte_pointer = (size_t*) (ptbr + kth_vpn(va, 0));
    
    for (int level = 0; level < LEVELS; ++level) {
        if (! ((*pte_pointer) & 0x1)) {
            return -1;
        }
        if (level < (LEVELS - 1)) {
          size_t temp = (*pte_pointer) & valid_bit_mask;
          pte_pointer = (size_t*) (temp + kth_vpn(va, level + 1));
        }
    }
    size_t physical_address = ((*pte_pointer) & valid_bit_mask) + page_offset;
    return physical_address;
}

void page_allocate(size_t va){
    if (!ptbr) {
        newpage((void**) &ptbr);
    }

    size_t* page_pointer = (size_t*) (ptbr + kth_vpn(va, 0)); 

    for (int level = 0; level < LEVELS; ++level) {
       if (! ((*page_pointer) & 0x1)) { 
          newpage((void**) page_pointer);
          (*page_pointer) += 0x1;
        }
        if (level < (LEVELS - 1)) {
            size_t temp = (*page_pointer) & valid_bit_mask;
            page_pointer = (size_t*) (temp + kth_vpn(va, level + 1));
        }
    }
    return;
}

int main() {
    //finally finished, yay :)
}


