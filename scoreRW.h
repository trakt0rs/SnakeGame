#pragma once

#include <fstream>
#include <cstdint>
#include <filesystem>

namespace scoreRW {

    inline void createScoreFile() {
        std::ofstream file("score.dat", std::ios::binary);
        uint32_t defaultScore = 0;
        file.write(reinterpret_cast<const char*>(&defaultScore), sizeof(defaultScore));
    }

    inline uint32_t getHighScoreFromFile() {
        if (!std::filesystem::exists("score.dat")) {
            createScoreFile();
        }

        std::ifstream file("score.dat", std::ios::binary);
        uint32_t score = 0;

        if (file.read(reinterpret_cast<char*>(&score), sizeof(score))) {
            return score;
        }
        else {
            createScoreFile();
            return 0;
        }
    }

    inline void writeNewHighScore(uint32_t score) {
        std::ofstream file("score.dat", std::ios::binary);
        file.write(reinterpret_cast<const char*>(&score), sizeof(score));
    }

}