#pragma once

#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Task.h"
#include "Poco/TaskManager.h"
#include "Poco/DateTimeFormatter.h"
#include <iostream>


using Poco::Util::Application;
using Poco::Util::ServerApplication;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::Task;
using Poco::TaskManager;
using Poco::DateTimeFormatter;

class DaemonServer : public ServerApplication {
public:
    DaemonServer();

    ~DaemonServer();

protected:
    void initialize(Application &self) override;

    void uninitialize() override;

    void defineOptions(OptionSet &options) override;

    void handleHelp(const std::string &name, const std::string &value);

    void displayHelp();

    int main(const ArgVec &args) override;

private:
    bool _helpRequested;
    std::string getDefaultConfigFileName();
};
