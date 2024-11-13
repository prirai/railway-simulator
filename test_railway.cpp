#include "lib/railway_lib.h"

int main() {
    Station<int> st1(10, 10);
    st1.addLineAndPlatform("South", 1);
    st1.addTrain(1, 100, true);
    st1.addLineAndPlatform("NW", 2);
    st1.addTrain(2, 200, true);
    st1.addTrain(1, 120, false);
    st1.addTrain(1, 130, true);
    st1.addTrain(2, 190, false);
    st1.stationStats();
}