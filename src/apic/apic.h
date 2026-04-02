#include <stdint.h>

typedef struct lapic_register_fields_t {
  uint8_t vector: 7;
  uint8_t delivery_mode: 3;
  uint8_t destination_mode: 1;
  uint8_t delivery_status: 1;
  uint8_t reserved: 1;
  uint8_t clear_init_level_deassert: 1;
  uint8_t set_init_level_deassert: 1;
  uint8_t destination_type: 2;
  uint8_t reserved2: 12;
} lapic_register_fields_t;

typedef struct lapic_register_t {
  lapic_register_fields_t fields;
  uint32_t boundary[3];
};

// see https://wiki.osdev.org/APIC#Inter-Processor_Interrupts for more info
typedef struct lapic_t {
  lapic_register_t reserved[2];
  lapic_register_t lapic_id;
  lapic_register_t lapic_version;
  lapic_register_t reserved2[3];
  lapic_register_t task_priority;
  lapic_register_t arbitration_priority;
  lapic_register_t processor_priority;
  lapic_register_t eoi;
  lapic_register_t remote_read;
  lapic_register_t logical_destination;
  lapic_register_t spurious_interrupt_vector;
  lapic_register_t in_service;
  lapic_register_t trigger_mode;
  lapic_register_t interrupt_request;
  lapic_register_t error_status;
  lapic_register_t reserved3[5];
  lapic_register_t lvt_correct_machine_check_interrupt;
  lapic_register_t interrupt_comamand[2];
  lapic_register_t lvt_time_register;
  lapic_register_t lvt_thermal_sensor; 
  lapic_register_t lvt_performance_monitor_counters;
  lapic_register_t lvt_lint0;
  lapic_register_t lvt_lint1;
  lapic_register_t timer_initial_count;
  lapic_register_t timer_current_count;
  lapic_register_t reserved4[4];
  lapic_register_t divide_configuration;
  lapic_register_t reserved5;
}
