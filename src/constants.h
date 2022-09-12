#pragma once

#include <array>

namespace rb
{
    using index_t = std::uint16_t;
}

static constexpr int MAX_TEXTURE_SLOTS = 32;

static constexpr int WIDTH = 1920;
static constexpr int HEIGHT = 1080;
static constexpr float ASPECT_RATIO = static_cast<float>(WIDTH) / HEIGHT;

static constexpr int NUM_BLOCKS = 2;
static constexpr int NUM_VERTICES = NUM_BLOCKS * 8;
static constexpr int NUM_INDICES = NUM_BLOCKS * 36;

static constexpr std::array<rb::index_t, 36> BLOCK_INDICES = {0, 3, 1, 3, 2, 1, 1, 2, 5, 2, 6, 5, 5, 6, 4, 6, 7, 4,
                                                              4, 7, 0, 7, 3, 0, 3, 7, 2, 7, 6, 2, 4, 0, 5, 0, 1, 5};

static constexpr char STARTUP_MESSAGE[] = R"(
  _____                _             ____        _   
 |  __ \              | |           |  _ \      | |  
 | |__) |___ _ __   __| | ___ _ __  | |_) | __ _| |_ 
 |  _  // _ \ '_ \ / _` |/ _ \ '__| |  _ < / _` | __|
 | | \ \  __/ | | | (_| |  __/ |    | |_) | (_| | |_ 
 |_|  \_\___|_| |_|\__,_|\___|_|    |____/ \__,_|\__|
                                                     
                                                     
By Root
)";
