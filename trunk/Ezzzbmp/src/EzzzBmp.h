/*----------------------------------------Ezb_Bmp.h----------------------------------------

文件名(FileName):		Ezb_Bmp.h

编码(Encoding):			UTF-8

编码测试(Encode Test):	釣魚島は中国に属している。
						일본어,한국의,매춘부,수정하지 마십시오 this src

日期(Date):				06/09/2010

作者(Author):			Ezzzcode

描述(Description):		Bitmap manipulate API

版本(Version):			1.0
						A [Bitmap file header]
						A [Bitmap info header]
						A [Bitmap operation about function]

------------------------------------------Ezb_Bmp.h----------------------------------------*/
#ifndef Ezb_Bmp_h
#define Ezb_Bmp_h

/* configuration macro */
#define EZB_ERRR_HANDLE
#ifdef __cplusplus
	#define EZB_C extern "C"
#else
	#define EZB_C
#endif

/* 32 bit platform datatype definations */
typedef unsigned char					uint8;
typedef char							int8;
typedef	unsigned short					uint16;
typedef short							int16;
typedef unsigned int					uint32;
typedef int								int32;

/* Bitmap stardard struct */
typedef struct Ezb_FileHeader			Ezb_FileHeader;
typedef struct Ezb_InfoHeaderV3			Ezb_InfoHeaderV3;
typedef struct Ezb_InfoHeaderV4			Ezb_InfoHeaderV4;
typedef struct Ezb_InfoHeaderV5			Ezb_InfoHeaderV5;
typedef struct Ezb_RGBQuad				Ezb_RGBQuad;
typedef struct Ezb_RGBTriple			Ezb_RGBTriple;

/* Constants for the stardard Ezb_InfoHeaderV3 unCompression field */
#define Ezb_bi_rgb						0L
#define Ezb_bi_rle8						1L
#define Ezb_bi_rle4						2L
#define Ezb_bi_bitfields				3L
#define Ezb_bi_jpeg						4L
#define Ezb_bi_png						5L
#define Ezb_bi_ezzz						10L

/* ErrorHandle */
typedef int (*Ezb_ErrrHandle)( const char* pszErrrSrc, const char* pszErrrFile, int nLine, void* pParam );
#define EZB_ERR_OK						0
#define EZB_ERR_MEMNOTENOUGH			1
#define EZB_ERR_PARAM					2
#define EZB_ERR_OPENFILE				3
#define EZB_ERR_MALLOC					4
#define EZB_ERR_FORMAT					5
#define EZB_ERR_READ_FILEHEADER			6
#define EZB_ERR_READ_INFOHEADER			7
#define EZB_ERR_READ_PALETTE			8
#define EZB_ERR_READ_BITDATA			9
#define EZB_ERR_UNKNOW					100

typedef struct Ezb_Bmp					Ezb_Bmp;
typedef int (*Ezb_IOCallback)( Ezb_Bmp* pB, size_t nCurIO, size_t nNeedIO );
#define Ezb_bv_v3						1
#define Ezb_bv_v4						2
#define Ezb_bv_v5						3
#define EZB_STATUS_INIT					0

#pragma pack(1)
struct Ezb_FileHeader
{
	char								szIdentify[2];
	uint32								unFileSize;
	uint16								unReserved1;
	uint16								unReserved2;
	uint32								unBitDataOffset;
};

struct Ezb_InfoHeaderV3
{
	uint32								nSize;
	int32								nWidth;
	int32								nHeight;
	uint16								unPlanes;
	uint16								unBitCount;
	uint32								unCompression;
	uint32								unImageSize;
	int32								nXPelsPerMeter;
	int32								nYPelsPerMeter;
	uint32								nColorUsedCount;
	uint32								nColorImportantCount;
};

struct Ezb_InfoHeaderV4
{
	Ezb_InfoHeaderV3					sBmpInfoHeaderV3;
	uint32								unV4RedMask;
	uint32								unV4GreenMask;
	uint32								unV4BlueMask;
	uint32								unV4AlphaMask;
	uint32								unV4CSType;
	int32								nV4RedX;
	int32								nV4RedY;
	int32								nV4RedZ;
	int32								nV4GreenX;
	int32								nV4GreenY;
	int32								nV4GreenZ;
	int32								nV4BlueX;
	int32								nV4BlueY;
	int32								nV4BlueZ;
	uint32								unV4GammaRed;
	uint32								unV4GammaGreen;
	uint32								unV4GammaBlue;
};

struct Ezb_InfoHeaderV5
{
	Ezb_InfoHeaderV4					sBmpInfoHeaderV4;
	uint32								unV5Intent;
	uint32								unV5ProfileData;
	uint32								unV5ProfileSize;
	uint32								unV5Reserved;
};

struct Ezb_RGBQuad
{
	uint8								unB;
	uint8								unG;
	uint8								unR;
	uint8								unReserved;
};

struct Ezb_RGBTriple
{
	uint8								unB;
	uint8								unG;
	uint8								unR;
};
#pragma pack()

struct Ezb_Bmp
{
	Ezb_FileHeader*						psBmpFileHeader;
	void*								psBmpInfoHeader;
	Ezb_RGBQuad*						psBmpPalette;
	uint32								unBmpPaletteEntryCount;
	uint8*								pBitData;

	Ezb_IOCallback						pfnWrite;
	Ezb_IOCallback						pfnRead;
	size_t								nWriteBlock;
	size_t								nReadBlock;

	int									nBmpVersion;
	int									unStatus;
	int									unLastErrr;
};

EZB_C int Ezb_SetGlobalErrrHandle( Ezb_ErrrHandle pfnErrrHandle );
EZB_C int Ezb_SetGlobalErrrHandleParam( void* pParam );

EZB_C Ezb_Bmp* Ezb_New();
EZB_C int Ezb_Delete( Ezb_Bmp* pB );
EZB_C int Ezb_Open( Ezb_Bmp* pB, const char* pszFile );
EZB_C int Ezb_Close( Ezb_Bmp* pB );

EZB_C int Ezb_SetReadCallback( Ezb_Bmp* pB, Ezb_IOCallback pfnRead, size_t nReadBlock );
EZB_C int Ezb_SetWriteCallback( Ezb_Bmp* pB, Ezb_IOCallback pfnWrite, size_t nWriteBlock );

#define Ezb_SetLastErrr(pB,nLastErrr) if(pB){ pB->unLastErrr = nLastErrr; }
#define Ezb_GetLastErrr(pB) pB ? pB->unLastErrr : EZB_ERR_UNKNOW;
#endif