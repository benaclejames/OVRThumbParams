﻿#pragma once

#include <WS2tcpip.h>
#include <WinSock2.h>

class OSC
{
    WSADATA data{};
    SOCKADDR_IN addr{};
    SOCKET sock;
    
public:
    OSC();

    void send(char* data, int size)
    {
        sendto(sock, data, size, 0, (SOCKADDR*)&addr, sizeof(addr));
    }
};

class osc_message
{

protected:
    osc_message(char* address, char type);

    inline static int quantize(int in, int multiple) {
        if (in % multiple == 0) {
            return in;
        }
        return in + multiple - (in % multiple);
    }

    inline static void swap_endianness(char* inBytes, int size) {
        for (int i=0, j = size - 1; i < size/2; i++, j--) {
            char temp = inBytes[i];
            inBytes[i] = inBytes[j];
            inBytes[j] = temp;
        }
    }

public:
    osc_message(char* data, int size);
    char* address;
    char type;
    int writerIndex;
    char message[256];
};

class osc_bool_message : public osc_message {
public:
    osc_bool_message(char* address, bool value) : osc_message(address, value ? 'T' : 'F') {}
};

class osc_int_message : public osc_message {
public:
    osc_int_message(char* address, int value);
    int parse();
};