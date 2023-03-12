#pragma once

#if (defined(PS4) && PS4)
#include <types.hpp>
#include <ps/sce/libkernel.hpp>
#ifdef LIBKERNEL
#include <ps/sce/usb/filesystems/mbr/master-boot-record-table.hpp>
#include <ps/sce/usb/filesystems/exfat/filesystem.hpp>
#include <ps/sce/usb/filesystems/exfat/directory.hpp>
#include <common/list.hpp>
#include <ps/sce/usb/usbd.hpp>
#include <ps/sce/usb/mass-store.hpp>

class Usb
{
public:
    static List<Usb> list();
public:
    Usb();
    Usb(uint64_t device);
    Usb(uint16_t vid, uint16_t pid);
    inline bool isOpen();
    bool open();
    bool mount();
    void unmount();
    inline bool isMounted();
    void close();
    uint32_t getClusterSize();

    bool root(List<exFAT::Directory>* directories, List<exFAT::File>* files);
    bool directory(const char* filepath, List<exFAT::Directory>* directories, List<exFAT::File>* files);
    bool directory(exFAT::Directory parent, List<exFAT::Directory>* directories, List<exFAT::File>* files);
    List<exFAT::Directory> directories(const char* filepath);
    List<exFAT::Directory> directories(exFAT::Directory parent);
    List<exFAT::File> files(const char* filepath);
    List<exFAT::File> files(exFAT::Directory parent);
    bool exists(const char* filepath);
    bool resetRead(exFAT::File file);
    bool readNextCluster(exFAT::File file, uint8_t* buffer);
private:
    bool updateDescriptors();
    bool readSector(uint32_t blockAddress, uint8_t* buffer);
    bool readSectors(uint32_t blockAddress, uint8_t* buffer, uint32_t count = 1);
    bool isSectorExFAT(uint8_t* sector);
    bool findFilesystem();
private:
    static uint64_t devices;
private:
    uint64_t device;
    uint16_t vid;
    uint16_t pid;
    uint8_t endpointIn;
    uint8_t endpointOut;
    uint16_t endpointInMaxPacket;
    uint16_t endpointOutMaxPacket;
    PS::Sce::Usbd usbd;
    PS::MassStore massStore;
    MasterBootRecord partition;
    exFAT::Filesystem filesystem;
    bool mounted;

    char vendor[9];
    char product[17];

    uint32_t sectorCount;
    uint32_t sectorSize;
};
#endif
#endif