#include <stdint.h>

typedef struct nvme_t {
  uint32_t capabilities;
  uint32_t version;
  uint32_t interrupt_mask_set;
  uint32_t interrupt_mask_clear;
  uint32_t configuration;
  uint32_t status;
  uint32_t admin_queue_attributes;
  uint64_t admin_submission_queue;
  uint64_t admin_completion_queue;
  void* sqxtdbl;
  void* cqxhdbl;
} nvme_t;

typedef struct nvme_queue_t {
  uint64_t queue_address;
  uint64_t queue_sizxe;
}

typedef struct nvme_submission_queue_t {
  uint16_t command;
  uint16_t nsid;
  uint32_t _;
  uint32_t metadata;
  uint64_t data_pointer;
}
