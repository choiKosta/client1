#include "control/ControlClient.h"

#include <iostream>

namespace {
int expect(bool condition, const char *message, int code) {
    if (!condition) {
        std::cerr << message << '\n';
        return code;
    }
    return 0;
}
} // namespace

int main() {
    control::ControlClient client;
    std::string session;

    bool ok = client.connect("rtsp://127.0.0.1/stream/1", session);
    if (int rc = expect(ok, "Expected connect to succeed for valid RTSP URL", 2)) return rc;
    if (int rc = expect(!session.empty(), "Expected non-empty session id after connect", 3)) return rc;
    if (int rc = expect(client.state() == control::ConnectionState::Connected,
                        "Expected connected state after connect", 4)) return rc;

    std::string invalidSession;
    bool invalidOk = client.connect("http://bad-url", invalidSession);
    if (int rc = expect(!invalidOk, "Expected connect to fail for invalid URL", 5)) return rc;
    if (int rc = expect(client.state() == control::ConnectionState::Failed,
                        "Expected failed state after invalid URL", 6)) return rc;

    ok = client.connect("rtsp://127.0.0.1/stream/1", session);
    if (int rc = expect(ok, "Expected reconnect to succeed", 7)) return rc;

    control::PlaybackSettings settings{30, "1280x720"};
    control::OperationResult settingsResult = client.setPlaybackSettings(settings);
    if (int rc = expect(settingsResult.ok, "Expected valid playback settings to succeed", 8)) return rc;

    control::PlaybackSettings badSettings{0, "720p"};
    control::OperationResult badSettingsResult = client.setPlaybackSettings(badSettings);
    if (int rc = expect(!badSettingsResult.ok, "Expected invalid playback settings to fail", 9)) return rc;

    ok = client.connect("rtsp://127.0.0.1/stream/1", session);
    if (int rc = expect(ok, "Expected reconnect after settings failure to succeed", 10)) return rc;

    client.disconnect(session);
    if (int rc = expect(client.state() == control::ConnectionState::Disconnected,
                        "Expected disconnected state after disconnect", 11)) return rc;

    std::cout << "All tests passed\n";
    return 0;
}
