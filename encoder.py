#!/usr/bin/python3

import argparse
from base64 import b64encode

# XOR Function
def xor_encrypt(data, key):
    l = len(key)
    key_bytes = key.encode()
    return bytes((data[i] ^ key_bytes[i % l]) for i in range(len(data)))

# Function to format as a C++ unsigned char array
def format_cpp_array(data, array_name):
    formatted = ", ".join(f"0x{b:02x}" for b in data)
    return f"unsigned char {array_name}[] = {{ {formatted} }};"

# Main function
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("shellcodeFile", help="Binary file containing raw shellcode")
    parser.add_argument("key", help="XOR key")
    args = parser.parse_args()

    # Read shellcode from file
    try:
        with open(args.shellcodeFile, "rb") as shellcodeFileHandle:
            shellcodeBytes = shellcodeFileHandle.read()
    except IOError:
        print(f"[!] Could not open or read file: {args.shellcodeFile}")
        exit(1)

    # XOR encrypt shellcode
    encryptedShellcode = xor_encrypt(shellcodeBytes, args.key)

    # Generate C++ formatted output
    cpp_encrypted_shellcode = format_cpp_array(encryptedShellcode, "encryptedShellcode")
    cpp_key = format_cpp_array(args.key.encode(), "key")

    # Print C++ output
    print(cpp_encrypted_shellcode)
    print(cpp_key)
