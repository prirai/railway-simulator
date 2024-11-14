#ifndef RAILWAY_LIB_H
#define RAILWAY_LIB_H
#include <iostream>
#include <algorithm>
#include <utility>
#include <sstream>
#include <vector>
#include <memory>
#include <iomanip>

inline int MAX_LINES = 20;

static void printError(const std::string &error_msg) {
    std::cout << "\033[31mx~~~x " << error_msg << "\033[0m" << std::endl;
}

class Platform {
private:
    int platformID;
    std::vector<std::pair<int, int> > schedule;

public:
    explicit Platform(const int platformID) : platformID(platformID) {
        std::cout << "Platform added with id " << platformID << std::endl;
    }

    ~Platform() = default;

    [[nodiscard]] int get_platform() const { return platformID; }

    static bool haveIntersection(const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return std::max(a.first, b.first) <= std::min(a.second, b.second);
    }

    void addTrain(int time, bool isThroughTrain) {
        int interval = isThroughTrain ? 10 : 30;
        std::pair<int, int> newtrain_sched = std::make_pair(time, time + interval);
        std::string extstr = isThroughTrain ? "through" : "stoppage";
        for (const auto &sched: schedule) {
            if (haveIntersection(newtrain_sched, sched)) {
                std::stringstream errorStream;
                errorStream << "Cannot add train at " << std::setw(4) << std::setfill('0') << newtrain_sched.first <<
                        " hrs which is a [" << extstr <<
                        "] train due to another train from " << std::setw(4) << std::setfill('0') << sched.first <<
                        " to " << std::setw(4) << std::setfill('0') << sched.second << " hrs";
                printError(errorStream.str());
                return;
            }
        }
        schedule.emplace_back(time, isThroughTrain ? time + 10 : time + 30); // Store 0 for through, 1 for stoppage
        std::cout << "Added [" << extstr << "] train for [" << time << "] hrs on platform number [" << platformID << "]"
                << std::endl;

        std::sort(schedule.begin(), schedule.end());
    }
};

class Line {
private:
    std::string name;
    std::weak_ptr<Platform> platform;

public:
    Line(std::string name, const std::shared_ptr<Platform> &pl) : name(std::move(name)), platform(pl) {
        std::cout << "New Line [" << this->name << "] associated with platform [" << platform.lock()->get_platform() <<
                "]" << std::endl;
    }

    [[nodiscard]] std::string getLineName() const { return name; }

    ~Line() = default;
};

class StationBase {
public:
    virtual ~StationBase() = default;

    virtual void addLineAndPlatform(const std::string &lineName, int platformNum) = 0;

    virtual void addTrain(int platformNo, int time, bool isThrough) = 0;

    virtual void stationStats() = 0;

    virtual void changeMaxPlatforms(int newSize) = 0;

    void printStationID() {
        std::cout << "Station ID: ";
        printStationIDImpl();
    }

protected:
    virtual void printStationIDImpl() = 0;
};

template<typename T>
class Station : public StationBase {
private:
    T stationID;
    int max_platforms;
    std::vector<std::shared_ptr<Platform> > platforms;
    std::vector<Line> lines;
    int line_index = 0;

public:
    Station(T id, const int max_platforms) : stationID(id), max_platforms(max_platforms) {
        std::cout << "Station added with id " << id << std::endl;
    }

    T get_station() {
        return stationID;
    }

    void printStationIDImpl() override {
        std::cout << stationID << std::endl;
    }

    void changeMaxPlatforms(const int newSize) override {
        max_platforms = newSize;
        std::cout << "Max platforms of " << get_station() << " changed to " << max_platforms << std::endl;
    }

    void addLineAndPlatform(const std::string &line_name, int platform_num) override {
        if (platform_num > max_platforms) {
            // throw std::out_of_range("Platform number out of range");
            std::stringstream errorStream;
            errorStream << "Platform number [" << platform_num << "] higher than maximum allowed platforms [" <<
                    max_platforms << "]";
            printError(errorStream.str());
        } else if (platform_num <= 0) {
            // throw std::out_of_range("Platform number should be between 1 and " + std::to_string(max_platforms));
            std::stringstream errorStream;
            errorStream << "Platform number should be a positive integer upto " << max_platforms;
            printError(errorStream.str());
        } else {
            bool no_error = true;

            for (auto pl: this->platforms) {
                // std::cout << pl->get_platform() << ", ";
                if (pl->get_platform() == platform_num) {
                    std::stringstream errorStream;
                    errorStream << "Cannot add platform " << platform_num << " which already exists.";
                    printError(errorStream.str());
                    no_error = false;
                    break;
                }
                // std::cout << std::endl;
            }
            if (no_error) {
                const auto new_platform = std::make_shared<Platform>(platform_num);
                platforms.push_back(new_platform);
                this->lines.emplace_back(line_name, new_platform);
            }
        }
    }

    void addTrain(int platform_no, int time, bool isThrough) override {
        for (const auto &platform: platforms) {
            if (platform->get_platform() == platform_no) {
                platform->addTrain(time, isThrough);
                return;
            }
        }
        // throw std::out_of_range("Platform number out of range");
        std::stringstream errorStream;
        errorStream << "Platform [" << platform_no << "] does not exist on the station";
        printError(errorStream.str());
    }

    void stationStats() override {
        std::cout << "\033[33m---------------------------------------------\033[0m" << std::endl;
        std::cout << "\x1b[1;36mStation Name: " << get_station() << "\x1b[0;0m" << std::endl;
        std::cout << "Platforms: " << std::endl;
        for (int i = 0; i < platforms.size(); ++i) {
            const auto platform = platforms.at(i)->get_platform();
            const auto line = lines.at(i).getLineName();
            std::cout << platform << "\t" << line << std::endl;
        }
        std::cout << "\033[33m---------------------------------------------\033[0m" << std::endl;
    }
};
#endif
