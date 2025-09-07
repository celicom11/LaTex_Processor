#include "stdafx.h"
#include "MathBox\WordItem.h"
#include "MathBox\ContainerItem.h"
#include "MathBox\RadicalBuilder.h"
#include "MathBox\FractionBuilder.h"

// Custom HINST_THISCOMPONENT for module handle
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
template <class T> void SafeRelease(T** ppT) {
   if (*ppT) {
      (*ppT)->Release();
      *ppT = nullptr;
   }
}

// Helper class to manage DirectX resources
struct D2DResources {

   ID2D1Factory* pD2DFactory = nullptr;
   IDWriteFactory* pDWriteFactory = nullptr;
   ID2D1HwndRenderTarget* pRenderTarget = nullptr;
   ID2D1SolidColorBrush* pBlackBrush = nullptr;
   IDWriteFontFile* pFontFile = nullptr;
   IDWriteFontFace* pFontFace = nullptr;
   DWRITE_FONT_METRICS     m_fm;
   ~D2DResources() {
      SafeRelease(&pD2DFactory);
      SafeRelease(&pDWriteFactory);
      SafeRelease(&pRenderTarget);
      SafeRelease(&pBlackBrush);
      SafeRelease(&pFontFile);
      SafeRelease(&pFontFace);
   }

   HRESULT Initialize(HWND hwnd) {
      HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
      if (SUCCEEDED(hr)) {
         hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&pDWriteFactory));
      }

      if (SUCCEEDED(hr)) {
         RECT rc;
         GetClientRect(hwnd, &rc);
         D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
         hr = pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd, size),
            &pRenderTarget
         );
         pRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
      }

      if (SUCCEEDED(hr))
         hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);

      // Correctly load the font file and create a font face directly.
      {
         WCHAR pstrExePath[MAX_PATH] = { 0 };
         GetCurrentDirectoryW(_countof(pstrExePath), pstrExePath);
         wstring sFontPath = wstring(pstrExePath) + L"\\LatinModernMath\\latinmodern-math.otf";

         hr = pDWriteFactory->CreateFontFileReference(sFontPath.c_str(), nullptr, &pFontFile);
         if (FAILED(hr)) {
            //MessageBox(m_hwnd, L"Could not create font file reference. Make sure latinmodern-math.otf is in the same folder as the executable.", L"Error", MB_OK);
            return hr;
         }
         BOOL isSupported;
         DWRITE_FONT_FILE_TYPE fileType;
         DWRITE_FONT_FACE_TYPE faceType;
         UINT32 numberOfFonts;
         pFontFile->Analyze(&isSupported, &fileType, &faceType, &numberOfFonts);

         IDWriteFontFile* fontFileArray[] = { pFontFile };
         hr = pDWriteFactory->CreateFontFace(
            faceType, //DWRITE_FONT_FACE_TYPE_CFF,
            1, // file count
            fontFileArray,
            0, // face index
            DWRITE_FONT_SIMULATIONS_NONE,
            &pFontFace
         );
         if (FAILED(hr)) {
            //MessageBox(m_hwnd, L"Could not create font face from file.", L"Error", MB_OK);
            return hr;
         }
         // Font metrics for baseline calculations (design units)
         pFontFace->GetMetrics(&m_fm);
         _ASSERT(m_fm.designUnitsPerEm == otfUnitsPerEm);

      }


      return hr;
   }
};

// --- Main Application Class ---

class DemoApp {
   float               m_fFontSizePt{ 12.0f }; //document font size, in points
   HWND                m_hwnd;
   D2DResources        m_d2d;
   CContainerItem      m_MainBox;//TODO: HBOX
public:
   DemoApp() : m_hwnd(nullptr), m_MainBox(eacHBOX, etsDisplay) {}
   ~DemoApp() {}

   HRESULT Initialize() {
      WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
      wcex.style = CS_HREDRAW | CS_VREDRAW;
      wcex.lpfnWndProc = DemoApp::WndProc;
      wcex.cbClsExtra = 0;
      wcex.cbWndExtra = sizeof(LONG_PTR);
      wcex.hInstance = HINST_THISCOMPONENT;
      wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
      wcex.lpszClassName = L"MathBoxDemoApp";

      RegisterClassEx(&wcex);

      m_hwnd = CreateWindow(
         L"MathBoxDemoApp",
         L"MathBox Demo",
         WS_OVERLAPPEDWINDOW,
         CW_USEDEFAULT, CW_USEDEFAULT,
         1000, 800,
         nullptr, nullptr, HINST_THISCOMPONENT, this
      );

      HRESULT hr = m_hwnd ? S_OK : E_FAIL;
      if (SUCCEEDED(hr)) {
         hr = m_d2d.Initialize(m_hwnd);
         if (SUCCEEDED(hr)) {
            BuildMainBox_();
            ShowWindow(m_hwnd, SW_SHOWNORMAL);
            UpdateWindow(m_hwnd);
         }
      }
      return hr;
   }

   void RunMessageLoop() {
      MSG msg;
      while (GetMessage(&msg, nullptr, 0, 0)) {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
   }

private:
   static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
      if (message == WM_CREATE) {
         LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
         DemoApp* pApp = (DemoApp*)pcs->lpCreateParams;
         SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pApp);
         return 1;
      }

      DemoApp* pApp = reinterpret_cast<DemoApp*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
      if (pApp) {
         switch (message) {
         case WM_PAINT:
            pApp->OnPaint();
            return 0;
         case WM_SIZE:
            if (pApp->m_d2d.pRenderTarget) {
               D2D1_SIZE_U size = D2D1::SizeU(LOWORD(lParam), HIWORD(lParam));
               pApp->m_d2d.pRenderTarget->Resize(size);
               InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
         case WM_DESTROY:
            PostQuitMessage(0);
            return 1;
         }
      }
      return DefWindowProc(hwnd, message, wParam, lParam);
   }

   void OnPaint() {
      m_d2d.pRenderTarget->BeginDraw();
      m_d2d.pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
      SDWRenderInfo dwri{
          false,
          m_fFontSizePt,
          m_d2d.pRenderTarget,
          m_d2d.pFontFace,
          m_d2d.pBlackBrush,
          nullptr
      };
      m_MainBox.Draw({ 10.0f,30.0f }, dwri);

      HRESULT hr = m_d2d.pRenderTarget->EndDraw();
      if (FAILED(hr)) {
         wchar_t errorMsg[256];
         swprintf_s(errorMsg, L"EndDraw failed with HRESULT: 0x%08X", hr);
         MessageBox(m_hwnd, errorMsg, L"Render Error", MB_OK);
      }
      ValidateRect(m_hwnd, nullptr);
   }

   // The core logic to render the growing radical
   void BuildMainBox_() {
      HRESULT hRes;
      CRadicalBuilder radicalBuilder(m_d2d.pFontFace);

      CMathStyle style(m_MainBox.GetStyle());
      const float aFontSize[] = { 9, 12 , 14, 18 , 21, 24, 30, 40, 50};
      //const UINT32 uRad1 = 0x01D453; //'𝘧' (MATHEMATICAL ITALIC SMALL F)
      //const UINT32 uRad2 = L'P';
      uint32_t nLeftEm = 0;
      CMathStyle styleNumerator(m_MainBox.GetStyle());
      styleNumerator.SetCramped(true);
      CMathStyle styleDenom(m_MainBox.GetStyle());
      styleDenom.SetCramped(true);
      styleDenom.Decrease();
      CMathStyle styleDegree(etsScriptScript);
      for (float fSizePt : aFontSize) {
         // TEST: radicand is a fraction now: \sqrt[ABC]{\frac{1}{2}}
         CWordItem* pNum = new CWordItem(m_d2d.pFontFace, styleNumerator, eacUNK, fSizePt / m_fFontSizePt);
         hRes = pNum->SetText({ L'1' });
         if (FAILED(hRes))
            return;//ERROR
         CWordItem* pDenom = new CWordItem(m_d2d.pFontFace, styleDenom, eacUNK, 1.0f);
         hRes = pDenom->SetText({ L'2' });
         CMathItem* pRadicand = CFractionBuilder::BuildFraction(m_MainBox.GetStyle(), 1.0f, pNum, pDenom);
         //Degree/Index
         CWordItem* pRadDegree = new CWordItem(m_d2d.pFontFace, styleDegree, eacUNK);
         //mathit ABC
         hRes = pRadDegree->SetText({ 0x1D434,0x1D435, 0x1D436 });
         if (FAILED(hRes))
            return;//ERROR

         CMathItem* pRadical0 = radicalBuilder.BuildRadical(m_MainBox.GetStyle(), 1.0f, pRadicand, pRadDegree);
         if (!pRadical0) {
            _ASSERT(0);
            delete pRadicand;
            delete pRadDegree;
            return;
         }
         CMathItem* pRadical = radicalBuilder.BuildRadical(styleNumerator, 1.0f, pRadical0);
         if (!pRadical) {
            _ASSERT(0);
            delete pRadicand;
            delete pRadDegree;
            return;
         }
         int32_t nNextY = 0;
         if (!m_MainBox.Box().IsEmpty())//keep baselines aligned
            nNextY = m_MainBox.Box().BaselineY() - pRadical->Box().Ascent();
         m_MainBox.AddBox(pRadical, nLeftEm, nNextY);
         nLeftEm += pRadical->Box().Width() + 1600;
      }
      m_MainBox.NormalizeOrigin(0, 0);
   }
};

// --- WinMain Entry Point ---

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
   HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);
   if (SUCCEEDED(CoInitialize(nullptr))) {
      DemoApp app;
      if (SUCCEEDED(app.Initialize())) {
         app.RunMessageLoop();
      }
      CoUninitialize();
   }
   return 0;
}

