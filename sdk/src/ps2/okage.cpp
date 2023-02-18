#include <ps2/okage.hpp>
#include <offsets/okage.hpp>
#include <ps2/ps2.hpp>

Okage::fMemoryCardOpen* Okage::MemoryCardOpen = (Okage::fMemoryCardOpen*)OKAGE_MEMORY_CARD_OPEN;
Okage::fMemoryCardRead*  Okage::MemoryCardRead = (Okage::fMemoryCardRead*)OKAGE_MEMORY_CARD_READ;
Okage::fMemoryCardWrite* Okage::MemoryCardWrite = (Okage::fMemoryCardWrite*)OKAGE_MEMORY_CARD_WRITE;
Okage::fMemoryCardSeek*  Okage::MemoryCardSeek = (Okage::fMemoryCardSeek*)OKAGE_MEMORY_CARD_SEEK;
Okage::fMemoryCardClose* Okage::MemoryCardClose = (Okage::fMemoryCardClose*)OKAGE_MEMORY_CARD_CLOSE;

void Okage::clearScreenText()
{
    // Clear name
    PS2::strncpy((char*)(OKAGE_SCREEN_LOAD_NAME), "\\f[2]Twitter\x81""F@_mccaulay", 25);

    // Move text to read-able area
    PS2::strncpy((char*)(OKAGE_SCREEN_LOAD_LOCATION), "\\n\\n", 4);

    // Clear previous text
    PS2::memset((char*)(OKAGE_SCREEN_LOAD_LOCATION + 4), 0x00, 0x160);
}

int Okage::printf(const char* format, ...)
{
    char* text = (char*)(OKAGE_SCREEN_LOAD_LOCATION + 4);
    char* current = text + PS2::strlen(text);

    va_list args;
    va_start(args, format);
    PS2::vsprintf(current, format, args);
    va_end(args);
    return 0;
}