#include "streaming/StreamClient.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>

namespace streaming {

MockFrameSource::MockFrameSource(std::vector<StreamFrame> frames, int openFailures, int readFailures)
    : frames_(std::move(frames)),
      openFailuresRemaining_(openFailures),
      readFailuresRemaining_(readFailures) {}

bool MockFrameSource::open(const std::string &rtspUrl, std::string &errorMessage) {
    if (rtspUrl.rfind("rtsp://", 0) != 0) {
        errorMessage = "invalid RTSP URL";
        return false;
    }
    if (openFailuresRemaining_ > 0) {
        --openFailuresRemaining_;
        errorMessage = "mock open failure";
        return false;
    }
    opened_ = true;
    errorMessage.clear();
    return true;
}

bool MockFrameSource::readFrame(StreamFrame &frame, std::string &errorMessage) {
    if (!opened_) {
        errorMessage = "source is not open";
        return false;
    }
    if (readFailuresRemaining_ > 0) {
        --readFailuresRemaining_;
        errorMessage = "mock frame receive failure";
        return false;
    }
    if (nextFrame_ >= frames_.size()) {
        errorMessage = "end of stream";
        return false;
    }
    frame = frames_[nextFrame_++];
    errorMessage.clear();
    return true;
}

void MockFrameSource::close() {
    opened_ = false;
}

StreamClient::StreamClient(std::unique_ptr<IFrameSource> source)
    : source_(std::move(source)) {}

void StreamClient::setRetryPolicy(RetryPolicy policy) {
    retryPolicy_ = policy;
    if (retryPolicy_.maxAttempts < 1) {
        retryPolicy_.maxAttempts = 1;
    }
    if (retryPolicy_.backoffMs < 0) {
        retryPolicy_.backoffMs = 0;
    }
}

StreamState StreamClient::state() const {
    return state_;
}

const std::string &StreamClient::lastLog() const {
    return lastLog_;
}

const StreamFrame &StreamClient::lastFrame() const {
    return lastFrame_;
}

std::size_t StreamClient::framesReceived() const {
    return framesReceived_;
}

StreamResult StreamClient::start(const std::string &rtspUrl, std::size_t targetFrames) {
    if (!source_) {
        return fail(StreamErrorCode::SourceOpenFailed, "stream source is not configured",
                    "Configure a frame source before starting playback");
    }
    if (!isValidRtspUrl(rtspUrl)) {
        return fail(StreamErrorCode::InvalidUrl, "invalid RTSP URL", "RTSP URL must start with rtsp://");
    }

    framesReceived_ = 0;
    state_ = StreamState::Connecting;
    log("INFO", "stream start requested: " + rtspUrl);

    std::string errorMessage;
    int retryAttempts = 0;
    bool opened = false;
    for (int attempt = 1; attempt <= retryPolicy_.maxAttempts; ++attempt) {
        if (source_->open(rtspUrl, errorMessage)) {
            opened = true;
            break;
        }
        retryAttempts = attempt;
        if (attempt < retryPolicy_.maxAttempts) {
            state_ = StreamState::Retrying;
            log("WARN", "stream open failed, retrying: " + errorMessage);
            if (retryPolicy_.backoffMs > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(retryPolicy_.backoffMs));
            }
        }
    }

    if (!opened) {
        state_ = StreamState::Failed;
        source_->close();
        return {false, state_, StreamErrorCode::RetryLimitExceeded,
                "stream open failed: " + errorMessage,
                "Check camera URL, network reachability, and server RTSP availability",
                framesReceived_, retryAttempts};
    }

    state_ = StreamState::Streaming;
    log("INFO", "stream opened");

    while (framesReceived_ < targetFrames) {
        StreamFrame frame;
        if (!source_->readFrame(frame, errorMessage)) {
            state_ = StreamState::Failed;
            source_->close();
            return {false, state_, StreamErrorCode::FrameReceiveFailed,
                    "frame receive failed: " + errorMessage,
                    "Retry playback or reconnect to the camera stream",
                    framesReceived_, retryAttempts};
        }
        lastFrame_ = frame;
        ++framesReceived_;
    }

    state_ = StreamState::Stopped;
    source_->close();
    log("INFO", "stream stopped after target frame count");
    return {true, state_, StreamErrorCode::None, "stream playback completed", {}, framesReceived_, retryAttempts};
}

void StreamClient::stop() {
    if (source_) {
        source_->close();
    }
    state_ = StreamState::Stopped;
    log("INFO", "stream stopped");
}

bool StreamClient::isValidRtspUrl(const std::string &rtspUrl) {
    return rtspUrl.rfind("rtsp://", 0) == 0 && rtspUrl.size() > std::string("rtsp://").size();
}

void StreamClient::log(const std::string &level, const std::string &message) {
    lastLog_ = "[" + level + "] " + message;
    std::clog << lastLog_ << '\n';
}

StreamResult StreamClient::fail(StreamErrorCode code, const std::string &message, const std::string &action) const {
    return {false, StreamState::Failed, code, message, action, framesReceived_, 0};
}

} // namespace streaming
