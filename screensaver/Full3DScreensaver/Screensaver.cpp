#include <Windows.h>
#include <strsafe.h>

#include <Windowsx.h>

#include <MMSystem.h>
#pragma comment (lib, "Winmm.lib")

#include <d3dx9.h>
#include <d3d9.h>
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")

#define WM_SCRSAVMSG (WM_USER + 33)

LPDIRECT3D9         _D3D = NULL;
LPDIRECT3DDEVICE9   _D3DDevice = NULL;

LPD3DXMESH _Scene					= NULL;
LPD3DXBUFFER _SceneBuffer			= NULL;
D3DMATERIAL9* _SceneMaterials		= NULL;
LPDIRECT3DTEXTURE9* _SceneTexture	= NULL;
DWORD dwNumber;

HRESULT InitD3D(HWND hwnd);
HRESULT LoadSceneFromFile();
void CheckMutex();
void MainRenderProc();
void Render3D();
void SetCamera();
void CleanOutPut();
void CratePrevWindow();
void CrateFullSSWindow();
LRESULT CALLBACK SaverWindowProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
LRESULT WINAPI FullSSWindowMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
LPWSTR FullPathTo(LPWSTR PathToFile, LPCSTR File);
LPWSTR AnsiToUnicode(LPCSTR lpAnsiStr);
void FatalError(LPTSTR lpszFunction);

int color = 0;
int cx,cy;
DWORD mm_xPos, mm_yPos;
HANDLE Pmutex;
HWND mainW, prevW, FullSSWindow;
HINSTANCE ProgHinst;

FLOAT kW;
bool bMouseInside;
float CurentAngleX = 0.0f, CurentAngleY = 0.0f;
DWORD tTime = timeGetTime();
DWORD eTime = 1;
bool lfm_down = false;

TCHAR PathToScene[1024] = {0};
DWORD buf_size = sizeof(PathToScene);

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
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR StartParam, INT )
{
	//MessageBox(NULL,  StartParam , L"?" , MB_OK);
	//wcscpy(StartParam,L"/s");
	//LPWSTR ScreenPath = (LPWSTR ) LocalAlloc(LPTR,256);
	//GetCurrentDirectory(256,ScreenPath);
	//MessageBox(NULL, ScreenPath, SkreenPath, MB_OK);
	ProgHinst = hInst;
	//EnableDebugPrivilege(true);
	HKEY hKey;
	//DWORD dwDispas;


	//if (RegCreateKeyEx(HKEY_CURRENT_USER,
	//	L"Software\\Full3DWallpaper", 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDispas) 
	//	!= ERROR_SUCCESS) FatalError(L"Reg crate key error");

		//if ((dwDispas != REG_CREATED_NEW_KEY) && (dwDispas != REG_OPENED_EXISTING_KEY))
	//if (RegSetValueEx( hKey, L"Path", NULL, REG_SZ, (BYTE*) L"C:\\Program Files\\Full3DWallpaper", 2*sizeof(L"C:\Program Files\Full3DWallpaper"))
	//	!= ERROR_SUCCESS) FatalError(L"Reg crate value error");
		//FatalError(L"Reg");
	//RegCloseKey(hKey);
		//EnableDebugPrivilege(false);

	if (RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\Full3DWallpaper", 0, KEY_ALL_ACCESS, &hKey)
		!= ERROR_SUCCESS) { FatalError(L"Can't find program key in registry"); ExitProcess(NULL);}

	if (RegQueryValueEx( hKey, L"Path", NULL, NULL, (LPBYTE) PathToScene, &buf_size)
		!= ERROR_SUCCESS) {FatalError(L"Can't find program value in registry"); ExitProcess(NULL);}
	//MessageBox(NULL, PathToScene, SkreenPath, MB_OK);

	RegCloseKey(hKey);
	//CheckMutex;
	
if ((StartParam[1] == 'p') || (StartParam[1] == 'P') )
	{
		//MessageBox(NULL, StartParam, L"Param" , MB_OK);

		size_t hStartP = wcscspn(StartParam, L"0123456789");
		size_t hLength = wcslen(StartParam);
		LPWSTR hString = (LPWSTR) LocalAlloc(LPTR,256);
		wcsncpy(hString,StartParam + hStartP, hLength);

		mainW = (HWND) _wtoi(hString);

		CratePrevWindow();
		if ((SUCCEEDED(InitD3D(prevW)))&&(SUCCEEDED(LoadSceneFromFile())))
		{			
		MSG msg;
		ZeroMemory( &msg, sizeof( msg ) );
		while( msg.message != WM_QUIT )
		{
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage ( &msg );
				DispatchMessage ( &msg );
			}
			else MainRenderProc();
		}
		
	} //else FatalError(L"Init 1D3D & Load Scene from path");
	CleanOutPut();
	}
	//////////////////////////SSS//////////////////////

if (( StartParam[1] == 's') || ( StartParam[1] == 'S'))
	{
		//HANDLE Pmutex = CreateMutex(NULL, FALSE, L"Full3DWallpaperMEx");
		//if (GetLastError() == ERROR_ALREADY_EXISTS) //IF RUNING
		CrateFullSSWindow();
		HWND MainProgHwnd = FindWindow(L"WC_Full3DWallpaper", L"Full3DWallpaper");
		if ( MainProgHwnd != NULL)
		{
			SendMessage( MainProgHwnd, WM_SCRSAVMSG, (WPARAM)NULL, (LPARAM)NULL);
		}
		else
		{
		LPWSTR  PathToProg = (LPWSTR) LocalAlloc(LPTR,256);
		StringCbPrintf((LPWSTR) PathToProg, 256 ,L"%s\\%s", PathToScene, L"Full3DWallpaper.exe");

		ShellExecute( NULL, NULL, PathToProg, L"/s", PathToScene, SW_SHOWDEFAULT);//MainProgHwnd L"Open"
		//CreateProcess( NULL,  PathToProg, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, PathToScene, &si, &pi );
		}
		//UINT oldval;
		//SystemParametersInfo( SPI_SETSCREENSAVERRUNNING, TRUE, &oldval, SPIF_SENDCHANGE);//SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);
		
		MSG msg;
		ZeroMemory( &msg, sizeof( msg ));			
		while (GetMessage(&msg,NULL,0,0))
		{	
			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		}
		//UINT oldval; 
		//SystemParametersInfo( SPI_SETSCREENSAVERRUNNING, FALSE, NULL, SPIF_SENDCHANGE | SPIF_UPDATEINIFILE );//SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);
		//SystemParametersInfo( SPI_SETSCREENSAVEACTIVE, FALSE, FALSE ,   NULL);//   SPIF_SENDCHANGE);//SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);
		DestroyWindow(FullSSWindow);
		//Sleep(1700);
	}
	
	//ExitProcess(NULL);
	return 0;

}

void CratePrevWindow()
{
	WNDCLASS wc;
	wc.style=CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc=SaverWindowProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=ProgHinst;
	wc.hIcon=NULL;
	wc.hCursor=NULL;
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName=NULL;
	wc.lpszClassName=L"ScrClass";
	RegisterClass(&wc);

	RECT rc; GetWindowRect(mainW,&rc);
	cx=rc.right-rc.left, cy=rc.bottom-rc.top;  
	prevW = CreateWindowEx(0, L"ScrClass", L"SaverPreview", WS_CHILD|WS_VISIBLE,
                              0,0,cx,cy,mainW,NULL,ProgHinst,NULL);
}
LRESULT CALLBACK SaverWindowProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{
	case WM_MOUSEMOVE:
		{
			if (lfm_down)
			{
			mm_xPos = GET_X_LPARAM (lParam); 
			mm_yPos = GET_Y_LPARAM (lParam); 
			bMouseInside = true;
			
			TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(tme);
            tme.hwndTrack = hwnd;
            tme.dwFlags = TME_LEAVE;
            TrackMouseEvent(&tme);
			}
			break;
		}
	case WM_MOUSELEAVE:
		{
			lfm_down = false;
			bMouseInside = false;
			mm_xPos = 0; mm_yPos = 0;
			CurentAngleY = 0;
			break;
		}
	case WM_LBUTTONUP:
		{
			lfm_down = false;
			break;
		}
	case WM_LBUTTONDOWN:
		{
			lfm_down = true;
			break;
		}
	//case WM_MBUTTONDOWN:
	//{
	//	//MessageBox(NULL, L"LButton click", L"", MB_OK);
	//	PostQuitMessage(0);
	//	break;
	//}
	
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	
	}

	return DefWindowProc(hwnd,msg,wParam,lParam);
}

HRESULT InitD3D(HWND hwnd)
{
if (NULL == (_D3D = Direct3DCreate9(D3D_SDK_VERSION))) return E_FAIL;

	D3DDISPLAYMODE Display;
	if (FAILED(_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&Display))) return E_FAIL;
	
	kW = (float) cx / (float) cy;

	D3DPRESENT_PARAMETERS D3DParam;
	ZeroMemory( &D3DParam, sizeof(D3DParam));
    D3DParam.Windowed = TRUE;
    D3DParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
    D3DParam.BackBufferFormat = Display.Format;
    D3DParam.EnableAutoDepthStencil = TRUE;
    D3DParam.AutoDepthStencilFormat = D3DFMT_D16;
	D3DParam.BackBufferCount = 2;
	D3DParam.hDeviceWindow = hwnd;
	//D3DParam.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
	D3DParam.BackBufferHeight = cy;
	D3DParam.BackBufferWidth  = cx;

	if (FAILED(_D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                   D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                   &D3DParam, &_D3DDevice ) ) )
	if (FAILED(_D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&D3DParam, &_D3DDevice ) ) ) return E_FAIL;
	
	_D3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW); //Crop on
	//_D3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);		//light off
	_D3DDevice->SetRenderState(D3DRS_AMBIENT,0xffffffff);	//light on

	//_D3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);	//z-buff on	AMBIENT
	//_D3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true); //mirror light(SPECULARENABLE)
	return S_OK;
}
void MainRenderProc()
{
	tTime = timeGetTime();
	Render3D();
	eTime = timeGetTime() - tTime;
	if (eTime<20) 
	{	Sleep(20-eTime); eTime = eTime+(20-eTime);}
}
void Render3D()
{
	_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(34,34,34), 1.0f, 0);
	_D3DDevice->BeginScene();

	SetCamera();

	for (DWORD i=0; i<dwNumber; i++)
	{
		_D3DDevice->SetMaterial(&_SceneMaterials[i]);
		_D3DDevice->SetTexture (0, _SceneTexture[i]);
		_Scene->DrawSubset(i);
	}

	_D3DDevice->EndScene();
	_D3DDevice->Present(NULL,NULL,NULL,NULL);
}
void SetCamera()
{
	D3DXMATRIX MatrixWorld,MatrixWorldX,MatrixWorldY;
	D3DXMATRIX MatrixView;
	D3DXMATRIX MatrixProjection;

	//UINT Time = timeGetTime() % 5000;
	//FLOAT Angle = Time * (2.0f * D3DX_PI) / 5000.0f;

	//if ((mm_xPos != 0)&&(mm_yPos != 0))
	//	D3DXMatrixRotationY(&MatrixWorld, mm_xPos);
	//else
	//float AngleX = 0;
	
	if ((bMouseInside)&&(lfm_down))
	{
		//if (CurentAngleX < D3DX_PI) CurentAngleX = CurentAngleX - 0.02f;
		//if (CurentAngleX > D3DX_PI) CurentAngleX = CurentAngleX + 0.02f;
		//if ((CurentAngleX < 0)||(CurentAngleX > D3DX_PI*2)) CurentAngleX = 0;
		CurentAngleX = ((mm_xPos*D3DX_PI)/cx)-D3DX_PI/2;
		CurentAngleY = ((mm_yPos*D3DX_PI)/cx)-D3DX_PI/2;
		//AngleX = mm_xPos/(cx/D3DX_PI*2)-D3DX_PI*2; //AngleX = ((mm_xPos)/((cx/2)/D3DX_PI))-cx/2;
		
		//D3DXMatrixRotationY(&MatrixWorld, AngleX = mm_xPos/(cx/D3DX_PI*2)-D3DX_PI*2);//+ (((float)cx+1)/((float)mm_xPos+1)));
		//if (CurentAngleX != 0)	CurentAngleX = CurentAngleX + 0.1;
	}
	else
	{
		UINT iTime = tTime % 8000;
		CurentAngleX = iTime * ( 2.0f * D3DX_PI ) / 8000.0f;
		//D3DXMatrixRotationY(&MatrixWorld, timeGetTime() / 2300.0f+ AngleX );

		//int RouX = (timeGetTime() / 2300.0f) / (2*D3DX_PI);
		//CurentAngleX = (timeGetTime() / 2300.0f) / RouX;// + CurentAngleX;		
	}
	D3DXMatrixRotationY(&MatrixWorldX,  CurentAngleX);
	D3DXMatrixRotationZ(&MatrixWorldY,  CurentAngleY);
	D3DXMatrixMultiply(&MatrixWorld,&MatrixWorldX,&MatrixWorldY);	
	_D3DDevice->SetTransform(D3DTS_WORLD, &MatrixWorld);

	D3DXMatrixLookAtLH(&MatrixView,
					   &D3DXVECTOR3(12.8f, 0.0f, 1.0f),
					   &D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
					   &D3DXVECTOR3( 0.0f, 1.0f, 0.0f));
	_D3DDevice->SetTransform(D3DTS_VIEW,&MatrixView);

	D3DXMatrixPerspectiveFovLH(&MatrixProjection,
							   D3DX_PI/4, kW, 1.0f, 100.0f);
	_D3DDevice->SetTransform(D3DTS_PROJECTION, &MatrixProjection);	
}
void CheckMutex()
{
	Pmutex = CreateMutex(NULL, FALSE, L"Full3DScreensaverEx");
	if (GetLastError() == ERROR_ALREADY_EXISTS) //IF ALLRADY RUNING
	{		
		//MessageBox(NULL , L"ALLRADY RUN!", L"", MB_OK);
		ReleaseMutex(Pmutex);
		CloseHandle(Pmutex);
		ExitProcess(NULL);
	}
	else //IF NOT RUNING
	{
		
	}
	
}
void CleanOutPut()
{
	//ReleaseMutex(Pmutex);
	//CloseHandle(Pmutex);

	if (_SceneMaterials != NULL) delete[] _SceneMaterials;
	if (_SceneTexture) 
	{
		for (DWORD i=0; i<dwNumber; i++)
			{
				if (_SceneTexture[i])	_SceneTexture[i]->Release();
			}
		delete[] _SceneTexture;
	}
	if (_Scene != NULL) _Scene->Release();

	//MessageBox(NULL , L"EXIT!", L"", MB_OK);
	if (_D3DDevice != NULL) _D3DDevice->Release();
	if (_D3D != NULL) _D3D->Release();
	DestroyWindow(prevW);
}
HRESULT LoadSceneFromFile()
{
	//if (!PathFileExists(ScreenPath)) return E_FAIL;
	//PathToScene + F3DScrSScene.f3d
	//PathToScene + D3DXMeshMaterials[i].pTextureFilename
	LPWSTR  PathToFile1 = (LPWSTR) LocalAlloc(LPTR,256);
	StringCbPrintf((LPWSTR) PathToFile1, 256 ,L"%s\\%s\\%s", PathToScene, L"ScrSsc", L"F3DScrSScene.f3d");

	if (FAILED(D3DXLoadMeshFromX( PathToFile1, D3DXMESH_SYSTEMMEM,//f3DwallPrev.X 
	//if (FAILED(D3DXLoadMeshFromX( L"F3DScrSScene.f3d", D3DXMESH_SYSTEMMEM,//f3DwallPrev.X
		_D3DDevice, NULL, &_SceneBuffer, NULL,
		&dwNumber, &_Scene))) return E_FAIL;

	D3DXMATERIAL* D3DXMeshMaterials;
	D3DXMeshMaterials = (D3DXMATERIAL *) _SceneBuffer->GetBufferPointer();

	_SceneMaterials = new D3DMATERIAL9[dwNumber];
	_SceneTexture = new LPDIRECT3DTEXTURE9[dwNumber];
	for (DWORD i = 0; i< dwNumber; i++)
	{
		_SceneMaterials[i] = D3DXMeshMaterials[i].MatD3D;
		_SceneMaterials[i].Ambient = _SceneMaterials[i].Diffuse;

		if (FAILED (D3DXCreateTextureFromFile(_D3DDevice, FullPathTo( PathToScene, D3DXMeshMaterials[i].pTextureFilename), &_SceneTexture[i])))
			_SceneTexture[i] = NULL;
	}
	_SceneBuffer->Release();

	return S_OK;
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
LPWSTR FullPathTo( LPWSTR PathToFile, LPCSTR File)
{
	if (File == 0) return NULL;
	LPWSTR  f_result = (LPWSTR) LocalAlloc(LPTR,256);

	StringCbPrintf((LPWSTR) f_result, 256 ,L"%s\\%s\\%s", PathToFile, L"ScrSsc", AnsiToUnicode(File));
	return f_result;
}

void CrateFullSSWindow()
{
	WNDCLASSEX mwwc =
    {
        sizeof( WNDCLASSEX ), CS_CLASSDC, FullSSWindowMsgProc, 0L, 0L,//CS_HREDRAW | CS_VREDRAW
        GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
        L"WC_Full3DScrSav", NULL
    };
	mwwc.hIcon = LoadIcon(ProgHinst, MAKEINTRESOURCE(101));
	RegisterClassEx( &mwwc );
	RECT	d_rc;
	GetWindowRect( GetDesktopWindow(), &d_rc);
	FullSSWindow = CreateWindow( L"WC_Full3DScrSav", L"Full3DScreensaver",
                          //WS_DLGFRAME, d_rc.right-400, d_rc.bottom-300, 350, 200,
						  WS_OVERLAPPEDWINDOW, 0, 0, d_rc.right, d_rc.bottom,
                          NULL, NULL, mwwc.hInstance, NULL );
	ShowWindow(FullSSWindow, SW_HIDE);
}
LRESULT WINAPI FullSSWindowMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
switch (msg)
	{
		case WM_SCRSAVMSG:
			{
				PostQuitMessage(0);
				return 0;
			}
	}
return DefWindowProc( hWnd, msg, wParam, lParam );
}

void FatalError(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s:\r\nFailed with error %d:\r\n%s%s"), lpszFunction, dw, lpMsgBuf,L"Send report?"); 

	if (MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error: Send report"), MB_YESNO | MB_ICONERROR | MB_DEFBUTTON1 | MB_APPLMODAL) == IDYES)
	{//OSVERSIONINFO
		ShellExecute(0, L"Open", L"mailto: support@full3dwallpaper.com?subject=fatal error&body= error", L"", L"", 1);
	}

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    //ExitProcess(dw);
	//PostQuitMessage(0);
}