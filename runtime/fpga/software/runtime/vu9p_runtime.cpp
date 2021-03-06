#include "common.h"
#include "utility.h"
#include "vu9p_runtime.h"
// #include "coreparam.h"
#include "loguru.hpp"

#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Note: this module will not control the really allocated buffer, this is controlled in the driver code
// More details on how this is achieved, please refer to the technical report
#define CMA_BUFFER 512L
uint32_t VU9PRuntime::get_cdma_control() {
  if(cdma == nullptr) {
    LOG_F(ERROR, "cdma is not initialized");
    std::abort();
  }
  *(volatile uint32_t*)((uint8_t*)cdma + 0x00);
}
void VU9PRuntime::reset_cdma(){
  if(cdma == nullptr) {
    LOG_F(ERROR, "cdma is not initialized");
    std::abort();
  }
  auto original = get_cdma_control();
  original = original | (1 << 2);
  *(volatile uint32_t*)((uint8_t*)cdma + 0x00) = original;
  original = get_cdma_control();
  LOG_F(INFO, "cdma resetting");
  while(original & (1 << 2) != 0) {
    original = get_cdma_control();
  }
  LOG_F(INFO, "cdma reset done");
}
bool VU9PRuntime::check_cdma_has_err() {
    auto status = get_cdma_status();
    bool err = false;
    if( (status >> 6) & 1 != 0) {
      LOG_F(ERROR, "cdma: DMADecErr");
      err = true;
    }
    if( (status >> 5) & 1 != 0) {
      LOG_F(ERROR, "cdma: DMASlvErr");
      err = true;
    }
    if( (status >> 4) & 1 != 0) {
      LOG_F(ERROR, "cdma: DMAIntErr");
      err = true;
    }
    return err;
}

uint32_t VU9PRuntime::get_cdma_status() {
  if(cdma == nullptr) {
    LOG_F(ERROR, "cdma is not initialized");
    std::abort();
  }
  *(volatile uint32_t*)((uint8_t*)cdma + 0x04);
}

void VU9PRuntime::set_src(address_t start) {
  LOG_F(INFO, "source set to %lx", start);
  *(volatile uint64_t*)((uint8_t*)cdma + 0x18) = start;
  // *(volatile uint32_t*)((uint8_t*)cdma + 0x18) = start & 0xffffffff;
  // *(volatile uint32_t*)((uint8_t*)cdma + 0x1C) = (start >> 32) & 0xffffffff;
}
void VU9PRuntime::set_dst(address_t dst) {
  LOG_F(INFO, "destination set to %lx", dst);
  *(volatile uint64_t*)((uint8_t*)cdma + 0x20) = dst;
  // *(volatile uint32_t*)((uint8_t*)cdma + 0x20) = dst & 0xffffffff;
  // *(volatile uint32_t*)((uint8_t*)cdma + 0x24) = (dst >> 32) & 0xffffffff;
}
// WARNING: This will fire a transfer
void VU9PRuntime::set_len(address_t len) {
  if(len > 33554432) {
    LOG_F(ERROR, "Cannot transfer more than 32MB (33554432 bytes)");
  }
  LOG_F(INFO, "set len = %x", len);
  *(volatile uint64_t*)((uint8_t*)cdma + 0x28) = len;
}

void VU9PRuntime::transfer_to_ddr(address_t start, size_t sz) {
  using namespace std::chrono_literals;
  if(!is_cdma_idle()) {
    LOG_F(ERROR, "cdma is not idle");
    std::abort();
  }
  reset_cdma();
  // TODO: update this..
  address_t src = 0x000000000LL + start; // this is the AXI to PCIe BAR, translation is done manually
  address_t dst = 0x120000000LL + start;
  unsigned char* current = reinterpret_cast<unsigned char*>(start); // this is the memory in the host

  /** we need to get the contiguous memory buffer **/
  int fd = open("/dev/vcu1525", O_RDWR | O_SYNC);
  // unsigned char* cbuffer = (unsigned char*)mmap(0, 2 * DMA_BEAT, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 6 * 0x1000);

  // LOG_F(INFO, "(BAR  6?) Mapped to: %p", cbuffer);
  // if(cbuffer == MAP_FAILED ) {
  //   perror("mmap");
  //   std::abort();
  // }

  while(sz > 0) {
    uint32_t len = sz < (DMA_BEAT) ? sz : DMA_BEAT;
    // we just copy all we have into the cma buffer
    // we may encouter unaligned error
    LOG_F(INFO, "Trying to copy the data...(len=%x)", len);
    // for(int i = 0; i < len; i++) {
    //   cbuffer[i] = current[i];
    // }
    ssize_t k = write(fd, current, len);
    if(k != len) {
      LOG_F(ERROR, "Cannot push all data into kernel");
      std::abort();
    }
    // memcpy(cbuffer, current, len);
    LOG_F(INFO, "Copy done...");
    uint32_t cd = *(uint32_t*)(current + 0x2000);
    // uint32_t bd = *(uint32_t*)(cbuffer + 0x2000);
    // LOG_F(INFO, "current[0x2000] = %x cbuffer[0x2000] = %x", cd, bd);
    // and let the cdma copy from that cma buffer
    set_src(/* src */0);
    set_dst(dst);
    set_len(len); // started
    LOG_F(INFO, "Trying to transfer one chunk...");
    // just wait
    while(!is_cdma_idle()) {
      std::this_thread::sleep_for(50ms);
    }
    if(check_cdma_has_err()) {
      LOG_F(ERROR, "CDMA Encounters error...");
      std::abort();
    }
    LOG_F(INFO, "Transfered %d bytes (%d M)", sz, sz/1024/1024);
    sz -= len;
    // our buffer is fixed now
    // src += len;
    current += len;
    dst += len;
  }

  // munmap(cbuffer, 2 * DMA_BEAT * 1024 * 1024);
}

void* VU9PRuntime::get_cdma() {
  if(this->cdma == nullptr) {
    int fd = open("/dev/vcu1525", O_RDWR | O_SYNC);
    void* cdma = mmap(0, 16 * 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 8 * 0x1000);
    if(cdma == MAP_FAILED) {
      std::abort();
    }
    close(fd);
    LOG_F(INFO, "CDMA mapped to: %p", cdma);
    return this->cdma = cdma;
  }
  return this->cdma;
}

void VU9PRuntime::setup_post(address_t start) {
  this->remap_mem(start);
}

void VU9PRuntime::remap_mem(address_t start) {
  // here, we need to utilize CDMA to transfer data from the dma buffer to the DDR
  void* cdma = get_cdma();
  transfer_to_ddr(start, CMA_BUFFER * 1024 * 1024 - start);
  munmap(reinterpret_cast<void*>(start), CMA_BUFFER * 1024 * 1024); 

  // the we map the register space and use that as memory...
  int fd = open("/dev/vcu1525", O_RDWR | O_SYNC);
  if(fd == -1) {
    LOG_F(ERROR, "Unable to open /dev/vcu1525, check whether the driver is loaded");
    perror("open");
    std::abort();
  }
  // two consecutive chunk formatting 512M of space
  void* VDMEM =      mmap(reinterpret_cast<void*>(start), CMA_BUFFER / 2 * 1024 * 1024 - start, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, fd, 4 * 0x1000);
  void* VDMEM_NEXT = mmap(reinterpret_cast<void*>(start) + CMA_BUFFER / 2 * 1024 * 1024 - start, CMA_BUFFER / 2 * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, fd, 10 * 0x1000);
  if((address_t)VDMEM != start || VDMEM == MAP_FAILED || VDMEM_NEXT == MAP_FAILED) {
    LOG_F(ERROR, "Cannot Remap Memory To Use IO, mapping to %lx", start);
    perror("mmap");
    std::abort();
  }
  close(fd);
  return;
}

uint64_t VU9PRuntime::get_stats(int offset) {
  // LOG_F(WARNING, "stats not implemented...");
  // return 0;
  return *(volatile uint64_t*)((uint8_t*)this->reset + 0x1000 + offset * 8);
}

void VU9PRuntime::set_init_pc(address_t pc) {
  *this->initPC = pc;
}

void VU9PRuntime::set_reset(int value) {
  *reset = value;
}

address_t VU9PRuntime::get_base_address() {
  // volatile uint32_t* baseAddrHi = (volatile uint32_t*)((volatile uint8_t*)baseAddr + 0x208);
  // volatile uint32_t* baseAddrLo = (volatile uint32_t*)((volatile uint8_t*)baseAddr + 0x20C);
  // return ((address_t)(*baseAddrHi) << 32) | ((address_t)(*baseAddrLo));
  return 0xdeadbeef;
}

void VU9PRuntime::set_base_address(address_t baseAddress) {
  /*
  volatile uint32_t* baseAddrHi = (volatile uint32_t*)((volatile uint8_t*)baseAddr + 0x208);
  volatile uint32_t* baseAddrLo = (volatile uint32_t*)((volatile uint8_t*)baseAddr + 0x20C);
  LOG_F(INFO, "setting base address to: %lx", baseAddress);

  *baseAddrHi =  (baseAddress >> 32) & 0xffffffff;
  *baseAddrLo =  baseAddress & 0xffffffff;
  */
  // does nothing, should not change
}

void VU9PRuntime::_allocate_host_memory(address_t start, size_t size) {
  // The dma
  // here we use the region as normal region and utilize the cma array just as an buffer
  // int fd = open("/dev/vcu1525", O_RDWR | O_SYNC);
  auto orig_base_addr = get_base_address();
  LOG_F(INFO, "original base address (physical): %lx", orig_base_addr);
  LOG_F(INFO, "Allocating contiguous memory: %d MB", CMA_BUFFER);
  // void* VDMEM = (void*)mmap(reinterpret_cast<void*>(start), CMA_BUFFER * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, fd, 6 * 0x1000);
  void* VDMEM = (void*)mmap(reinterpret_cast<void*>(start), CMA_BUFFER * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED | MAP_ANON, -1, 0);
  if(VDMEM == MAP_FAILED || (uint64_t) VDMEM != start) {
    perror("mmap");
    LOG_F(WARNING, "orig_base_addr: %lx", orig_base_addr);
    LOG_F(ERROR, "Cannot map contiguous memory to needed address");
    std::abort();
  }
  LOG_F(INFO, "mapped to: %p", VDMEM);
  orig_base_addr -= start;
  this->phys_start = orig_base_addr;
  memset((void*)start, 0, CMA_BUFFER * 1024 * 1024);
  // close(fd);
}
VU9PRuntime::VU9PRuntime() {
  int fd = open("/dev/vcu1525", O_RDWR | O_SYNC);
  if(fd == -1) {
    perror("open");
    LOG_F(ERROR, "Cannot open /dev/mem for mapping control registers, maybe check for permission and whether driver is loaded.");
    std::abort();
  }

  this->reset = (volatile uint64_t*)mmap(0, 4 * 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 2 * 0x1000);
  if(this->reset == MAP_FAILED) {
    LOG_F(ERROR, "Cannot mmap resetRegister.");
    std::abort();
  }
  LOG_F(WARNING, "Reset mapped to 0x%x", this->reset);

  this->initPC = (volatile uint64_t*)((volatile uint8_t*)this->reset + 0x3000);
  LOG_F(WARNING, "initPC mapped to 0x%x", this->initPC);

  // this->baseAddr = (volatile uint64_t*)mmap(0, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x1000);
  // if(this->baseAddr == MAP_FAILED) {
  //   perror("mmap");
  //   LOG_F(ERROR, "Cannot mmap baseAddr.");
  //   std::abort();
  // }

  close(fd);
  LOG_F(WARNING, "baseAddr is handled within kernel.");
  LOG_F(WARNING, "Done");
}
