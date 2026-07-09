#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace streaming {

enum class StreamState {
    Idle,
    Connecting,
    Streaming,
    Retrying,
    Failed,
    Stopped
};

enum class StreamErrorCode {
    None,
    InvalidUrl,
    SourceOpenFailed,
    FrameReceiveFailed,
    RetryLimitExceeded,
    EndOfStream
};

struct StreamFrame {
    std::uint64_t sequence = 0;
    std::int64_t timestampMs = 0;
    std::vector<std::uint8_t> bytes;
};

struct RetryPolicy {
    int maxAttempts = 3;
    int backoffMs = 0;
};

struct StreamResult {
    bool ok = false;
    StreamState state = StreamState::Idle;
    StreamErrorCode errorCode = StreamErrorCode::None;
    std::string message;
    std::string recommendedAction;
    std::size_t framesReceived = 0;
    int retryAttempts = 0;
};

class IFrameSource {
public:
    virtual ~IFrameSource() = default;
    virtual bool open(const std::string &rtspUrl, std::string &errorMessage) = 0;
    virtual bool readFrame(StreamFrame &frame, std::string &errorMessage) = 0;
    virtual void close() = 0;
};

class MockFrameSource final : public IFrameSource {
public:
    MockFrameSource(std::vector<StreamFrame> frames, int openFailures = 0, int readFailures = 0);

    bool open(const std::string &rtspUrl, std::string &errorMessage) override;
    bool readFrame(StreamFrame &frame, std::string &errorMessage) override;
    void close() override;

private:
    std::vector<StreamFrame> frames_;
    std::size_t nextFrame_ = 0;
    int openFailuresRemaining_ = 0;
    int readFailuresRemaining_ = 0;
    bool opened_ = false;
};

class StreamClient {
public:
    explicit StreamClient(std::unique_ptr<IFrameSource> source);

    void setRetryPolicy(RetryPolicy policy);
    StreamState state() const;
    const std::string &lastLog() const;
    const StreamFrame &lastFrame() const;
    std::size_t framesReceived() const;

    StreamResult start(const std::string &rtspUrl, std::size_t targetFrames);
    void stop();

private:
    static bool isValidRtspUrl(const std::string &rtspUrl);
    void log(const std::string &level, const std::string &message);
    StreamResult fail(StreamErrorCode code, const std::string &message, const std::string &action) const;

    std::unique_ptr<IFrameSource> source_;
    RetryPolicy retryPolicy_;
    StreamState state_ = StreamState::Idle;
    StreamFrame lastFrame_;
    std::size_t framesReceived_ = 0;
    std::string lastLog_;
};

} // namespace streaming
