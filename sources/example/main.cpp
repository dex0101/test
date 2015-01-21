#include <stdio.h>
#include <string.h>
#include <vector>
#include <iterator>
//using namespace std;

#include "dlm-pci-util.h"

#include "../dxversion.h"

//--------------------------------------------------------------

void print( TdlmUnits& pci )
{
	printf( "--- все найденные устройства ---\n" );
	for( std::vector<TdlmUnit>::const_iterator it = pci.begin(); it != pci.end(); ++it )
		printf( "%02d:%02d.%d - %04X:%04X\n", it->busnum,it->devnum,it->funcnum,it->vendor,it->device );
	printf( "\n" );
}
//--------------------------------------------------------------

void find_device_by_vendev( TdlmUnits& pci, char* vendevstr )
{
	TdlmUnits units;
	int count = dlmPCIGetUnitsByVendorDeviceMask( pci, vendevstr, units );

	if( count == -1 ) printf( "%s - неверная запись\n", vendevstr );
	else
	{
		printf( "поиск %s - найдено %d:\n", vendevstr, count );
	
		for( std::vector<TdlmUnit>::const_iterator it = units.begin(); it != units.end(); ++it )
			printf( "%02d:%02d.%d - %04X:%04X\n", it->busnum,it->devnum,it->funcnum, it->vendor,it->device );
	}
	printf( "\n" );
}
//--------------------------------------------------------------

void find_devices_from_file( TdlmUnits& pci, char* filename )
{
	printf( "--- поиск устройств из файла %s --- \n\n", filename );
	
	FILE* fd;
	char buf[ 16 ];

	if( !( fd = fopen( filename, "r" ) ) ) { printf( "файл не найден\n" ); return; }

	while( !feof( fd ) )
	{
		if( !fgets( buf, sizeof(buf), fd ) ) break;
		
		while( char* z = strpbrk( buf, "\n\r") ) *z = 0;
		
		find_device_by_vendev( pci, buf );
	}
	fclose( fd );

	printf( "\n" );  
}
//--------------------------------------------------------------


int main( int argc, char **argv ) 
{
	//strcpy( EXE,  strrchr(argv[0], '/') + 1 );
	//strncpy( PATH, argv[0], strrchr(argv[0], '/') - argv[0] );
	//printf( "EXE '%s'  PATH '%s'\n", EXE, PATH );
	
	struct tm *ts;
	ts = localtime( &VERSION.time );
	printf( "Version %s build #%d  ( %02d.%02d.%02d %02d:%02d )\n", VERSION.name, VERSION.build, ts->tm_mday, ts->tm_mon + 1, ts->tm_year - 100, ts->tm_hour, ts->tm_min ); 



	TdlmUnits PCI;        // список устройств на шине PCI
	dlmPCIScan( PCI );    // сканирование шины PCI


	if( ( argc > 2 )&&( strcmp( argv[1], "-d") == 0 ) )
	{
		// поиск устройства по vendor:device
		find_device_by_vendev( PCI, argv[2] );
	}
	else
	if( ( argc > 2 )&&( strcmp( argv[1], "-f") == 0 ) )
	{
		// поиск устройств по vendor:device взятых из файла
		find_devices_from_file( PCI, argv[2] );
	}
	else
		// вывод на экран списка всех найденных устройств
		print( PCI );

	PCI.clear( );
	return 0;
}
//--------------------------------------------------------------

