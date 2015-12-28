#include "EzzzUtility\EzzzUtility.h"
#include "EzzzBmp.h"
#include <stdio.h>
#include <windows.h>

int ErrorHandle( Ezb_Bmp* pB )
{
	if( pB )
	{
		printf( "Error Occurs\n" );
		printf( "LastErrr:%d\n", pB->unLastErrr );
	}
	else
	{
		printf( "Ezb_Bmp is NULL\n" );
	}
	return 0;
}

int Ezb_GlobalErrrHandle( const char* pszErrrSrc, const char* pszErrrFile, int nLine, void* pParam )
{
	printf( "%s\n", pszErrrSrc );
	printf( "%s\n", pszErrrFile );
	printf( "%d\n", nLine );
	printf( "\n" );
	return 0;
}
int main( int argn, char** argv )
{
	/*
	printf( "Bitmap File Header:%d\n", sizeof( EzzzBmpFileHeader ) );
	printf( "Bitmap Info Header V3:%d\n", sizeof( EzzzBmpInfoHeaderV3 ) );
	printf( "Bitmap Info Header V4:%d\n", sizeof( EzzzBmpInfoHeaderV4 ) ); 
	printf( "Bitmap Info Header V5:%d\n", sizeof( EzzzBmpInfoHeaderV5 ) ); 
	printf( "Bitmap RGBQuad:%d\n", sizeof( EzzzBmpRGBQuad ) );
	printf( "Bitmap RGBTriple:%d\n", sizeof( EzzzBmpRGBTriple ) );

	printf( "Windows Bitmap Info Header V4:%d\n", sizeof( BITMAPV4HEADER ) );
	printf( "Windows Bitmap Info Header V5:%d\n", sizeof( BITMAPV5HEADER ) );
	printf( "Windows Bitmap RGBQUAD:%d\n", sizeof(RGBQUAD) );
	printf( "Windows Bitmap RGBTRIPLE:%d\n", sizeof(RGBTRIPLE) );
	*/

	int			bResult	=	-1;
	Ezb_Bmp*	psBmp	=	NULL;
	
	Ezu_Error( Ezb_SetGlobalErrrHandle( Ezb_GlobalErrrHandle ) );
	Ezu_Error( psBmp = Ezb_New() );
	Ezu_Error( Ezb_Open( psBmp, "test_100x100x24.bmp" ) );
	Ezu_Error( Ezb_Open( psBmp, "test_100x100x8.bmp" ) );
	Ezu_Error( Ezb_Open( psBmp, "test_100x100x4.bmp" ) );
	Ezu_Error( Ezb_Open( psBmp, "test_100x100x1.bmp" ) );
	bResult = 0;

	Ezu_ExitGate(1)
	if( -1 == bResult )
	{
		ErrorHandle( psBmp );
	}
	Ezb_Delete( psBmp );
	psBmp = NULL;
	return bResult;
}