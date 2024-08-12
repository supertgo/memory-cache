#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class Item {
  bool valid;
  uint32_t address;
};

class Cache {
public:
  int line;
  int associativity;
  std::vector<std::deque<Item>> row;

  Cache(int _line, int _associativity) {
    this->line = _line;
    this->associativity = _associativity;
    this->row.resize(line / associativity);

    for (int i = 0; i < this->row.size(); i++) {
      this->row[i].resize(associativity);
    }
  }

  void insert(uint32_t address) {
    int position = address % line;

    if (this->row[position].size() < associativity) {
    }

    // if (this->row[position].size() == associativity &&
    // !this->row[position]) {
    //   this->row[position].pop();
    // }
  }
};

// std::cout << std::hex << (((address >> offset) << offset) >> offset)
//           << std::endl;
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
    cache.insert(address);
  }
  return 0;
}
