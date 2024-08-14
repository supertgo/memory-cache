#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

class Item {
public:
  bool valid;
  uint32_t address;

  Item(uint32_t _address) : valid(true), address(_address){};
  Item(bool valid, uint32_t _address) : valid(valid), address(_address){};
};

class Row {
public:
  std::deque<Item> list;
  int first = 0;
};

class Cache {
public:
  unsigned int hits = 0, misses = 0, size = 0;
  u_int32_t line;
  u_int32_t associativity;
  std::vector<Row> row;

  Cache(int _line, int _associativity) {
    this->line = _line;
    this->associativity = _associativity;
    this->row.resize(line / associativity);
  }

  void insert(uint32_t address, int offset) {
    uint32_t addr = ((address >> offset) << offset) >> offset;
    uint32_t cardinality = line / associativity;
    int position = address % cardinality;

    if (this->found(position, addr)) {
      this->hits++;
      return;
    }

    this->misses++;

    if (this->row[position].list.size() >= associativity) {
      this->row[position].list[(this->row[position].first % associativity)] =
          addr;
      this->row[position].first++;
    } else {
      this->row[position].list.push_back(Item(true, addr));
    }

    this->print();
  }

  bool found(int position, uint32_t address) {
    for (size_t i = 0; i < this->row[position].list.size(); i++) {
      if (this->row[position].list[i].address == address &&
          this->row[position].list[i].valid) {
        return true;
      }
    }

    return false;
  }

  void parseAddressToHex(std::uint32_t address) {
    std::cout << std::uppercase << "0x" << std::hex << address << std::endl;
  }

  void print() {
    size_t cont = 0;
    size_t i = 0;
    std::cout << "================" << std::endl;
    std::cout << "IDX V ** ADDR **" << std::endl;

    for (; i < this->row.size(); i++) {
      if (this->row[i].list.size() == 0) {
        std::cout << std::setfill('0') << std::setw(3) << cont << " " << 0
                  << " " << std::endl;
        cont++;
        continue;
      }

      for (size_t j = 0; j < this->row[i].list.size(); j++) {
        std::cout << std::setfill('0') << std::setw(3) << cont << " "
                  << this->row[i].list[j].valid << " ";

        parseAddressToHex(this->row[i].list[j].address);
        cont++;
      }
    }

    while (cont < this->line) {
      std::cout << std::dec << std::setfill('0') << std::setw(3) << cont << " "
                << 0 << std::endl;
      cont++;
    }
  }
};

int main(int argc, char *argv[]) {
  uint32_t cache_size = 4096;
  uint32_t line_size = 1024;
  int associativity = 4;
  std::ifstream input_file("laura.txt");

  if (!input_file.is_open()) {
    std::cerr << "Unable to open file!" << std::endl;
  }

  Cache cache(cache_size / line_size, associativity);

  int offset = std::log2(line_size);

  std::string line, transformed_address;
  std::vector<std::uint32_t> addresses;
  while (std::getline(input_file, line)) {
    std::stringstream ss;

    ss << std::hex << line;
    uint32_t address;
    ss >> address;

    addresses.push_back(address);
  }

  for (auto address : addresses) {
    cache.insert(address, offset);
  }

  std::cout << std::endl;
  std::cout << "#hits: " << cache.hits << std::endl;
  std::cout << "#miss: " << cache.misses << std::endl;

  std::cout << std::endl;
  uint32_t b = 113019322;
  std::cout << (((b >> offset) << offset) >> offset) << std::endl;

  return 0;
}
