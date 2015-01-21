#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <vector>
#include <iterator>

#include "dlm-pci-util.h"

//--------------------------------------------------------------

typedef uint8_t  BYTE;
typedef uint16_t WORD;

//--------------------------------------------------------------

int read_hex_from_file( char* sysdir, char* busname, char* filename )
{
	FILE* fd;
	char fullname[ 1024 ];
	char buf[ 16 ];

	sprintf( fullname, "%s/%s/%s", sysdir, busname, filename );
	
	if( !( fd = fopen( fullname, "r" ) ) ) return -1;
	if( !fgets( buf, sizeof(buf), fd ) ) return -2;
	fclose( fd );
	
	return (int)strtol( buf, NULL, 16 );
}
//--------------------------------------------------------------

bool hexstr2valuemask( const char* str, WORD& value, WORD& mask )
{
	value = 0; mask = 0;
	
	for( int i = 0; i < 4; i++ )
	{
		BYTE v=0, m=0;
		char c = str[i];
		
		if( c == '*' ) { v = 0; m = 0; } else
		if( ( c >= '0' )&&( c <= '9') ) { v = c - '0'; m = 0xF; } else
		if( ( c >= 'A' )&&( c <= 'F') ) { v = c - 'A' + 10; m = 0xF; } else
		if( ( c >= 'a' )&&( c <= 'f') ) { v = c - 'a' + 10; m = 0xF; } 
		else return false;
			
		value |= ( v&0x0F )<<(((3-i))*4);
		mask  |= ( m&0x0F )<<(((3-i))*4);
	}
	return true;
}
//--------------------------------------------------------------

bool get_vendordevice_from_str( const char* devicestr, WORD& ven, WORD& venmask, WORD& dev, WORD& devmask )
{
	if( strlen( devicestr ) != 9 ) return false;
	if( devicestr[4] != ':' ) return false;
	if( !hexstr2valuemask( devicestr+0, ven, venmask ) ) return false;
	if( !hexstr2valuemask( devicestr+5, dev, devmask ) ) return false;
	return true;
}
//--------------------------------------------------------------



int dlmPCIScan( TdlmUnits& pci )
{
	pci.clear( );
	pci.reserve( 50 );
	
	DIR* dir;
	struct dirent* entry;
	char* sysdir = (char*)"/sys/bus/pci/devices";

	if( !( dir = opendir( sysdir ) ) ) return -1;

	while( ( entry = readdir( dir ) ) != NULL )
	{
		if( strcmp( entry->d_name, "." ) == 0 || strcmp( entry->d_name, ".." ) == 0 ) continue;

		int z, bus, dev, func;
		sscanf( entry->d_name, "%d:%d:%d.%d", &z, &bus, &dev, &func );
		
		int vendor = read_hex_from_file( sysdir, entry->d_name, (char*)"vendor" );
		int device = read_hex_from_file( sysdir, entry->d_name, (char*)"device" );

		TdlmUnit unit = { bus,dev,func, vendor,device };
		pci.push_back( unit );
	}

	closedir(dir);
	
	return pci.size();
}
//--------------------------------------------------------------

int  dlmPCIGetUnitsByVendorDeviceMask( const TdlmUnits& pci, const char* devicemaskstr, TdlmUnits& units )
{
	WORD vendor,vendormask, device,devicemask;
	
	if( !get_vendordevice_from_str( devicemaskstr, vendor,vendormask, device,devicemask ) )
		return -1;

	units.clear( );

	for( std::vector<TdlmUnit>::const_iterator it = pci.begin(); it != pci.end(); ++it )
		if( ( (it->vendor & vendormask) == vendor )&&( (it->device & devicemask) == device ) )
			units.push_back( *it );
			
	return units.size();
}
//--------------------------------------------------------------


