/*
Racedata is the class that holds the information to set up a race, such as the gamemode and the characters each player chose
Contributors:
    Seeky (main documentation)
    TheLordScruffy (main documentation)
    WhatIsLoaf (ghost player type)
    riidefi (ParameterFile)
    CLF78 (local player count)
References:
    http://wiki.tockdom.com/wiki/Mission_Mode#mission_single.kmt
    https://github.com/riidefi/mkw/blob/master/source/game/host_system/ParameterFile.hpp
*/

#pragma once
#include <Kamek/kamek.hpp>
#include <Misc/Mii.hpp>

enum VehicleId
{
	STANDARD_KART_S	    =0X00,
	STANDARD_KART_M	    =0X01,
	STANDARD_KART_L	    =0X02,
	BABY_BOOSTER        =0X03,
	CLASSIC_DRAGSTER    =0X04,
	OFFROADER		    =0X05,
	MINI_BEAST		    =0X06,
	WILD_WING		    =0X07,
	FLAME_FLYER		    =0X08,
	CHEEP_CHARGER	    =0X09,
	SUPER_BLOOPER	    =0X0A,
	PIRANHA_PROWLER	    =0X0B,
	RALLY_ROMPER	    =0X0C,
	ROYAL_RACER		    =0X0D,
	JETSETTER		    =0X0E,
	BLUE_FALCON		    =0X0F,
	SPRINTER		    =0X10,
	HONEYCOUPE		    =0X11,
	STANDARD_BIKE_S	    =0X12,
	STANDARD_BIKE_M	    =0X13,
	STANDARD_BIKE_L	    =0X14,
	BULLET_BIKE		    =0X15,
	MACH_BIKE		    =0X16,
	BOWSER_BIKE		    =0X17,
	BIT_BIKE		    =0X18,
	BON_BON		        =0X19,
	WARIO_BIKE		    =0X1A,
	QUACKER			    =0X1B,
	RAPIDE			    =0X1C,
	SHOOTING_STAR	    =0X1D,
	MAGIKRUISER		    =0X1E,
	NITROCYCLE		    =0X1F,
	SPEAR			    =0X20,
	JET_BUBBLE		    =0X21,
	DOLPHIN_DASHER	    =0X22,
	PHANTOM			    =0X23
};

enum CharacterId
{
	MARIO		    	    =0X00,
    BABY_PEACH	    	    =0X01,
    WALUIGI		    	    =0X02,
    BOWSER		    	    =0X03,
    BABY_DAISY	    	    =0X04,
    DRY_BONES	    	    =0X05,
    BABY_MARIO	    	    =0X06,
    LUIGI		    	    =0X07,
    TOAD		    	    =0X08,
    DONKEY_KONG	    	    =0X09,
    YOSHI		    	    =0X0A,
    WARIO		    	    =0X0B,
    BABY_LUIGI	    	    =0X0C,
    TOADETTE	    	    =0X0D,
    KOOPA_TROOPA	    	=0X0E,
    DAISY		    	    =0X0F,
    PEACH		    	    =0X10,
    BIRDO		    	    =0X11,
    DIDDY_KONG	    	    =0X12,
    KING_BOO		    	=0X13,
    BOWSER_JR	    		=0X14,
    DRY_BOWSER	    		=0X15,
    FUNKY_KONG	    		=0X16,
    ROSALINA	    		=0X17,
    MII_S_A_MALE	        =0X18,
    MII_S_A_FEMALE	        =0X19,
    MII_S_B_MALE	        =0X1A,
    MII_S_B_FEMALE	        =0X1B,
    MII_S_C_MALE	        =0X1C,
    MII_S_C_FEMALE	        =0X1D,
    MII_M_A_MALE	        =0X1E,
    MII_M_A_FEMALE          =0X1F,
    MII_M_B_MALE	        =0X20,
    MII_M_B_FEMALE	        =0X21,
    MII_M_C_MALE	        =0X22,
    MII_M_C_FEMALE          =0X23,
    MII_L_A_MALE	        =0X24,
    MII_L_A_FEMALE	        =0X25,
    MII_L_B_MALE	        =0X26,
    MII_L_B_FEMALE	        =0X27,
    MII_L_C_MALE 	        =0X28,
    MII_L_C_FEMALE	        =0X29,
    MII_M			        =0X2A,
    MII_S			        =0X2B,
    MII_L			        =0X2C,
    PEACH_BIKER		        =0X2D,
    DAISY_BIKER		        =0X2E,
    ROSALINA_BIKER		    =0X2F
};

enum CourseId
{
    LUIGI_CIRCUIT = 0X08,
    MOO_MOO_MEADOWS = 0X01,
    MUSHROOM_GORGE = 0X02,
    TOADS_FACTORY = 0X04,

    MARIO_CIRCUIT = 0X00,
    COCONUT_MALL = 0X05,
    DK_SUMMIT = 0X06,
    WARIOS_GOLD_MINE = 0X07,

    DAISY_CIRCUIT = 0X09,
    KOOPA_CAPE = 0X0F,
    MAPLE_TREEWAY = 0X0B,
    GRUMBLE_VOLCANO = 0X03,

    DRY_DRY_RUINS = 0X0E,
    MOONVIEW_HIGHWAY = 0X0A,
    BOWSERS_CASTLE = 0X0C,
    RAINBOW_ROAD = 0X0D,

    GCN_PEACH_BEACH = 0X10,
    DS_YOSHI_FALLS = 0X14,
    SNES_GHOST_VALLEY_2 = 0X19,
    N64_MARIO_RACEWAY = 0X1A,

    N64_SHERBET_LAND = 0X1B,
    GBA_SHY_GUY_BEACH = 0X1F,
    DS_DELFINO_SQUARE = 0X17,
    GCN_WALUIGI_STADIUM = 0X12,

    DS_DESERT_HILLS = 0X15,
    GBA_BOWSER_CASTLE_3 = 0X1E,
    N64_DKS_JUNGLE_PARKWAY = 0X1D,
    GCN_MARIO_CIRCUIT = 0X11,

    SNES_MARIO_CIRCUIT_3 = 0X18,
    DS_PEACH_GARDENS = 0X16,
    GCN_DK_MOUNTAIN = 0X13,
    N64_BOWSERS_CASTLE = 0X1C,

    BLOCK_PLAZA = 0X21,
    DELFINO_PIER = 0X20,
    FUNKY_STADIUM = 0X23,
    CHAIN_CHOMP_ROULETTE = 0X22,
    THWOMP_DESERT = 0X24,

    SNES_BATTLE_COURSE_4 = 0X27,
    GBA_BATTLE_COURSE_3 = 0X28,
    N64_SKYSCRAPER = 0X29,
    GCN_COOKIE_LAND = 0X25,
    DS_TWILIGHT_HOUSE = 0X26,

    GALAXY_COLOSSEUM = 0X36,
    SUNSET_LUIGI_CIRCUIT = 0X3A,

    WINNING_DEMO = 0X37,
    LOSING_DEMO = 0X38,
    DRAW_DEMO = 0X39
};

typedef enum PlayerType{
    PLAYER_REAL_LOCAL,
    PLAYER_CPU,
    PLAYER_UNKNOWN2,
    PLAYER_GHOST,
    PLAYER_REAL_ONLINE,
    PLAYER_NONE
};

typedef enum Team{
    TEAM_RED,
    TEAM_BLUE
};

typedef enum BattleType{
    BATTLE_BALLOON,
    BATTLE_COIN
};

typedef enum CpuMode{
    CPU_EASY,
    CPU_NORMAL,
    CPU_HARD,
    CPU_NONE
};

typedef enum GameMode{
    MODE_GRAND_PRIX,
    MODE_VS_RACE,
    MODE_TIME_TRIAL,
    MODE_BATTLE,
    MODE_MISSION_TOURNAMENT,
    MODE_GHOST_RACE,
    MODE_6,
    MODE_PRIVATE_VS,
    MODE_PUBLIC_VS,
    MODE_PUBLIC_BATTLE,
    MODE_PRIVATE_BATTLE,
    MODE_AWARD,
    MODE_CREDITS
};

typedef enum GameType{
    TIME_ATTACK = 0x0,
    CPU_RACE = 0x5,
    ONLINE_SPECTATOR = 0x6
};

typedef enum EngineClass{
    CC_50,
    CC_100,
    CC_150,
    CC_BATTLE // Note: Battle mode actually sets it to 50cc (which is ignored by code), but setting it to this in other modes results in Battle CC
};

typedef enum ItemMode{
    ITEMS_BALANCED,
    ITEMS_FRANTIC,
    ITEMS_STRATEGIC,
    ITEMS_NONE
};

typedef struct RacedataSettings{
public:
    CourseId courseId; // http://wiki.tockdom.com/wiki/List_of_Identifiers#Courses
    EngineClass engineClass;
    GameMode gamemode;
    GameType gametype;
    BattleType battleType;
    CpuMode cpuMode;
    ItemMode itemMode;
    u8 hudPlayerIds[4];
    u32 cupId; // http://wiki.tockdom.com/wiki/List_of_Identifiers#Cups
    u8 raceNumber; // resets at 100 for some reason
    u8 lapCount;
    u8 unknown_0x26;
    u8 unknown_0x27;
    u32 modeFlags; // bit flags:
    /*
        2 is teams enabled
        1 is mirror mode
        Others unknown
    */
    u32 unknown_0x2c;
    u32 unknown_0x30;
}; // Total size 0x34

class RacedataPlayer {
public:
    RacedataPlayer(); // 8052d96c
    u32 computeGpRank(); // 8052daf0
    
    // vtable 808b3294
    virtual ~RacedataPlayer(); // 8052DC68
    u8 unknown_0x4;
    s8 localPlayerNum;
    s8 realControllerId; // id of the controller at the console for this player, -1 if not controlled by one
    u8 unknown_0x7; // possibly padding
    VehicleId vehicleId; // http://wiki.tockdom.com/wiki/List_of_Identifiers#Vehicles
    CharacterId characterId; // http://wiki.tockdom.com/wiki/List_of_Identifiers#Characters
    PlayerType playerType; //0x10
    Mii mii; //0x14
    Team team;
    u8 unknown_0xd0[8];
    u16 previousScore;
    u16 score;
    u8 unknown_0xdc[2];
    s16 gpRankScore;
    u8 unknown_0xe0;
    u8 prevFinishPos;
    u8 unknown_0xe2[6];
    s16 rating; // vr or br, depending on mode
    u8 unknown_0xea[6];
}; // Total size 0xf0

class RacedataScenario {
public:
    RacedataScenario(void * ghost); // 8052dbc8, never used - racedata's constructor does it inline

    // vtable 808b3288
    virtual ~RacedataScenario();
    u8 playerCount;
    u8 unknown_0x5;
    u8 localPlayerCount;
    u8 unknown_0x7;
    RacedataPlayer players[12];
    RacedataSettings settings;
    u8 mission[0x70]; // 0x70 struct, see http://wiki.tockdom.com/wiki/Mission_Mode#mission_single.kmt
    void * ghost; // Scenario 0 points to the one you race, 1 points to one I'm not sure about, 2 points to null
}; // Total size 0xbf0

class RacedataMain {
public:
  // Always constructed inline, functions seem to always be members of Racedata rather than specifically RacedataMain

  //vtable 808b3260 (empty)  
  virtual void unknown_vtable();
  RacedataScenario scenarios[3]; // 0 is in race, 1 is in menu, not sure what 2 is
  u8 ghosts[2*0x2800]; // 0x2800 size each, 0 is the one you're racing, not sure what 1 is, see http://wiki.tockdom.com/wiki/RKG_(File_Format)
};  // Total size 0x73d4

// ParameterFile size is 0x1c, Racedata's is /boot/menuset.prm
class Racedata {
public: // vtable override 808b3268
    static Racedata * sInstance; // 0x809bd728
    static Racedata * getStaticInstance(); // 8052fe58
    static void destroyStaticInstance(); // 8052ffe8

    virtual ~Racedata(); // 80530038
    virtual int FUN_80009ddc();
    virtual int FUN_80532078(); // just a blr
    virtual int FUN_80532074(); // just a blr
    virtual int FUN_80532070(); // just a blr
    // Always constructed inline
    
    u8 unknown_0x4[0x1c - 0x4];
  
    RacedataMain main;
};  // Total size 0x73f0

extern Racedata *raceData;