#pragma once

#include <types.hpp>
#include "mast1c0re.hpp"

#define LOG_EXTRA true

class Dumper
{
public:
    static bool dumpDirectory(PS::TcpClient* client, const char* dirpath);
private:
    static bool dumpFile(PS::TcpClient* client, const char* dirpath, const char* filename);
    static bool sendFileData(PS::TcpClient* client, uint64_t address, uint64_t size, uint64_t offset);
    static uint64_t searchElfHeader(int fd, uint64_t address, uint64_t maxOffset);
};