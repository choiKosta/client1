#!/usr/bin/env python3
import socket
import threading
import json

HOST = '127.0.0.1'
PORT = 9000

def handle_conn(conn, addr):
    with conn:
        data = b''
        while True:
            chunk = conn.recv(4096)
            if not chunk:
                break
            data += chunk
            # try to parse by newline or full JSON
            try:
                txt = data.decode('utf-8').strip()
                if not txt:
                    continue
                req = json.loads(txt)
                resp = {
                    'type': 'response',
                    'request_id': req.get('request_id'),
                    'status': 200,
                    'message': 'connected',
                    'data': { 'session_id': 's-mock-12345' }
                }
                conn.sendall((json.dumps(resp) + '\n').encode('utf-8'))
                break
            except json.JSONDecodeError:
                # wait for more data
                continue

def run_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((HOST, PORT))
        s.listen()
        print(f"Mock server listening on {HOST}:{PORT}")
        try:
            while True:
                conn, addr = s.accept()
                threading.Thread(target=handle_conn, args=(conn, addr), daemon=True).start()
        except KeyboardInterrupt:
            print('Mock server shutting down')

if __name__ == '__main__':
    run_server()
