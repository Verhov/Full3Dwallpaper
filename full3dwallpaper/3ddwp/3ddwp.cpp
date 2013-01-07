#define INITGUID
// WIN LIB
#include <Windows.h>
#include <Commctrl.h>
#include <strsafe.h>
#include <stdio.h>
#include <Shlwapi.h>
#pragma comment (lib, "Shlwapi.lib")
//STD
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <Shlobj.h>

#include <wtsapi32.h>
#pragma comment (lib, "Wtsapi32.lib")

#define WM_SCRSAVMSG (WM_USER + 33)
#define WM_MUTEXMSG (WM_USER + 34)
#define WM_NOTIFYICON_MSG (WM_USER + 1)
#define	WM_VISITSITE	1000
#define	WM_MAKESCR		1001
#define	WM_OPTWND		1002
#define	WM_WPMENU		1003
#define	WM_SHUTDWN		1004
#define	WM_EXITPRG		1005

#define	WM_SHOWACTIV	1007
#define	WM_OPENFOL		1008
#include "resource.h"
#pragma comment (lib, "Winmm.lib")

#define LEFT_BUTTON			0x00
#define RIGHT_BUTTON		0x01
#define WHELL_BUTTON		0x02

// DX9 LIB
#include <d3dx9.h>
#include <d3d9.h>
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")	
#pragma comment (lib, "dxguid.lib")

//FUNCTIONS Dx
HRESULT InitD3D(HWND hwnd);
bool CheckMultiSampleType(DWORD QualityModeIndex);
HRESULT LoadSceneFromFile(LPWSTR ScreenPath);
HRESULT ChangeWallPaper( LPWSTR NewWallpaperPath);
LPWSTR PathToTexture(LPWSTR PathToFile, LPCSTR TextName);
LPWSTR AnsiToUnicode( LPCSTR lpAnsiStr );
BOOL WINAPI InitInput();
void WINAPI DeleteInput();
BOOL WINAPI GetMouseEvent();
void GetDeviceState();
VOID SetLight();
void RenderMainProc();
void Render3D();
void ResetDevice(HWND hWnd);
void SaveScreen(LPCWSTR _FileName, bool fJPG);
void ShutDownON();
void ShutDownOFF();
void DropWallpaper();
void ScreenSaverStartProcOn(bool cmdSparam);
void ScreenSaverProcOff();
void SetFiltrationMode(byte FiltIn);
void InitDefCamPos();
void vChangeCameraPos();
void DrawFPS();
void DrawScrCaption();
void cStopCameraMove();
void cCalculateLookAngles();
void ChangeMeshMode();
void ClearDevice(bool bDeleteInput);

//FUNCTIONS Win
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK MainDlgWndProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam); //OptionsW
void CheckMutex();
void CheckForegroundWindow();
BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam);
void DisableAero(bool EorD);
void CreateMainWindow();
void AddTrayIcon();
void SetWallpaper();
void CreateAndLoadPopupMenu();
HMENU LoadMenuFromPath(LPWSTR CurPath, int *MIid);
void SetLayared();
LPWSTR INIRead(LPWSTR FilePath, LPWSTR section, LPWSTR key, LPWSTR Def);
LPWSTR ScanStrFormat( LPWSTR ScanedString );
bool INIWrite(LPWSTR filename, LPWSTR section, LPWSTR key, LPWSTR data);
bool GetWpOptFromFile();
void ChangeWalpaperByTime();
void FatalError(LPTSTR lpszFunction);
void CleanOutPut();

//FUNCTIONS OPTIONS
void ApplyLanguage(bool Dlg, HWND dlgWnd, int byIndex);
void CheckDlgForm(HWND dlgWnd);
void LoadOptFromReg();
void SaveOptToReg();
bool AddDelInAutoRun( bool bRunShutDown, bool bDelInAutoRun);
bool UseAsScrSav(bool AddScrSav);
void SetDefOpt(HWND hDlg);
void ExecFontDialog(HWND hDlg, bool DlgShow);
void RemoveWpStyleInReg();

//OTHER
void ReCreateCaptionFont();
void CalcSphereCoord();
int CALLBACK EnumFontFamExProc( ENUMLOGFONTEX *lpelfe,NEWTEXTMETRICEX *lpntme,DWORD FontType,LPARAM lParam);
void AutoSleepCheck();
void fVisitSite();
VOID CALLBACK WinEventCallback(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
bool bResetdivaceOnProc = false;

BOOL EnableDebugPrivilege( BOOL fEnable );
byte GetWinVer();
//DX VAR
LPDIRECT3D9         _D3D = NULL;
LPDIRECT3DDEVICE9   _D3DDevice = NULL;
LPD3DXMESH _Scene					= NULL;
LPD3DXBUFFER _SceneBuffer			= NULL;
D3DMATERIAL9* _SceneMaterials		= NULL;
LPDIRECT3DTEXTURE9* _SceneTexture	= NULL;
DWORD dwNumber;
D3DLIGHT9	 Light;
LPD3DXFONT	FPSFont		= NULL;
RECT		FPSRect = { 100, 42, 230, 280};
LPWSTR OutFPSString		= (LPWSTR ) LocalAlloc(LPTR,256);	
LPD3DXFONT	CaptionFont = NULL;
RECT		CaptionRect = { 0, 0, 0, 0};

DWORD		WinH,WinW;
FLOAT		kWy;
float		_La=0.0f,_Lb=0.34f,_R=17.0f;
float		minLb = 0.1f, maxLb = 1.34f;
float		minR  = 0.1f, maxR  = 100.0f;
float		koefX1, koefX2, koefY1, koefY2;//double
//CAMERA
D3DXVECTOR3 vCam		(7.542f, 2.667f, 0.0f);
D3DXVECTOR3 vLookTo		(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 StrPos		(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 EndPos		(0.0f, 0.0f, 0.0f);
bool			EnableOptFile = false;
typedef			std::map<int,D3DXVECTOR3> cCameraPosVecA;
cCameraPosVecA	cCameraPosFile, cCameraPosDefa;
std::map<int,D3DXVECTOR3> :: size_type	cCamPos_size;
bool			bOneClickMove = false;
//CAPTIONS
typedef			std::map<int, LPWSTR> TcCaptions;
LPWSTR			CurentCation = (LPWSTR ) LocalAlloc(LPTR,256);	
TcCaptions		cCaptions;
bool			cCaptionOnProc = false;
DWORD			cCaptionStartTime = timeGetTime()-1;
//CAPTION'S FONT
LPWSTR fFONTstr = (LPWSTR ) LocalAlloc(LPTR,64);
DWORD		fColor = D3DCOLOR_XRGB( 0x00, 0x80, 0x80 ); 
DWORD		fSize = 70;
DWORD		cCamPosIndex = 0;
bool		bCamOnProc	= false;
DWORD		cLastPosChTime	= timeGetTime()-10;//60000*3;
float		cScalar			= 0.0f;
bool		o7_RotateEyesWhell_OnProc = false;
bool		o7_RotateEyesWhell_OnDesk = false;

POINT CursorPos;	FLOAT posx=0;	FLOAT posy=0;
//input
LPDIRECTINPUT8 pInput = NULL;
LPDIRECTINPUTDEVICE8 pMouse = NULL;
DIMOUSESTATE mouse;
bool LMBDown = false;
bool DownOnDesk = false;
byte SlowTransp = 0;
// old-ch WP
LPDIRECT3DVERTEXBUFFER9 Wp_Vert = NULL;
LPDIRECT3DINDEXBUFFER9  Wp_indx = NULL;
LPDIRECT3DTEXTURE9		Wp_Text = NULL;

struct CUSTOMVERTEX
{
	D3DXVECTOR3 xyzV;
	D3DXVECTOR3 xyzN;
	DWORD col;
	FLOAT tu,tv;	
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
CUSTOMVERTEX VertT[];
DWORD VertColor = 0xffffffff;
const unsigned short Index[] =	{0,1,3,		1,2,3};
bool wp_onproc = false;
float Dwp_k = 1.0f;
D3DXMATRIX MatrixView;
D3DXMATRIX MatrixProjection;
D3DXMATRIX MatrixWorld;

//WIN VAR
HWND hP,hD,hMain;
NOTIFYICONDATA IconData;
HINSTANCE ProgHinst;
wchar_t _DPbuffer[256];
FILETIME WPLastWriteTime;
COLORREF WPLastDesktColor;
byte bWinVer7XP = 0;
HANDLE Pmutex;
bool bFirstPopupM = false;
HMENU hmFirstPopupM;
HWINEVENTHOOK UAC_hook;
//GLOBAL VAR
LPWSTR GlLang = (LPWSTR ) LocalAlloc(LPTR,6);
DWORD	AvtoChWP	= 30;	//0-3-10-30-60	30min 1 800 000
float	ChCamPos	= 0;	//n-0-1-2-3		1min	60 000
bool	ChCamPosEnable = false;
//byte	UsedFilt	= 1;	//0 - none 1-bi		2-tr	3-an
byte	GQualityMode = 1;	//1 - 

bool	o1_StartWithWin		= false;
bool	o2_RunInShutdown	= false;
bool	o3_ShutdownInUnprod	= false;
bool	o4_UseAsScrSav		= false;
bool	o5_RotateNearBorder	= false;
bool	o6_RotateWithCursor	= false;
bool	o7_RotateEyesWhell	= false;
bool	o8_ZoomWhell		= false;
bool	o9_UseMeshSomeT		= false;
bool	o10_ShowFPS			= false;
bool	o11_ShowCaptions	= false;

bool	bAutoRunZ	= false;

COLORREF TpColor = RGB(34, 34, 34);
DWORD	cFonR = 210, cFonG = 210, cFonB = 210;
COLORREF DefDeskColor;
HMENU PopupMenu;
//OPT
bool	bDlgShowed = false;
LPWSTR	FirstAndCurentScreenPath;
bool	bShutDown = false;
bool	bShutDownProcesd = false;
bool	bMyWPchanged = true;
bool	bScrSavWrk = false, StartAsSS = false;
bool	bSleepMode = false;
POINT	ScrSavStartCoord;

//WHEL ROT ANGLE
float	WRAngleX = 0.0f, WRAngleY = 0.0f;
//bool bControlDown = false;
DWORD tTime = timeGetTime();

DWORD eTime = 1;
DWORD iFGW	= 0;
DWORD EnumMaxWH = 0, EnumMaxWW = 0;
//AUTO SLEEP VAR
DWORD AutoSleepTime = tTime-1;
POINT AutoSleepCursorPos;
//WALLPAPER CHANGE
DWORD TotalWallpapers = 0, CurentWPIndex = 1, LastChangeTime = tTime-1;
//MESH MODE INDEX
DWORD iMeshModeIndex = 0;
DWORD LastChangeMeshMode = tTime-1;
DWORD NextChangeMeshMode = 40000;

DWORD	iTotalSleepCounts = 0;
POINT gMouse;
bool	bCleanOutPutOnProc = false;
bool	bFirstStart = false;
DWORD	gCamMoveTime = 30000;
DWORD	gCamCapShowTime = 23000;
bool	bSoftwareOne = false;


INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR StartParam, INT )
{
	bShutDown = false;

	CheckMutex();
	ProgHinst = hInst;
	bWinVer7XP = GetWinVer();
	InitDefCamPos();	

	if ( (StartParam[1] == 'a')||(StartParam[1] == 'z') ) 
	{			
		HKEY hKey;
		WCHAR PathToProg[1024] = {0};		DWORD buf_size = sizeof(PathToProg);		

		RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\Full3DWallpaper\\", 0, KEY_ALL_ACCESS, &hKey);

		if (RegQueryValueEx( hKey, L"Path", NULL, NULL, (LPBYTE) PathToProg, &buf_size) == ERROR_SUCCESS)
			SetCurrentDirectory(PathToProg);
		else
			ExitProcess(NULL);
		RegCloseKey(hKey);
	}
	if (StartParam[1] == 'a')	Sleep( 34*1000 );
	if (StartParam[1] == 'z')	{bShutDown  = true; bAutoRunZ = true;}

	if ( (StartParam[1] != 's') && (StartParam[1] != 'z') )
		DisableAero(true);

	hP = FindWindow(L"ProgMan", NULL);	
	hD = GetDesktopWindow();

	CreateMainWindow();
	
	AddTrayIcon();
	CreateAndLoadPopupMenu();

	LoadOptFromReg();

	if ((StartParam[1] != 's') && (StartParam[1] != 'z'))
	{		
		if (!SUCCEEDED(InitD3D(hD))) { FatalError( L"Can't init Direct3D" ); CleanOutPut(); ExitProcess(NULL);}
		if (!SUCCEEDED(LoadSceneFromFile( FirstAndCurentScreenPath ))) { FatalError( L"Can't load scene" ); CleanOutPut(); ExitProcess(NULL);}
		if (!(InitInput())) FatalError( L"Can't init input" );
		SetLayared();
	} else	
		if (StartParam[1] == 's') { StartAsSS = true; ScreenSaverStartProcOn(true);}
	
	WTSRegisterSessionNotification(hMain, NOTIFY_FOR_THIS_SESSION);
	UAC_hook = SetWinEventHook( EVENT_SYSTEM_DESKTOPSWITCH, EVENT_SYSTEM_DESKTOPSWITCH, NULL, WinEventCallback, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNTHREAD);
		
	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );
	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage ( &msg );
			DispatchMessage ( &msg );
		}
		else 
			if ((bShutDown == false) || ( bScrSavWrk == true )) RenderMainProc();
			else Sleep(170);
	}
CleanOutPut();
ExitProcess(NULL);
return 0;
}
//****************************************
// DIRECTX FUNCTIONS
//****************************************
HRESULT InitD3D(HWND hwnd)//+QualityMode
{
	if (NULL == (_D3D = Direct3DCreate9(D3D_SDK_VERSION))) 
		return E_FAIL;	

	D3DDISPLAYMODE Display;
	if (FAILED(_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&Display))) return E_FAIL;
	WinH = Display.Height;
	WinW = Display.Width;
	kWy = (float) WinW / (float) WinH;

	D3DPRESENT_PARAMETERS D3DParam;
	ZeroMemory( &D3DParam, sizeof(D3DParam));	
		//if (hwnd == hMain) D3DParam.Windowed = FALSE;
	//if (bScrSavWrk)
	if (hwnd == hMain)
	{
		D3DParam.Windowed = FALSE;
		D3DParam.hDeviceWindow = hMain;
	}
	else
	{
		D3DParam.Windowed = TRUE;
		D3DParam.hDeviceWindow = hD;
	}
    D3DParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
    D3DParam.BackBufferFormat = Display.Format;
    D3DParam.EnableAutoDepthStencil = TRUE;
    D3DParam.AutoDepthStencilFormat = D3DFMT_D16;
	D3DParam.BackBufferCount = 2;
		//D3DParam.hDeviceWindow = hwnd;
		//D3DParam.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE; //D3DPRESENT_INTERVAL_ONE
	D3DParam.BackBufferHeight = WinH;
	D3DParam.BackBufferWidth  = WinW;
	switch(GQualityMode)
	{
		case 1: D3DParam.MultiSampleType = D3DMULTISAMPLE_NONE;		break;
		case 2: D3DParam.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;break;
		case 3: D3DParam.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;break;
	}

	if (FAILED(_D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                   D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                   &D3DParam, &_D3DDevice ) ) )
	{
		if (bSoftwareOne == false) {MessageBox( hMain, L"Failed to create device in Hardware mode,\nwill try to run application in Software mode.", L"Notification", MB_OK); bSoftwareOne == true;}
								   if (FAILED(_D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&D3DParam, &_D3DDevice ) ) )
								   { 
											return E_FAIL;
								   }}
	
	_D3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,TRUE);
	_D3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	_D3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	_D3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	_D3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
	_D3DDevice->SetRenderState(D3DRS_AMBIENT,0xffffffff);

	if (GQualityMode != 1)	_D3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS  , TRUE);
	else	_D3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS  , FALSE);

	SetFiltrationMode(GQualityMode);
	if (o10_ShowFPS) 
		if (D3DXCreateFont(_D3DDevice,23,0,FW_BOLD,0,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,L"Arial",&FPSFont) != S_OK) o10_ShowFPS = false;
	ReCreateCaptionFont();
	SetLight();
	return S_OK;
}
bool CheckMultiSampleType(DWORD QualityModeIndex)
{
D3DMULTISAMPLE_TYPE		MultiTest;

if (bShutDown == true ) return true;
switch(QualityModeIndex)
	{
		case 1: MultiTest = D3DMULTISAMPLE_NONE;	 break;
		case 2: MultiTest = D3DMULTISAMPLE_2_SAMPLES;break;
		case 3: MultiTest = D3DMULTISAMPLE_4_SAMPLES;break;
	}
if( SUCCEEDED(_D3D->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT,
					D3DDEVTYPE_HAL , D3DFMT_D16, TRUE, 
					MultiTest, NULL ) ) ) return true;
else return false;
}
HRESULT LoadSceneFromFile( LPWSTR ScreenPath )
{
	if (!PathFileExists(ScreenPath)) {MessageBox(NULL, L"Can't find wallpaper file", L"Notification", MB_ICONERROR|MB_OK);return E_FAIL;}

	if (FAILED(D3DXLoadMeshFromX( ScreenPath, D3DXMESH_SYSTEMMEM ,
		_D3DDevice, NULL, &_SceneBuffer, NULL,
		&dwNumber, &_Scene))) return E_FAIL;

	D3DXMATERIAL* D3DXMeshMaterials;
	D3DXMeshMaterials = (D3DXMATERIAL *) _SceneBuffer->GetBufferPointer();

	_SceneMaterials		= new D3DMATERIAL9[dwNumber];
	_SceneTexture		= new LPDIRECT3DTEXTURE9[dwNumber];
//_Scene->OptimizeInplace(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);

using namespace std;
typedef map<string, LPDIRECT3DTEXTURE9> TTextureTable;
TTextureTable TextureTable;

for (DWORD i = 0; i< dwNumber; i++)
{	
	_SceneMaterials[i] = D3DXMeshMaterials[i].MatD3D;
	_SceneMaterials[i].Ambient = _SceneMaterials[i].Diffuse;

	if (D3DXMeshMaterials[i].pTextureFilename != NULL)
	{
		TTextureTable::iterator iFind = TextureTable.find( D3DXMeshMaterials[i].pTextureFilename );
		if ( iFind == TextureTable.end() )
		{
			IDirect3DTexture9 *pTex;
			if ( SUCCEEDED( D3DXCreateTextureFromFile(_D3DDevice, PathToTexture(ScreenPath , D3DXMeshMaterials[i].pTextureFilename), &pTex)))//&_SceneTexture[i])))
				{
					pTex->QueryInterface( IID_IDirect3DBaseTexture9, (LPVOID*)&_SceneTexture[i]);
					pTex->Release();
					TextureTable.insert( make_pair(D3DXMeshMaterials[i].pTextureFilename, _SceneTexture[i]));
				}
			else 
			{
				_SceneTexture[i] = NULL;
			}
		}
		else
		{
			_SceneTexture[i] = iFind->second;
		}
	} else _SceneTexture[i] = NULL;
}
	TextureTable.clear();
	_SceneBuffer->Release();

	FirstAndCurentScreenPath = ScreenPath;
	EnableOptFile = GetWpOptFromFile();	
	return S_OK;
}
bool GetWpOptFromFile()
{
	cCameraPosFile.clear();
	cCaptions.clear();
	cCamPos_size = 0;

	maxLb = 1.33f;	minLb = 0.1f;	maxR = 100.0f;	minR = 0.1f;
	cFonR = 210;	cFonG = 210;	cFonB = 210;

	LPWSTR PathToOptF = (LPWSTR ) LocalAlloc(LPTR,256);	
	StringCbPrintf((LPWSTR) PathToOptF, 256 ,L"%s", PathToTexture( FirstAndCurentScreenPath, "wp_data.ini"));
	if (!PathFileExists(PathToOptF)) return false;
	
	//Reading Camera pos	
	DWORD sec_N = 0;
	LPWSTR INIKeyX = ( LPWSTR ) LocalAlloc(LPTR,32);
	LPWSTR INIKeyY = ( LPWSTR ) LocalAlloc(LPTR,32);
	LPWSTR INIKeyZ = ( LPWSTR ) LocalAlloc(LPTR,32);
	LPWSTR CAPTI_N = ( LPWSTR ) LocalAlloc(LPTR,32);
	StringCbPrintf( INIKeyX, 256 ,L"POS%dX", sec_N);
	StringCbPrintf( INIKeyY, 256 ,L"POS%dY", sec_N);
	StringCbPrintf( INIKeyZ, 256 ,L"POS%dZ", sec_N);
	StringCbPrintf( CAPTI_N, 256 ,L"CAP%d" , sec_N);
	
	LPWSTR TempStrKeyX = (LPWSTR ) LocalAlloc(LPTR,64);
	LPWSTR TempStrKeyY = (LPWSTR ) LocalAlloc(LPTR,64);
	LPWSTR TempStrKeyZ = (LPWSTR ) LocalAlloc(LPTR,64);
	LPWSTR CAPTION	   = (LPWSTR ) LocalAlloc(LPTR,256);

	TempStrKeyX = L"0";	TempStrKeyY = L"0";	TempStrKeyZ = L"0";
	using namespace std;
	
	while ((TempStrKeyX[0]!=0)&&(TempStrKeyY[0]!=0)&&(TempStrKeyZ[0]!=0))
	{
		TempStrKeyX = INIRead( PathToOptF, L"CAMERA", INIKeyX, NULL);
		TempStrKeyY = INIRead( PathToOptF, L"CAMERA", INIKeyY, NULL);
		TempStrKeyZ = INIRead( PathToOptF, L"CAMERA", INIKeyZ, NULL);

		if ( (TempStrKeyX[0]==0)||(TempStrKeyY[0]==0)||(TempStrKeyZ[0]==0) ) break;

		cCameraPosFile.insert( make_pair(sec_N, D3DXVECTOR3( _wtof(TempStrKeyX), _wtof(TempStrKeyY), _wtof(TempStrKeyZ))));
		cCamPos_size = cCameraPosFile.size()-1;
		//CAPTIONS ADD
		CAPTION = INIRead( PathToOptF, L"CAPTION", CAPTI_N, NULL);
		if (CAPTION[0]!=0)
		{
			cCaptions.insert( make_pair(sec_N, ScanStrFormat(CAPTION)) );
		}
		sec_N++;
		StringCbPrintf( INIKeyX, 256 ,L"POS%dX", sec_N);
		StringCbPrintf( INIKeyY, 256 ,L"POS%dY", sec_N);
		StringCbPrintf( INIKeyZ, 256 ,L"POS%dZ", sec_N);
		StringCbPrintf( CAPTI_N, 256 ,L"CAP%d" , sec_N);
	}	

	if (cCamPosIndex>cCamPos_size) cCamPosIndex=0;
	//Gamma color	
	cFonR = _wtoi(INIRead( PathToOptF, L"FONCOLOR", L"R", L"210")),
	cFonG = _wtoi(INIRead( PathToOptF, L"FONCOLOR", L"G", L"210")),
	cFonB = _wtoi(INIRead( PathToOptF, L"FONCOLOR", L"B", L"210"));
	//Angles
	maxLb = _wtof(INIRead( PathToOptF, L"ANGLE", L"HIGHER", L"1.33"));
	minLb = _wtof(INIRead( PathToOptF, L"ANGLE", L"LOWER",  L"0.1"));
	//Radius	maxR = 100.0f;	minR = 0.1f;
	maxR = _wtof(INIRead( PathToOptF, L"RADIUS", L"MAX", L"100.0"));
	minR = _wtof(INIRead( PathToOptF, L"RADIUS", L"MIN", L"0.1"));

	//LocalFree(CAPTION);
	LocalFree(PathToOptF);
	LocalFree(INIKeyX);	LocalFree(INIKeyY);	LocalFree(INIKeyZ);
	LocalFree(CAPTI_N);
	LocalFree(TempStrKeyX);	LocalFree(TempStrKeyY);	LocalFree(TempStrKeyZ);
	return true;
}
HRESULT ChangeWallPaper( LPWSTR NewWallpaperPath)
{
	if (!PathFileExists(NewWallpaperPath)) {MessageBox(NULL, L"Can't find wallpaper file", L"Notification", MB_ICONERROR|MB_OK);return E_FAIL;}
	///
	if ((bShutDown == false)||(bScrSavWrk))
	{
		ClearDevice(false);		

		if (bScrSavWrk) InitD3D(hMain);
		else InitD3D(hD);
		LoadSceneFromFile(NewWallpaperPath);
		cStopCameraMove();
	}
	else
	{
		FirstAndCurentScreenPath = NewWallpaperPath;
		ShutDownOFF();
	}
	return S_OK;
}
BOOL WINAPI InitInput()
{
	HRESULT hr;
	if (FAILED(DirectInput8Create(ProgHinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pInput, NULL))) return E_FAIL;

	hr = pInput->CreateDevice(GUID_SysMouse, &pMouse, NULL);
		if FAILED(hr) {DeleteInput(); return FALSE;}
	hr = pMouse->SetDataFormat(&c_dfDIMouse);
		if FAILED(hr) {DeleteInput(); return FALSE;}	
	hr = pMouse->SetCooperativeLevel(NULL, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	//if FAILED(hr) {DeleteInput(); return FALSE;}
	hr = pMouse->Acquire();
	if FAILED(hr) {DeleteInput(); return FALSE;}

	return TRUE;
}
void WINAPI DeleteInput()
{
	if (pInput)
	{
		pMouse->Unacquire();
		if (pMouse != NULL) pMouse->Release();
		if (pInput != NULL) pInput->Release(); pInput = NULL;
	}
}
BOOL WINAPI GetMouseEvent()
{
	if (FAILED(pMouse->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&mouse)))
	{DeleteInput(); return FALSE;}
	//LEFT_BUTTON DOWN
	if ((DownOnDesk != true)&&(mouse.rgbButtons[LEFT_BUTTON]&0x80))
	{
		if (WindowFromPoint(gMouse) == hD) 
			{
				if (LMBDown == false)
				{					
					mouse_event( MOUSEEVENTF_LEFTUP, gMouse.x ,gMouse.y, 0, 0); 					
					SetLayeredWindowAttributes(hP,TpColor, 23, LWA_ALPHA &~ LWA_COLORKEY);					
					mouse_event( MOUSEEVENTF_LEFTDOWN, gMouse.x ,gMouse.y, 0, 0);
				}
				else 
				{
					SetLayeredWindowAttributes(hP,TpColor, 23, LWA_ALPHA &~ LWA_COLORKEY);	
				}
				DownOnDesk = true;
			}
		LMBDown = true;
	}

	if (LMBDown == true) if (!(mouse.rgbButtons[LEFT_BUTTON]&0x80)) LMBDown = false;

	if ((DownOnDesk == true )&&(SlowTransp < 120))	
	{SlowTransp = SlowTransp + 0.3*eTime; SetLayeredWindowAttributes( hP, TpColor, SlowTransp, LWA_ALPHA &~ LWA_COLORKEY);}

	if ((SlowTransp > 1)&&(!(mouse.rgbButtons[LEFT_BUTTON]&0x80)))
	{		
		DownOnDesk = false; //LMBDown = false;
		//SlowTransp = SlowTransp - 0.3*eTime;
		//SetLayeredWindowAttributes( hP, TpColor, SlowTransp, LWA_ALPHA | LWA_COLORKEY);
		//if (SlowTransp<=50) {
		Sleep(10);
		SetLayeredWindowAttributes( hP, TpColor, 220, LWA_ALPHA | LWA_COLORKEY);
		SlowTransp=0;
		//}
	}
	//RIGHT_BUTTON DOWN
	if (mouse.rgbButtons[RIGHT_BUTTON]&0x80)
	{
		if (WindowFromPoint(gMouse) == hD)
		{	
			HWND LW = FindWindowEx(FindWindowEx(hP,NULL,L"SHELLDLL_DefView",NULL),NULL, L"SysListView32", NULL);
			DWORD pID ;	GetWindowThreadProcessId(LW, &pID);
			BOOL bResult;
			HANDLE hProcess;
			LRESULT lResult;
			LVITEM lv, *lvrp;
			SIZE_T WriteCount;
			//int count = ListView_GetItemCount(LW);		
			//lv.iItem = -1;
			//lv.iSubItem = 0;
			lv.mask = LVIF_STATE;
			lv.state = 0;//0x0;//LVIS_SELECTED | LVIS_FOCUSED;
			lv.stateMask = 0x000f;
			
			EnableDebugPrivilege(true);
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, true, pID);
			lvrp = (LVITEM*)VirtualAllocEx(hProcess, 0, sizeof(LVITEM), MEM_COMMIT,PAGE_READWRITE);
			bResult = WriteProcessMemory(hProcess, lvrp, &lv, sizeof(lv), &WriteCount);
			
			lResult = SendMessage(LW, LVM_SETITEMSTATE, (WPARAM)-1, (LPARAM)lvrp);
			//if (lResult != 1) ListView_SetItemState(LW, -1, 0, LVIS_SELECTED);

			bResult = VirtualFreeEx(hProcess, lvrp, 0, MEM_RELEASE);
			bResult = CloseHandle(hProcess);
	
			SendMessage(LW, WM_RBUTTONUP, (WPARAM)NULL, MAKELONG(gMouse.x,gMouse.y));
			EnableDebugPrivilege(false);
		}
	}
	//WHELL SCROLL
	if ( (o8_ZoomWhell)&&(mouse.lZ) )
	{
		if (WindowFromPoint(gMouse) == hD)
		{
			_R = _R + (mouse.lZ/50000.0f*eTime)*(-1);
		}
	}
	//WHELL DOWN
	if (mouse.rgbButtons[WHELL_BUTTON]&0x80)
	{
		if (o7_RotateEyesWhell)
		{	
			if (WindowFromPoint(gMouse) == hD)
			{
				if (!o7_RotateEyesWhell_OnDesk) o7_RotateEyesWhell_OnDesk = true;
				if (!o7_RotateEyesWhell_OnProc) o7_RotateEyesWhell_OnProc = true;
			}
		}
	}
	else 
		if(o7_RotateEyesWhell_OnDesk) o7_RotateEyesWhell_OnDesk = false;
	
	return TRUE;
}
VOID SetCamera()
{
	D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction, &vCam);
	_D3DDevice->SetLight(0,&Light);

	if ((o6_RotateWithCursor)&&(bCamOnProc == false))
	{
		posx = (gMouse.x/(WinW/0.1))-0.05f;
		posy = (gMouse.y/(WinH/0.1))-0.05f;
	}
	if ((o5_RotateNearBorder)&&(!bCamOnProc)&&(!o7_RotateEyesWhell_OnProc))
	{
		if (gMouse.x<3)				if (koefX1<0.04f)	koefX1 = koefX1 + (0.00017f*eTime);						//(0.17f/1000.0f*eTime) = 0.00017f
		if (koefX1>0)				{ _La = _La + (koefX1/134.0f*eTime); koefX1 = koefX1 - (0.000047f*eTime);}	//_La = _La + (10.0f*koefX1/1000.0f*eTime); = _La + (koefX1/100.0f*eTime)
		if (gMouse.x>(WinW-3))		if (koefX2<0.04f)	koefX2 = koefX2 + (0.00017f*eTime);
		if (koefX2>0)				{ _La = _La - (koefX2/134.0f*eTime); koefX2 = koefX2 - (0.000047f*eTime);}

		if (gMouse.y<3)				if (koefY1<0.03)	koefY1 = koefY1 + (0.00017f*eTime);
		if (koefY1>0)				{ _Lb = _Lb + (koefY1/134.0f*eTime); koefY1 = koefY1 - (0.000047f*eTime);}	//_La = _La + (~8.0f*koefX1/1000.0f*eTime); = _La + (koefX1/100.0f*eTime)
		if (gMouse.y>(WinH-3))		if (koefY2<0.03)	koefY2 = koefY2 + (0.00017f*eTime);
		if (koefY2>0)				{ _Lb = _Lb - (koefY2/134.0f*eTime); koefY2 = koefY2 - (0.000047f*eTime);}
	}	

	if (!bCamOnProc)
		if ( (o5_RotateNearBorder)||(o6_RotateWithCursor)||(o7_RotateEyesWhell_OnProc)||(o8_ZoomWhell) )
	{
		if (_La > D3DX_PI*2) _La = 0.0f;
		if (_La < 0) _La = D3DX_PI*2;
		if (_Lb > maxLb) _Lb = maxLb;
		if (_Lb < minLb) _Lb = minLb; 
		if (_R	> maxR ) _R  = maxR;
		if (_R	< minR ) _R  = minR;

		vCam.x = _R * cos(_La-posx) * cos(_Lb-posy);
		vCam.y = _R * sin(_Lb-posy);
		vCam.z = _R * sin(_La-posx) * cos(_Lb-posy);
	}	
	vChangeCameraPos();	
	// MATRIX VIEW
	D3DXMatrixLookAtLH(&MatrixView,
					   &vCam,
					   &vLookTo,
					   &D3DXVECTOR3(0.0f, 0.1f, 0.0f));

	if (o7_RotateEyesWhell_OnProc) cCalculateLookAngles();
	_D3DDevice->SetTransform(D3DTS_VIEW,&MatrixView);
	
	// MATRIX PROJECTION
	D3DXMatrixPerspectiveFovLH(&MatrixProjection, D3DX_PI/4, kWy, 1.0f, 1000.0f);
	_D3DDevice->SetTransform(D3DTS_PROJECTION, &MatrixProjection);

	D3DXMatrixIdentity(&MatrixWorld);
	_D3DDevice->SetTransform(D3DTS_WORLD,&MatrixWorld);
}
VOID SetLight()
{
	ZeroMemory(&Light,sizeof(D3DLIGHT9));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Diffuse.r = 1.0f;
	Light.Diffuse.g = 1.0f;
	Light.Diffuse.b = 1.0f;
	Light.Range = 3.0f;

	D3DXVECTOR3 VectorDir;
	VectorDir = D3DXVECTOR3(1.0f, 1.0f, 1.0f),
	D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction, &VectorDir);

	_D3DDevice->SetLight(0,&Light);
	_D3DDevice->LightEnable(0, TRUE);

	_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	_D3DDevice->SetRenderState(D3DRS_AMBIENT, 0);
}
VOID DrawScene()
{
	for (DWORD i=0; i<dwNumber; i++)
	{
		_D3DDevice->SetMaterial(&_SceneMaterials[i]);
		_D3DDevice->SetTexture (0, _SceneTexture[i]);
		_Scene->DrawSubset(i);
	}
}
void RenderMainProc()
{
	tTime = timeGetTime();

	if (AvtoChWP != 0)			ChangeWalpaperByTime();

	GetCursorPos(&gMouse);	

	Render3D();

	if (o9_UseMeshSomeT)		ChangeMeshMode();
	if (bScrSavWrk == false)	CheckForegroundWindow();

	eTime = timeGetTime() - tTime;
	if (eTime<20) 
	{
		Sleep(20-eTime);
		eTime=eTime+(20-eTime);
	}//25ms = 40 fps	20ms = 50 fps
	
}
void Render3D()
{
	GetMouseEvent();
	_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB(cFonR,cFonG,cFonB), 1.0f, 0);

	_D3DDevice->BeginScene();

	SetCamera();
	DrawScene();
	if (wp_onproc)		DropWallpaper();

	if (o10_ShowFPS)	DrawFPS();
	if (o11_ShowCaptions)	if( cCaptionOnProc)	DrawScrCaption();

	_D3DDevice->EndScene();
	_D3DDevice->Present(NULL,NULL,NULL,NULL);
}
void ResetDevice(HWND hWnd)
{
	bResetdivaceOnProc = true;
	ClearDevice(true);

	if (!SUCCEEDED(InitD3D(hWnd)))
	{
		bool	bSucceeded	= false;
		byte	iCount		= 0;
		while (bSucceeded == false)
		{
			iCount++;
			if (iCount>17)	{
								SetWindowLong(hP,GWL_EXSTYLE, GetWindowLong(hP,GWL_EXSTYLE) &~ WS_EX_LAYERED );
								bShutDown = true;
								CleanOutPut();
								ExitProcess(NULL);
							}
			Sleep(800);
			if (SUCCEEDED(InitD3D(hWnd)))
				bSucceeded = true;			
		}
	}
	LoadSceneFromFile(FirstAndCurentScreenPath);
	InitInput();
	bResetdivaceOnProc = false;
}
void SaveScreen(LPCWSTR _FileName, bool fJPG)
{
	if (_D3DDevice != NULL)
	{
		LPDIRECT3DSURFACE9	_Surface = NULL;
		_D3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO,&_Surface);
		if (fJPG) D3DXSaveSurfaceToFile(_FileName,D3DXIFF_JPG,_Surface,NULL,NULL);
		else
		D3DXSaveSurfaceToFile(_FileName,D3DXIFF_BMP,_Surface,NULL,NULL);
		_Surface->Release();
	}
}
void MakeScreenshot()
{	
	LPWSTR SkreenPath = (LPWSTR ) LocalAlloc(LPTR,256);
	LPWSTR NewSkreenName = (LPWSTR ) LocalAlloc(LPTR,256);
	LPWSTR NewName = (LPWSTR ) LocalAlloc(LPTR,34);

	//GetCurrentDirectory(256,SkreenPath);
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, SkreenPath);
	StringCbPrintf( SkreenPath, 256 ,L"%s\\Full3DWallpaper\\", SkreenPath);
	if (!PathFileExists(SkreenPath)) CreateDirectory(SkreenPath,NULL);

	StringCbPrintf( SkreenPath, 256 ,L"%sScreenshots\\", SkreenPath);
	if (!PathFileExists(SkreenPath)) CreateDirectory(SkreenPath,NULL);

	int  n = 0;
	while (n!=1024)
	{
		n++;
		StringCbPrintf(NewSkreenName, 256, L"%sScreen_%i.jpg", SkreenPath, n);
		if (!PathFileExists(NewSkreenName)) 
		{
			SaveScreen(NewSkreenName, true);
			StringCbPrintf(NewName, 34, L"Screen_%i.jpg", n);
			wcscpy( IconData.szInfoTitle, L"Saved as: " );
			wcscpy( IconData.szInfoTitle, NewName );
			//wcscpy( IconData.szInfo, NewName );
			wcscpy( IconData.szInfo, SkreenPath );
			//ShellExecute(NULL, L"open", SkreenPath, NULL, NULL, SW_SHOWNORMAL);
			IconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO ;
			Shell_NotifyIcon(NIM_MODIFY,&IconData);
			IconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP &~ NIF_INFO ;
			Shell_NotifyIcon(NIM_MODIFY,&IconData);
			break;
		}
	}	
}
void SetWallpaper()
{	
	WCHAR PathToDoc[MAX_PATH];	//%USERPROFILE%
	HRESULT hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, PathToDoc);

	LPWSTR SaveSkreenPath = (LPWSTR ) LocalAlloc(LPTR,256);
	StringCchPrintf( SaveSkreenPath, 256 ,L"%s\\Full3DWallpaper\\", PathToDoc);
	if (!PathFileExists(SaveSkreenPath)) CreateDirectory(SaveSkreenPath,NULL);

	StringCchPrintf( SaveSkreenPath, 256 ,L"%s\\Full3DWallpaper\\Wallpaper.bmp", PathToDoc);
	SaveScreen(SaveSkreenPath, false);
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, SaveSkreenPath,  SPIF_UPDATEINIFILE );
}
void ShutDownON()
{
bShutDownProcesd = true;
bShutDown  = true;

ModifyMenu( PopupMenu, WM_MAKESCR, MF_BYCOMMAND | MF_GRAYED, WM_MAKESCR, L"Make screenshot");

MENUITEMINFO mii = {NULL};
mii.cbSize = sizeof (MENUITEMINFO);
mii.fMask = MIIM_STATE;
GetMenuItemInfo(PopupMenu, WM_SHUTDWN, FALSE, &mii);
mii.fState = MFS_CHECKED;
SetMenuItemInfo(PopupMenu, WM_SHUTDWN, FALSE, &mii);
//if (mii.fState == MFS_CHECKED) {	mii.fState = MFS_UNCHECKED;	bShutDown = FALSE;}//else{	mii.fState = MFS_CHECKED;	bShutDown  = TRUE;}

IconData.hIcon = LoadIcon(ProgHinst, MAKEINTRESOURCE(IDI_ICO_TRAY_Z));	
Shell_NotifyIcon(NIM_MODIFY,&IconData);

	int aiElements[1] = {COLOR_BACKGROUND};
	DWORD aColors[1] = {DefDeskColor};
	SetSysColors(1, aiElements, aColors);

	//SaveScreen(L"DesktopScreen.bmp", false);
	SetWallpaper();//SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, L"DesktopScreen.bmp",  SPIF_UPDATEINIFILE );//BW
	SetWindowLong(hP,GWL_EXSTYLE, GetWindowLong(hP,GWL_EXSTYLE) &~ WS_EX_LAYERED );//-L

	ClearDevice(true);

	DisableAero(false);
	bShutDownProcesd = false;
}
void ShutDownOFF()
{
bShutDownProcesd = true;

ModifyMenu( PopupMenu, WM_MAKESCR, MF_BYCOMMAND | MF_ENABLED, WM_MAKESCR, L"Make screenshot");
DisableAero(true);

	if (bAutoRunZ == true)
	{
		bAutoRunZ = false;
		hP = FindWindow(L"ProgMan", NULL);	
		hD = GetDesktopWindow();
	}

MENUITEMINFO mii = {NULL};
mii.cbSize = sizeof (MENUITEMINFO);
mii.fMask = MIIM_STATE;
GetMenuItemInfo(PopupMenu, WM_SHUTDWN, FALSE, &mii);
mii.fState = MFS_UNCHECKED;
SetMenuItemInfo(PopupMenu, WM_SHUTDWN, FALSE, &mii);

IconData.hIcon = LoadIcon(ProgHinst, MAKEINTRESOURCE(IDI_ICO_TRAY));	
Shell_NotifyIcon(NIM_MODIFY,&IconData);

	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, L"", SPIF_SENDCHANGE | SPIF_UPDATEINIFILE );
	RemoveWpStyleInReg();

	SetWindowLong(hP,GWL_EXSTYLE, GetWindowLong(hP,GWL_EXSTYLE) | WS_EX_LAYERED );
	SetLayeredWindowAttributes(hP,TpColor,220, LWA_COLORKEY | LWA_ALPHA);

	//!!!!DefDeskColor = COLOR_BACKGROUND;
	int aiElements[1] = {COLOR_BACKGROUND};
	DWORD aColors[1] = {TpColor};
	SetSysColors(1, aiElements, aColors);

bShutDown  = false;
	InitD3D(hD);
	LoadSceneFromFile(FirstAndCurentScreenPath);
	InitInput();


bShutDownProcesd = false;

iFGW = 41;
bSleepMode = false;
cLastPosChTime = tTime-1;
CheckForegroundWindow();
}
void DropWallpaper()
{
	if (wp_onproc == false)
	{
		wp_onproc = true;
		LPWSTR PathToWallpaper = (LPWSTR) LocalAlloc(LPTR,256);
		StringCbPrintf( PathToWallpaper, 256, L"%s", _DPbuffer);

		if (PathFileExists(PathToWallpaper)) {D3DXCreateTextureFromFile(_D3DDevice, PathToWallpaper, &Wp_Text);VertColor = 0xffffffff;}
		else
		{			
			//Wp_Text = NULL; ?????????????????????????
			//if (Wp_Text != NULL ) Wp_Text->Release();
			VertColor = D3DCOLOR_ARGB( 0xf7, (GetRValue(DefDeskColor)& 0xf8)>>3, (GetGValue(DefDeskColor)& 0xfc)>>3, (GetBValue(DefDeskColor)& 0xf8)>>3);
		}

		if (FAILED(_D3DDevice->CreateVertexBuffer(6*sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &Wp_Vert, NULL))) return;
		if (FAILED(_D3DDevice->CreateIndexBuffer (6*sizeof(CUSTOMVERTEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &Wp_indx,NULL))) return;
		Dwp_k = 0.9f;
	}
			D3DXMATRIX matrRA, matrSC, matrTR;
			D3DXVECTOR3 P0,P1,P2,P3;

			P0 = D3DXVECTOR3(0.0f,  0.5f, -0.5f);
			P1 = D3DXVECTOR3(0.0f,  0.5f,  0.5f);
			P2 = D3DXVECTOR3(0.0f, -0.5f,  0.5f);
			P3 = D3DXVECTOR3(0.0f, -0.5f, -0.5f);

			Dwp_k = Dwp_k + (0.0023* (float) eTime);

			D3DXMatrixRotationYawPitchRoll(&matrRA, -_La, (Dwp_k*(-1))/2.3f, _Lb + (Dwp_k*(-1))/2.4f); // Z+ ((float)(Dwp_k+1)/15)		+ ( log((float)Dwp_k)
			D3DXMatrixScaling(&matrSC, _R, _R, kWy*_R);
			D3DXMatrixMultiply(&matrRA, &matrSC, &matrRA);

			D3DXMatrixScaling(&matrTR, 1,  1/Dwp_k,  1/Dwp_k ); //(float) 1/(Dwp_k+1)
			D3DXMatrixMultiply(&matrRA, &matrTR, &matrRA);
			
			
			D3DXVec3TransformCoord( &P0, &P0, &matrRA);
			D3DXVec3TransformCoord( &P1, &P1, &matrRA);
			D3DXVec3TransformCoord( &P2, &P2, &matrRA);
			D3DXVec3TransformCoord( &P3, &P3, &matrRA);

			D3DXVec3Lerp(&P0, &P0, &vCam, 0.5f);
			D3DXVec3Lerp(&P1, &P1, &vCam, 0.5f);
			D3DXVec3Lerp(&P2, &P2, &vCam, 0.5f);
			D3DXVec3Lerp(&P3, &P3, &vCam, 0.5f);

		CUSTOMVERTEX VertT[] =
		{
			{ P0,	(-1)*vCam,	VertColor,	0.0f, 0.0f},
			{ P1,	(-1)*vCam,	VertColor,	1.0f, 0.0f},
			{ P2,	(-1)*vCam,	VertColor,  1.0f, 1.0f},
			{ P3,	(-1)*vCam,	VertColor,  0.0f, 1.0f},			
		};

			VOID* pBV;
			if (FAILED(Wp_Vert->Lock(0, sizeof(VertT), (void**)&pBV, 0))) return;
			memcpy(pBV,VertT,sizeof(VertT));
			Wp_Vert->Unlock();			
			
			VOID* pBI;
			if (FAILED(Wp_indx->Lock(0, sizeof(Index), (void**)&pBI, 0))) return;
			memcpy(pBI,Index,sizeof(Index));
			Wp_indx->Unlock();
	

	_D3DDevice->SetStreamSource(0,Wp_Vert, 0,sizeof(CUSTOMVERTEX));
	_D3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	_D3DDevice->SetIndices(Wp_indx);
	
	if (Wp_Text != NULL) _D3DDevice->SetTexture(0, Wp_Text);
	_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	_D3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	
	_D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,6,0,4);
	///////////////////////

	if (Dwp_k>5)
	{		
		if (Wp_Text != NULL ) {
			Wp_Text->Release(); 
			//Wp_Text->FreePrivateData(NULL);
			Wp_Text = NULL;
		}
		if (Wp_indx != NULL) Wp_indx->Release();
		if (Wp_Vert != NULL) Wp_Vert->Release();
		wp_onproc = false;
	}
}
void ScreenSaverStartProcOn(bool cmdSparam)
{
	bScrSavWrk = true;
	GetCursorPos(&ScrSavStartCoord);

	if (bShutDown == false)			//||(cmdSparam == false))	
	{
		ClearDevice(true);
	}
	else
		LastChangeTime = tTime-1;
	
	//create
	SetWindowLong( hMain, GWL_EXSTYLE, GetWindowLong( hMain, GWL_EXSTYLE) | WS_EX_TOPMOST );
	SetWindowLong( hMain, GWL_STYLE  , GetWindowLong( hMain, GWL_STYLE  ) | WS_VISIBLE );//| WS_OVERLAPPEDWINDOW | WS_VISIBLE );
		//bShutDown = false;
	ShowWindow( hMain, SW_SHOW);
	InitD3D(hMain);
	LoadSceneFromFile(FirstAndCurentScreenPath);
	InitInput();
	ShowCursor( false );
}
void ScreenSaverProcOff()
{
	
	ShowCursor( true );
		//SystemParametersInfo( SPI_SCREENSAVERRUNNING, FALSE, NULL, 0 );
		//SetWindowLong( hMain, GWL_EXSTYLE, GetWindowLong( hMain, GWL_EXSTYLE) &~ WS_EX_TOPMOST);
		//SetWindowLong( hMain, GWL_STYLE  , GetWindowLong( hMain, GWL_STYLE  ) &~ WS_POPUP &~ WS_VISIBLE );//&~ WS_POPUP &~ WS_VISIBLE
		//ShowWindow( hMain, SW_HIDE );	

	if (StartAsSS == false)
	{		
		if (bShutDown == true)	ShutDownON();
		else ResetDevice(hD);
			SystemParametersInfo( SPI_SCREENSAVERRUNNING, FALSE, NULL, 0 );
			SetWindowLong( hMain, GWL_EXSTYLE, GetWindowLong( hMain, GWL_EXSTYLE) &~ WS_EX_TOPMOST);
			ShowWindow( hMain, SW_HIDE );
		SendMessage( FindWindow(L"WC_Full3DScrSav", L"Full3DScreensaver") , WM_SCRSAVMSG, (WPARAM)NULL, (LPARAM)NULL);
		bScrSavWrk = false;
	}
	else
	{
			CleanOutPut();
			SendMessage( FindWindow(L"WC_Full3DScrSav", L"Full3DScreensaver") , WM_SCRSAVMSG, (WPARAM)NULL, (LPARAM)NULL);
			ExitProcess(NULL);
	}

}
void SetFiltrationMode(byte FiltIn)
{
	if (bShutDown == false)
	switch(FiltIn)
	{
	case 0: //NON USE
		_D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);//D3DTEXF_NONE
		_D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);		
		_D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		break;
	case 1: 
		_D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		_D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		_D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		break;
	case 2: 
		_D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		_D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		_D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		break;
	case 3: 
		_D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		_D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		_D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		_D3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 4);
		break;
	}
}
void InitDefCamPos()
{
	using namespace std;
	cCameraPosDefa.insert( make_pair(0, D3DXVECTOR3( 0.0f,  1.0f, 17.0f)));
	cCameraPosDefa.insert( make_pair(1, D3DXVECTOR3(-12.0f, 1.0f, 12.0f)));
	cCameraPosDefa.insert( make_pair(2, D3DXVECTOR3(-17.0f, 1.0f, 0.0f)));
	cCameraPosDefa.insert( make_pair(3, D3DXVECTOR3(-12.0f, 1.0f,-12.0f)));
	cCameraPosDefa.insert( make_pair(4, D3DXVECTOR3( 0.0f,  1.0f,-17.0f)));
	cCameraPosDefa.insert( make_pair(5, D3DXVECTOR3( 12.0f, 1.0f,-12.0f)));
	cCameraPosDefa.insert( make_pair(6, D3DXVECTOR3(17.0f,  1.0f, 0.0f)));
	cCameraPosDefa.insert( make_pair(7, D3DXVECTOR3( 12.0f, 1.0f, 12.0f)));
}
void vChangeCameraPos()
{
	//Def
	//bFirstStart = false
	//gCamMoveTime = 30000
	//gCamCapShowTime = 23000
	//ChCamPos = ~2*60000
	if (bCamOnProc)
	{
		cScalar = (float)(tTime-cLastPosChTime)/gCamMoveTime;
		vCam = (EndPos - StrPos) * cScalar + StrPos;
		if (tTime-cLastPosChTime>gCamMoveTime)
		{
			bCamOnProc = false;
			CalcSphereCoord();
			if( (koefX1+koefX2+koefY1+koefY2)>0 )
				{ koefX1 = 0; koefX2 = 0; koefY1 = 0; koefY2 = 0;}
			if ( (!cCaptionOnProc)&&(o11_ShowCaptions) ) if( cCaptions[cCamPosIndex] != 0 ) 
			{
				cCaptionOnProc = true;
				cCaptionStartTime = tTime;
				StringCbPrintf((LPWSTR) CurentCation, 256 ,L"%s", cCaptions[cCamPosIndex]);
			}
			if (bOneClickMove) bOneClickMove= false;
			//////////OFF FIRST START
			if (bFirstStart)
				if (cCamPosIndex==0)
				{
					PostMessage( hMain, WM_COMMAND, (WPARAM) WM_SHOWACTIV, (LPARAM) NULL);	//SendMessage( hMain, WM_COMMAND, (WPARAM) WM_SHOWACTIV, (LPARAM) NULL );
					Render3D();Render3D();
				}
		}
	}

	if ( (ChCamPosEnable) || (bOneClickMove) )
	if ( (tTime - cLastPosChTime) > ( ChCamPos * 60000) )
	{
		if (bCamOnProc == false)
		{
			bCamOnProc = true;
			cLastPosChTime = tTime-1;
			StrPos = vCam;
			if (cCamPos_size>0) 
			{
				cCamPosIndex++;
				if (cCamPosIndex>cCamPos_size) cCamPosIndex=0;
				EndPos = cCameraPosFile[cCamPosIndex];				
				//if (cCamPosIndex>cCamPos_size) cCamPosIndex=0;
			}
			else
			{	
				cCamPosIndex++;
				if (cCamPosIndex>7) cCamPosIndex=0;
				EndPos = cCameraPosDefa[cCamPosIndex];				
				//if (cCamPosIndex>7) cCamPosIndex=0;
			}
		}
	}
}
void DrawFPS()
{
	static DWORD FPSTime;
	static DWORD FPSTimeCount;
	static DWORD FPSTrue;

	FPSTime = FPSTime + eTime;
	FPSTimeCount++;
	if (FPSTime>1000)
	{
		FPSTrue = FPSTimeCount;
		FPSTime = 0;
		FPSTimeCount = 0;		
	}
	StringCbPrintf((LPWSTR) OutFPSString, 256 ,L"FPS= %d\r\nx= %f\r\ny= %f\r\nz= %f\r\nCam.pos.№: %d\r\nr= %f\r\na= %f\r\nb= %f", FPSTrue, vCam.x, vCam.y, vCam.z, cCamPosIndex, _R, _La, _Lb);	
	FPSFont->DrawText(NULL, OutFPSString , -1, &FPSRect, DT_TOP | DT_LEFT ,  0xffc0c0c0);//DT_WORDBREAK
}
void DrawScrCaption()
{
	CaptionFont->DrawText(NULL, CurentCation , -1, &CaptionRect, DT_TOP | DT_CENTER | DT_WORDBREAK,  fColor);//fColor	D3DCOLOR
	if (tTime-cCaptionStartTime>gCamCapShowTime) cCaptionOnProc = false;//23000
}
void cStopCameraMove()
{
	if (bCamOnProc)
	{
		bCamOnProc = false;
		CalcSphereCoord();
		if( (koefX1+koefX2+koefY1+koefY2)>0 )
			{ koefX1 = 0; koefX2 = 0; koefY1 = 0; koefY2 = 0;}
		cLastPosChTime = tTime - ( ChCamPos * 60000) - 1000;
	}
	if (cCaptionOnProc) cCaptionOnProc = false;
	if (bOneClickMove)   bOneClickMove = false;
}
void cCalculateLookAngles()
{
	//POINT p;
	//GetCursorPos(&p);
	float	AngleX = 0.0f, AngleY = 0.0f;
	D3DXMATRIX	MatrixViewX,
				MatrixViewY,
				MatrixViewLookXY;

	if ( o7_RotateEyesWhell_OnDesk )
	{
		AngleX = (float) gMouse.x/(WinW/D3DX_PI*2)-D3DX_PI/4;
		WRAngleX = WRAngleX + (AngleX-WRAngleX)/10;

		AngleY = (float) gMouse.y/(WinH/D3DX_PI*2)-D3DX_PI/4;
		WRAngleY = WRAngleY + (AngleY-WRAngleY)/10;
	}
	else
	{
		if (WRAngleX != 0 ) WRAngleX = WRAngleX/1.34;
		if (WRAngleY != 0 ) WRAngleY = WRAngleY/1.34;
	}

	if ( (WRAngleX > 0-0.001) && (WRAngleX < 0+0.001)) 
			WRAngleX=0; 
	if ( (WRAngleY > 0-0.001) && (WRAngleY < 0+0.001) )
		WRAngleY=0;

	if ( (WRAngleX == 0) && (WRAngleY == 0) )
		o7_RotateEyesWhell_OnProc = false;

	D3DXMatrixRotationY(&MatrixViewX, WRAngleX);
	D3DXMatrixRotationX(&MatrixViewY, WRAngleY);

	D3DXMatrixMultiply(&MatrixViewLookXY,&MatrixViewX,&MatrixViewY);
	D3DXMatrixMultiply(&MatrixView,&MatrixView,&MatrixViewLookXY);
	//D3DXMatrixMultiply(&MatrixView,&MatrixView,&MatrixViewLook);
}
void ChangeMeshMode()
{
	if ( (tTime-LastChangeMeshMode)>(NextChangeMeshMode) )
	{
		LastChangeMeshMode = tTime-1;
		iMeshModeIndex++;
		if (iMeshModeIndex>3) iMeshModeIndex = 0;

		switch(iMeshModeIndex)
		{
			case 0: //NORM
				_D3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
				_D3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				_D3DDevice->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
				NextChangeMeshMode = 40000;
				break;
			case 1: //MESH
				_D3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
				_D3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
				_D3DDevice->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_ONE );	
				NextChangeMeshMode = 20000;
				break;
			case 2: //NORM
				_D3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
				_D3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				_D3DDevice->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
				NextChangeMeshMode = 40000;
				break;
			case 3: //NEGA
				_D3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
				_D3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR );
				_D3DDevice->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_INVSRCCOLOR );
				NextChangeMeshMode = 20000;
				break;
		}
	}
}
void ClearDevice(bool bDeleteInput)
{
	if (bDeleteInput == true) DeleteInput();
	if (wp_onproc)
	{
		if (Wp_Text != NULL ) Wp_Text->Release();
		if (Wp_indx != NULL ) Wp_indx->Release();
		if (Wp_Vert != NULL ) Wp_Vert->Release();
		wp_onproc =false;
	}
	if (o10_ShowFPS) if (FPSFont != NULL) FPSFont->Release();
	if (o11_ShowCaptions) if (CaptionFont != NULL) CaptionFont->Release();

	if (_SceneMaterials != NULL) delete[] _SceneMaterials;	
	if (_SceneTexture)
	{
		using namespace std;
		typedef vector<LPDIRECT3DTEXTURE9> TTextRefVector;
		TTextRefVector TextureRef;
		for (DWORD i=0; i<dwNumber; i++)
			{
				if (_SceneTexture[i])
					{
						vector<LPDIRECT3DTEXTURE9>::iterator iFind = std::find( TextureRef.begin(), TextureRef.end(), _SceneTexture[i]);
						if ( iFind == TextureRef.end()  )
						{
							TextureRef.push_back(_SceneTexture[i] );
							_SceneTexture[i]->Release();								
						}
					}
			}
		delete[] _SceneTexture;
		TextureRef.clear();
	}
	if (_Scene != NULL) _Scene->Release();

	if (_D3DDevice != NULL) _D3DDevice->Release();
	if (_D3D != NULL) _D3D->Release();
}
//****************************************
// WINDOWS FUNCTIONS
//****************************************
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
	case WM_QUERYENDSESSION:
		{
			//DESKTOP_SWITCHDESKTOP
			CleanOutPut();
			ExitProcess(NULL);
			return TRUE;//TRUE - YES FALSE - NO
		}
	case WM_ENDSESSION:
		{
			CleanOutPut();
			ExitProcess(NULL);
			return TRUE;
		}
	case WM_CLOSE:
		{
			CleanOutPut();
			ExitProcess(NULL);
			return TRUE;
		}
	case WM_WTSSESSION_CHANGE:
		{
			//SESSION_QUERY_ACCESS
			if ( (wParam == WTS_SESSION_LOCK) || (wParam == WTS_CONSOLE_DISCONNECT) )
				if (bShutDown == false) ShutDownON();
			//if (wParam == WTS_SESSION_LOGOFF)
			//	PostQuitMessage(0);
			return 0;
		}
			//
	case WM_POWERBROADCAST:
		{
			if (!bCleanOutPutOnProc)
			{
				CleanOutPut();
				ExitProcess(NULL);
			}
			//PostQuitMessage(0);
			return TRUE;
		}	

	case WM_SCRSAVMSG:
			{
				if (bScrSavWrk == false) ScreenSaverStartProcOn(false);				
				return 0;
			}
		case WM_MOUSEMOVE:
			{
				if (bScrSavWrk == true)
				{
					POINT SSCurentCursor;
					GetCursorPos(&SSCurentCursor);
					if ( ((SSCurentCursor.x - ScrSavStartCoord.x)>3) || ((ScrSavStartCoord.x - SSCurentCursor.x)>3) || ((SSCurentCursor.y - ScrSavStartCoord.y)>3) || ((ScrSavStartCoord.y - SSCurentCursor.y)>3) )
						ScreenSaverProcOff();
				}
				return 0;
			}
		case WM_LBUTTONDOWN: case WM_MBUTTONDOWN: case WM_RBUTTONDOWN: case WM_KEYDOWN:
			{
				if (bScrSavWrk == true)
					ScreenSaverProcOff();					
				return 0;
			}
		case WM_MUTEXMSG:
			{
				if (bShutDownProcesd != true)
				if (bShutDown == true) ShutDownOFF();
				return 0;
			}
		case WM_DISPLAYCHANGE:
			{
				if ((bShutDown == false)&&(bShutDownProcesd == false)&&(bScrSavWrk == false)) ResetDevice(hD);
				return 0;
			}
		case WM_SETTINGCHANGE:
		if ((bShutDown != true)&&(bShutDownProcesd != true))
		{
			WIN32_FIND_DATA fd;

			SystemParametersInfo(SPI_GETDESKWALLPAPER, 256, &_DPbuffer , 0 );
			DefDeskColor = GetSysColor(1);

			HANDLE hFind=FindFirstFile(_DPbuffer, &fd);
			GetFileTime(hFind, NULL, NULL, &WPLastWriteTime);

			//DATE != or !last color begin				OR			if(*(__int64*) & WriteTime1 == *(__int64*) & WriteTime2)
			if ((*(__int64*) & fd.ftLastWriteTime != *(__int64*) & WPLastWriteTime) || (DefDeskColor != WPLastDesktColor))
			{
				if (bMyWPchanged == true) 
				{
					bMyWPchanged = false;
					WPLastWriteTime = fd.ftLastWriteTime;
					WPLastDesktColor = DefDeskColor;
				}
				else
				{					
					if (wp_onproc == false) DropWallpaper();
						Render3D();

					bMyWPchanged = true;

					SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, L"", SPIF_SENDCHANGE | SPIF_UPDATEINIFILE );					
					int aiElements[1] = {COLOR_BACKGROUND};
					DWORD aColors[1] = {TpColor};
					SetSysColors(1, aiElements, aColors);
					RemoveWpStyleInReg();

				}
				return 0;
			}
			else	bMyWPchanged = false;
		return 0;
		}

		case WM_NOTIFYICON_MSG:
			switch((UINT)lParam)
			{			
			case NIN_BALLOONUSERCLICK:
				{
					ShellExecute(NULL, L"open",  IconData.szInfo, NULL, NULL, SW_SHOWNORMAL);
					break;
				}

			case WM_RBUTTONUP:
				{
					//if (bFirstStart == true) break; not good practic
				POINT Point;
				GetCursorPos(&Point);
				SetForegroundWindow(hWnd);
				TrackPopupMenuEx(PopupMenu, TPM_CENTERALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON , Point.x, Point.y, hWnd, NULL);//TPM_LAYOUTRTL
				break;
				}

			case WM_MBUTTONUP:
				{
					if (ChCamPosEnable)
					{
						if (!bCamOnProc) cLastPosChTime = tTime - (60000*ChCamPos);
					}
					else
						if (!bCamOnProc)
						{
							bOneClickMove	= true;
							ChCamPos = 1;
							cLastPosChTime = tTime - (60000*ChCamPos);
						}
				break;
				}

			case WM_LBUTTONDBLCLK:
				{
					if (bShutDownProcesd != true)
					if (bShutDown == true) ShutDownOFF();else ShutDownON();
				return 0;
				}
			}
		case WM_COMMAND:
			{
//
				if ( wParam == WM_SHOWACTIV )
				{
					if ((bShutDownProcesd == false) && (bShutDown == false) && (bFirstStart == true))
					{
						bFirstStart = false;
						gCamMoveTime = 30000;
						gCamCapShowTime = 23000;
						ChCamPos = 2;
						ShutDownON();
					}
					//ShowForm
					return 0;
				}
//
				if ( wParam == WM_VISITSITE )
				{
					fVisitSite();
					return 0;
				}
				if ( wParam == WM_OPENFOL )
				{
					MENUITEMINFO mii = {NULL};
					mii.cbSize = sizeof (MENUITEMINFO);
					mii.fMask = MIIM_DATA;
					GetMenuItemInfo(hmFirstPopupM, wParam, FALSE, &mii);

					ShellExecute(NULL, L"open", (LPWSTR) mii.dwItemData, NULL, NULL, SW_SHOWNORMAL);
					return 0;
				}

				if ( wParam == WM_MAKESCR )
				{
					MakeScreenshot();
					return 0;
				}
				if ( wParam == WM_OPTWND )
				{
					//DialogBox(ProgHinst, MAKEINTRESOURCE(IDD_OPTIONS_0), NULL, MainDlgWndProc);
					if (bDlgShowed==false) CreateDialog(ProgHinst, MAKEINTRESOURCE(IDD_OPTIONS_0), NULL, MainDlgWndProc);
					return 0;
				}
				if (( wParam > 1010 )&&( wParam < 2034 ))
				{
					MENUITEMINFO mii = {NULL};
					mii.cbSize = sizeof (MENUITEMINFO);
					mii.fMask = MIIM_DATA;
					GetMenuItemInfo(PopupMenu, wParam, FALSE, &mii);
					
					if ( (lstrcmpi(FirstAndCurentScreenPath, (LPWSTR) mii.dwItemData) != 0 ) || (bShutDown == true)) ChangeWallPaper( (LPWSTR) mii.dwItemData );
					CurentWPIndex = wParam - 1010;
					return 0;
				}
				 if ( wParam == WM_SHUTDWN )
				 {
					 if (bShutDownProcesd != true)
					 if (bShutDown == true) ShutDownOFF();
					 else ShutDownON();

					return 0;
				 }
				 if ( wParam == WM_EXITPRG )
				 {
					 PostQuitMessage(0);
					 return 0;
				 }
				 return 0;
			}
			return 0;
		}
    return DefWindowProc( hWnd, msg, wParam, lParam );
}
void CheckMutex()
{
	Pmutex = CreateMutex(NULL, FALSE, L"Full3DWallpaperMEx");
	//DWORD result;
	//result = WaitForSingleObject(Pmutex,0);
	//if (result == WAIT_OBJECT_0)
	if (GetLastError() == ERROR_ALREADY_EXISTS) //IF ALLRADY RUNING
	{
		SendMessage(FindWindow(L"WC_Full3DWallpaper", L"Full3DWallpaper"), WM_MUTEXMSG, (WPARAM)NULL, (LPARAM)NULL);
		ReleaseMutex(Pmutex);
		CloseHandle(Pmutex);
		ExitProcess(NULL);
	}
	else //IF NOT RUNING
	{
		
	}
	
}
void DisableAero(bool EorD)
{

	typedef  HRESULT  (__stdcall *lpfnDwmIsCompositionEnabled)(BOOL *state);
	typedef  HRESULT  (__stdcall *lpfnDwmEnableComposition)(UINT setstate);	

    lpfnDwmIsCompositionEnabled     lpfnIsEnabled = 0;
    lpfnDwmEnableComposition        lpfnSetEnable = 0;

    HMODULE library = ::LoadLibrary(L"dwmapi.dll");
    bool result = 0;

    if (0 != library)
    {
        lpfnIsEnabled =(lpfnDwmIsCompositionEnabled)GetProcAddress(library,"DwmIsCompositionEnabled");
        lpfnSetEnable =(lpfnDwmEnableComposition)GetProcAddress(library,"DwmEnableComposition");

		BOOL enabled = FALSE;
		if ((lpfnIsEnabled != 0)&&(SUCCEEDED(lpfnIsEnabled(&enabled)))&&(EorD)&&enabled)
		{
			lpfnSetEnable(FALSE);
		}

		if ((lpfnIsEnabled != 0)&&(SUCCEEDED(lpfnIsEnabled(&enabled)))&&(EorD == false)&&!enabled)
		{
			lpfnSetEnable(TRUE);
		}

		FreeLibrary(library);
    }
}
void CreateMainWindow()
{
	WNDCLASSEX mwwc =
    {
        sizeof( WNDCLASSEX ), CS_HREDRAW | CS_VREDRAW, MsgProc, 0L, 0L,
        ProgHinst, NULL, NULL, NULL, NULL,
        L"WC_Full3DWallpaper", NULL
    };
	mwwc.hIcon = LoadIcon(ProgHinst, MAKEINTRESOURCE(IDI_ICO_MAIN));
	RegisterClassEx( &mwwc );
	RECT	d_rc;
	GetWindowRect(hD,&d_rc);
	hMain = CreateWindow( L"WC_Full3DWallpaper", L"Full3DWallpaper",
						  WS_DLGFRAME, 0, 0, d_rc.right, d_rc.bottom,
                          NULL, NULL, mwwc.hInstance, NULL );
	ShowWindow(hMain, SW_HIDE);
}
void AddTrayIcon()
{
	memset(&IconData, 0, sizeof(IconData));
	IconData.cbSize = NOTIFYICONDATA_V3_SIZE;
	IconData.hWnd = hMain;
	IconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP ;
	IconData.uCallbackMessage = WM_NOTIFYICON_MSG;
	IconData.uID = 34;
	lstrcpyn(IconData.szTip, L"Full 3D Wallpaper v.0.9", sizeof(IconData.szTip)/sizeof(IconData.szTip[0]));
	if (bAutoRunZ)	IconData.hIcon = LoadIcon(ProgHinst, MAKEINTRESOURCE(IDI_ICO_TRAY_Z));
	else			IconData.hIcon = LoadIcon(ProgHinst, MAKEINTRESOURCE(IDI_ICO_TRAY));
	
	Shell_NotifyIcon(NIM_ADD,&IconData);
}
void CreateAndLoadPopupMenu()
{
PopupMenu = CreatePopupMenu();

//HBITMAP bmVisitSite = LoadBitmap( ProgHinst, MAKEINTRESOURCE( IDB_VISITSITE ));
//InsertMenu(PopupMenu, -1, MF_BYPOSITION | MF_BITMAP , WM_VISITSITE , (LPCWSTR) bmVisitSite);
InsertMenu(PopupMenu, -1, MF_BYCOMMAND | MF_STRING , WM_VISITSITE, L"Visit Full3DWallpaper.com");
InsertMenu(PopupMenu, -1, MF_BYPOSITION | MF_SEPARATOR, NULL, NULL);

if (bAutoRunZ)	InsertMenu(PopupMenu, -1, MF_BYPOSITION | MF_STRING | MF_DISABLED , WM_MAKESCR, L"Make screenshot");
else			InsertMenu(PopupMenu, -1, MF_BYPOSITION | MF_STRING , WM_MAKESCR, L"Make screenshot");

InsertMenu(PopupMenu, -1, MF_BYPOSITION | MF_STRING , WM_OPTWND, L"Options...");
	LPWSTR szPath = (LPWSTR ) LocalAlloc(LPTR,256);
	GetCurrentDirectory(256,szPath);
	StringCbPrintf( szPath, 256 , L"%s\\Wallpapers\\",szPath);
	int MIid = 1010;
	//InsertMenu(PopupMenu, -1, MF_BYPOSITION | MF_POPUP | MF_STRING, (UINT_PTR) LoadMenuFromPath( szPath, &MIid ) , L"Wallpapers");
				MENUITEMINFO miiv = {NULL};
				miiv.cbSize		= sizeof(MENUITEMINFO);
				miiv.fMask		= MIIM_STRING | MIIM_ID | MIIM_FTYPE | MIIM_DATA | MIIM_SUBMENU;
				miiv.fType		= MFT_STRING;
				miiv.wID		= WM_WPMENU;
				miiv.dwTypeData	= L"Wallpapers";
				miiv.hSubMenu	= LoadMenuFromPath( szPath, &MIid );
				InsertMenuItem( PopupMenu, -1 , FALSE , &miiv);

				MENUITEMINFO mii = {NULL};
				mii.cbSize	= sizeof(MENUITEMINFO);
				mii.fMask	= MIIM_STRING | MIIM_ID | MIIM_FTYPE | MIIM_DATA | MIIM_STATE;
				mii.fType	= MFT_STRING;
				mii.fState	= MFS_ENABLED | MFS_DEFAULT;
				mii.wID		= WM_OPENFOL;
				mii.dwItemData = (ULONG_PTR) szPath;
				mii.dwTypeData = L"Open folder";
				InsertMenuItem( hmFirstPopupM, -1 , FALSE , &mii);
	//InsertMenu( hmFirstPopupM, -1, MF_BYCOMMAND | MF_STRING , WM_WPMENU, L"Open folder");	
InsertMenu(PopupMenu, -1, MF_SEPARATOR , NULL, NULL);
if (bShutDown == TRUE)	InsertMenu(PopupMenu, -1, MF_BYPOSITION | MF_STRING | MF_CHECKED, WM_SHUTDWN, L"Shutdown");
else					InsertMenu(PopupMenu, -1, MF_BYPOSITION | MF_STRING | MF_UNCHECKED, WM_SHUTDWN, L"Shutdown");
InsertMenu(PopupMenu, -1, MF_BYPOSITION | MF_STRING, WM_EXITPRG, L"Exit");

TotalWallpapers = MIid-1010;
}
HMENU LoadMenuFromPath( LPWSTR CurPath, int *MIid)
{
	WIN32_FIND_DATA fd;
	HMENU TempMenu = CreatePopupMenu();
	if (bFirstPopupM == false)
		{
			hmFirstPopupM = TempMenu;
			bFirstPopupM = true;
		}
	
	LPWSTR SerPathStr = (LPWSTR ) LocalAlloc(LPTR,256);
	LPWSTR tmpStr     = (LPWSTR ) LocalAlloc(LPTR,256);
	
	StringCbPrintf((LPWSTR) SerPathStr, 256 ,L"%s%s", CurPath, L"*.*");
	
    HANDLE hFind=FindFirstFile(SerPathStr, &fd);
    if(hFind != INVALID_HANDLE_VALUE)
    {
        do
		{
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)&&( lstrcmp(fd.cFileName,L".") != 0  )&&(lstrcmp(fd.cFileName,L"..") != 0))
			{
				StringCbPrintf((LPWSTR) tmpStr, 256 ,L"%s%s\\", CurPath, fd.cFileName);
				InsertMenu(TempMenu, -1, MF_BYPOSITION | MF_POPUP | MF_STRING,(UINT_PTR) LoadMenuFromPath( tmpStr, &*MIid), fd.cFileName);
			}
			if ((fd.dwFileAttributes & 32)&&( lstrcmp(fd.cFileName,L".") != 0  )&&(lstrcmp(fd.cFileName,L"..") != 0)&&(  wcsstr( fd.cFileName, L".f3dw") != NULL  ))		//strchr(fd.cFileName, L"3ddwp")
			{
				*MIid = *MIid + 1;
				//StringCbPrintf((LPWSTR) tmpStr, 256 ,L"%s%s", CurPath, fd.cFileName);

				if (*MIid == 1011)
				{
					FirstAndCurentScreenPath = (LPWSTR ) LocalAlloc( LPTR, 256);
					StringCbPrintf((LPWSTR) FirstAndCurentScreenPath, 256 ,L"%s%s", CurPath, fd.cFileName);
				}

				MENUITEMINFO mii = {NULL};
				mii.cbSize = sizeof(MENUITEMINFO);
				mii.fMask = MIIM_STRING | MIIM_ID | MIIM_FTYPE | MIIM_DATA | MIIM_BITMAP;
				mii.fType = MFT_STRING;
				//mii.cch = wcslen((LPCTSTR)fd.cFileName) * sizeof(TCHAR);
				mii.cch = wcslen(fd.cFileName) * sizeof(WCHAR);	
				mii.dwTypeData = (LPWSTR) LocalAlloc( LMEM_ZEROINIT, mii.cch);
				wcsncpy(mii.dwTypeData, fd.cFileName, wcslen(fd.cFileName)-5);

				mii.wID = *MIid;

				//mii.dwItemData = (ULONG_PTR) tmpStr;
				mii.dwItemData = (ULONG_PTR) LocalAlloc( LMEM_ZEROINIT,256);
				//mii.dwItemData = (ULONG_PTR) tmpStr;
				StringCbPrintf((LPWSTR) mii.dwItemData, 256 ,L"%s%s", CurPath, fd.cFileName);

				mii.hbmpItem = LoadBitmap( ProgHinst, MAKEINTRESOURCE( IDB_MENUITEM ));
				InsertMenuItem(TempMenu, -1, FALSE, &mii);
			}
        }
		while(FindNextFile(hFind, &fd));
        FindClose(hFind);
    }
	return TempMenu;
}
LPWSTR INIRead(LPWSTR FilePath, LPWSTR section, LPWSTR key, LPWSTR Def)
{
	LPWSTR out = (LPWSTR) LocalAlloc(LPTR, 256);
	GetPrivateProfileString((LPCWSTR) section, (LPCWSTR) key, (LPCWSTR) Def, out, 256, (LPCWSTR) FilePath); 
	return out;
}
LPWSTR ScanStrFormat( LPWSTR ScanedString )
{
	LPWSTR  str_temp_result = (LPWSTR) LocalAlloc(LPTR,256);

	StringCbPrintf((LPWSTR) str_temp_result, 256 ,L"%s", ScanedString);

	for ( int i=0; i<lstrlen(str_temp_result); i++ )
	{
		if (str_temp_result[i] == '#' )
			if (str_temp_result[i+1] == 'n' )
			{
				str_temp_result[i] = '\n';
				str_temp_result[i+1] = ' ';
			}
	}
	return str_temp_result;
}
bool INIWrite(LPWSTR filename, LPWSTR section, LPWSTR key, LPWSTR data)
{
	LPWSTR tmp = (LPWSTR ) LocalAlloc(LPTR,256);
	GetCurrentDirectory(256,tmp);
	StringCbPrintf((LPWSTR) tmp, 256 ,L"%s\\%s", tmp , filename);

	return WritePrivateProfileString( (LPCWSTR) section, (LPCWSTR) key, (LPCWSTR) data, (LPCWSTR) tmp);
}
void SetLayared()
{
	DefDeskColor = GetSysColor(1);
	WPLastDesktColor = DefDeskColor;
	int aiElements[1] = {COLOR_BACKGROUND};
	DWORD aColors[1] = {TpColor};
	SetSysColors(1, aiElements, aColors);

	SetWindowLong(hP,GWL_EXSTYLE, GetWindowLong(hP,GWL_EXSTYLE) | WS_EX_LAYERED );
	SetLayeredWindowAttributes(hP,TpColor,220, LWA_COLORKEY | LWA_ALPHA);

	SystemParametersInfo(SPI_GETDESKWALLPAPER, 256, &_DPbuffer , 0 );
	
	WIN32_FIND_DATA fd;
	HANDLE hFind=FindFirstFile(_DPbuffer, &fd);
	GetFileTime(hFind, NULL, NULL, &WPLastWriteTime);
	//WPLastWriteTime = fd.ftLastWriteTime;	

	if (wp_onproc == false) DropWallpaper();
	//Render3D();//Render3D();
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, L"", SPIF_SENDCHANGE | SPIF_UPDATEINIFILE );
	RemoveWpStyleInReg();
	RedrawWindow(hD, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);//F5
}
void FatalError(LPTSTR lpszFunction) 
{ 
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );

	int io = lstrlen((LPCTSTR) L"error code: ") + 5;
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction)  + io ) * sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s:\r\nerror code: %d\r\n%s%s"), lpszFunction, dw, lpMsgBuf); 
	
	MessageBox (NULL, (LPCTSTR)lpDisplayBuf, TEXT("Notification"), MB_OK | MB_ICONSTOP );
	
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}
void ChangeWalpaperByTime()
{
	if ( (tTime - LastChangeTime)>(AvtoChWP*60000) )//20000))//
	{
		CurentWPIndex++;
		if (CurentWPIndex>TotalWallpapers) CurentWPIndex = 1;
		//SendMessage( hMain, WM_COMMAND, (WPARAM) (1010+CurentWPIndex), (LPARAM) NULL);
						MENUITEMINFO mii = {NULL};
						mii.cbSize = sizeof (MENUITEMINFO);
						mii.fMask = MIIM_DATA;
						GetMenuItemInfo(PopupMenu, (UINT) (1010+CurentWPIndex), FALSE, &mii);//UINT

						if ( (lstrcmpi(FirstAndCurentScreenPath, (LPWSTR) mii.dwItemData) != 0 ) || (bShutDown == true)) ChangeWallPaper( (LPWSTR) mii.dwItemData );
						//CurentWPIndex = wParam - 1010;

		//////

		LastChangeTime = tTime-1;
	}
}
void RemoveWpStyleInReg()
{
	//TileWallpaper		//WallpaperStyle	//if Vista-7 delete slide show
	HKEY hKey;

	RegOpenKeyEx( HKEY_CURRENT_USER, L"Control Panel\\Desktop\\", 0, KEY_ALL_ACCESS, &hKey);
	RegSetValueEx( hKey, L"TileWallpaper", NULL, REG_SZ, (BYTE*) L"0", sizeof(WCHAR));

	RegSetValueEx( hKey, L"WallpaperStyle", NULL, REG_SZ, (BYTE*) L"2", sizeof(WCHAR));//wcslen(L"2")*sizeof(WCHAR)

	if (bWinVer7XP == 2)
	{
		DWORD cht = 0x5265c00;
		if (RegOpenKeyEx( HKEY_CURRENT_USER, L"Control Panel\\Personalization\\Desktop Slideshow", 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
			RegSetValueEx( hKey, L"Interval", NULL, REG_DWORD, (BYTE*) &cht, sizeof(DWORD));
	}
	RegCloseKey(hKey);
}
void CleanOutPut()
{	
	bCleanOutPutOnProc = true;
	WTSUnRegisterSessionNotification(hMain);
	UnhookWinEvent(UAC_hook);

	ReleaseMutex(Pmutex);
	CloseHandle(Pmutex);
	SaveOptToReg();

if (bShutDown == false)
	{
	SetWallpaper();
	SetWindowLong(hP,GWL_EXSTYLE, GetWindowLong(hP,GWL_EXSTYLE) &~ WS_EX_LAYERED );

	ClearDevice(true);

	Shell_NotifyIcon(NIM_DELETE, &IconData);

	int aiElements[1] = {COLOR_BACKGROUND};
	DWORD aColors[1] = {DefDeskColor};
	SetSysColors(1, aiElements, aColors);
	}
		DestroyMenu(PopupMenu);
		Shell_NotifyIcon(NIM_DELETE, &IconData);
		DestroyWindow(hMain);
		RedrawWindow(hD, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);//F5
}
//****************************************
// OTHER FUNCTIONS
//****************************************
LPWSTR PathToTexture(LPWSTR PathToFile, LPCSTR TextName)
{
	if (TextName == 0) return NULL;
	LPWSTR  f_result = (LPWSTR) LocalAlloc(LPTR,256);
	LPWSTR temp_oper = (LPWSTR) LocalAlloc(LPTR,256);

	StringCbPrintf((LPWSTR) temp_oper, 256 ,L"%s", PathToFile);

	int n = lstrlen(temp_oper);
	while ( temp_oper[n] != '\\' )
	{
		n--;
	}	
	temp_oper[n] = 0;
	StringCbPrintf( f_result, 256 , L"%s\\%s", temp_oper, AnsiToUnicode(TextName));
	return f_result;
}
LPWSTR AnsiToUnicode( LPCSTR lpAnsiStr )
{
    LPWSTR lpUnicodeStr =NULL;
    int iLen;
    if( ( iLen =MultiByteToWideChar( CP_ACP, 0, lpAnsiStr, -1, NULL, 0 ) ) != 0 )
        if( ( lpUnicodeStr =(LPWSTR)malloc( iLen*sizeof(WCHAR) ) ) !=NULL )
            if( MultiByteToWideChar( CP_ACP, 0, lpAnsiStr, -1, lpUnicodeStr, iLen ) == 0 )
            {
                free(lpUnicodeStr);
                lpUnicodeStr =NULL;
            }
 
    return lpUnicodeStr;
}
void CheckForegroundWindow()
{
	iFGW++;
	if (iFGW > 50) //= 50 - One time in sec if 50fps
	{
		if (o3_ShutdownInUnprod) AutoSleepCheck();
		EnumWindows(&EnumWindowsProc, NULL);
		if ((EnumMaxWW>0)&&(EnumMaxWH>0)) 
		{
			if (bSleepMode == false)
			{
				IconData.hIcon = LoadIcon( ProgHinst, MAKEINTRESOURCE( IDI_ICO_TRAY_S));
				Shell_NotifyIcon(NIM_MODIFY,&IconData);
			}
			bSleepMode = true;
			Sleep(1000);

			iTotalSleepCounts++;
			if (iTotalSleepCounts > 180)
			{
				iTotalSleepCounts = 0;
				if (bShutDownProcesd != true) ShutDownON();
			}
		}
		else 
		{
			iFGW = 0;
			if (bSleepMode == true)
			{
				IconData.hIcon = LoadIcon( ProgHinst, MAKEINTRESOURCE( IDI_ICO_TRAY));
				Shell_NotifyIcon(NIM_MODIFY,&IconData);
			}
			bSleepMode = false;
		}
		EnumMaxWW = 0; EnumMaxWH = 0;
	}
}
void AutoSleepCheck()
{
	if ( (gMouse.x != AutoSleepCursorPos.x) || (gMouse.y != AutoSleepCursorPos.y) )
	{
		AutoSleepCursorPos = gMouse;
		AutoSleepTime = tTime;
	}
	else
	{
		if ( (tTime-AutoSleepTime) > 180000)  if (bShutDownProcesd != true) ShutDownON();
	}
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
		if (IsWindowVisible(hwnd))
		{
			LONG hLong   = GetWindowLong( hwnd, GWL_STYLE);
			LONG hLongEx = GetWindowLong( hwnd, GWL_EXSTYLE);
			if (!(hLongEx & WS_EX_TOOLWINDOW)&&!(hLong & WS_MINIMIZE)&&(GetWindow(hwnd, GW_OWNER)== NULL))//3?
			{			
				RECT	r_fgw;
				GetWindowRect( hwnd, &r_fgw);
				if (r_fgw.left	< 0)		r_fgw.left	= 0;	//	x1
				if (r_fgw.right	> WinW)		r_fgw.right	= WinW;	//	x2
				if (r_fgw.top	< 0)		r_fgw.top	= 0;	//	y1
				if (r_fgw.bottom> WinH)		r_fgw.bottom= WinH;	//	y2

				
				if ((r_fgw.right-r_fgw.left)>(WinW-(WinW/10)))
				if ((r_fgw.right-r_fgw.left)>EnumMaxWW)
				if ((r_fgw.bottom-r_fgw.top)>(WinH-(WinH/10)))
				if ((r_fgw.bottom-r_fgw.top)>EnumMaxWH) {EnumMaxWW = (r_fgw.right-r_fgw.left); EnumMaxWH = (r_fgw.bottom-r_fgw.top);}
			}
		}
	return TRUE;
}
byte GetWinVer()
{
	OSVERSIONINFOEX osvi;
	BOOL result;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (GetVersionEx((OSVERSIONINFO*) &osvi) == NULL) return 0;
	if (osvi.dwMajorVersion <= 5) return 1;
	else return 2;
}
BOOL EnableDebugPrivilege( BOOL fEnable ) 
{ 
   HANDLE hToken = NULL; 
   BOOL   fOk    = FALSE; 

    TOKEN_PRIVILEGES tp = {0}; 

   if( OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken ) )
   { 
       tp.PrivilegeCount = 1; 
       LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid ); 
       tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED : 0; 
       AdjustTokenPrivileges( hToken, FALSE, &tp, sizeof(tp), NULL, NULL ); 
       fOk = ( GetLastError() == ERROR_SUCCESS ); 
       CloseHandle( hToken ); 
   } 

   return fOk; 
}
void ReCreateCaptionFont()
{
	CaptionRect.left	= WinW * 0.1777;
	CaptionRect.top		= WinH * 0.773;
	CaptionRect.right	= WinW - CaptionRect.left;
	CaptionRect.bottom	= WinH * 0.944;

	if (o11_ShowCaptions)
	{
		if (D3DXCreateFont(_D3DDevice, fSize,0,FW_BOLD,0,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE, fFONTstr,&CaptionFont) != S_OK) o11_ShowCaptions = false;
		//D3DXCreateFont(_D3DDevice, WinW * 0.05,0,FW_BOLD,0,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE, fFONTstr,&CaptionFont);
	}
}
void CalcSphereCoord()
{
	_R  = sqrt( vCam.x*vCam.x + vCam.y*vCam.y + vCam.z*vCam.z );
	_Lb = atan2(vCam.y, sqrt(vCam.x*vCam.x+vCam.z*vCam.z));
	if (vCam.z<0)
		_La = atan2(vCam.z,vCam.x)+2*D3DX_PI;
	else 
		_La = atan2(vCam.z,vCam.x);
}
void fVisitSite()
{
	if (lstrcmpi(GlLang, L"EN") == 0) ShellExecute(NULL, L"open", L"http://full3dwallpaper.com/", NULL, NULL, SW_SHOWNORMAL);
	if (lstrcmpi(GlLang, L"FR") == 0) ShellExecute(NULL, L"open", L"http://full3dwallpaper.com/?fra.html", NULL, NULL, SW_SHOWNORMAL);
	if (lstrcmpi(GlLang, L"DE") == 0) ShellExecute(NULL, L"open", L"http://full3dwallpaper.com/?deu.html", NULL, NULL, SW_SHOWNORMAL);
	if (lstrcmpi(GlLang, L"ES") == 0) ShellExecute(NULL, L"open", L"http://full3dwallpaper.com/?spa.html", NULL, NULL, SW_SHOWNORMAL);
	if (lstrcmpi(GlLang, L"PT") == 0) ShellExecute(NULL, L"open", L"http://full3dwallpaper.com/?por.html", NULL, NULL, SW_SHOWNORMAL);
	if (lstrcmpi(GlLang, L"RU") == 0) ShellExecute(NULL, L"open", L"http://full3dwallpaper.com/?rus.html", NULL, NULL, SW_SHOWNORMAL);
	if (lstrcmpi(GlLang, L"CN") == 0) ShellExecute(NULL, L"open", L"http://full3dwallpaper.com/?zho.html", NULL, NULL, SW_SHOWNORMAL);
	if (lstrcmpi(GlLang, L"IN") == 0) ShellExecute(NULL, L"open", L"http://full3dwallpaper.com/?ind.html", NULL, NULL, SW_SHOWNORMAL);
	if (lstrcmpi(GlLang, L"JP") == 0) ShellExecute(NULL, L"open", L"http://full3dwallpaper.com/?jpn.html", NULL, NULL, SW_SHOWNORMAL);
	
}
VOID CALLBACK WinEventCallback(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	switch( event )
	{
		case EVENT_SYSTEM_DESKTOPSWITCH:
			Sleep(1700);
			if ((!bShutDown)&&(!bResetdivaceOnProc)&&(!bScrSavWrk))					
					ResetDevice(hD);
		break;
	}
}
//****************************************
// DIALOG + OPTICONS WINDOWS
//****************************************
INT_PTR CALLBACK MainDlgWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		ApplyLanguage( true, hDlg, -1);
		CheckDlgForm(hDlg);
		RECT	w_rc;	GetWindowRect(hDlg,&w_rc);		
		RECT	d_rc;	GetWindowRect(GetDesktopWindow(),&d_rc);

		SetWindowPos( hDlg, HWND_TOP, (d_rc.right/2)-(w_rc.right/2), (d_rc.bottom/2)-(w_rc.bottom/2), NULL, NULL, SWP_NOSIZE);

		bDlgShowed = true;
		SetWindowLong( hDlg, GWL_EXSTYLE, GetWindowLong(hDlg,GWL_EXSTYLE) | WS_EX_LAYERED );
		SetLayeredWindowAttributes( hDlg, 0, 220, LWA_ALPHA);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
	if (LOWORD(wParam) == IDCANCEL || LOWORD(wParam) == IDC_B_CANCEL_40)
		{
			bDlgShowed = false;
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	//CHECK 1	AUTO RUN A
	if (LOWORD(wParam) == IDC_C_2)
	{
		if (IsDlgButtonChecked(hDlg, IDC_C_2))//ADD
		{
			if (AddDelInAutoRun( true, false)){ o1_StartWithWin = true; o2_RunInShutdown = true;
			CheckDlgButton(hDlg, IDC_C_3, BST_CHECKED);
			SetWindowLong(GetDlgItem(hDlg, IDC_C_3), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_C_3),GWL_STYLE) &~ WS_DISABLED);}
			else//IF CAN'T CHANGE
			{CheckDlgButton(hDlg, IDC_C_2, BST_UNCHECKED);}
		}
		else//DEL
		{
			if (AddDelInAutoRun( NULL, true)){ o1_StartWithWin = false; o2_RunInShutdown = false;
			CheckDlgButton(hDlg, IDC_C_3, BST_UNCHECKED);
			SetWindowLong(GetDlgItem(hDlg, IDC_C_3), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_C_3),GWL_STYLE) | WS_DISABLED);}
			else//IF CAN'T CHANGE
			{CheckDlgButton(hDlg, IDC_C_2, BST_CHECKED);}
		}
		InvalidateRect(GetDlgItem(hDlg,	IDC_C_3),NULL,NULL);
		return (INT_PTR)TRUE;
	}
	//CHECK 2	AUTO RUN Z
	if (LOWORD(wParam) == IDC_C_3)
	{
		if (IsDlgButtonChecked(hDlg, IDC_C_3))	
		{
			if ( AddDelInAutoRun(true, false)) o2_RunInShutdown = true;
			else CheckDlgButton(hDlg, IDC_C_3, BST_UNCHECKED);
		}
		else 
		{
			if ( AddDelInAutoRun(false,false)) o2_RunInShutdown = false;
			else CheckDlgButton(hDlg, IDC_C_3, BST_CHECKED);
		}

		return (INT_PTR)TRUE;
	}	
	//CHECK 3	SLOW DOWN
	if (LOWORD(wParam) == IDC_C_4)
	{
		if (IsDlgButtonChecked(hDlg, IDC_C_4)) o3_ShutdownInUnprod = true;
		else o3_ShutdownInUnprod = false;
		return (INT_PTR)TRUE;
	}
	//COMBO LANG 
	if ((LOWORD(wParam) == IDC_COMBO_6) && (HIWORD(wParam)==CBN_SELCHANGE))
		{
			int CurSel;
			CurSel = SendMessage( GetDlgItem(hDlg, IDC_COMBO_6) , CB_GETCURSEL, 0, 0);

			SendMessage( GetDlgItem(hDlg, IDC_COMBO_6 ) , CB_RESETCONTENT, 0, 0);
			SendMessage( GetDlgItem(hDlg, IDC_COMBO_17) , CB_RESETCONTENT, 0, 0);
			SendMessage( GetDlgItem(hDlg, IDC_COMBO_23) , CB_RESETCONTENT, 0, 0);
			SendMessage( GetDlgItem(hDlg, IDC_COMBO_33) , CB_RESETCONTENT, 0, 0);
			ApplyLanguage( true, hDlg, CurSel);
			ApplyLanguage(false, NULL, -1);
			CheckDlgForm (hDlg);
			return (INT_PTR)TRUE;
		}
	//CHECK USE AS SCR SAV	
	if (LOWORD(wParam) == IDC_C_6)
	{
		if (IsDlgButtonChecked(hDlg, IDC_C_6)) 
			{
				if (UseAsScrSav(true))
					o4_UseAsScrSav = true;
				else CheckDlgButton(hDlg, IDC_C_6, BST_UNCHECKED);
			}
		else 
			{
				if (UseAsScrSav(false))
					o4_UseAsScrSav = false;
				else CheckDlgButton(hDlg, IDC_C_6, BST_CHECKED);
			}
		return (INT_PTR)TRUE;
	}
	//COMBO WP CHANGE TIME	//DWORD	AvtoChWP	= 30;	//0-3-10-30-60	30min 1 800 000	
	if (LOWORD(wParam) == IDC_C_16)
	{
		if (IsDlgButtonChecked(hDlg, IDC_C_16))
		{
			SetWindowLong(GetDlgItem(hDlg, IDC_COMBO_17), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_COMBO_17),GWL_STYLE) &~ WS_DISABLED);
			SendMessage  (GetDlgItem(hDlg, IDC_COMBO_17), CB_SETCURSEL, 3, NULL);
			AvtoChWP	= 30;
		}
		else
		{
			SetWindowLong(GetDlgItem(hDlg, IDC_COMBO_17), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_COMBO_17),GWL_STYLE) | WS_DISABLED);
			SendMessage  (GetDlgItem(hDlg, IDC_COMBO_17), CB_SETCURSEL, 0, NULL);
			AvtoChWP	= 0;
		}
		InvalidateRect(GetDlgItem(hDlg,	IDC_COMBO_17),NULL,NULL);
		return (INT_PTR)TRUE;
	}
	if ((LOWORD(wParam) == IDC_COMBO_17) && (HIWORD(wParam)==CBN_SELCHANGE))
	{
		int CurSel;
			CurSel = SendMessage( GetDlgItem(hDlg, IDC_COMBO_17) , CB_GETCURSEL, 0, 0);
		if (CurSel == 0)
		{
			AvtoChWP	= 0;
			CheckDlgButton(hDlg, IDC_C_16, BST_UNCHECKED);
			SetFocus(GetDlgItem(hDlg, IDC_C_16));
			SetWindowLong (GetDlgItem(hDlg, IDC_COMBO_17), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_COMBO_17),GWL_STYLE) | WS_DISABLED);
			InvalidateRect(GetDlgItem(hDlg,	IDC_COMBO_17),NULL,NULL);
		}
		else 
			switch(CurSel)	{case 1: AvtoChWP = 3;break; case 2: AvtoChWP = 10;break; case 3: AvtoChWP = 30;break; case 4: AvtoChWP = 60;break;	}
		return (INT_PTR)TRUE;
	}
	//USE CAPTIONS
	if (LOWORD(wParam) == IDC_C_18)
	{
		if (IsDlgButtonChecked(hDlg, IDC_C_18))
		{
			SetWindowLong(GetDlgItem(hDlg, IDC_B_19), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_B_19),GWL_STYLE) &~ WS_DISABLED);
			o11_ShowCaptions = true;
			if (bShutDown == false) ReCreateCaptionFont();
			//ADD
		}
		else
		{
			SetWindowLong(GetDlgItem(hDlg, IDC_B_19), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_B_19),GWL_STYLE) | WS_DISABLED);

			if ((o11_ShowCaptions)&&(bShutDown == false)) if (CaptionFont != NULL) CaptionFont->Release();//DEL
			o11_ShowCaptions = false;
			ExecFontDialog(hDlg, false);			
			
		}
		InvalidateRect(GetDlgItem(hDlg,	IDC_B_19),NULL,NULL);
		return (INT_PTR)TRUE;
	}
	//FONT BUTTON
	if (LOWORD(wParam) == IDC_B_19)
	{
		ExecFontDialog(hDlg, true);
		return (INT_PTR)TRUE;
	}	
	//CH CAMERA POS		//DWORD	ChCamPos	= 2;	//0-1-2-3-5	1min	60 000
	if (LOWORD(wParam) == IDC_C_22)
	{
		if (IsDlgButtonChecked(hDlg, IDC_C_22))
		{
			SetWindowLong(GetDlgItem(hDlg, IDC_COMBO_23), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_COMBO_23),GWL_STYLE) &~ WS_DISABLED);
			SendMessage  (GetDlgItem(hDlg, IDC_COMBO_23), CB_SETCURSEL, 2, NULL);
			ChCamPos = 2;
			ChCamPosEnable = true;
		}
		else
		{
			SetWindowLong(GetDlgItem(hDlg, IDC_COMBO_23), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_COMBO_23),GWL_STYLE) | WS_DISABLED);
			SendMessage  (GetDlgItem(hDlg, IDC_COMBO_23), CB_SETCURSEL, 0, NULL);
			ChCamPos = 0;
			ChCamPosEnable = false;
			if (bCamOnProc)
			{
				cStopCameraMove();
			}
		}
		InvalidateRect(GetDlgItem(hDlg,	IDC_COMBO_23),NULL,NULL);
		return (INT_PTR)TRUE;
	}
	if ((LOWORD(wParam) == IDC_COMBO_23) && (HIWORD(wParam)==CBN_SELCHANGE))
	{
		int CurSel;
			CurSel = SendMessage( GetDlgItem(hDlg, IDC_COMBO_23) , CB_GETCURSEL, 0, 0);
		if (CurSel == 0)
		{
			ChCamPos = 0;
			ChCamPosEnable = false;
			CheckDlgButton(hDlg, IDC_C_22, BST_UNCHECKED);
			SetFocus(GetDlgItem(hDlg, IDC_C_22));
			SetWindowLong (GetDlgItem(hDlg, IDC_COMBO_23), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_COMBO_23),GWL_STYLE) | WS_DISABLED);
			InvalidateRect(GetDlgItem(hDlg,	IDC_COMBO_23),NULL,NULL);
			if (bCamOnProc)
			{
				bCamOnProc = false;
				CalcSphereCoord();
				if( (koefX1+koefX2+koefY1+koefY2)>0 )
					{ koefX1 = 0; koefX2 = 0; koefY1 = 0; koefY2 = 0;}
			}
		}
		else 
		{
			ChCamPosEnable = true;
			switch(CurSel)	{case 1: ChCamPos = 1;break; case 2: ChCamPos = 2;break; case 3: ChCamPos = 3;break; case 4: ChCamPos = 0;break;	}
		}
		return (INT_PTR)TRUE;
	}
//////BORDER ROTETE
if (LOWORD(wParam) == IDC_C_27)
	{
		if (IsDlgButtonChecked(hDlg, IDC_C_27)) o5_RotateNearBorder	= true;
		else o5_RotateNearBorder = false;
		return (INT_PTR)TRUE;
	}
//////CURSOR ROTETE
if (LOWORD(wParam) == IDC_C_28)
	{
		if (IsDlgButtonChecked(hDlg, IDC_C_28)) o6_RotateWithCursor	= true;
		else o6_RotateWithCursor = false;
		return (INT_PTR)TRUE;
	}
//////CURSOR ROTETE
if (LOWORD(wParam) == IDC_C_29)
	{
		if (IsDlgButtonChecked(hDlg, IDC_C_29)) o7_RotateEyesWhell	= true;//Eyes
		else o7_RotateEyesWhell = false;
		return (INT_PTR)TRUE;
	}
//////WHELL ZOOM
if (LOWORD(wParam) == IDC_C_30)
	{
		if (IsDlgButtonChecked(hDlg, IDC_C_30)) o8_ZoomWhell	= true;
		else o8_ZoomWhell = false;
		return (INT_PTR)TRUE;
	}
////////////////////////
//////USE MESH SOME TIME
if (LOWORD(wParam) == IDC_C_36)
	{
		if (IsDlgButtonChecked(hDlg, IDC_C_36)) o9_UseMeshSomeT	= true;
		else 
		{
			o9_UseMeshSomeT = false;
			if (bShutDown == false)
			{				
				_D3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
				_D3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				_D3DDevice->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
			}
		}
		return (INT_PTR)TRUE;
	}
//////SHOW FPS
if (LOWORD(wParam) == IDC_C_37)
	{
		if (IsDlgButtonChecked(hDlg, IDC_C_37)) 
		{
			if (bShutDown == false)
			if (D3DXCreateFont(_D3DDevice,23,0,FW_BOLD,0,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,L"Arial",&FPSFont) == S_OK)
			o10_ShowFPS	= true;
		}
		else 
		{
			o10_ShowFPS = false;
			if (bShutDown == false)
			if (FPSFont != NULL) FPSFont->Release();
		}
		return (INT_PTR)TRUE;
	}
	
	if ((LOWORD(wParam) == IDC_COMBO_33) && (HIWORD(wParam)==CBN_SELCHANGE))
	{
		int CurSel;
			CurSel = SendMessage( GetDlgItem(hDlg, IDC_COMBO_33) , CB_GETCURSEL, 0, 0);
			if ( CurSel != (GQualityMode-1) )
			{
				switch(CurSel)	
				{
					case 0: GQualityMode = 1;break;
					case 1: GQualityMode = 2;break;
					case 2: GQualityMode = 3;break;
				}
				if (bShutDown == false)
				{
					SetFocus(GetDlgItem(hDlg, IDC_C_32));
					ResetDevice(hD);
				}
			}
			//SetFiltrationMode(UsedFilt);}
		return (INT_PTR)TRUE;
	}
//DEFAULT BUTTON
	if (LOWORD(wParam) == IDC_B_DEFAULT_38)
	{
		SetDefOpt(hDlg);		
		return (INT_PTR)TRUE;
	}
	//FONT'S COMBO'S
	//FONT							//StringCbPrintf((LPWSTR) fFONTstr, 64 ,L"%s", ??);	CB_GETLBTEXTLEN 
	if ((LOWORD(wParam) == IDC_FONT_COMBO) && (HIWORD(wParam)==CBN_SELCHANGE))
	{
		if ((o11_ShowCaptions)&&(bShutDown == false)) if (CaptionFont != NULL) CaptionFont->Release();

		int CurSel;
		CurSel = SendMessage( GetDlgItem(hDlg, IDC_FONT_COMBO) , CB_GETCURSEL, 0, 0);
		
		DWORD lenIN;
		lenIN = SendMessage( GetDlgItem(hDlg, IDC_FONT_COMBO) , CB_GETLBTEXTLEN, CurSel, 0);
		
		wchar_t* szIN;
		szIN = new wchar_t[lenIN+1];
		SendMessage( GetDlgItem(hDlg, IDC_FONT_COMBO) , CB_GETLBTEXT, CurSel, (LPARAM) szIN);

		StringCbPrintf((LPWSTR) fFONTstr, 64 ,L"%s", szIN);
		if ( bShutDown == false ) ReCreateCaptionFont();

		delete []szIN;
		return (INT_PTR)TRUE;
	}
	//COLOR
	if ((LOWORD(wParam) == IDC_FONT_COLOR) && (HIWORD(wParam)==CBN_SELCHANGE))
	{
		int CurSel;
		CurSel = SendMessage( GetDlgItem(hDlg, IDC_FONT_COLOR) , CB_GETCURSEL, 0, 0);
		switch(CurSel)
			{
			case 0: fColor = D3DCOLOR_XRGB( 0x00, 0x00, 0x00 );break;
			case 1: fColor = D3DCOLOR_XRGB( 0x80, 0x00, 0x00 );break;
			case 2: fColor = D3DCOLOR_XRGB( 0x00, 0x80, 0x00 );break;
			case 3: fColor = D3DCOLOR_XRGB( 0x80, 0x80, 0x00 );break;
			case 4: fColor = D3DCOLOR_XRGB( 0x00, 0x00, 0x80 );break;
			case 5: fColor = D3DCOLOR_XRGB( 0x80, 0x00, 0x80 );break;
			case 6: fColor = D3DCOLOR_XRGB( 0x00, 0x80, 0x80 );break;
			case 7: fColor = D3DCOLOR_XRGB( 0x80, 0x80, 0x80 );break;
			case 8: fColor = D3DCOLOR_XRGB( 0xC0, 0xC0, 0xC0 );break;
			case 9: fColor = D3DCOLOR_XRGB( 0xFF, 0x00, 0x00 );break;
			case 10: fColor = D3DCOLOR_XRGB( 0x00, 0xFF, 0x00 );break;
			case 11: fColor = D3DCOLOR_XRGB( 0xFF, 0xFF, 0x00 );break;
			case 12: fColor = D3DCOLOR_XRGB( 0x00, 0x00, 0xFF );break;
			case 13: fColor = D3DCOLOR_XRGB( 0xFF, 0x00, 0xFF );break;
			case 14: fColor = D3DCOLOR_XRGB( 0x00, 0xFF, 0xFF );break;
			case 15: fColor = D3DCOLOR_XRGB( 0xFF, 0xFF, 0xFF );break;
			}		
		return (INT_PTR)TRUE;
	}
	//SIZE
	if ((LOWORD(wParam) == IDC_FONT_SIZE) && (HIWORD(wParam)==CBN_SELCHANGE))
	{
		int CurSel;
		CurSel = SendMessage( GetDlgItem(hDlg, IDC_FONT_SIZE) , CB_GETCURSEL, 0, 0);
		fSize = (10*CurSel)+30;

		if ((o11_ShowCaptions)&&(bShutDown == false)) if (CaptionFont != NULL) 
		{
			CaptionFont->Release();
			ReCreateCaptionFont();
		}
		return (INT_PTR)TRUE;
	}
	}
	return (INT_PTR)FALSE;
}
void LoadOptFromReg()
{
	HKEY hKey;
	DWORD dwDispas;
	
	WCHAR PathToProg[1024] = {0};	DWORD buf_size = sizeof(PathToProg);				
	WCHAR PathToWp[1024] = {0};		DWORD buf_size_wp = sizeof(PathToWp);

	if (RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\Full3DWallpaper\\", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)//KEY_READ????
		RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Full3DWallpaper", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDispas);
	
	if ( (RegQueryValueEx( hKey, L"Path", NULL, NULL, (LPBYTE) PathToProg, &buf_size) != ERROR_SUCCESS) || (!PathFileExists(PathToProg)) )
	{
		LPWSTR CurPathToProg = (LPWSTR ) LocalAlloc(LPTR,256);
		GetCurrentDirectory( 256, CurPathToProg);
		RegSetValueEx( hKey, L"Path", NULL, REG_SZ, (BYTE*) CurPathToProg, wcslen(CurPathToProg)*sizeof(WCHAR));
	}

	if (RegQueryValueEx( hKey, L"PathToLastWp", NULL, NULL, (LPBYTE) PathToWp, &buf_size_wp) != ERROR_SUCCESS)
	{
		if ( !PathFileExists(FirstAndCurentScreenPath) ) {MessageBox( hMain, L"Sorry, You have not wallpapers.", L"Notification", MB_OK); CleanOutPut(); ExitProcess(NULL);}
		else RegSetValueEx( hKey, L"PathToLastWp", NULL, REG_SZ, (BYTE*) FirstAndCurentScreenPath, wcslen(FirstAndCurentScreenPath)*sizeof(WCHAR));//2*wcslen(FirstAndCurentScreenPath));		
	}
	if ( PathFileExists(PathToWp) ) StringCbPrintf((LPWSTR) FirstAndCurentScreenPath, 256 ,L"%s", PathToWp);//FirstAndCurentScreenPath = PathToWp;

	//??????????????????wcslen(FirstAndCurentScreenPath)*sizeof(WCHAR)
	DWORD buf_size_ln = 16*sizeof(WCHAR);//sizeof(GlLang);
	if (RegQueryValueEx( hKey, L"Language", NULL, NULL, (LPBYTE) GlLang, &buf_size_ln) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"Language", NULL, REG_SZ, (BYTE*) L"EN", 2*sizeof(WCHAR)); GlLang = L"EN";}


	//		\\Full3DWallpaper\\OPTIONS
	if (RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\Full3DWallpaper\\Options\\", 0, KEY_ALL_ACCESS, &hKey)	!= ERROR_SUCCESS)
		RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Full3DWallpaper\\Options", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDispas);

	LPWSTR TempOperStr = (LPWSTR ) LocalAlloc(LPTR,64);
	DWORD  Temp_BufSize = 64*sizeof(WCHAR);

	//START IN WINDOWS
		HKEY hKeyAR;
		WCHAR PathToProgRun[1024] = {0};
		DWORD buf_size_pr = sizeof(PathToProgRun);

		RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run\\", 0, KEY_ALL_ACCESS, &hKeyAR);

		if ( (RegQueryValueEx( hKeyAR, L"Full3DWallpaper", NULL, NULL, (LPBYTE) PathToProgRun, &buf_size_pr) != ERROR_SUCCESS) && (!PathFileExists(PathToProgRun)) )
			{o1_StartWithWin = false; o2_RunInShutdown = false;}
		else 
		{
			o1_StartWithWin = true;
			if (PathToProgRun[wcslen(PathToProgRun)-1] == 'z') o2_RunInShutdown = true;
			else o2_RunInShutdown = false;
		}
	
	//USE AS SCR SAV
		buf_size_pr = sizeof(PathToProgRun);
		RegOpenKeyEx( HKEY_CURRENT_USER, L"Control Panel\\Desktop\\", 0, KEY_ALL_ACCESS, &hKeyAR);
		RegQueryValueEx ( hKeyAR, L"SCRNSAVE.EXE", NULL, NULL, (LPBYTE) PathToProgRun, &buf_size_pr);

		LPWSTR WinSysDir  = ( LPWSTR ) LocalAlloc(LPTR,256);
		LPWSTR SysDirScrS = ( LPWSTR ) LocalAlloc(LPTR,256);
		GetSystemDirectory( WinSysDir, 256);
		StringCbPrintf( SysDirScrS, 256 ,L"%s\\FULL3D~1.SCR", WinSysDir);

		if ( lstrcmpi( SysDirScrS, PathToProgRun) == 0 )
			 o4_UseAsScrSav = true;
		else o4_UseAsScrSav = false;

		if (o4_UseAsScrSav == true)
		{
			RegQueryValueEx( hKeyAR, L"ScreenSaveActive", NULL, NULL, (LPBYTE) PathToProgRun, &buf_size_pr);
			if ( lstrcmpi( PathToProgRun, L"1") == 0 )
				 o4_UseAsScrSav = true;
			else o4_UseAsScrSav = false;
		}
		RegCloseKey(hKeyAR);

	//SHUTDOWN IN UNPRODUCTIVE
	if (RegQueryValueEx( hKey, L"ShutSlowDown", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"ShutSlowDown", NULL, REG_SZ, (BYTE*) L"1", 1*sizeof(WCHAR)); o3_ShutdownInUnprod = true;}
	else 
		if (TempOperStr[0] == '0') o3_ShutdownInUnprod = false;
		else o3_ShutdownInUnprod = true;

	//AUTO CHANGE WALLPAPER		//DWORD	AvtoChWP	= 30;	//0-3-10-30-60	30min 1 800 000
	Temp_BufSize = 64*sizeof(WCHAR);
	if (RegQueryValueEx( hKey, L"AutoChangeW", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"AutoChangeW", NULL, REG_SZ, (BYTE*) L"0", 2*sizeof(WCHAR)); AvtoChWP = 0;}
	else AvtoChWP = _wtoi(TempOperStr);
	//CHENGE CAMERA POS	//DWORD	ChCamPos	= 2;	//0-1-2-3-5	1min	60 000
	Temp_BufSize = 64*sizeof(WCHAR);
	if (RegQueryValueEx( hKey, L"ChangeCamPos", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"ChangeCamPos", NULL, REG_SZ, (BYTE*) L"n", 2*sizeof(WCHAR)); ChCamPosEnable = false;}
	else
		if	(TempOperStr[0] == 'n') ChCamPosEnable = false;
		else {ChCamPos = _wtoi(TempOperStr); ChCamPosEnable = true;}
	//ROTATE NEAR BORDER
	if (RegQueryValueEx( hKey, L"BorderRotate", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"BorderRotate", NULL, REG_SZ, (BYTE*) L"1", 1*sizeof(WCHAR)); o5_RotateNearBorder = true;}
	else 
		if (TempOperStr[0] == '0') o5_RotateNearBorder = false;
		else o5_RotateNearBorder = true;	
	//ROTATE FOR CURSOR
	if (RegQueryValueEx( hKey, L"CursorRotate", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"CursorRotate", NULL, REG_SZ, (BYTE*) L"0", 1*sizeof(WCHAR)); o6_RotateWithCursor = false;}
	else 
		if (TempOperStr[0] == '0') o6_RotateWithCursor = false;
		else o6_RotateWithCursor = true;
	//WHELL EYES FOR CURSOR
	if (RegQueryValueEx( hKey, L"WhelEyesRotate", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"WhelEyesRotate", NULL, REG_SZ, (BYTE*) L"1", 1*sizeof(WCHAR)); o7_RotateEyesWhell = true;}
	else 
		if (TempOperStr[0] == '0') o7_RotateEyesWhell = false;
		else o7_RotateEyesWhell = true;
	//WHELL ZOOM
	if (RegQueryValueEx( hKey, L"WhelZoom", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"WhelZoom", NULL, REG_SZ, (BYTE*) L"1", 1*sizeof(WCHAR)); o8_ZoomWhell = true;}
	else 
		if (TempOperStr[0] == '0') o8_ZoomWhell = false;
		else o8_ZoomWhell = true;
	//USE filtering		byte		UsedFilt	= 1;	//0 - none 1-bi		2-tr	3-an	graphics quality
	Temp_BufSize = 64*sizeof(WCHAR);
	if (RegQueryValueEx( hKey, L"GraphicsQuality", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"GraphicsQuality", NULL, REG_SZ, (BYTE*) L"1", 1*sizeof(WCHAR)); GQualityMode = 1;}
	else GQualityMode = _wtoi(TempOperStr);
	//MESH MODE
	if (RegQueryValueEx( hKey, L"MeshMode", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"MeshMode", NULL, REG_SZ, (BYTE*) L"0", 1*sizeof(WCHAR)); o9_UseMeshSomeT = false;}
	else 
		if (TempOperStr[0] == '0') o9_UseMeshSomeT = false;
		else o9_UseMeshSomeT = true;		
	//SHOW FPS
	if (RegQueryValueEx( hKey, L"ShowFPS", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"ShowFPS", NULL, REG_SZ, (BYTE*) L"0", 1*sizeof(WCHAR)); o10_ShowFPS = false;}
	else 
		if (TempOperStr[0] == '0') o10_ShowFPS = false;
		else o10_ShowFPS = true;
	
	//LAST CAM POS READ
	Temp_BufSize = 32*sizeof(WCHAR);
	if (RegQueryValueEx( hKey, L"LastCamR", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"LastCamR", NULL, REG_SZ, (BYTE*) L"8.0", wcslen(L"8.0")*sizeof(WCHAR)); _R = 8.0f;}
	else
		_R = _wtof(TempOperStr);
	//Y
	Temp_BufSize = 32*sizeof(WCHAR);
	if (RegQueryValueEx( hKey, L"LastCamA", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"LastCamA", NULL, REG_SZ, (BYTE*) L"0.0", wcslen(L"0.0")*sizeof(WCHAR)); _La = 0.0f;}
	else
		_La = _wtof(TempOperStr);
	//Z
	Temp_BufSize = 32*sizeof(WCHAR);
	if (RegQueryValueEx( hKey, L"LastCamB", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"LastCamB", NULL, REG_SZ, (BYTE*) L"0.34", wcslen(L"0.34")*sizeof(WCHAR)); _Lb = 0.34f;}
	else
		_Lb = _wtof(TempOperStr);
	//CALC CAM POS
		vCam.x = _R * cos(_La) * cos(_Lb);
		vCam.y = _R * sin(_Lb);
		vCam.z = _R * sin(_La) * cos(_Lb);

	// FONT CAPTIONS USE
	Temp_BufSize = 16*sizeof(WCHAR);
	if (RegQueryValueEx( hKey, L"UseCaptions", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
		{ RegSetValueEx( hKey, L"UseCaptions", NULL, REG_SZ, (BYTE*) L"1", wcslen(L"1")*sizeof(WCHAR)); o11_ShowCaptions = true;}
	else
	{
		if (TempOperStr[0] == '0')	o11_ShowCaptions = false;
		else						o11_ShowCaptions = true ;
	}
	// FONT
	Temp_BufSize = 64*sizeof(WCHAR);
	if (RegQueryValueEx( hKey, L"FontCaptions", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
	{     RegSetValueEx( hKey, L"FontCaptions", NULL, REG_SZ, (BYTE*) L"Arial", wcslen(L"Arial")*sizeof(WCHAR)); StringCbPrintf((LPWSTR) fFONTstr, 64 ,L"%s", L"Arial");}
	else
		StringCbPrintf((LPWSTR) fFONTstr, 64 ,L"%s", TempOperStr);

	//FONT COLOR
	Temp_BufSize = 64*sizeof(WCHAR);
	if (RegQueryValueEx( hKey, L"FontColor", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
	{	  RegSetValueEx( hKey, L"FontColor", NULL, REG_SZ, (BYTE*) L"-1", wcslen(L"-1")*sizeof(WCHAR));	fColor = D3DCOLOR_XRGB( 0x00, 0x80, 0x80 );}
	else
		fColor = _wtoi(TempOperStr);

	//FONT SIZE
	Temp_BufSize = 16*sizeof(WCHAR);
	if (RegQueryValueEx( hKey, L"FontSize", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
	{	RegSetValueEx  ( hKey, L"FontSize", NULL, REG_SZ, (BYTE*) L"90", wcslen(L"70")*sizeof(WCHAR)); fSize = 70;}
	else			
		fSize = _wtoi(TempOperStr);

	//LAST WP INDEX
	Temp_BufSize = 16*sizeof(WCHAR);
	if (RegQueryValueEx( hKey, L"LastWpIn", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) != ERROR_SUCCESS)
	{	RegSetValueEx  ( hKey, L"LastWpIn", NULL, REG_SZ, (BYTE*) L"1", wcslen(L"1")*sizeof(WCHAR)); CurentWPIndex = 1;}
	else			
		CurentWPIndex = _wtoi(TempOperStr);

//
	//FIRST START - dont used - can play first start animation...
	if (RegQueryValueEx( hKey, L"FirstStart", NULL, NULL, (LPBYTE) TempOperStr, &Temp_BufSize) == ERROR_SUCCESS)
	{		
		bFirstStart = true;
		gCamMoveTime = 8000;
		gCamCapShowTime = 7000;
		ChCamPos = 0.134;
		RegDeleteValue( hKey, L"FirstStart");
		cLastPosChTime	= tTime-60000*2;
	}
//
	ApplyLanguage( false, NULL, -1 );
	RegCloseKey(hKey);
}
void SaveOptToReg()
{
	HKEY hKey;
	DWORD dwDispas;
	WCHAR PathToProg[1024] = {0};		DWORD buf_size = sizeof(PathToProg);					
	WCHAR PathToLastWp[1024] = {0};		DWORD buf_size_wp = sizeof(PathToLastWp);	
// KEY
	if (RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\Full3DWallpaper\\", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)//KEY_READ????
		RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Full3DWallpaper", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDispas);
// PATH	
	if ( (RegQueryValueEx( hKey, L"Path", NULL, NULL, (LPBYTE) PathToProg, &buf_size) != ERROR_SUCCESS) || (!PathFileExists(PathToProg)) )
	{
		LPWSTR CurPathToProg = (LPWSTR ) LocalAlloc(LPTR,256);
		GetCurrentDirectory( 256, CurPathToProg);
		RegSetValueEx( hKey, L"Path", NULL, REG_SZ, (BYTE*) CurPathToProg, wcslen(CurPathToProg)*sizeof(WCHAR));
	}
// PATH TO LAST WP
	RegSetValueEx( hKey, L"PathToLastWp", NULL, REG_SZ, (BYTE*) FirstAndCurentScreenPath, wcslen(FirstAndCurentScreenPath)*sizeof(WCHAR));//2*wcslen(FirstAndCurentScreenPath));		
// LANG
	 RegSetValueEx( hKey, L"Language", NULL, REG_SZ, (BYTE*) GlLang, wcslen(GlLang)*sizeof(WCHAR));
	
// Full3DWallpaper\\OPTIONS
	if (RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\Full3DWallpaper\\Options\\", 0, KEY_ALL_ACCESS, &hKey)	!= ERROR_SUCCESS)
		RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Full3DWallpaper\\Options", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDispas);

	LPWSTR TempOperStr = (LPWSTR ) LocalAlloc(LPTR,64);
	DWORD  Temp_BufSize = 64*sizeof(WCHAR);
// SHUTDOWN IN UNPRODUCTIVE		//o3_ShutdownInUnprod
	if (o3_ShutdownInUnprod) RegSetValueEx( hKey, L"ShutSlowDown", NULL, REG_SZ, (BYTE*) L"1", 1*sizeof(WCHAR));
	else					 RegSetValueEx( hKey, L"ShutSlowDown", NULL, REG_SZ, (BYTE*) L"0", 1*sizeof(WCHAR));

	
	//AUTO CHANGE WALLPAPER		//DWORD	AvtoChWP	= 30;	//0-3-10-30-60	30min 1 800 000
	StringCbPrintf( TempOperStr, 256 ,L"%d", AvtoChWP);
	RegSetValueEx( hKey, L"AutoChangeW", NULL, REG_SZ, (BYTE*) TempOperStr, 2*sizeof(WCHAR));

	//CHENGE CAMERA POS				//DWORD	ChCamPos	= 2;	//0-1-2-3-5	1min	60 000
	if (ChCamPosEnable)
	{
		StringCbPrintf( TempOperStr, 256 ,L"%f", ChCamPos);
		RegSetValueEx( hKey, L"ChangeCamPos", NULL, REG_SZ, (BYTE*) TempOperStr, 1*sizeof(WCHAR));
	}
	else
	{
		StringCbPrintf( TempOperStr, 256 ,L"n");
		RegSetValueEx( hKey, L"ChangeCamPos", NULL, REG_SZ, (BYTE*) TempOperStr, 1*sizeof(WCHAR));
	}

	//ROTATE NEAR BORDER
	if (o5_RotateNearBorder) RegSetValueEx( hKey, L"BorderRotate", NULL, REG_SZ, (BYTE*) L"1", 1*sizeof(WCHAR));
	else					 RegSetValueEx( hKey, L"BorderRotate", NULL, REG_SZ, (BYTE*) L"0", 1*sizeof(WCHAR));

	//ROTATE FOR CURSOR
	if (o6_RotateWithCursor) RegSetValueEx( hKey, L"CursorRotate", NULL, REG_SZ, (BYTE*) L"1", 1*sizeof(WCHAR));
	else					 RegSetValueEx( hKey, L"CursorRotate", NULL, REG_SZ, (BYTE*) L"0", 1*sizeof(WCHAR));

	//WHELL EYES FOR CURSOR
	if (o7_RotateEyesWhell)  RegSetValueEx( hKey, L"WhelEyesRotate", NULL, REG_SZ, (BYTE*) L"1", 1*sizeof(WCHAR));
	else					RegSetValueEx( hKey, L"WhelEyesRotate", NULL, REG_SZ, (BYTE*) L"0", 1*sizeof(WCHAR));

	//WHELL ZOOM
	if (o8_ZoomWhell) RegSetValueEx( hKey, L"WhelZoom", NULL, REG_SZ, (BYTE*) L"1", 1*sizeof(WCHAR));
	else			  RegSetValueEx( hKey, L"WhelZoom", NULL, REG_SZ, (BYTE*) L"0", 1*sizeof(WCHAR));

	//USE filtering		byte		UsedFilt	= 1;	//0 - none 1-bi		2-tr	3-an
	StringCbPrintf( TempOperStr, 256 ,L"%d", GQualityMode);
	RegSetValueEx ( hKey, L"GraphicsQuality", NULL, REG_SZ, (BYTE*) TempOperStr, 1*sizeof(WCHAR));

	//MESH MODE
	if (o9_UseMeshSomeT) RegSetValueEx( hKey, L"MeshMode", NULL, REG_SZ, (BYTE*) L"1", 1*sizeof(WCHAR));
	else				 RegSetValueEx( hKey, L"MeshMode", NULL, REG_SZ, (BYTE*) L"0", 1*sizeof(WCHAR));
		
	//SHOW FPS
	if (o10_ShowFPS) RegSetValueEx( hKey, L"ShowFPS", NULL, REG_SZ, (BYTE*) L"1", 1*sizeof(WCHAR));
	else			 RegSetValueEx( hKey, L"ShowFPS", NULL, REG_SZ, (BYTE*) L"0", 1*sizeof(WCHAR));

	//LAST CAM POS SAVE
	if 	(bCamOnProc)
	{
			_R  = sqrt( vCam.x*vCam.x + vCam.y*vCam.y + vCam.z*vCam.z );
			_Lb = atan2(vCam.y, sqrt(vCam.x*vCam.x+vCam.z*vCam.z));
			_La = atan2(vCam.z,vCam.x);
	}
	StringCbPrintf( TempOperStr, 128 ,L"%f", _R);
	RegSetValueEx( hKey, L"LastCamR", NULL, REG_SZ, (BYTE*) TempOperStr,  wcslen(TempOperStr)*sizeof(WCHAR));
	StringCbPrintf( TempOperStr, 128 ,L"%f", _La);
	RegSetValueEx( hKey, L"LastCamA", NULL, REG_SZ, (BYTE*) TempOperStr,  wcslen(TempOperStr)*sizeof(WCHAR));
	StringCbPrintf( TempOperStr, 128 ,L"%f", _Lb);
	RegSetValueEx( hKey, L"LastCamB", NULL, REG_SZ, (BYTE*) TempOperStr,  wcslen(TempOperStr)*sizeof(WCHAR));
	// CAPTIONS
	if (o11_ShowCaptions) RegSetValueEx( hKey, L"UseCaptions", NULL, REG_SZ, (BYTE*) L"1", 1*sizeof(WCHAR));
	else				  RegSetValueEx( hKey, L"UseCaptions", NULL, REG_SZ, (BYTE*) L"0", 1*sizeof(WCHAR));

	StringCbPrintf( TempOperStr, 64 ,L"%s", fFONTstr);
	RegSetValueEx( hKey, L"FontCaptions", NULL, REG_SZ, (BYTE*) TempOperStr,  wcslen(TempOperStr)*sizeof(WCHAR));
	StringCbPrintf( TempOperStr, 64 ,L"%d", fColor);
	RegSetValueEx( hKey, L"FontColor", NULL, REG_SZ, (BYTE*) TempOperStr,  wcslen(TempOperStr)*sizeof(WCHAR));
	StringCbPrintf( TempOperStr, 16 ,L"%d", fSize);
	RegSetValueEx( hKey, L"FontSize", NULL, REG_SZ, (BYTE*) TempOperStr,  wcslen(TempOperStr)*sizeof(WCHAR));

	//LAST WP INDEX 
	StringCbPrintf( TempOperStr, 64 ,L"%d", CurentWPIndex);
	RegSetValueEx( hKey, L"LastWpIn", NULL, REG_SZ, (BYTE*) TempOperStr,  wcslen(TempOperStr)*sizeof(WCHAR));

	RegCloseKey(hKey);
}
void ApplyLanguage(bool Dlg, HWND dlgWnd, int byIndex  )//, LPWSTR TmpLang	
{
LPWSTR PathToINI = (LPWSTR ) LocalAlloc(LPTR,256);
LPWSTR CurentDir = (LPWSTR ) LocalAlloc(LPTR,256);
GetCurrentDirectory(256, CurentDir);

StringCbPrintf( PathToINI, 256 ,L"%s\\Lang\\%s.ini", CurentDir, GlLang);
if (!PathFileExists(PathToINI)) {MessageBox( hMain, L"Catn't find *language*.ini file", L"Notification", MB_OK); return;}

	if (Dlg == false)
	{
		for (DWORD mii_id = 1000; mii_id < 1006; mii_id++)
		{
			LPWSTR INIKey = (LPWSTR ) LocalAlloc(LPTR,10);
			StringCbPrintf( INIKey, 256 ,L"%d", mii_id);

			LPWSTR TempStr = (LPWSTR ) LocalAlloc(LPTR,64);		//StringCbPrintf( TempStr, 256 ,L"%s", INIRead( PathToINI, L"MENU", INIKey, NULL));
			TempStr = INIRead( PathToINI, L"MENU", INIKey, NULL);
			if ( TempStr[0] != 0)
			{

				MENUITEMINFO mii = {NULL};
				mii.cbSize = sizeof (MENUITEMINFO);
				mii.fMask = MIIM_STRING | MIIM_FTYPE;
				mii.fType = MFT_STRING ;					
				GetMenuItemInfo( PopupMenu, mii_id, FALSE, &mii);
				mii.dwTypeData = TempStr;
				SetMenuItemInfo( PopupMenu, mii_id, FALSE, &mii);
			}
		}
				MENUITEMINFO mii = {NULL};
				mii.cbSize = sizeof (MENUITEMINFO);
				mii.fMask = MIIM_STRING | MIIM_FTYPE;
				mii.fType = MFT_STRING  ;					
				GetMenuItemInfo( hmFirstPopupM, WM_OPENFOL, FALSE, &mii);
				mii.dwTypeData = INIRead( PathToINI, L"MENU", L"1008", L"Open folder");
				SetMenuItemInfo( hmFirstPopupM, WM_OPENFOL, FALSE, &mii);
	}
	///
	else
	{
		//COMBOBOXE'S LANG
		LPWSTR PathToMLngF = ( LPWSTR ) LocalAlloc(LPTR,256);
		StringCbPrintf( PathToMLngF, 256 ,L"%s\\Lang\\LANGUAGES.ini", CurentDir);
		if (!PathFileExists(PathToMLngF)) {MessageBox( hMain, L"Catn't find LANGUAGES.ini file", L"Notification", MB_OK); return;}

		DWORD sec_N = 0;
		LPWSTR INIKey = ( LPWSTR ) LocalAlloc(LPTR,10);
		StringCbPrintf( INIKey, 256 ,L"%d", sec_N);

		LPWSTR TempStrSec = (LPWSTR ) LocalAlloc(LPTR,64);
		TempStrSec = L"0";
		
		while ( TempStrSec[0] != 0 )
		{
			TempStrSec = INIRead( PathToMLngF, L"FILES", INIKey, NULL);
			if ( TempStrSec[0] == 0 ) break;
			SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_6), CB_INSERTSTRING, sec_N, (LPARAM) INIRead( PathToMLngF, L"LANGUAGES", INIKey, NULL));
			if (( (TempStrSec[0] == GlLang[0])&&(TempStrSec[1] == GlLang[1]) ) && (byIndex == -1))
				SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_6), CB_SETCURSEL, sec_N, NULL);//CB_SELECTSTRING
			if (sec_N == byIndex)
			{
				GlLang = INIRead( PathToMLngF, L"FILES", INIKey, NULL);
				StringCbPrintf( PathToINI, 256 ,L"%s\\Lang\\%s.ini", CurentDir, GlLang );
				SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_6), CB_SETCURSEL, sec_N, NULL);				
			}
			sec_N++;
			StringCbPrintf( INIKey, 256 ,L"%d", sec_N);	
		}
		//OPTIONS
		SetWindowText(dlgWnd, INIRead(PathToINI, L"OPT_DIALOG", L"1000", L"Options"));
		// OPT DLG FORM
		for (DWORD mii_id = 1001; mii_id < 1023; mii_id++)
		{
			LPWSTR INIKey = ( LPWSTR ) LocalAlloc(LPTR,10);
			StringCbPrintf( INIKey, 256 ,L"%d", mii_id);

			LPWSTR TempStr = (LPWSTR ) LocalAlloc(LPTR,64);
			TempStr = INIRead( PathToINI, L"OPT_DIALOG", INIKey, NULL);
			if ( TempStr[0] != 0)
			{
				SetWindowText(GetDlgItem(dlgWnd, mii_id), TempStr);
			}			
		}
		//AVTO CHANGE COMBO BOX		PathToINI	CurentDir	0-3-10-30-60
		sec_N = 0;
		for (DWORD ini_key = 1024; ini_key < 1029; ini_key++)
		{
			StringCbPrintf( INIKey, 256 ,L"%d", ini_key);
			TempStrSec = INIRead( PathToINI, L"OPT_DIALOG", INIKey, NULL);
			SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_17), CB_INSERTSTRING, sec_N, (LPARAM) TempStrSec);
			sec_N++;			
		}
		//AVTO CHANGE CAMERA VIEW		ChCamPos	= 2;	//n-0-1-2-3
		sec_N = 0;
		for (DWORD ini_key = 1029; ini_key < 1034; ini_key++)
		{
			StringCbPrintf( INIKey, 256 ,L"%d", ini_key);
			TempStrSec = INIRead( PathToINI, L"OPT_DIALOG", INIKey, NULL);
			SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_23), CB_INSERTSTRING, sec_N, (LPARAM) TempStrSec);
			sec_N++;			
		}
		//GRAPHIC QUALITY		UsedFilt	= 1;	//0 - none 1-bi		2-tr	3-an	IDC_C_32	IDC_COMBO_33
		sec_N = 0;
		for (DWORD ini_key = 1035; ini_key < 1038; ini_key++)
		{
			StringCbPrintf( INIKey, 256 ,L"%d", ini_key);
			TempStrSec = INIRead( PathToINI, L"OPT_DIALOG", INIKey, NULL);
			if (CheckMultiSampleType(ini_key-1034))
				SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_33), CB_INSERTSTRING, sec_N, (LPARAM) TempStrSec);
			sec_N++;			
		}
	}
}
void CheckDlgForm(HWND dlgWnd)
{
		//AVTO CHANGE COMBO BOX
		if (AvtoChWP == 3 ) SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_17), CB_SETCURSEL, 1, NULL);
		if (AvtoChWP == 10) SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_17), CB_SETCURSEL, 2, NULL);
		if (AvtoChWP == 30) SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_17), CB_SETCURSEL, 3, NULL);
		if (AvtoChWP == 60) SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_17), CB_SETCURSEL, 4, NULL);
		if (AvtoChWP == 0 )	
		{
			CheckDlgButton(dlgWnd, IDC_C_16, BST_UNCHECKED);
			SetWindowLong(GetDlgItem(dlgWnd, IDC_COMBO_17), GWL_STYLE, GetWindowLong(GetDlgItem(dlgWnd, IDC_COMBO_17),GWL_STYLE) | WS_DISABLED);			
			SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_17), CB_SETCURSEL, 0, NULL);
		}
		else
		{
			SetWindowLong(GetDlgItem(dlgWnd, IDC_COMBO_17), GWL_STYLE, GetWindowLong(GetDlgItem(dlgWnd, IDC_COMBO_17),GWL_STYLE) &~ WS_DISABLED);
			CheckDlgButton(dlgWnd, IDC_C_16, BST_CHECKED);
		}
		InvalidateRect(GetDlgItem(dlgWnd,		IDC_C_16),NULL,NULL);
		InvalidateRect(GetDlgItem(dlgWnd,	IDC_COMBO_17),NULL,NULL);
		//AVTO CHANGE CAMERA VIEW
		if (ChCamPosEnable)
		{			
			if (ChCamPos == 1) SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_23), CB_SETCURSEL, 1, NULL);
			if (ChCamPos == 2) SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_23), CB_SETCURSEL, 2, NULL);
			if (ChCamPos == 3) SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_23), CB_SETCURSEL, 3, NULL);
			if (ChCamPos == 0) SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_23), CB_SETCURSEL, 4, NULL);
			SetWindowLong(GetDlgItem(dlgWnd, IDC_COMBO_23), GWL_STYLE, GetWindowLong(GetDlgItem(dlgWnd, IDC_COMBO_23),GWL_STYLE) &~ WS_DISABLED);
			CheckDlgButton(dlgWnd, IDC_C_22, BST_CHECKED);
		}
		else
		{
			CheckDlgButton(dlgWnd, IDC_C_22, BST_UNCHECKED);
			SetWindowLong(GetDlgItem(dlgWnd, IDC_COMBO_23), GWL_STYLE, GetWindowLong(GetDlgItem(dlgWnd, IDC_COMBO_23),GWL_STYLE) | WS_DISABLED);			
			SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_23), CB_SETCURSEL, 0, NULL);
		}
		InvalidateRect(GetDlgItem(dlgWnd,		IDC_C_22),NULL,NULL);
		InvalidateRect(GetDlgItem(dlgWnd,	IDC_COMBO_23),NULL,NULL);
		//GRAPHIC QUALITY			IDC_C_32
		if (GQualityMode == 1) SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_33), CB_SETCURSEL, 0, NULL);
		if (GQualityMode == 2) SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_33), CB_SETCURSEL, 1, NULL);
		if (GQualityMode == 3) SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_33), CB_SETCURSEL, 2, NULL);
		/*if (UsedFilt == 0)	
		{
			CheckDlgButton(dlgWnd, IDC_C_32, BST_UNCHECKED);
			SetWindowLong(GetDlgItem(dlgWnd, IDC_COMBO_33), GWL_STYLE, GetWindowLong(GetDlgItem(dlgWnd, IDC_COMBO_33),GWL_STYLE) | WS_DISABLED);			
			SendMessage(GetDlgItem(dlgWnd, IDC_COMBO_33), CB_SETCURSEL, 0, NULL);
		}
		else
		{
			SetWindowLong(GetDlgItem(dlgWnd, IDC_COMBO_33), GWL_STYLE, GetWindowLong(GetDlgItem(dlgWnd, IDC_COMBO_33),GWL_STYLE) &~ WS_DISABLED);
			CheckDlgButton(dlgWnd, IDC_C_32, BST_CHECKED);
		}
		InvalidateRect(GetDlgItem(dlgWnd,		IDC_C_22),NULL,NULL);*/
		InvalidateRect(GetDlgItem(dlgWnd,	IDC_COMBO_33),NULL,NULL);

		//OTHER BOOL
		if (o1_StartWithWin)
		{
			CheckDlgButton(dlgWnd, IDC_C_2, BST_CHECKED);
			SetWindowLong( GetDlgItem(dlgWnd, IDC_C_3), GWL_STYLE, GetWindowLong(GetDlgItem(dlgWnd, IDC_C_3),GWL_STYLE) &~ WS_DISABLED);
			if (o2_RunInShutdown) CheckDlgButton(dlgWnd, IDC_C_3, BST_CHECKED);
		}
		else 
		{
			CheckDlgButton(dlgWnd, IDC_C_2, BST_UNCHECKED);
			CheckDlgButton(dlgWnd, IDC_C_3, BST_UNCHECKED);
			SetWindowLong( GetDlgItem(dlgWnd, IDC_C_3), GWL_STYLE, GetWindowLong(GetDlgItem(dlgWnd, IDC_C_3),GWL_STYLE) | WS_DISABLED);			
		}
		InvalidateRect(GetDlgItem(dlgWnd, IDC_C_3), NULL, NULL);

		if (o3_ShutdownInUnprod) CheckDlgButton(dlgWnd, IDC_C_4, BST_CHECKED); else CheckDlgButton(dlgWnd, IDC_C_4, BST_UNCHECKED);

		if (o4_UseAsScrSav)			CheckDlgButton(dlgWnd, IDC_C_6,  BST_CHECKED); else CheckDlgButton(dlgWnd, IDC_C_6, BST_UNCHECKED);

		if (o5_RotateNearBorder)	CheckDlgButton(dlgWnd, IDC_C_27, BST_CHECKED); else CheckDlgButton(dlgWnd, IDC_C_27, BST_UNCHECKED);
		if (o6_RotateWithCursor)	CheckDlgButton(dlgWnd, IDC_C_28, BST_CHECKED); else CheckDlgButton(dlgWnd, IDC_C_28, BST_UNCHECKED);
		if (o7_RotateEyesWhell)		CheckDlgButton(dlgWnd, IDC_C_29, BST_CHECKED); else CheckDlgButton(dlgWnd, IDC_C_29, BST_UNCHECKED);
		if (o8_ZoomWhell)			CheckDlgButton(dlgWnd, IDC_C_30, BST_CHECKED); else CheckDlgButton(dlgWnd, IDC_C_30, BST_UNCHECKED);

		if (o9_UseMeshSomeT)		CheckDlgButton(dlgWnd, IDC_C_36, BST_CHECKED); else CheckDlgButton(dlgWnd, IDC_C_36, BST_UNCHECKED);
		if (o10_ShowFPS)			CheckDlgButton(dlgWnd, IDC_C_37, BST_CHECKED); else CheckDlgButton(dlgWnd, IDC_C_37, BST_UNCHECKED);

		if (o11_ShowCaptions) 
		{
			CheckDlgButton(dlgWnd, IDC_C_18, BST_CHECKED);
			SetWindowLong(GetDlgItem(dlgWnd, IDC_B_19), GWL_STYLE, GetWindowLong(GetDlgItem(dlgWnd, IDC_B_19),GWL_STYLE) &~ WS_DISABLED);
		}
		else
		{
			CheckDlgButton(dlgWnd, IDC_C_18, BST_UNCHECKED);
			SetWindowLong(GetDlgItem(dlgWnd, IDC_B_19), GWL_STYLE, GetWindowLong(GetDlgItem(dlgWnd, IDC_B_19),GWL_STYLE) | WS_DISABLED);
		}
		InvalidateRect(GetDlgItem(dlgWnd,	IDC_B_19),NULL,NULL);
}
bool AddDelInAutoRun( bool bRunShutDown, bool bDelInAutoRun)
{
	HKEY hKey;	//DWORD dwDispas;

	LPWSTR PathToProg   = ( LPWSTR ) LocalAlloc(LPTR,256);
	LPWSTR PathToProgEx = ( LPWSTR ) LocalAlloc(LPTR,256);
	GetCurrentDirectory( 256, PathToProg);
	if (RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run\\", 0, KEY_ALL_ACCESS, &hKey)!= ERROR_SUCCESS ) return false;

	if (bDelInAutoRun == false)
	{
		if (bRunShutDown == false)
			StringCbPrintf( PathToProgEx, 256 ,L"\"%s\\Full3DWallpaper.exe\" -a", PathToProg);
		else
			StringCbPrintf( PathToProgEx, 256 ,L"\"%s\\Full3DWallpaper.exe\" -z", PathToProg);		
		if ( RegSetValueEx( hKey, L"Full3DWallpaper", NULL, REG_SZ, (BYTE*) PathToProgEx, wcslen(PathToProgEx)*sizeof(WCHAR)) != ERROR_SUCCESS ) return false;		
	}
	else
		if ( RegDeleteValue( hKey, L"Full3DWallpaper") != ERROR_SUCCESS) return false;

	return true;
}
bool UseAsScrSav(bool AddScrSav)
{
		//USE AS SCR SAV
		HKEY hKeyAR;
		WCHAR PathToProgRun[1024] = {0};
		DWORD buf_size_pr = sizeof(PathToProgRun);

		RegOpenKeyEx( HKEY_CURRENT_USER, L"Control Panel\\Desktop\\", 0, KEY_ALL_ACCESS, &hKeyAR);
		RegQueryValueEx ( hKeyAR, L"SCRNSAVE.EXE", NULL, NULL, (LPBYTE) PathToProgRun, &buf_size_pr);

		LPWSTR WinSysDir  = ( LPWSTR ) LocalAlloc(LPTR,256);
		LPWSTR SysDirScrS = ( LPWSTR ) LocalAlloc(LPTR,256);
		GetSystemDirectory( WinSysDir, 256);
		StringCbPrintf( SysDirScrS, 256 ,L"%s\\FULL3D~1.SCR", WinSysDir);

		if ( AddScrSav == false) if ( RegDeleteValue( hKeyAR, L"SCRNSAVE.EXE") != ERROR_SUCCESS) return false;

		if ( AddScrSav == true)  if ( (RegSetValueEx ( hKeyAR, L"SCRNSAVE.EXE", NULL, REG_SZ, (BYTE*) SysDirScrS, wcslen(SysDirScrS)*sizeof(WCHAR)) != ERROR_SUCCESS) 
								   && (RegSetValueEx ( hKeyAR, L"ScreenSaveActive", NULL, REG_SZ, (BYTE*) L"1", 1*sizeof(WCHAR)) != ERROR_SUCCESS) ) return false;

		RegCloseKey(hKeyAR);
		return true;
}
void SetDefOpt(HWND hDlg)
{		
		if (AddDelInAutoRun( true, false))
		{
			o1_StartWithWin = true; o2_RunInShutdown = true;

			CheckDlgButton(hDlg, IDC_C_2, BST_CHECKED);

			CheckDlgButton(hDlg, IDC_C_3, BST_CHECKED);
			SetWindowLong (GetDlgItem(hDlg, IDC_C_3), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_C_3),GWL_STYLE) &~ WS_DISABLED);
			InvalidateRect(GetDlgItem(hDlg,	IDC_C_3),NULL,NULL);
		}

		CheckDlgButton(hDlg, IDC_C_4, BST_CHECKED);	o3_ShutdownInUnprod = true;

		if (UseAsScrSav(true))
		{
			o4_UseAsScrSav = true;
			CheckDlgButton(hDlg, IDC_C_6, BST_CHECKED);
		}

		CheckDlgButton(hDlg, IDC_C_16, BST_CHECKED);
		SetWindowLong (GetDlgItem(hDlg, IDC_COMBO_17), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_COMBO_17),GWL_STYLE) &~ WS_DISABLED);
		SendMessage   (GetDlgItem(hDlg, IDC_COMBO_17), CB_SETCURSEL, 3, NULL);
		InvalidateRect(GetDlgItem(hDlg,	IDC_COMBO_17),NULL,NULL);
		AvtoChWP	= 30;

		CheckDlgButton(hDlg, IDC_C_22, BST_CHECKED);
		SetWindowLong (GetDlgItem(hDlg, IDC_COMBO_23), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_COMBO_23),GWL_STYLE) &~ WS_DISABLED);
		SendMessage   (GetDlgItem(hDlg, IDC_COMBO_23), CB_SETCURSEL, 2, NULL);
		InvalidateRect(GetDlgItem(hDlg,	IDC_COMBO_23),NULL,NULL);
		ChCamPos = 2;
		ChCamPosEnable = true;

		CheckDlgButton(hDlg, IDC_C_27, BST_CHECKED);	o5_RotateNearBorder = true;
		CheckDlgButton(hDlg, IDC_C_28, BST_UNCHECKED);	o6_RotateWithCursor = false;
		CheckDlgButton(hDlg, IDC_C_29, BST_CHECKED);	o7_RotateEyesWhell	= true;
		CheckDlgButton(hDlg, IDC_C_30, BST_CHECKED);	o8_ZoomWhell		= true;

		//CheckDlgButton(hDlg, IDC_C_32, BST_CHECKED);
		//SetWindowLong (GetDlgItem(hDlg, IDC_COMBO_33), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_COMBO_33),GWL_STYLE) &~ WS_DISABLED);
		//SendMessage   (GetDlgItem(hDlg, IDC_COMBO_33), CB_SETCURSEL, 1, NULL);
		SendMessage   (GetDlgItem(hDlg, IDC_COMBO_33), CB_SETCURSEL, 0, NULL);
		InvalidateRect(GetDlgItem(hDlg,	IDC_COMBO_33),NULL,NULL);
		//UsedFilt = 1;
		if ( GQualityMode != 1) 
		{
			GQualityMode = 1;
			if (bShutDown == false) ResetDevice(hD);
		}
		//SetFiltrationMode(1);
		CheckDlgButton(hDlg, IDC_C_36, BST_UNCHECKED);	o9_UseMeshSomeT		= false;
			//o9_UseMeshSomeT = false;
			//_D3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
			//_D3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			//_D3DDevice->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

		CheckDlgButton(hDlg, IDC_C_37, BST_UNCHECKED);	if (o10_ShowFPS) if (FPSFont != NULL) if (bShutDown == false) FPSFont->Release();
		o10_ShowFPS		= false;

		CheckDlgButton(hDlg, IDC_C_18, BST_CHECKED);			
			SetWindowLong(GetDlgItem (hDlg, IDC_B_19), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, IDC_B_19),GWL_STYLE) &~ WS_DISABLED);
			InvalidateRect(GetDlgItem(hDlg, IDC_B_19), NULL, NULL);

			StringCbPrintf((LPWSTR) fFONTstr, 64 ,L"%s", L"Arial");		SendMessage( GetDlgItem(hDlg,IDC_FONT_COMBO) , CB_SELECTSTRING, (WPARAM)-1, (LPARAM)fFONTstr);
			fColor = D3DCOLOR_XRGB( 0xFF, 0xFF, 0xFF );					SendMessage( GetDlgItem(hDlg, IDC_FONT_COLOR), CB_SETCURSEL, 15, NULL);
			fSize = 70;													SendMessage( GetDlgItem(hDlg, IDC_FONT_SIZE) , CB_SETCURSEL, 4, NULL);
			if ((o11_ShowCaptions)&&(bShutDown == false))
			{
				if (CaptionFont != NULL) CaptionFont->Release();			
				ReCreateCaptionFont();
			}
			o11_ShowCaptions = true;
}
void ExecFontDialog(HWND hDlg, bool DlgShow)
{	
	HWND	hwndFONT = GetDlgItem(hDlg, IDC_FONT_COMBO),
			hwndCOLOR= GetDlgItem(hDlg, IDC_FONT_COLOR),
			hwndSIZE = GetDlgItem(hDlg, IDC_FONT_SIZE );

	if (DlgShow==false)
	{
		ShowWindow( hwndFONT , SW_HIDE);
		ShowWindow( hwndCOLOR, SW_HIDE);
		ShowWindow( hwndSIZE , SW_HIDE);
	}

	if (DlgShow==true)
	{
		//COMBO FONT
		if ( IsWindowVisible(hwndFONT) )
			ShowWindow( hwndFONT, SW_HIDE);
		else
		{
			ShowWindow( hwndFONT, SW_SHOW);
			//ADD FONTS
			HDC hdc = GetDC(hwndFONT);
			LOGFONT		lf;
			lf.lfFaceName[0] = '\0';
			lf.lfCharSet = DEFAULT_CHARSET;

			EnumFontFamiliesEx(hdc, &lf, (FONTENUMPROC)EnumFontFamExProc, (LPARAM)hwndFONT, (DWORD)0);
			ReleaseDC(hwndFONT,hdc);

			SendMessage( hwndFONT, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)fFONTstr);		//if (lstrcmpi(bigTempBuffer, fFONTstr)
		}
		//COMBO COLOR
		if ( IsWindowVisible(hwndCOLOR) )
			ShowWindow( hwndCOLOR, SW_HIDE);
		else
		{
			ShowWindow( hwndCOLOR, SW_SHOW);
			//ADD COLOR'S	fColor
			SendMessage(hwndCOLOR, CB_RESETCONTENT, 0, 0);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 0, (LPARAM) L"Black" );			if (fColor == D3DCOLOR_XRGB( 0x00, 0x00, 0x00 )) SendMessage( hwndCOLOR, CB_SETCURSEL, 0, NULL);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 1, (LPARAM) L"Maroon" );		if (fColor == D3DCOLOR_XRGB( 0x80, 0x00, 0x00 )) SendMessage( hwndCOLOR, CB_SETCURSEL, 1, NULL);									
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 2, (LPARAM) L"Green" );			if (fColor == D3DCOLOR_XRGB( 0x00, 0x80, 0x00 )) SendMessage( hwndCOLOR, CB_SETCURSEL, 2, NULL);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 3, (LPARAM) L"Olive" );			if (fColor == D3DCOLOR_XRGB( 0x80, 0x80, 0x00 )) SendMessage( hwndCOLOR, CB_SETCURSEL, 3, NULL);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 4, (LPARAM) L"Navy" );			if (fColor == D3DCOLOR_XRGB( 0x00, 0x00, 0x80 )) SendMessage( hwndCOLOR, CB_SETCURSEL, 4, NULL);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 5, (LPARAM) L"Purple" );		if (fColor == D3DCOLOR_XRGB( 0x80, 0x00, 0x80 )) SendMessage( hwndCOLOR, CB_SETCURSEL, 5, NULL);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 6, (LPARAM) L"Teal" );			if (fColor == D3DCOLOR_XRGB( 0x00, 0x80, 0x80 )) SendMessage( hwndCOLOR, CB_SETCURSEL, 6, NULL);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 7, (LPARAM) L"Grey" );			if (fColor == D3DCOLOR_XRGB( 0x80, 0x80, 0x80 )) SendMessage( hwndCOLOR, CB_SETCURSEL, 7, NULL);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 8, (LPARAM) L"Silver" );		if (fColor == D3DCOLOR_XRGB( 0xC0, 0xC0, 0xC0 )) SendMessage( hwndCOLOR, CB_SETCURSEL, 8, NULL);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 9, (LPARAM) L"Red" );			if (fColor == D3DCOLOR_XRGB( 0xFF, 0x00, 0x00 )) SendMessage( hwndCOLOR, CB_SETCURSEL, 9, NULL);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 10, (LPARAM) L"Lime" );			if (fColor == D3DCOLOR_XRGB( 0x00, 0xFF, 0x00 )) SendMessage( hwndCOLOR, CB_SETCURSEL, 10, NULL);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 11, (LPARAM) L"Yellow" );		if (fColor == D3DCOLOR_XRGB( 0xFF, 0xFF, 0x00 )) SendMessage( hwndCOLOR, CB_SETCURSEL, 11, NULL);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 12, (LPARAM) L"Blue" );			if (fColor == D3DCOLOR_XRGB( 0x00, 0x00, 0xFF )) SendMessage( hwndCOLOR, CB_SETCURSEL, 12, NULL);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 13, (LPARAM) L"Fuschia" );		if (fColor == D3DCOLOR_XRGB( 0xFF, 0x00, 0xFF )) SendMessage( hwndCOLOR, CB_SETCURSEL, 13, NULL);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 14, (LPARAM) L"Aqua" );			if (fColor == D3DCOLOR_XRGB( 0x00, 0xFF, 0xFF )) SendMessage( hwndCOLOR, CB_SETCURSEL, 14, NULL);
			SendMessage(hwndCOLOR, CB_INSERTSTRING, 15, (LPARAM) L"White" );		if (fColor == D3DCOLOR_XRGB( 0xFF, 0xFF, 0xFF )) SendMessage( hwndCOLOR, CB_SETCURSEL, 15, NULL);
		}
		//COMBO SIZE
		if ( IsWindowVisible(hwndSIZE) )
			ShowWindow( hwndSIZE, SW_HIDE);
		else
		{
			ShowWindow( hwndSIZE, SW_SHOW);
			//ADD SIZE'S
			LPWSTR TempSizeStr	= (LPWSTR ) LocalAlloc(LPTR,32);
			DWORD selpos = 0;
			SendMessage(hwndSIZE, CB_RESETCONTENT, 0, 0);
			for ( DWORD i = 30; i<=140; i=i+10)
			{
				StringCbPrintf((LPWSTR) TempSizeStr, 32 ,L"%d", i);
				SendMessage(hwndSIZE, CB_INSERTSTRING, -1, (LPARAM) TempSizeStr );
				if (i == fSize)	
					selpos = ((i/10)-3);
			}
			SendMessage( hwndSIZE, CB_SETCURSEL, selpos, NULL);
			LocalFree(TempSizeStr);
		}
	}
	InvalidateRect(hwndFONT , NULL, NULL);
	InvalidateRect(hwndCOLOR, NULL, NULL);
	InvalidateRect(hwndSIZE , NULL, NULL);
}
int CALLBACK EnumFontFamExProc( ENUMLOGFONTEX *lpelfe,NEWTEXTMETRICEX *lpntme,DWORD FontType,LPARAM lParam)
{
	DWORD	E;
	HWND	hWndCombo = (HWND) lParam;

	LPWSTR bigTempBuffer	= (LPWSTR ) LocalAlloc(LPTR,256);	
	StringCbPrintf((LPWSTR) bigTempBuffer, 256 ,L"%s", lpelfe->elfLogFont.lfFaceName);

	if (SendMessage(hWndCombo, CB_GETCOUNT, NULL, NULL))
	{
		E = SendMessage(hWndCombo, CB_FINDSTRINGEXACT, 0, (LPARAM)bigTempBuffer);
		if (E == CB_ERR)
			E = SendMessage(hWndCombo, CB_FINDSTRING, 0, (LPARAM)bigTempBuffer);

		if (E == CB_ERR)
			SendMessage(hWndCombo, CB_ADDSTRING, 0, (LPARAM) (LPCSTR) bigTempBuffer);
     }
     else
		SendMessage(hWndCombo, CB_ADDSTRING, 0, (LPARAM) (LPCSTR) bigTempBuffer);

	LocalFree(bigTempBuffer);
	return true;
}