#include <iostream>

#include "parsers/http_request.h"
#include "parsers/http_response.h"
#include "services/resource_manager.h"
#include "sockets/server_manager.h"
#include "sockets/web_socket_exception.h"
#include "utils/logger.h"

int main() {

    ServerManager server_manager;
    server_manager.startServer();

    return 0;
}
