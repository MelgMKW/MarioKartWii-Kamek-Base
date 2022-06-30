#include <Kamek/kamek.hpp>
#include <System/fileformats.hpp>

class SaveDataManager{
public:
    u8 unknown_0x0[0x10-0x0];
    virtual void func_vtable(); //808b3c98
    u8 unknown_0x14[4];
    RKG *rkg;
    u8 unknown_0x1C[0x25008-0x1C];
}; //total size 0x25008

extern SaveDataManager *saveDataManager;