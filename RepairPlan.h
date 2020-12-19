//
// Created by Sveta Morkva on 12/10/20.
//

#ifndef COURSE_WORK_REPAIRPLAN_H
#define COURSE_WORK_REPAIRPLAN_H
#include <vector>
#include <map>

class RepairPlan {
public:
    int process(bool hesitation = false);

    std::map<int, std::vector<int>> getExpenses();
    void setExpenses(int row, int col, int value);

    std::vector<std::pair<int, int>> getTiming();
    int setTiming(int row, int col, int value);

    std::vector<int> getPlan();
    std::vector<int> weeklyCost();

private:
    void calculateWeeklyCosts(std::vector<int> starts, bool hesitation);

    int allMachineNum = 5;
    int allWeekNum = 8;
    std::map<int, std::vector<int>> expenses = {{0, {4, 6, 3}},
                                                {1, {3, 2, 5}},
                                                {2, {7, 1, 1}},
                                                {3, {1, 3, 6}},
                                                {4, {5, 3, 4}}};
    std::vector<std::pair<int, int>> startRange = {{0, 4},
                                                   {0, 3},
                                                   {1, 5},
                                                   {1, 6},
                                                   {2, 5}};
    int minMaxCost = 100000;
    std::vector<int> plan;
    std::vector<int> mWeeklyCost;
};


#endif //COURSE_WORK_REPAIRPLAN_H
