#pragma once
inline constexpr float statMultipliers[] = { 2.0f / 8.0f, 2.0f / 7.0f, 2.0f / 6.0f, 2.0f / 5.0f, 2.0f / 4.0f, 2.0f / 3.0f, 2.0f / 2.0f, 3.0f / 2.0f, 4.0f / 2.0f, 5.0f / 2.0f, 6.0f / 2.0f, 7.0f / 2.0f, 8.0f / 2.0f };
inline constexpr float accuracyMultipliers[] = { 33.0f / 100.0f, 36.0f / 100.0f, 43.0f / 100.0f, 50.0f / 100.0f, 60.0f / 100.0f, 75.0f / 100.0f, 100.0f / 100.0f, 133.0f / 100.0f, 166.0f / 100.0f, 200.0f / 100.0f, 250.0f / 100.0f, 266.0f / 100.0f, 300.0f / 100.0f };
inline constexpr float critChances[] = { 1.0f / 16.0f, 1.0f / 8.0f, 1.0f / 4.0f, 1.0f / 3.0f };


float statStageMultiplier(int stage);
float accuracyStageMultiplier(int accuracyStage);
float critChanceFromStage(int stage);