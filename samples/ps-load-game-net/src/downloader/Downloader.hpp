#pragma once

#include <types.hpp>
#include "mast1c0re.hpp"

#define MAGIC 0x0000EA6E
#define DOWNLOAD_CHUNK_SIZE 4096
#define DOWNLOAD_BAR_UPDATE_FREQUENCY 2500

class Downloader
{
public:
    static bool download(const char* filepath, uint16_t port);
private:
    static void setProgress(PS::Sce::MsgDialogProgressBar dialog, const char* basename, size_t downloaded, size_t total);
};