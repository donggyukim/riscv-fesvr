// See LICENSE for license details.

#ifndef __HTIF_H
#define __HTIF_H

#include "memif.h"
#include "syscall.h"
#include "device.h"
#include <string.h>
#include <vector>

class htif_t
{
 public:
  htif_t(const std::vector<std::string>& target_args);
  virtual ~htif_t();

  virtual void start();
  virtual void stop();

  int run();
  bool done();
  int exit_code();

  virtual memif_t& memif() { return mem; }
  virtual uint32_t num_cores();

 protected:
  virtual void reset() = 0;

  virtual void read_chunk(addr_t taddr, size_t len, void* dst) = 0;
  virtual void write_chunk(addr_t taddr, size_t len, const void* src) = 0;
  virtual void clear_chunk(addr_t taddr, size_t len);

  virtual size_t chunk_align() = 0;
  virtual size_t chunk_max_size() = 0;

  virtual void load_program();
  virtual void idle() {}

  const std::vector<std::string>& host_args() { return hargs; }

  std::string config_string;

  addr_t get_tohost_addr() const { return tohost_addr; }
  addr_t get_fromhost_addr() const { return fromhost_addr; }
  int get_exitcode() const { return exitcode; }
  device_list_t& get_device_list() { return device_list; }

 private:
  memif_t mem;
  bool writezeros;
  uint32_t _num_cores;
  std::vector<std::string> hargs;
  std::vector<std::string> targs;
  std::string sig_file;
  addr_t sig_addr; // torture
  addr_t sig_len; // torture
  addr_t tohost_addr;
  addr_t fromhost_addr;
  int exitcode;
  bool stopped;

  device_list_t device_list;
  syscall_t syscall_proxy;
  bcd_t bcd;
  std::vector<device_t*> dynamic_devices;

  const std::vector<std::string>& target_args() { return targs; }
  std::string read_config_string(reg_t addr);

  friend class memif_t;
  friend class syscall_t;
};

#endif // __HTIF_H
