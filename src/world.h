#pragma once

namespace rb
{

class World
{
public:
    World(const std::string& filepath);

private:
    glm::ivec3 size;
};

}  // namespace rb
