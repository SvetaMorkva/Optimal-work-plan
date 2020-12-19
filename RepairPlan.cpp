//
// Created by Sveta Morkva on 12/10/20.
//

#include <iostream>
#include <algorithm>
#include "RepairPlan.h"

int RepairPlan::process(bool hesitation) {
    for (int row1 = startRange[0].first; row1 < startRange[0].second; row1++) {
        for (int row2 = startRange[1].first; row2 < startRange[1].second; row2++) {
            for (int row3 = startRange[2].first; row3 < startRange[2].second; row3++) {
                for (int row4 = startRange[3].first; row4 < startRange[3].second; row4++) {
                    for (int row5 = startRange[4].first; row5 < startRange[4].second; row5++) {
                        std::vector<int> starts = {row1, row2, row3, row4, row5};
                        calculateWeeklyCosts(starts, hesitation);
                    }
                }
            }
        }
    }

    auto value = minMaxCost;
    minMaxCost = 10000;
    return value;
}

std::map<int, std::vector<int> > RepairPlan::getExpenses()
{
    return expenses;
}

void RepairPlan::setExpenses(int row, int col, int value)
{
    expenses[row].at(col) = value;
}

std::vector<std::pair<int, int> > RepairPlan::getTiming()
{
    auto timing = startRange;
    for (auto &time: timing) {
        time.first++;
    }
    return timing;
}

int RepairPlan::setTiming(int row, int col, int value)
{
    auto &pair = startRange.at(row);
    if (col == 0) {
        if (value > 6 || value > pair.second) {
            return pair.first;
        }
        pair.first = value;
        return -1;
    }
    if (value > 6 || value < pair.first) {
        return pair.second;
    }
    pair.second = value;
    return -1;
}

std::vector<int> RepairPlan::getPlan()
{
    return plan;
}

std::vector<int> RepairPlan::weeklyCost()
{
    return mWeeklyCost;
}

void RepairPlan::calculateWeeklyCosts(std::vector<int> starts, bool hesitation) {
    std::vector<int> weeklyCost;
    for (int weekNum = 0; weekNum < allWeekNum; weekNum++) {
        weeklyCost.push_back(0);
    }
    for (int machineNum = 0; machineNum < allMachineNum; machineNum++) {
        for (int weekNum = 0; weekNum < allWeekNum; weekNum++) {
            bool repair = weekNum - starts[machineNum] < 3 && weekNum - starts[machineNum] > -1;
            if (repair) {
                weeklyCost.at(weekNum) += expenses[machineNum].at(weekNum - starts[machineNum]);
            }
        }
    }
    int maxCost = 0;
    if (!hesitation) {
        maxCost = *(std::max_element(weeklyCost.begin(), weeklyCost.end()));
    } else {
        std::vector<int> hesitations;
        for (int weekNum = 1; weekNum < allWeekNum; weekNum++) {
            maxCost += std::abs(weeklyCost[weekNum]-weeklyCost[weekNum-1]);
        }
    }
    auto newMinMax = std::min(minMaxCost, maxCost);
    if (newMinMax != minMaxCost) {
        minMaxCost = newMinMax;
        plan = starts;
        mWeeklyCost = weeklyCost;
    }
}
