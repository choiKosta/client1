#!/usr/bin/env python3
import socket
import json
import sys

HOST = '127.0.0.1'
PORT = 9000

def send_connect(rtsp_url, request_id='r-test'):
    req = {
        'type': 'command',
        'command': 'connect',
        'params': { 'rtsp_url': rtsp_url },
        'request_id': request_id
    }
    with socket.create_connection((HOST, PORT), timeout=5) as sock:
        sock.sendall((json.dumps(req) + '\n').encode('utf-8'))
        data = sock.recv(4096)
        if not data:
            print('No response', file=sys.stderr)
            return 2
        try:
            resp = json.loads(data.decode('utf-8').strip())
        except json.JSONDecodeError:
            print('Bad JSON response', file=sys.stderr)
            return 3
        print(json.dumps(resp, indent=2))
        if resp.get('status') == 200:
            return 0
        return 4

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage: control_integration_client.py <rtsp_url>')
        sys.exit(1)
    sys.exit(send_connect(sys.argv[1]))
