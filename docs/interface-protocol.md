# Interface Protocol

This document extracts the Control Protocol and Streaming Interface used by Developer and Reviewer agents.

## Streaming Interface (RTSP/H.264)
- RTSP 1.0 for session control (SETUP, PLAY, PAUSE, TEARDOWN)
- RTP/H.264 payload; support RTP over UDP or RTP over TCP (interleaved)
- Example URL: `rtsp://<host>:554/stream/<camera-id>`

## Control Protocol (JSON-over-TCP) - Recommended Baseline
- Transport: TCP (port example: 9000)
- Encoding: UTF-8 JSON objects

Request example:
```json
{
  "type": "command",
  "command": "connect",
  "params": { "rtsp_url": "rtsp://10.0.0.2:554/stream/1" },
  "request_id": "r-12345"
}
```

Response example:
```json
{
  "type": "response",
  "request_id": "r-12345",
  "status": 200,
  "message": "connected",
  "data": { "session_id": "s-98765" }
}
```

Supported commands (example): `connect`, `disconnect`, `set_param`, `get_status`, `ping`

Status codes: 200 OK, 400 Bad Request, 401 Unauthorized, 404 Not Found, 500 Internal Error

Security: token-based auth (Bearer) or TLS; include `auth_token` in message or use TLS channel

Retry policy: exponential backoff (e.g., 1s, 2s, 5s) for transient network errors

Place-holder: If server exposes a different control API (WebSocket/HTTP/gRPC), update this file to reflect the agreed protocol.
