#include <Kamek/kamek.hpp>
#include <System/dvd.hpp>
#include <System/system.hpp>
#include <egg/egg.hpp>

struct loaderFunctions;

typedef void (*OSReport_t) (const char *str, ...);
typedef void (*OSFatal_t) (u32 *fg, u32 *bg, const char *str, ...);
typedef int (*DVDConvertPathToEntrynum_t) (const char *path);
typedef bool (*DVDFastOpen_t) (int entrynum, DVDCommandBlock *fileInfo);
typedef int (*DVDReadPrio_t) (DVDCommandBlock *fileInfo, void *buffer, int length, int offset, int unk);
typedef bool (*DVDClose_t) (DVDCommandBlock *fileInfo);
typedef int (*sprintf_t) (char *str, const char *format, ...);


struct loaderFunctions {
	OSReport_t OSReport;
	OSFatal_t OSFatal;
	DVDConvertPathToEntrynum_t DVDConvertPathToEntrynum;
	DVDFastOpen_t DVDFastOpen;
	DVDReadPrio_t DVDReadPrio;
	DVDClose_t DVDClose;
	sprintf_t sprintf;
    RKSystem *rkSystem;
};

void loadKamekBinaryFromDisc(loaderFunctions *funcs, const char *path);

