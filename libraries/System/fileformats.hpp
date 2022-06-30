#pragma once
#include <Kamek/kamek.hpp>

struct RKGHeader{
    char magic[4]; //RKGD
    u8 unknown_0x4[0xC-0x4];
    u16 compressedAndType;
    u8 unknown_0xE[0x88-0xE]; //data is packed idk
}; //total size 0x88

struct RKGInputData{
    u16 faceButtonInputsCount;
    u16 directionInputsCount;
    u16 trickInputsCount;
    u16 unknown_0x6; //padding
    u8 unknown_0x8[0x2774-0x8]; //idk how to do that in c++
}; //total size 0x2774

class RKG{
public:
    void ClearBuffer();
    void CheckValidity(); //8051c120
    void Decompress(RKG *copyBuffer);
    RKGHeader header;
    RKGInputData data;
    u32 CRC32;
};//total size 0x2800

struct BRCTRAnimSubHeader{
    u16 firstGroupOffset;
    u16 groupsCount;
    u16 firstAnimOffset;
    u16 animationsCount;
};

struct BRCTRAnimGroup{
    u16 offsetToName;
    u16 offsetToBRLYTPaneName;
    u16 firstAnimIndex;
    u16 groupAnimationsCount;
}; //total 0x8

struct BRCTRAnimation{
    u16 offsetToName;
    u16 offsetToBRLANName;
    u16 offsetToNextAnimBRLANName;
    u16 isAnimationReversed;
    float animationSpeed; 
}; //total 0xC

struct BRCTRLayoutSubHeader{
    u16 offsetToFirstVariant;
    u16 variantsCount;
    u16 offsetToFirstMessage;
    u16 messagesCount;
    u16 offsetToFirstPicture;
    u16 picturesCount;
}; //total size 0xC

struct BRCTRVariant{
    u16 offsetToName;
    u16 alpha;
    u16 isAnimated;
    u16 padding;
    float animationDelay;
    Vec3 translation43;
    Vec2 scale43;
    Vec3 translation169;
    Vec2 scale169;
    u16 firstMessageIndex;
    u16 messagesCount;
    u16 firstPictureIndex;
    u16 picturesCount;
}; //total size 0x3C

struct BRCTRMessage{
    u16 offsetToBRLYTPaneName;
    u16 offsetToName;
    u32 bmgId;
}; //total size 0x8

struct BRCTRPicture{
    u16 offsetToBRLYTPaneName;
    u16 offsetToSourceBRLYTPaneName;
}; //total size 0x4