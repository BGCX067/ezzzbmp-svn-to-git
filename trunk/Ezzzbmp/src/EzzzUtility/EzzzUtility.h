/*----------------------------------------EzzzUtility.h----------------------------------------

文件名(FileName):		EzzzUtility.h

编码(Encoding):			UTF-8

编码测试(Encode Test):	釣魚島は中国に属している。
						일본어,한국의,매춘부,수정하지 마십시오 this src

日期(Date):				04/09/2010

作者(Author):			Ezzzcode

描述(Description):		Functions and macros as utilities used by Ezzzcode in programming.

版本(Version):			1.0
						A [Memory Management]
						A [Function error handle macro ]
						A [Function code block custom separator macro ]

------------------------------------------EzzzUtility.h----------------------------------------*/
#ifndef EzzzUtility_h
#define EzzzUtility_h

#include <stdlib.h>

//[Code block separator]
#define Ezu_Public			public:
#define Ezu_Protect			protected:
#define Ezu_Private			private:
#define Ezu_Head
#define Ezu_Body
#define Ezu_End

#define _____(info)			Ezu_##info	//5 _ characters info only can be predefined symbol
#define __________(info)	Ezu_##info	//10 _ characters info only can be predefined symbol
#define ___________(info)				//11 _ characters info can be any symbol
#define ______							//6 _ characters
#define ____________					//12 _ characters

//[Memory operation]
#define Ezu_Free(x) if( (x) ){ free((x)); (x) = NULL; }
#define Ezu_Delete(x) if( (x) ){ delete (x); (x) = NULL; }
#define Ezu_Release(x,function) if( (x) ) { function((x)); (x) = NULL; }

//[Error handle]
#define Ezu_Exit(cond)									if(!(cond)){goto Ezu_ExitGateSymbol(1);}
#define Ezu_ExitEx(cond,sentence)						if(!(cond)){sentence; goto Ezu_ExitGateSymbol(1);}
#define Ezu_Error(cond)									if(!(cond)){goto Ezu_ExitGateSymbol(1);}
#define Ezu_Error_s(cond,nSymbol)						if(!(cond)){goto Ezu_ExitGateSymbol(nSymbol);}
#define Ezu_Error_f(cond,pfnCallback,pParam)			if(!(cond)){if((pfnCallback)){(pfnCallback)(#cond,__FILE__,__LINE__,(void*)pParam);} goto Ezu_ExitGateSymbol(1); }
#define	Ezu_Error_sf(cond,nSymbol,pfnCallback,pParam)	if(!(cond)){if((pfnCallback)){(pfnCallback)(#cond,__FILE__,__LINE__,(void*)pParam);} goto Ezu_ExitGateSymbol(nSymbol); }
#define Ezu_Warn_f(cond,pfnCallback,pParam)				if(!(cond)){if((pfnCallback)){(pfnCallback)(#cond,__FILE__,__LINE__,(void*)pParam);}}
#define Ezu_ExitGateSymbol(nErrrSymbol)					Ezu_ExitGate_##nErrrSymbol
#define Ezu_ExitGate(nErrrSymbol)						Ezu_ExitGateSymbol(nErrrSymbol)##:

#endif