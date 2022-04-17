#include <iostream>

#include "asio.hpp"

#include "config/config.h"
#include "proxy_manager/manager.h"

using namespace std;
using namespace asio;

io_context global_ctx;
const char *global_config_path;
Config::Config *global_config;

void init() {
    global_config = Config::loadConfig(global_config_path);
    AcceptorManager::initWithContext(global_ctx);
    DialerManager::init();
}

int main(int argc, char **argv) {
    cout << "config path: " << argv[1] << endl;
    global_config_path = argv[1];
    init();
    signal_set signal_set(global_ctx);
    signal_set.add(SIGINT);
    signal_set.add(SIGTERM);
    signal_set.async_wait([&](const error_code error, int signum) {
        exit(0);
    });
    AcceptorManager::acceptAll();   // block!
    return 0;
}