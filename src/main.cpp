#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class Item {
public:
  bool valid;
  uint32_t address;

  Item() : valid(false), address(0){};
  Item(uint32_t _address) : valid(true), address(_address){};
  Item(bool valid, uint32_t _address) : valid(valid), address(_address){};
};

class Cache {
public:
  unsigned int hit, misses;
  u_int32_t line;
  u_int32_t associativity;
  std::vector<std::deque<Item>> row;

  Cache(int _line, int _associativity) {
    this->line = _line;
    this->associativity = _associativity;
    this->row.resize(line / associativity);
  }

  void insert(uint32_t address, int offset) {
    uint32_t addr = ((address >> offset) << offset) >> offset;
    int position = address % (line / associativity);

    if (this->found(position, addr)) {
      this->hit++;
      return;
    }

    this->misses++;

    if (this->row[position].size() >= associativity) {
      this->row[position].pop_back();
    }

    this->row[position].push_back(Item(true, addr));
  }

  bool found(int position, uint32_t address) {
    for (size_t i = 0; i < this->row[position].size(); i++) {
      if (this->row[position][i].address == address &&
          this->row[position][i].valid) {
        return true;
      }
    }

    return false;
  }

  void print() {
    for (size_t i = 0; i < this->row.size(); i++) {
      for (size_t j = 0; j < this->row[i].size(); j++) {
        std::cout << std::hex << this->row[i][j].address << std::endl;
      }
    }
  }
};

int main(int argc, char *argv[]) {
  uint32_t cache_size = std::atoi(argv[1]);
  uint32_t line_size = std::atoi(argv[2]);
  int associativity = std::atoi(argv[3]);
  std::ifstream input_file(argv[4]);

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

  cache.print();

  std::cout << cache.hit << std::endl;

  return 0;
}
