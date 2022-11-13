#include "world.h"

#include "nbt/nbt.h"

namespace rb
{

World::World(const std::string& filepath)
{
    nbt::NBT root {std::ifstream {filepath, std::ios::binary}};

    const auto& size_vector = root["size"].data<nbt::TagInt>();
    this->size.x = size_vector[0];
    this->size.y = size_vector[1];
    this->size.z = size_vector[2];

    const auto& block_palette = root["structure"]["palette"]["default"]["block_palette"];

    std::cout << "Successfully loaded NBT!\n";
}

}  // namespace rb
