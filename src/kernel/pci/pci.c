#include <stdint.h>
#include <io.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

// refer to https://wiki.osdev.org/PCI#Configuration_Space_Access_Mechanism_#1
uint16_t pci_config_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = 0;
    uint32_t lbus  = (uint32_t) bus;
    uint32_t lslot = (uint32_t) slot;
    uint32_t lfunc = (uint32_t) func;
    uint16_t tmp = 0;

    address |= 0x80000000;
    address |= lbus << 16;
    address |= lslot << 11;
    address |= lfunc << 8;
    address |= offset & 0xFC;
 
    outl(PCI_CONFIG_ADDRESS, address);

    tmp = (uint16_t) ((inl(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}

uint16_t pci_check_vendor(uint8_t bus, uint8_t slot) {
    uint16_t vendor, device;
    if ((vendor = pci_config_read_word(bus, slot, 0, 0)) != 0xFFFF) {
       device = pci_config_read_word(bus, slot, 0, 2);
    }
    return vendor;
}

//TODO add custom 
