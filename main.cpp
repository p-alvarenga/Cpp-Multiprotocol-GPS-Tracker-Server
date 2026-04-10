#include "server/server.h"

int main() {
    server::server s(9000, 10);

    s.init();
    s.run();

    return 0;
}