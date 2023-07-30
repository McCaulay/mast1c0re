#pragma once

#include <types.hpp>
#include "mast1c0re.hpp"

class Dumper
{
public:
    static bool dumpDirectory(PS::TcpClient* client, const char* dirpath);
    static bool dumpFile(PS::TcpClient* client, const char* dirpath, const char* filename);
};