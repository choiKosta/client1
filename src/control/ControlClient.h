#pragma once

#include <cstdint>
#include <string>

namespace control {

enum class ConnectionState {
    Disconnected,
    Connecting,
    Connected,
    Retrying,
    Failed
};

enum class ErrorCode {
    None,
    InvalidRtspUrl,
    InvalidSession,
    InvalidPlaybackSettings,
    NetworkError,
    ProtocolError,
    ServerError
};

struct PlaybackSettings {
    int frameRate = 30;
    std::string resolution = "1280x720";
};

struct OperationResult {
    bool ok = false;
    int status = 0;
    std::string message;
    std::string sessionId;
    ErrorCode errorCode = ErrorCode::None;
    std::string recommendedAction;
};

class ControlClient {
public:
    ControlClient() = default;
    ControlClient(std::string host, std::uint16_t port);

    void setEndpoint(std::string host, std::uint16_t port);
    void setNetworkTransportEnabled(bool enabled);

    ConnectionState state() const;
    const std::string &lastLog() const;
    const std::string &currentSessionId() const;

    OperationResult connectStream(const std::string &rtspUrl);
    OperationResult disconnectSession();
    OperationResult setPlaybackSettings(const PlaybackSettings &settings);
    OperationResult getStatus();

    bool connect(const std::string &rtspUrl, std::string &sessionId);
    void disconnect(const std::string &sessionId);

private:
    static bool isValidRtspUrl(const std::string &rtspUrl);
    static bool isValidSettings(const PlaybackSettings &settings);
    static std::string escapeJson(const std::string &value);
    static std::string makeRequestId();
    static OperationResult parseResponse(const std::string &response);

    OperationResult sendCommand(const std::string &command, const std::string &paramsJson);
    OperationResult localConnect(const std::string &rtspUrl);
    OperationResult localDisconnect();
    OperationResult localSetPlaybackSettings(const PlaybackSettings &settings);
    void log(const std::string &level, const std::string &message);
    void markFailure(const std::string &message, const std::string &action);

    std::string host_ = "127.0.0.1";
    std::uint16_t port_ = 9000;
    bool networkTransportEnabled_ = false;
    ConnectionState state_ = ConnectionState::Disconnected;
    std::string sessionId_;
    PlaybackSettings settings_;
    std::string lastLog_;
};

} // namespace control
