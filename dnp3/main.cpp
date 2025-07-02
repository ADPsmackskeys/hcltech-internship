// This file should be located at
// opendnp3/cpp/examples/master

#include "ExampleListenCallbacks.h"
#include <opendnp3/ConsoleLogger.h>
#include <opendnp3/DNP3Manager.h>
#include <opendnp3/channel/PrintingChannelListener.h>
#include <opendnp3/logging/LogLevels.h>
#include <opendnp3/master/DefaultMasterApplication.h>
#include <opendnp3/master/PrintingCommandResultCallback.h>
#include <opendnp3/master/PrintingSOEHandler.h>
#include <opendnp3/master/IMaster.h>

#include <nlohmann/json.hpp>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using json = nlohmann::json;
using namespace opendnp3;
using namespace std;

std::map<std::string, std::shared_ptr<IMaster>> masterMap;

void LoadAndRunScheduledTasks(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) return;

    json schedule;
    f >> schedule;

    for (const auto& task : schedule["tasks"]) {
        std::thread([task]() {
            std::tm tm = {};
            std::istringstream ss(task["time"].get<std::string>());
            ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

            if (ss.fail()) {
                std::cerr << "Failed to parse time: " << task["time"] << std::endl;
                return;
            }

            const auto targetTime = std::chrono::system_clock::from_time_t(std::mktime(&tm));
            std::this_thread::sleep_until(targetTime);

            const std::string device = task["device"];
            if (masterMap.find(device) != masterMap.end()) {
                auto master = masterMap[device];
                if (task["command"] == "crob") {
                    ControlRelayOutputBlock crob(OperationType::LATCH_ON);
                    master->SelectAndOperate(crob, task["index"], PrintingCommandResultCallback::Get());
                }
                else if (task["command"] == "integrity") {
                    master->ScanClasses(ClassField::AllClasses(), PrintingSOEHandler::Create());
                }
            }
        }).detach();
    }
}

void RunMaster(DNP3Manager& manager, const json& config) {
    const auto logLevels = levels::NORMAL | levels::ALL_APP_COMMS;

    auto localAddr = config["local_addr"].get<uint16_t>();
    auto timeoutSec = config["response_timeout_seconds"].get<int>();

    for (const auto& out : config["outstations"]) {
        std::string id = out["id"];
        auto channel = manager.AddTCPClient(id, logLevels, ChannelRetry::Default(),
                                            {IPEndpoint(out["ip"], out["port"])} ,
                                            "0.0.0.0", PrintingChannelListener::Create());

        MasterStackConfig stack;
        stack.link.LocalAddr = localAddr;
        stack.link.RemoteAddr = out["remote_addr"];
        stack.master.responseTimeout = TimeDuration::Seconds(timeoutSec);
        stack.master.disableUnsolOnStartup = config["disable_unsolicited_on_startup"];

        auto master = channel->AddMaster(id, PrintingSOEHandler::Create(),
                                         DefaultMasterApplication::Create(), stack);

        master->AddClassScan(ClassField::AllClasses(),
            TimeDuration::Minutes(config["scan_intervals"]["integrity_minutes"]), PrintingSOEHandler::Create());
        master->AddClassScan(ClassField(ClassField::CLASS_1),
            TimeDuration::Seconds(config["scan_intervals"]["exception_seconds"]), PrintingSOEHandler::Create());

        master->Enable();
        masterMap[id] = master;
    }

    LoadAndRunScheduledTasks("commands.json");

    while (true) {
        std::cout << "Command (e.g., integrity <device>, crob <device> <index>, x to exit): ";
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

        if (tokens.empty()) continue;
        if (tokens[0] == "x") break;
        if (tokens[0] == "integrity" && tokens.size() == 2) {
            auto it = masterMap.find(tokens[1]);
            if (it != masterMap.end())
                it->second->ScanClasses(ClassField::AllClasses(), PrintingSOEHandler::Create());
            else
                std::cerr << "Unknown device" << std::endl;
        }
        else if (tokens[0] == "crob" && tokens.size() == 3) {
            auto it = masterMap.find(tokens[1]);
            if (it != masterMap.end()) {
                ControlRelayOutputBlock crob(OperationType::LATCH_ON);
                it->second->SelectAndOperate(crob, stoi(tokens[2]), PrintingCommandResultCallback::Get());
            }
            else
                std::cerr << "Unknown device" << std::endl;
        }
        else {
            std::cerr << "Unknown or malformed command" << std::endl;
        }
    }
}

int main() {
    std::ifstream f("config.json");
    if (!f.is_open()) {
        std::cerr << "config.json not found" << std::endl;
        return 1;
    }

    json config;
    f >> config;

    DNP3Manager manager(std::thread::hardware_concurrency(), ConsoleLogger::Create());

    RunMaster(manager, config["master"]);
    return 0;
}
