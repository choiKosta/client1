#include "control/ControlClient.h"
#include <cassert>
#include <iostream>

int main() {
    control::ControlClient client;
    std::string session;

    bool ok = client.connect("rtsp://127.0.0.1/stream/1", session);
    if (!ok) {
        std::cerr << "Expected connect to succeed for valid RTSP URL\n";
        return 2;
    }
    if (session.empty()) {
        std::cerr << "Expected non-empty session id after connect\n";
        return 3;
    }

    // invalid url
    std::string s2;
    bool ok2 = client.connect("http://bad-url", s2);
    if (ok2) {
        std::cerr << "Expected connect to fail for invalid URL\n";
        return 4;
    }

    client.disconnect(session);
    std::cout << "All tests passed\n";
    return 0;
}
