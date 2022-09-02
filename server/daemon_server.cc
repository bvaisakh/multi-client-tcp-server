#include "./daemon_server.h"

#include <iostream>

#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Task.h"
#include "Poco/TaskManager.h"
#include "Poco/DateTimeFormatter.h"

#include "./tcp_server.h"


using Poco::Util::Application;
using Poco::Util::ServerApplication;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::TaskManager;
using Poco::DateTimeFormatter;
using std::string;


DaemonServer::DaemonServer() : _helpRequested(false) {
}

DaemonServer::~DaemonServer() {
}

void DaemonServer::initialize(Application &self) {
    std::string config_path = config().getString("config-file", getDefaultConfigFileName());
    logger().information("loading configurations from '%s'", config_path);
    loadConfiguration(config_path);
    loadConfiguration();
    ServerApplication::initialize(self);
    logger().information("starting up");
}

void DaemonServer::uninitialize() {
    logger().information("shutting down");
    ServerApplication::uninitialize();
}

void DaemonServer::defineOptions(OptionSet &options) {
    ServerApplication::defineOptions(options);

    options.addOption(
            Option("help", "h", "display help information on command line arguments").required(false).repeatable(
                    false).callback(OptionCallback<DaemonServer>(this, &DaemonServer::handleHelp)));
    options.addOption(
            Option("config-file", "c", "path to the configuration file").required(false).repeatable(false).argument(
                    "config-file").binding("config-file"));
}

void DaemonServer::handleHelp(const std::string &name, const std::string &value) {
    _helpRequested = true;
    displayHelp();
    stopOptionsProcessing();
}

void DaemonServer::displayHelp() {
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader(
            "A sample server application that demonstrates some of the features of the Util::ServerApplication class.");
    helpFormatter.format(std::cout);
}

int DaemonServer::main(const ArgVec &args) {
    if (!_helpRequested) {
        TaskManager tm;
        tm.start(new TCPServer);
        waitForTerminationRequest();
        tm.cancelAll();
        tm.joinAll();
    }
    return Application::EXIT_OK;
}

string DaemonServer::getDefaultConfigFileName() {
    return "config.xml";
}

POCO_SERVER_MAIN(DaemonServer)
