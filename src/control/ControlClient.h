#pragma once
#include <string>

namespace control {

class ControlClient {
public:
    ControlClient() = default;
    // Attempts to connect to rtsp_url via Control Protocol; on success fills session_id and returns true.
    bool connect(const std::string &rtsp_url, std::string &session_id);
    // Disconnects session; no-op for this stub implementation.
    void disconnect(const std::string &session_id);
};

} // namespace control
