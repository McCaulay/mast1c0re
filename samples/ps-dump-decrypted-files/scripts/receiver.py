#!/usr/bin/env python3

import socket
import struct
import sys
import os
import shutil

PROTO_TAG_START = 0x6e92f11b
PROTO_TAG_FILENAME = 0x41c11d9c
PROTO_TAG_FILEDATA = 0x55e58570
PROTO_TAG_END = 0xdabea3b6

BUFFER_SIZE = 4096

currentFilename = None

# Create output directory
if os.path.exists('output/'):
    shutil.rmtree('output/')
print(f'[#] Creating output/ directory')
os.mkdir('output/')

# Create server socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    print(f'[#] Waiting for connections...')
    s.bind(('0.0.0.0', 9031))
    s.listen()
    conn, addr = s.accept()

    print(f'[#] {addr[0]} connected!')
    print(f'[#] Waiting for start tag...')

    # Receive start packet
    tag = struct.unpack('<I', conn.recv(4))[0]
    if tag != PROTO_TAG_START:
        print(f'[-] Received unexpected start packet!')
        conn.close()
        sys.exit(1)

    # Process packets
    while True:
        tag = struct.unpack('<I', conn.recv(4))[0]
        if tag == PROTO_TAG_END:
            break
        if tag == PROTO_TAG_FILENAME:
            filenameLen = struct.unpack('<I', conn.recv(4))[0]
            currentFilename = conn.recv(filenameLen).decode()

            print(f'[+] Dumping {currentFilename}...')
            continue
        if tag == PROTO_TAG_FILEDATA:
            # Append data to file
            dataOffset = struct.unpack('<Q', conn.recv(8))[0]
            dataLen = struct.unpack('<Q', conn.recv(8))[0]

            # Create file if it does not exist
            if not os.path.exists('output/' + currentFilename):
                with open('output/' + currentFilename, 'wb') as f:
                    pass

            with open('output/' + currentFilename, 'r+b') as f:
                f.seek(0, os.SEEK_END)
                filesize = f.tell()

                # If offset is beyond filesize, pad with 0x00
                if dataOffset > filesize:
                    for i in range(0, dataOffset - filesize):
                        f.write(b'\x00')

                # Set the data offset
                f.seek(dataOffset, os.SEEK_SET)

                # Read data
                current = 0
                while True:
                    copySize = BUFFER_SIZE
                    if current + copySize > dataLen:
                        copySize = dataLen - current
                    
                    buffer = conn.recv(copySize)
                    if len(buffer) > 0:
                        f.write(buffer)
                        current += len(buffer)

                    if current >= dataLen:
                        break
            continue
        else:
            print(f'[-] Unexpected tag: {tag:04x}')
            conn.close()
            sys.exit(2)

    print(f'[#] Fin!')
    conn.close()
