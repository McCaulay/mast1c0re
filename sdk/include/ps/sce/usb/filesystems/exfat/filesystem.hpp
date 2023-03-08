#pragma once

#if (defined(PS4) && PS4) || (defined(PS5) && PS5)
#include <ps/sce/libkernel.hpp>
#ifdef LIBKERNEL
#include <ps/sce/usb/filesystems/exfat/boot-sector.hpp>
#include <ps/sce/usb/filesystems/exfat/directory.hpp>
#include <ps/sce/usb/filesystems/exfat/file.hpp>
#include <ps/sce/usb/mass-store.hpp>
#include <ps/sce/sce.hpp>
#include <common/list.hpp>

#define SECTOR_SIZE 512
#define EXFAT_END_OF_CLUSTER_CHAIN 0xFFFFFFFF

// See: https://github.com/MicrosoftDocs/win32/blob/docs/desktop-src/FileIO/exfat-specification.md

namespace exFAT
{
    class Filesystem
    {
    public:
        Filesystem();
        Filesystem(PS::MassStore* massStore, uint32_t baseAddress, exFAT::BootSector bootSector);
        bool isMounted();
        bool mount();
        uint32_t getClusterSize();
        bool root(List<exFAT::Directory>* directories, List<exFAT::File>* files);
        bool directory(const char* filepath, List<exFAT::Directory>* directories, List<exFAT::File>* files = nullptr);
        bool directory(exFAT::Directory parent, List<exFAT::Directory>* directories, List<exFAT::File>* files = nullptr);
        List<exFAT::Directory> directories(const char* filepath);
        List<exFAT::Directory> directories(exFAT::Directory parent);
        List<exFAT::File> files(const char* filepath);
        List<exFAT::File> files(exFAT::Directory parent);
        bool exists(const char* filepath);
        bool resetRead(exFAT::File file);
        bool readNextCluster(exFAT::File file, uint8_t* buffer);
    private:
        bool getDirectoryFromFilepath(const char* filepath, exFAT::Directory* directory);
        bool directory(uint32_t cluster, List<exFAT::Directory>* directories, List<exFAT::File>* files = nullptr);
        bool readCluster(uint32_t blockOffset, uint8_t* buffer);
        bool cacheSector(uint32_t blockOffset);
        uint32_t getNextCluster(uint32_t cluster);
        void sortDirectories(List<exFAT::Directory>* directories);
        void sortFiles(List<exFAT::File>* files);
        char* strcatwn(char* destination, uint8_t* source, uint32_t slen);

        inline uint32_t getSectorIndexForCluster(uint32_t index)
        {
            if (index < 2)
                index = 2;
            return this->dataBase + ((index - 2) * this->bootSector.getSectorsPerCluster());
        }

        inline bool readSector(uint32_t blockOffset, uint8_t* buffer)
        {
            return this->readSectors(blockOffset, buffer, 1);
        }

        inline bool readSectors(uint32_t blockOffset, uint8_t* buffer, uint32_t count)
        {
            return this->massStore->readBlock(this->baseAddress + blockOffset, count, SECTOR_SIZE, buffer) == SCE_OK;
        }
    private:
        bool mounted;
        PS::MassStore* massStore;
        uint32_t baseAddress;
        exFAT::BootSector bootSector;
        uint32_t clusterSize;
        uint32_t fatSize;
        uint32_t fatBase;
        uint32_t dataBase;

        uint8_t cache[SECTOR_SIZE];
        uint32_t cacheIndex;

        uint32_t readStartClusterIndex;
        uint32_t readClusterIndex;
    };
}
#endif
#endif