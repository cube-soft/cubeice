#include "cubeice-propsheet.h"
#include "cubeice-comment.h"
#include "resource.h"

namespace cube {
namespace propsheet {

namespace {

	INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg) {

		case WM_INITDIALOG:
		{
			std::wstring	comment = GetArchiveComment(reinterpret_cast<CShellPropSheetExt*>(reinterpret_cast<LPPROPSHEETPAGE>(lParam)->lParam)->filepath);
			SetDlgItemText(hDlg, IDC_COMMENT, comment.c_str());
			return TRUE;
		}

		/*
		case WM_NOTIFY: {
			LPNMHDR lpNmhdr = (LPNMHDR)lParam;

			if (lpNmhdr->code == PSN_APPLY)
				MessageBox(NULL, TEXT("OKボタンが押されました。"), TEXT("OK"), MB_OK);
			else if (lpNmhdr->code == PSN_RESET)
				MessageBox(NULL, TEXT("ダイアログが閉じられました。"), TEXT("OK"), MB_OK);
			else
				;
			break;
		}
		*/

		default:
			break;

		}

		return FALSE;
	}

	UINT CALLBACK PropSheetPageProc(HWND hwnd, UINT uMsg, LPPROPSHEETPAGE ppsp)
	{
		if (uMsg == PSPCB_RELEASE) {
			CShellPropSheetExt *p = (CShellPropSheetExt *)ppsp->lParam;
			p->Release();
		}

		return 1;
	}

}

STDMETHODIMP CShellPropSheetExt::AddPages(LPFNADDPROPSHEETPAGE pfnAddPage, LPARAM lParam)
{
	PROPSHEETPAGE  psp;
	HPROPSHEETPAGE hPage;

	size_t	pos = filepath.find_last_of('.');
	if(pos == std::wstring::npos || _tcsicmp(filepath.substr(pos + 1).c_str(), TEXT("zip")))
		return S_OK;

	psp.dwSize        = sizeof(PROPSHEETPAGE);
	psp.dwFlags       = PSP_USEREFPARENT | PSP_USETITLE | PSP_USECALLBACK;
	psp.hInstance     = hDllInstance;
	psp.pszTemplate   = MAKEINTRESOURCE(IDD_PROPSHEET1);
	psp.hIcon         = 0;
	psp.pszTitle      = TEXT("CubeICE");
	psp.pfnDlgProc    = (DLGPROC)DialogProc;
	psp.lParam        = (LPARAM)this;
	psp.pfnCallback   = PropSheetPageProc;
	psp.pcRefParent   = (PUINT)&dllRefCount;

	hPage = CreatePropertySheetPage(&psp);
	if (hPage == NULL)
		return E_OUTOFMEMORY;
	
	if (!pfnAddPage(hPage, lParam)) {
		DestroyPropertySheetPage(hPage);
		return E_FAIL;
	}

	AddRef();

	return S_OK;
}

STDMETHODIMP CShellPropSheetExt::ReplacePage(UINT uPageID, LPFNADDPROPSHEETPAGE pfnReplacePage, LPARAM lParam)
{
	return E_NOTIMPL;
}

STDMETHODIMP CShellPropSheetExt::Initialize(PCIDLIST_ABSOLUTE pidlFolder, IDataObject *pdtobj, HKEY hkeyProgID)
{
	HRESULT   hr;
	FORMATETC formatetc;
	STGMEDIUM medium;
	LPTSTR    lpszFilePath;

	formatetc.cfFormat = RegisterClipboardFormat(CFSTR_FILENAME);
	formatetc.ptd      = NULL;
	formatetc.dwAspect = DVASPECT_CONTENT;
	formatetc.lindex   = -1;
	formatetc.tymed    = TYMED_HGLOBAL;

	hr = pdtobj->GetData(&formatetc, &medium);
	if (FAILED(hr))
		return E_FAIL;

	lpszFilePath = (LPTSTR)GlobalLock(medium.hGlobal);
	filepath = lpszFilePath;
	GlobalUnlock(medium.hGlobal);

	ReleaseStgMedium(&medium);

	return S_OK;
}

}
}