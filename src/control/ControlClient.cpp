#include "control/ControlClient.h"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <exception>
#include <functional>
#include <iostream>
#include <netdb.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <utility>
#include <unistd.h>

namespace control {

namespace {
constexpr int kOkStatus = 200;
constexpr int kBadRequestStatus = 400;
constexpr int kNetworkFailureStatus = 503;

std::string extractJsonString(const std::string &json, const std::string &key) {
    const std::string quotedKey = "\"" + key + "\"";
    std::size_t pos = json.find(quotedKey);
    if (pos == std::string::npos) {
        return {};
    }
    pos = json.find(':', pos + quotedKey.size());
    if (pos == std::string::npos) {
        return {};
    }
    pos = json.find('"', pos + 1);
    if (pos == std::string::npos) {
        return {};
    }

    std::ostringstream value;
    bool escaped = false;
    for (std::size_t i = pos + 1; i < json.size(); ++i) {
        const char ch = json[i];
        if (escaped) {
            value << ch;
            escaped = false;
        } else if (ch == '\\') {
            escaped = true;
        } else if (ch == '"') {
            return value.str();
        } else {
            value << ch;
        }
    }
    return {};
}

int extractJsonInt(const std::string &json, const std::string &key, int fallback) {
    const std::string quotedKey = "\"" + key + "\"";
    std::size_t pos = json.find(quotedKey);
    if (pos == std::string::npos) {
        return fallback;
    }
    pos = json.find(':', pos + quotedKey.size());
    if (pos == std::string::npos) {
        return fallback;
    }
    ++pos;
    while (pos < json.size() && std::isspace(static_cast<unsigned char>(json[pos]))) {
        ++pos;
    }

    std::size_t end = pos;
    while (end < json.size() && (std::isdigit(static_cast<unsigned char>(json[end])) || json[end] == '-')) {
        ++end;
    }
    if (end == pos) {
        return fallback;
    }

    try {
        return std::stoi(json.substr(pos, end - pos));
    } catch (const std::exception &) {
        return fallback;
    }
}
} // namespace

ControlClient::ControlClient(std::string host, std::uint16_t port)
    : host_(std::move(host)), port_(port), networkTransportEnabled_(true) {}

void ControlClient::setEndpoint(std::string host, std::uint16_t port) {
    host_ = std::move(host);
    port_ = port;
}

void ControlClient::setNetworkTransportEnabled(bool enabled) {
    networkTransportEnabled_ = enabled;
}

ConnectionState ControlClient::state() const {
    return state_;
}

const std::string &ControlClient::lastLog() const {
    return lastLog_;
}

const std::string &ControlClient::currentSessionId() const {
    return sessionId_;
}

OperationResult ControlClient::connectStream(const std::string &rtspUrl) {
    if (!isValidRtspUrl(rtspUrl)) {
        markFailure("invalid RTSP URL", "RTSP URL must start with rtsp://");
        return {false, kBadRequestStatus, "invalid RTSP URL", {}, ErrorCode::InvalidRtspUrl,
                "RTSP URL must start with rtsp://"};
    }

    state_ = ConnectionState::Connecting;
    log("INFO", "connect requested: " + rtspUrl);

    OperationResult result = networkTransportEnabled_
                                 ? sendCommand("connect", "{\"rtsp_url\":\"" + escapeJson(rtspUrl) + "\"}")
                                 : localConnect(rtspUrl);
    if (result.ok) {
        state_ = ConnectionState::Connected;
        sessionId_ = result.sessionId;
        log("INFO", "connected: " + sessionId_);
    } else {
        markFailure(result.message, result.recommendedAction);
    }
    return result;
}

OperationResult ControlClient::disconnectSession() {
    if (sessionId_.empty()) {
        markFailure("no active session", "Connect to a stream before disconnecting");
        return {false, kBadRequestStatus, "no active session", {}, ErrorCode::InvalidSession,
                "Connect to a stream before disconnecting"};
    }

    log("INFO", "disconnect requested: " + sessionId_);
    OperationResult result = networkTransportEnabled_
                                 ? sendCommand("disconnect", "{\"session_id\":\"" + escapeJson(sessionId_) + "\"}")
                                 : localDisconnect();
    if (result.ok) {
        sessionId_.clear();
        state_ = ConnectionState::Disconnected;
        log("INFO", "disconnected");
    } else {
        markFailure(result.message, result.recommendedAction);
    }
    return result;
}

OperationResult ControlClient::setPlaybackSettings(const PlaybackSettings &settings) {
    if (!isValidSettings(settings)) {
        markFailure("invalid playback settings", "Use a frame rate from 1 to 120 and a WIDTHxHEIGHT resolution");
        return {false, kBadRequestStatus, "invalid playback settings", {}, ErrorCode::InvalidPlaybackSettings,
                "Use a frame rate from 1 to 120 and a WIDTHxHEIGHT resolution"};
    }

    std::ostringstream params;
    params << "{\"frame_rate\":" << settings.frameRate << ",\"resolution\":\""
           << escapeJson(settings.resolution) << "\"}";
    OperationResult result = networkTransportEnabled_ ? sendCommand("set_param", params.str())
                                                      : localSetPlaybackSettings(settings);
    if (result.ok) {
        settings_ = settings;
        log("INFO", "playback settings updated");
    } else {
        markFailure(result.message, result.recommendedAction);
    }
    return result;
}

OperationResult ControlClient::getStatus() {
    if (networkTransportEnabled_) {
        return sendCommand("get_status", "{}");
    }
    const bool connected = state_ == ConnectionState::Connected;
    return {true, kOkStatus, connected ? "connected" : "disconnected", sessionId_, ErrorCode::None, {}};
}

bool ControlClient::connect(const std::string &rtspUrl, std::string &sessionId) {
    OperationResult result = connectStream(rtspUrl);
    sessionId = result.sessionId;
    return result.ok;
}

void ControlClient::disconnect(const std::string &sessionId) {
    if (sessionId == sessionId_) {
        (void)disconnectSession();
        return;
    }
    log("WARN", "disconnect ignored for unknown session: " + sessionId);
}

bool ControlClient::isValidRtspUrl(const std::string &rtspUrl) {
    const std::string prefix = "rtsp://";
    return rtspUrl.size() > prefix.size() && std::equal(prefix.begin(), prefix.end(), rtspUrl.begin());
}

bool ControlClient::isValidSettings(const PlaybackSettings &settings) {
    if (settings.frameRate < 1 || settings.frameRate > 120) {
        return false;
    }
    const std::size_t x = settings.resolution.find('x');
    if (x == std::string::npos || x == 0 || x == settings.resolution.size() - 1) {
        return false;
    }
    return std::all_of(settings.resolution.begin(), settings.resolution.end(), [](char ch) {
        return std::isdigit(static_cast<unsigned char>(ch)) || ch == 'x';
    });
}

std::string ControlClient::escapeJson(const std::string &value) {
    std::ostringstream escaped;
    for (char ch : value) {
        if (ch == '"' || ch == '\\') {
            escaped << '\\';
        }
        escaped << ch;
    }
    return escaped.str();
}

std::string ControlClient::makeRequestId() {
    static unsigned long counter = 0;
    return "r-" + std::to_string(++counter);
}

OperationResult ControlClient::parseResponse(const std::string &response) {
    const int status = extractJsonInt(response, "status", 0);
    const std::string message = extractJsonString(response, "message");
    const std::string sessionId = extractJsonString(response, "session_id");
    if (status == kOkStatus) {
        return {true, status, message.empty() ? "ok" : message, sessionId, ErrorCode::None, {}};
    }
    if (status >= 500) {
        return {false, status, message.empty() ? "server error" : message, {}, ErrorCode::ServerError,
                "Retry after checking the control server"};
    }
    return {false, status, message.empty() ? "protocol error" : message, {}, ErrorCode::ProtocolError,
            "Check request parameters and server compatibility"};
}

OperationResult ControlClient::sendCommand(const std::string &command, const std::string &paramsJson) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo *addresses = nullptr;
    const std::string port = std::to_string(port_);
    const int gai = getaddrinfo(host_.c_str(), port.c_str(), &hints, &addresses);
    if (gai != 0) {
        return {false, kNetworkFailureStatus, gai_strerror(gai), {}, ErrorCode::NetworkError,
                "Check the control server address and network connection"};
    }

    int sock = -1;
    for (addrinfo *addr = addresses; addr != nullptr; addr = addr->ai_next) {
        sock = ::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sock < 0) {
            continue;
        }

        timeval timeout{};
        timeout.tv_sec = 3;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

        if (::connect(sock, addr->ai_addr, addr->ai_addrlen) == 0) {
            break;
        }
        ::close(sock);
        sock = -1;
    }
    freeaddrinfo(addresses);

    if (sock < 0) {
        return {false, kNetworkFailureStatus, std::strerror(errno), {}, ErrorCode::NetworkError,
                "Check whether the control server is running and retry"};
    }

    const std::string request = "{\"type\":\"command\",\"command\":\"" + escapeJson(command)
                                + "\",\"params\":" + paramsJson + ",\"request_id\":\"" + makeRequestId()
                                + "\"}\n";
    const ssize_t sent = ::send(sock, request.data(), request.size(), 0);
    if (sent < 0 || static_cast<std::size_t>(sent) != request.size()) {
        const std::string error = std::strerror(errno);
        ::close(sock);
        return {false, kNetworkFailureStatus, error, {}, ErrorCode::NetworkError,
                "Retry after checking the network connection"};
    }

    std::string response;
    char buffer[4096]{};
    while (true) {
        const ssize_t received = ::recv(sock, buffer, sizeof(buffer), 0);
        if (received <= 0) {
            break;
        }
        response.append(buffer, static_cast<std::size_t>(received));
        if (response.find('\n') != std::string::npos) {
            break;
        }
    }
    ::close(sock);

    if (response.empty()) {
        return {false, kNetworkFailureStatus, "empty response", {}, ErrorCode::NetworkError,
                "Retry after checking the control server"};
    }
    return parseResponse(response);
}

OperationResult ControlClient::localConnect(const std::string &rtspUrl) {
    return {true, kOkStatus, "connected",
            "s-" + std::to_string(std::hash<std::string>{}(rtspUrl)), ErrorCode::None, {}};
}

OperationResult ControlClient::localDisconnect() {
    return {true, kOkStatus, "disconnected", {}, ErrorCode::None, {}};
}

OperationResult ControlClient::localSetPlaybackSettings(const PlaybackSettings &) {
    return {true, kOkStatus, "settings updated", sessionId_, ErrorCode::None, {}};
}

void ControlClient::log(const std::string &level, const std::string &message) {
    lastLog_ = "[" + level + "] " + message;
    std::clog << lastLog_ << '\n';
}

void ControlClient::markFailure(const std::string &message, const std::string &action) {
    state_ = ConnectionState::Failed;
    log("ERROR", message + (action.empty() ? "" : " - " + action));
}

} // namespace control
