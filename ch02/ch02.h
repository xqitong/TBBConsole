#pragma once
#include <vector>
namespace ch02 {

    struct DataItem {
        int id;
        double value;
        DataItem(int i, double v) : id{ i }, value{ v } {}
    };

    using QSVector = std::vector<DataItem>;
} // namespace ch02