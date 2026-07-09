#include "streaming/StreamClient.h"

#include <iostream>
#include <memory>
#include <vector>

namespace {
int expect(bool condition, const char *message, int code) {
    if (!condition) {
        std::cerr << message << '\n';
        return code;
    }
    return 0;
}

std::vector<streaming::StreamFrame> sampleFrames() {
    return {
        {1, 1000, {0x00, 0x00, 0x01, 0x65}},
        {2, 1033, {0x00, 0x00, 0x01, 0x41}},
        {3, 1066, {0x00, 0x00, 0x01, 0x41}},
    };
}
} // namespace

int main() {
    {
        auto source = std::make_unique<streaming::MockFrameSource>(sampleFrames());
        streaming::StreamClient client(std::move(source));
        streaming::StreamResult result = client.start("rtsp://127.0.0.1/stream/1", 3);
        if (int rc = expect(result.ok, "Expected stream playback to complete", 2)) return rc;
        if (int rc = expect(result.framesReceived == 3, "Expected three frames", 3)) return rc;
        if (int rc = expect(client.lastFrame().sequence == 3, "Expected last frame sequence 3", 4)) return rc;
        if (int rc = expect(client.state() == streaming::StreamState::Stopped,
                            "Expected stopped state after target frames", 5)) return rc;
    }

    {
        auto source = std::make_unique<streaming::MockFrameSource>(sampleFrames(), 1);
        streaming::StreamClient client(std::move(source));
        client.setRetryPolicy({2, 0});
        streaming::StreamResult result = client.start("rtsp://127.0.0.1/stream/1", 1);
        if (int rc = expect(result.ok, "Expected retry to recover from one open failure", 6)) return rc;
        if (int rc = expect(result.retryAttempts == 1, "Expected one retry attempt", 7)) return rc;
        if (int rc = expect(result.framesReceived == 1, "Expected one received frame", 8)) return rc;
    }

    {
        auto source = std::make_unique<streaming::MockFrameSource>(sampleFrames(), 3);
        streaming::StreamClient client(std::move(source));
        client.setRetryPolicy({2, 0});
        streaming::StreamResult result = client.start("rtsp://127.0.0.1/stream/1", 1);
        if (int rc = expect(!result.ok, "Expected retry limit failure", 9)) return rc;
        if (int rc = expect(result.errorCode == streaming::StreamErrorCode::RetryLimitExceeded,
                            "Expected retry limit error", 10)) return rc;
    }

    {
        auto source = std::make_unique<streaming::MockFrameSource>(sampleFrames());
        streaming::StreamClient client(std::move(source));
        streaming::StreamResult result = client.start("http://127.0.0.1/stream/1", 1);
        if (int rc = expect(!result.ok, "Expected invalid URL failure", 11)) return rc;
        if (int rc = expect(result.errorCode == streaming::StreamErrorCode::InvalidUrl,
                            "Expected invalid URL error code", 12)) return rc;
    }

    {
        auto source = std::make_unique<streaming::MockFrameSource>(sampleFrames(), 0, 1);
        streaming::StreamClient client(std::move(source));
        streaming::StreamResult result = client.start("rtsp://127.0.0.1/stream/1", 1);
        if (int rc = expect(!result.ok, "Expected frame receive failure", 13)) return rc;
        if (int rc = expect(result.errorCode == streaming::StreamErrorCode::FrameReceiveFailed,
                            "Expected frame receive error code", 14)) return rc;
    }

    std::cout << "Streaming tests passed\n";
    return 0;
}
