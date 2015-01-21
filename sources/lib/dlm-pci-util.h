#ifndef libdlmpciutilH
#define libdlmpciutilH
//--------------------------------------------------------------

// устройство на шине PCI
struct TdlmUnit
{
	int busnum;  // номер шины
	int devnum;  // номер устройства
	int funcnum; // номер функции
	int vendor;  // vendor ID
	int device;  // device ID
};

typedef std::vector< TdlmUnit > TdlmUnits;  // список устройств

// сканирование шины PCI
//    заполняет список устройств pci
//    возвращает кол-во найденных устройств
int  dlmPCIScan( TdlmUnits& pci );

// поиск устройств по devicemaskstr в списке pci
//    заполняет список units
//    возвращает кол-во найденных устройств
int  dlmPCIGetUnitsByVendorDeviceMask( const TdlmUnits& pci, const char* devicemaskstr, TdlmUnits& units );

//--------------------------------------------------------------
#endif

