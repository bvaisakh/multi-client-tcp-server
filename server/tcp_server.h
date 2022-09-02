#include "Poco/Task.h"

class TCPServer : public Poco::Task {
public:
    TCPServer();

    void runTask();
};