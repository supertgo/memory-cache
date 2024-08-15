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

class Block {
public:
  std::deque<Item> list;
  int first = 0;
};

class Cache {
public:
  unsigned int hits = 0, misses = 0;
  u_int32_t line, associativity;
  std::vector<Block> block;
  std::ofstream output_file;

  Cache(int _line, int _associativity) {
    this->line = _line;
    this->associativity = _associativity;
    this->block.resize(line / associativity);

    output_file.open("output.txt");

    if (!output_file.is_open()) {
      std::cerr << "Não foi possível abrir o arquivo output.xt" << std::endl;
      std::exit(1);
    }
  }

  ~Cache() {
    if (output_file.is_open()) {
      output_file.close();
    }
  }

  void insert(uint32_t address, int offset) {
    // Aneeeeeeeeeeeenha Isso é o memso que address >> offset, não?
    uint32_t addr = ((address >> offset) << offset) >> offset;
    uint32_t cardinality = line / associativity;
    int position = address % cardinality;

    if (this->found(position, addr)) {
      this->hits++;
      return;
    }

    this->misses++;

    if (this->block[position].list.size() >= associativity) {
      this->block[position]
          .list[(this->block[position].first % associativity)] = addr;
      this->block[position].first++;
    } else {
      this->block[position].list.push_back(Item(true, addr));
    }

    this->print();
  }

  bool found(int position, uint32_t address) const {
    for (size_t i = 0; i < this->block[position].list.size(); i++) {
      if (this->block[position].list[i].address == address &&
          this->block[position].list[i].valid) {
        return true;
      }
    }

    return false;
  }

  void parseAddressToHex(std::uint32_t address) {
    output_file << std::uppercase << "0x" << std::hex << address << std::endl;
  }

  void print() {
    size_t cont = 0;
    size_t i = 0;
    output_file << "================" << std::endl;
    output_file << "IDX V ** ADDR **" << std::endl;

    for (; i < this->block.size(); i++) {
      if (this->block[i].list.empty()) {
        output_file << std::setfill('0') << std::setw(3) << cont << " " << 0
                    << " " << std::endl;
        cont++;
        continue;
      }

      for (size_t j = 0; j < this->block[i].list.size(); j++) {
        output_file << std::setfill('0') << std::setw(3) << cont << " "
                    << this->block[i].list[j].valid << " ";

        parseAddressToHex(this->block[i].list[j].address);
        cont++;
      }
    }

    while (cont < this->line) {
      output_file << std::dec << std::setfill('0') << std::setw(3) << cont
                  << " " << 0 << std::endl;
      cont++;
    }
  }
};

int main(int argc, char *argv[]) {
  uint32_t cache_size = 4096;
  uint32_t line_size = 1024;
  int associativity = 4;
  std::ifstream input_file("src/laura.txt");

  if (!input_file.is_open()) {
    std::cerr << "Não foi possível abrir o arquivo de entrada" << std::endl;
    return 1;
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

  cache.output_file << std::endl;
  cache.output_file << "#hits: " << cache.hits << std::endl;
  cache.output_file << "#miss: " << cache.misses << std::endl;

  return 0;
}
