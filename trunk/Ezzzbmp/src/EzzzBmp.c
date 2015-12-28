/*----------------------------------------EzzzBmp.c----------------------------------------

文件名(FileName):		EzzzBmp.c

编码(Encoding):			UTF-8

编码测试(Encode Test):	釣魚島は中国に属している。
						일본어,한국의,매춘부,수정하지 마십시오 this src

日期(Date):				08/09/2010

作者(Author):			Ezzzcode

描述(Description):		Bitmap manipulate function implements

版本(Version):			1.0
						A [Bitmap operation about function implements]

------------------------------------------EzzzBmp.c----------------------------------------*/
#include "EzzzBmp.h"
#include "EzzzUtility/EzzzUtility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#ifndef EZB_ERRR_HANDLE
#define Ezb_Error(cond) Ezu_Error(cond);
#define Ezb_ErrorEx(cond,sentence) {sentence;Ezu_Error(cond)}
#define Ezb_Warn(cond)
#else
#define Ezb_Error(cond) Ezu_Error_f(cond, g_pfnEzbErrrHandle, g_pParam );
#define Ezb_ErrorEx(cond,sentence) {sentence;Ezu_Error_f(cond, g_pfnEzbErrrHandle, g_pParam);}
#define Ezb_Warn(cond) Ezu_Warn_f(cond,g_pfnEzbErrrHandle, g_pParam)
#endif

static int _Ezb_ReadFileHeader( Ezb_Bmp* pB, FILE* pFile );
static int _Ezb_ReadInfoHeader( Ezb_Bmp* pB, FILE* pFile );
static int _Ezb_ReadPalette( Ezb_Bmp* pB, FILE* pFile );
static int _Ezb_ReadBitData( Ezb_Bmp* pB, FILE* pFile );
static int _Ezb_IsSupportFormat( Ezb_FileHeader* pFileHeader );
static int _Ezb_IsNeedPalette( Ezb_InfoHeaderV3* pInfoV3 );
static int _Ezb_ComputeImageOffset( Ezb_Bmp* pB, unsigned int* pnImageOffset );
static int _Ezb_GetImageOffset( Ezb_FileHeader* psFileHeader, unsigned int* pnImageOffset );
static int _Ezb_ComputeImageSize( Ezb_InfoHeaderV3* psInfoV3, unsigned int* pnImageSize );
static int _Ezb_GetImageSize( Ezb_InfoHeaderV3* psInfoV3, unsigned int* pnImageSize );

Ezb_ErrrHandle g_pfnEzbErrrHandle = 0;
void* g_pParam = 0;

int Ezb_SetGlobalErrrHandle( Ezb_ErrrHandle pfnErrrHandle )
{
	g_pfnEzbErrrHandle = pfnErrrHandle;
	return 1;
}

int Ezb_SetGlobalErrrHandleParam( void* pParam )
{
	g_pParam = pParam;
	return 1;
}

Ezb_Bmp* Ezb_New()
{
	___________(Head)____________
	Ezb_Bmp*	psBmpNew	=	0;

	___________(Body)____________
	Ezb_Error( psBmpNew = (Ezb_Bmp*)calloc( 1, sizeof( Ezb_Bmp ) ) );
	psBmpNew->unLastErrr = EZB_ERR_OK;
	psBmpNew->unStatus = EZB_STATUS_INIT;

	___________(End)____________
	Ezu_ExitGate(1)
	return psBmpNew;
}

int Ezb_Delete( Ezb_Bmp* pB )
{
	___________(Head)____________
	int bResult = 0;

	___________(Body)____________
	Ezu_ExitEx( pB, bResult = 1 );
	Ezb_Error( Ezb_Close( pB ) );
	Ezu_Free( pB );
	bResult = 1;

	___________(End)____________
	Ezu_ExitGate(1)
	return bResult;
}

int Ezb_Open( Ezb_Bmp* pB, const char* pszFile )
{
	___________(Head)____________
	int					bResult				=	0;
	int					nLastErrr			=	EZB_ERR_OK;
	FILE*				pFile				=	NULL;
	Ezb_FileHeader*		psFileHeader		=	NULL;
	Ezb_InfoHeaderV3*	psInfoV3			=	NULL;
	unsigned int		unImageSize			=	0;
	unsigned int		unRealImageSize		=	0;
	unsigned int		unImageOffset		=	0;
	unsigned int		unRealImageOffset	=	0;

	___________(Body)____________
	Ezb_Error( pB );
	Ezb_ErrorEx( pszFile, nLastErrr = EZB_ERR_PARAM );
	Ezb_Error( Ezb_Close( pB ) );
	Ezb_ErrorEx( pFile = fopen( pszFile, "rb" ), nLastErrr = EZB_ERR_OPENFILE );

	//Read file info header
	Ezb_ErrorEx( _Ezb_ReadFileHeader( pB, pFile ), nLastErrr = Ezb_GetLastErrr( pB ) );
	//Read bitmap info header
	Ezb_ErrorEx( _Ezb_ReadInfoHeader( pB, pFile ), nLastErrr = Ezb_GetLastErrr( pB ) );
	//Read Palette
	Ezb_ErrorEx( _Ezb_ReadPalette( pB, pFile ), nLastErrr = Ezb_GetLastErrr( pB ) );

	//Check format
	Ezb_ErrorEx( psFileHeader = pB->psBmpFileHeader, nLastErrr = EZB_ERR_FORMAT );
	Ezb_ErrorEx( psInfoV3 = (Ezb_InfoHeaderV3*)pB->psBmpInfoHeader, nLastErrr = EZB_ERR_FORMAT );

	Ezb_ErrorEx( _Ezb_GetImageSize( psInfoV3, &unImageSize ), nLastErrr = EZB_ERR_FORMAT );
	Ezb_ErrorEx( _Ezb_ComputeImageSize( psInfoV3, &unRealImageSize ), nLastErrr = EZB_ERR_FORMAT );
	Ezb_ErrorEx( _Ezb_GetImageOffset( psFileHeader, &unImageOffset ), nLastErrr = EZB_ERR_FORMAT );
	Ezb_ErrorEx( _Ezb_ComputeImageOffset( pB, &unRealImageOffset ), nLastErrr = EZB_ERR_FORMAT );

	/*
	Ezb_ErrorEx( unRealImageSize == unImageSize, nLastErrr = EZB_ERR_FORMAT );		//will be allowed in the further,only give a warnning
	Ezb_ErrorEx( unRealImageOffset == unImageOffset, nLastErrr = EZB_ERR_FORMAT );	//will be allowed in the further,only give a warninng
	*/
	Ezb_Warn( unRealImageSize == unImageSize );
	Ezb_Warn( unRealImageOffset == unImageOffset );
	Ezb_ErrorEx( unImageOffset >= unRealImageOffset, nLastErrr = EZB_ERR_FORMAT );
	Ezb_ErrorEx( 0 == fseek( pFile, unImageOffset - unRealImageOffset, SEEK_CUR ), nLastErrr = EZB_ERR_FORMAT );

	//Read bitdata
	Ezb_ErrorEx( _Ezb_ReadBitData( pB, pFile ), nLastErrr = Ezb_GetLastErrr( pB ) );

	bResult = 1;
	nLastErrr = EZB_ERR_OK;

	___________(End)____________
	Ezu_ExitGate(1)
	if( 0 == bResult )
	{
		Ezb_Close( pB );
	}

	Ezu_Release( pFile, fclose );
	Ezb_SetLastErrr( pB, nLastErrr );
	return bResult;
}

int Ezb_Close( Ezb_Bmp* pB )
{

	Ezu_Exit( pB );
	Ezu_Free( pB->psBmpFileHeader );
	Ezu_Free( pB->psBmpInfoHeader );
	Ezu_Free( pB->psBmpPalette );
	Ezu_Free( pB->pBitData );
	pB->nBmpVersion = 0;
	pB->unBmpPaletteEntryCount = 0;
	pB->unLastErrr = EZB_ERR_OK;
	pB->unStatus = EZB_STATUS_INIT;

	Ezu_ExitGate(1)
	return 1;

}

int Ezb_SetReadCallback( Ezb_Bmp* pB, Ezb_IOCallback pfnRead, size_t nReadBlock )
{
	___________(Head)____________
	int		bResult		=	0;
	int		nLastErrr	=	EZB_ERR_OK;

	___________(Body)____________
	Ezb_Error( pB );
	pB->pfnRead = pfnRead;
	pB->nReadBlock = nReadBlock;
	bResult = 1;

	___________(End)____________
	Ezu_ExitGate(1)
	Ezb_SetLastErrr( pB, nLastErrr );
	return bResult;
}

int Ezb_SetWriteCallback( Ezb_Bmp* pB, Ezb_IOCallback pfnWrite, size_t nWriteBlock )
{
	___________(Head)____________
	int		bResult		=	0;
	int		nLastErrr	=	EZB_ERR_OK;

	___________(Body)____________
	Ezb_Error( pB );
	pB->pfnWrite = pfnWrite;
	pB->nWriteBlock = nWriteBlock;
	bResult = 1;

	___________(End)____________
	Ezu_ExitGate(1)
	Ezb_SetLastErrr( pB, nLastErrr );
	return bResult;
}

static int _Ezb_ReadFileHeader( Ezb_Bmp* pB, FILE* pFile )
{
	int	bResult		=	0;
	int	nLastErrr	=	EZB_ERR_OK;

	assert( pB && pFile );
	Ezu_Free( pB->psBmpFileHeader );
	Ezb_ErrorEx( pB->psBmpFileHeader = (Ezb_FileHeader*)malloc( sizeof(Ezb_Bmp) ), nLastErrr = EZB_ERR_MALLOC );
	Ezb_ErrorEx( sizeof(Ezb_FileHeader) == fread( pB->psBmpFileHeader, 1, sizeof(Ezb_FileHeader), pFile ), nLastErrr = EZB_ERR_READ_FILEHEADER );
	Ezb_ErrorEx( _Ezb_IsSupportFormat( pB->psBmpFileHeader ), nLastErrr = EZB_ERR_FORMAT );
	bResult = 1;
	nLastErrr = EZB_ERR_OK;

	Ezu_ExitGate(1)
	if( 0 == bResult )
	{
		//Ezb_Close( pB );
		Ezu_Free( pB->psBmpFileHeader );
	}
	Ezb_SetLastErrr( pB, nLastErrr );
	return bResult;
}
static int _Ezb_ReadInfoHeader( Ezb_Bmp* pB, FILE* pFile )
{
	int					bResult		=	0;
	int					nLastErrr	=	EZB_ERR_OK;
	Ezb_InfoHeaderV3*	psInfoV3	=	NULL;

	assert( pB && pFile );
	Ezu_Free( pB->psBmpInfoHeader );
	Ezb_ErrorEx( psInfoV3 = (Ezb_InfoHeaderV3*)malloc( sizeof(Ezb_InfoHeaderV3) ), nLastErrr = EZB_ERR_MALLOC );
	Ezb_ErrorEx( sizeof(Ezb_InfoHeaderV3) == fread( psInfoV3, 1, sizeof(Ezb_InfoHeaderV3), pFile ), nLastErrr = EZB_ERR_READ_INFOHEADER );
	
	Ezb_ErrorEx( sizeof(Ezb_InfoHeaderV3) >= psInfoV3->nSize, nLastErrr = EZB_ERR_FORMAT );
	if ( psInfoV3->nSize > sizeof(Ezb_InfoHeaderV3) )
	{
		Ezb_ErrorEx( sizeof(Ezb_InfoHeaderV4) == psInfoV3->nSize || sizeof(Ezb_InfoHeaderV5) == psInfoV3->nSize, nLastErrr = EZB_ERR_FORMAT );
		if( sizeof( Ezb_InfoHeaderV4 ) == psInfoV3->nSize )
		{
			pB->nBmpVersion = Ezb_bv_v4;	
		}
		else
		{
			pB->nBmpVersion = Ezb_bv_v5;
		}
		Ezb_ErrorEx( pB->psBmpInfoHeader = (void*)malloc( psInfoV3->nSize ), nLastErrr = EZB_ERR_MALLOC );
		memcpy( pB->psBmpInfoHeader, psInfoV3, sizeof(Ezb_InfoHeaderV3) );
		Ezb_ErrorEx( ( psInfoV3->nSize - sizeof(Ezb_InfoHeaderV3) ) == fread( ((char*)pB->psBmpInfoHeader) + sizeof(Ezb_InfoHeaderV3), 1, psInfoV3->nSize - sizeof(Ezb_InfoHeaderV3), pFile ), nLastErrr = EZB_ERR_READ_INFOHEADER );
	}
	else
	{
		pB->nBmpVersion = Ezb_bv_v3;
		pB->psBmpInfoHeader = (void*)psInfoV3;
		psInfoV3 = NULL;
	}
	bResult = 1;
	nLastErrr = EZB_ERR_OK;

	Ezu_ExitGate(1)
	if ( 0 == bResult )
	{
		Ezu_Free( pB->psBmpInfoHeader );
		pB->nBmpVersion = 0;
	}
	Ezb_SetLastErrr( pB, nLastErrr );
	return bResult;
}
static int _Ezb_ReadPalette( Ezb_Bmp* pB, FILE* pFile )
{
	int					bResult			=	0;
	int					nLastErrr		=	EZB_ERR_OK;
	uint32				nPaletteCount	=	0;
	Ezb_InfoHeaderV3*	psInfoV3		=	NULL;

	assert( pB && pFile );
	Ezu_Free( pB->psBmpPalette );
	Ezb_ErrorEx( pB->psBmpInfoHeader, nLastErrr = EZB_ERR_FORMAT );
	psInfoV3 = (Ezb_InfoHeaderV3*)pB->psBmpInfoHeader;

	Ezu_ExitEx( _Ezb_IsNeedPalette( psInfoV3 ), (bResult = 1,nLastErrr = EZB_ERR_OK) );
	if ( Ezb_bi_bitfields == psInfoV3->unCompression )
	{
		//three color mask
		nPaletteCount = 3;
	}
	else
	{
		nPaletteCount = (uint32)pow( (double)2.0, (double)psInfoV3->unBitCount );
		if ( psInfoV3->nColorUsedCount > 0 )
			nPaletteCount = psInfoV3->nColorUsedCount;
		if ( psInfoV3->nColorImportantCount > 0 )
			nPaletteCount = psInfoV3->nColorImportantCount;
	}

	if ( nPaletteCount > 0 )
	{
		Ezb_ErrorEx( pB->psBmpPalette = (Ezb_RGBQuad*)malloc( sizeof(Ezb_RGBQuad) * nPaletteCount ), nLastErrr = EZB_ERR_MALLOC );
		Ezb_ErrorEx( sizeof( Ezb_RGBQuad ) * nPaletteCount == fread( pB->psBmpPalette, 1, sizeof( Ezb_RGBQuad ) * nPaletteCount, pFile ), nLastErrr = EZB_ERR_READ_PALETTE );
	}
	pB->unBmpPaletteEntryCount = nPaletteCount;
	bResult = 1;
	nLastErrr = EZB_ERR_OK;

	Ezu_ExitGate(1)
	if ( 0 == bResult )
	{
		Ezu_Free( pB->psBmpPalette );
		pB->unBmpPaletteEntryCount = 0;
	}
	Ezb_SetLastErrr( pB, nLastErrr );
	return bResult;
}
static int _Ezb_ReadBitData( Ezb_Bmp* pB, FILE* pFile )
{
	int					bResult		=	0;
	int					nLastErrr	=	EZB_ERR_OK;
	Ezb_InfoHeaderV3*	psInfoV3	=	NULL;
	uint32				unImageSize	=	0;

	assert( pB && pFile );
	Ezu_Free( pB->pBitData );
	Ezb_ErrorEx( psInfoV3 = (Ezb_InfoHeaderV3*)pB->psBmpInfoHeader, nLastErrr = EZB_ERR_FORMAT );

	//unImageSize = psInfoV3->unImageSize;
	Ezb_ErrorEx( _Ezb_ComputeImageSize( psInfoV3, &unImageSize ), nLastErrr = EZB_ERR_FORMAT );
	Ezb_ErrorEx( pB->pBitData = (uint8*)malloc( unImageSize ), nLastErrr = EZB_ERR_MALLOC );
	Ezb_ErrorEx( unImageSize == (uint32)fread( pB->pBitData, sizeof(uint8), unImageSize, pFile ), nLastErrr = EZB_ERR_READ_BITDATA );
	bResult = 1;
	nLastErrr = EZB_ERR_OK;
	
	Ezu_ExitGate(1)
	if( 0 == bResult )
	{
		Ezu_Free( pB->pBitData );
	}
	Ezb_SetLastErrr( pB, nLastErrr );
	return bResult;
}
static int _Ezb_IsSupportFormat( Ezb_FileHeader* pFileHeader )
{
	int	bResult	=	0;

	assert( pFileHeader );
	if( pFileHeader->szIdentify[0] == 'B' && pFileHeader->szIdentify[1] == 'M' )
		bResult = 1;

	return bResult;
}
static int _Ezb_IsNeedPalette( Ezb_InfoHeaderV3* pInfoV3 )
{
	int	bResult	=	0;

	assert( pInfoV3 );
	if( pInfoV3->unBitCount == 1|| pInfoV3->unBitCount == 4 || pInfoV3->unBitCount == 8 )
		bResult = 1;
	else if ( Ezb_bi_bitfields == pInfoV3->unCompression )
		bResult = 1;
	return bResult;
}

static int _Ezb_ComputeImageOffset( Ezb_Bmp* pB, unsigned int* pnImageOffset )
{
	int					bResult			=	0;
	unsigned int		unImageOffset	=	0;
	Ezb_InfoHeaderV3*	pInfoV3			=	NULL;

	assert( pB && pnImageOffset );
	Ezb_Error( pInfoV3 = (Ezb_InfoHeaderV3*)pB->psBmpInfoHeader );
	
	unImageOffset = sizeof(Ezb_FileHeader) + pInfoV3->nSize + pB->unBmpPaletteEntryCount * sizeof(Ezb_RGBQuad);
	*pnImageOffset = unImageOffset;
	bResult = 1;

	Ezu_ExitGate(1)
	return bResult;
}

static int _Ezb_ComputeImageSize( Ezb_InfoHeaderV3* psInfoV3, unsigned int* pnImageSize )
{
	int					bResult				=	0;
	unsigned int		nBytePerLine		=	0;
	unsigned int		nAlignBytePerLine	=	0;
	unsigned int		nByteTotal			=	0;

	assert( psInfoV3 && pnImageSize );
	
	nBytePerLine = ( psInfoV3->unBitCount * psInfoV3->nWidth );
	nAlignBytePerLine = ( nBytePerLine % 32 ? (32-nBytePerLine % 32) : 0 );
	nBytePerLine += nAlignBytePerLine;
	nBytePerLine = nBytePerLine / 8;
	nByteTotal = nBytePerLine * psInfoV3->nHeight;
	*pnImageSize = nByteTotal;
	bResult = 1;

	return bResult;
}

static int _Ezb_GetImageOffset( Ezb_FileHeader* psFileHeader, unsigned int* pnImageOffset )
{
	assert( psFileHeader && pnImageOffset );
	*pnImageOffset = psFileHeader->unBitDataOffset;
	return 1;
}

static int _Ezb_GetImageSize( Ezb_InfoHeaderV3* psInfoV3, unsigned int* pnImageSize )
{
	assert( psInfoV3 && pnImageSize );
	*pnImageSize = psInfoV3->unImageSize;
	return 1;
}