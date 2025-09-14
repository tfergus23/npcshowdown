#include "gtest/gtest.h"
#include <filesystem>
#include <fstream>
#include "nlohmann/json.hpp"
#include "api/Utils.hpp"


using json = nlohmann::json;

TEST(TestBattles, TestBattles){
    namespace fs = std::filesystem;
    std::string folderPath = "./test_battles";

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (fs::is_regular_file(entry.status())) {
            const fs::path& filePath = entry.path();
            std::ifstream file(filePath, std::ios::binary);

            ASSERT_TRUE(file);
            std::string contents((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
            json battleJson = json::parse(contents);
            ASSERT_EQ(validateBattleRequest(battleJson), "");

            Battle battle(battleJson["trainer1"], battleJson["trainer2"], seedFromString(battleJson["seed"]));
            battle.simulate();
            EXPECT_FALSE(battle.invalid);
            if (battle.invalid){
                ADD_FAILURE() << "Test battle failed: " + filePath.string();
            }
        }
    }
}