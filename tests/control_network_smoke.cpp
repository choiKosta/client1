#include "control/ControlClient.h"

#include <iostream>

int main() {
    control::ControlClient client("127.0.0.1", 9000);
    control::OperationResult result = client.connectStream("rtsp://127.0.0.1/stream/1");
    if (!result.ok) {
        std::cerr << "Expected network connect to succeed: " << result.message << '\n';
        return 2;
    }
    if (result.sessionId.empty()) {
        std::cerr << "Expected session_id in network response\n";
        return 3;
    }
    if (client.state() != control::ConnectionState::Connected) {
        std::cerr << "Expected connected state after network response\n";
        return 4;
    }

    std::cout << "Network smoke passed: " << result.sessionId << '\n';
    return 0;
}
