#pragma once

struct BlockStats {
    float** Data;
    int Rows;
    int Cols;
};

struct AudiosurfData {
    BlockStats Stats; 
    float GoldMedalThreshold;
    float Score;
};