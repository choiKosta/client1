#include "control/ControlClient.h"
#include <algorithm>

namespace control {

bool ControlClient::connect(const std::string &rtsp_url, std::string &session_id) {
    // Simple validation: must start with "rtsp://"
    const std::string prefix = "rtsp://";
    if (rtsp_url.size() >= prefix.size() && std::equal(prefix.begin(), prefix.end(), rtsp_url.begin())) {
        // generate a dummy session id
        session_id = "s-" + std::to_string(std::hash<std::string>{}(rtsp_url));
        return true;
    }
    session_id.clear();
    return false;
}

void ControlClient::disconnect(const std::string &session_id) {
    (void)session_id; // stub
}

} // namespace control
