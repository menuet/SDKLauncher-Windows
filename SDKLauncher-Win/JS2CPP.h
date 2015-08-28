//
//  Copyright (c) 2014 Readium Foundation and/or its licensees. All rights reserved.
//  
//  Redistribution and use in source and binary forms, with or without modification,
//  are permitted provided that the following conditions are met :
//  1. Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation and / or
//  other materials provided with the distribution.
//  3. Neither the name of the organization nor the names of its contributors may
//  be used to endorse or promote products derived from this software without
//  specific prior written permission.
//  
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
//  THE POSSIBILITY OF SUCH DAMAGE.#include "stdafx.h"

#pragma once
#include <MsHtmHst.h>
#include <exdisp.h>
#include <Mshtml.h>

#include "atlbase.h"
#include "atlcom.h"
#include "atldispa.h"

// ReadiumJS -> C++ callbacks
class CMyObject :
    public IDispDynImpl<CMyObject>
{
public:
    BEGIN_DISPATCH_MAP(CMyObject)
        // Methods		
        DISP_METHOD1(onPaginationChanged, VT_EMPTY, VT_BSTR)
        DISP_METHOD1(onMediaOverlayStatusChanged, VT_EMPTY, VT_BSTR)
        DISP_METHOD1(onIsMediaOverlayAvailable, VT_EMPTY, VT_BSTR)
        DISP_METHOD0(onMediaOverlayTTSSpeak, VT_EMPTY)
        DISP_METHOD0(onMediaOverlayTTSStop, VT_EMPTY)
        DISP_METHOD1(getBookmarkData, VT_EMPTY, VT_BSTR)

        DISP_METHOD1(onOpenPage, VT_EMPTY, VT_BSTR)	//external.onOpenPage(JSON.stringify(pageChangeData.paginationInfo));

        

        DISP_METHOD0(onReaderInitialized, VT_EMPTY)
        DISP_METHOD0(onSettingsApplied, VT_EMPTY)
        DISP_METHOD0(onContentLoaded, VT_EMPTY)
        DISP_METHOD0(onPageLoaded, VT_EMPTY)

        // Properties
        //DISP_PROP(PropReadWrite, VT_BOOL)
        //DISP_PROPGET(PropReadOnly, VT_BSTR)
        //DISP_PROPPUT(PropAssignOnly, VT_I4)

        //SHORT __stdcall get_PropReadWrite();
        //void __stdcall put_PropReadWrite(SHORT value);
        //BSTR __stdcall get_PropReadOnly();
        //void __stdcall put_PropAssignOnly(LONG value);

    END_DISPATCH_MAP()

    void __stdcall getBookmarkData(BSTR currentPagesInfo)
    {
        /*
        AlertDialog.Builder builder = new AlertDialog.Builder(WebViewActivity.this).
        setTitle(R.string.add_bookmark);

        final EditText editText = new EditText(WebViewActivity.this);
        editText.setId(android.R.id.edit);
        editText.setHint(R.string.title);
        builder.setView(editText);
        builder.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {

            @Override
            public void onClick(DialogInterface dialog, int which) {
                if (which == DialogInterface.BUTTON_POSITIVE) {
                    String title = editText.getText().toString();
                    try {
                        JSONObject bookmarkJson = new JSONObject(bookmarkData);
                        BookmarkDatabase.getInstance().addBookmark(mContainer.getName(), title,
                        bookmarkJson.getString("idref"), bookmarkJson.getString("contentCFI"));
                    }
                    catch (JSONException e) {
                        Log.e(TAG, "" + e.getMessage(), e);
                    }
                }
            }
        });
        builder.setNegativeButton(android.R.string.cancel, null);
        builder.show();
        */

    }
    void __stdcall onPaginationChanged(BSTR currentPagesInfo)
    {
        //Log.d(TAG, "onPaginationChanged: " + currentPagesInfo);
        /*
        try
        {
            PaginationInfo paginationInfo = PaginationInfo.fromJson(currentPagesInfo);
            List<Page> openPages = paginationInfo.getOpenPages();
            if (!openPages.isEmpty()) {
                final Page page = openPages.get(0);

                runOnUiThread(new Runnable() {
                    public void run() {
                        mPageInfo.setText(getString(R.string.page_x_of_y,
                        page.getSpineItemPageIndex() + 1,
                        page.getSpineItemPageCount()));
                        SpineItem spineItem = mPackage.getSpineItem(page.getIdref());
                        boolean isFixedLayout = spineItem.isFixedLayout();
                        mWebview.getSettings().setBuiltInZoomControls(isFixedLayout);
                        mWebview.getSettings().setDisplayZoomControls(false);
                    }
                });
            }
        }
        catch (JSONException e)
        {
        Log.e(TAG, "" + e.getMessage(), e);
        }
        */

    }
    void __stdcall onIsMediaOverlayAvailable(BSTR currentPagesInfo)
    {
        //Log.d(TAG, "onIsMediaOverlayAvailable:" + available);
        //mIsMoAvailable = available.equals("true");
        //invalidateOptionsMenu();

    }
    void __stdcall onOpenPage(BSTR currentPagesInfo)
    {

    }
    void __stdcall onMediaOverlayStatusChanged(BSTR currentPagesInfo)
    {
        //Log.d(TAG, "onMediaOverlayStatusChanged:" + status);
        //this should be real json parsing if there will be more data that needs to be extracted

        /*if (status.indexOf("isPlaying") > -1) {
            mIsMoPlaying = status.indexOf("\"isPlaying\":true") > -1;
        }

        invalidateOptionsMenu();*/

    }
    void __stdcall onMediaOverlayTTSSpeak()
    {
        //Log.d(TAG, "onMediaOverlayTTSSpeak");
    }
    void __stdcall onMediaOverlayTTSStop()
    {
        //Log.d(TAG, "onMediaOverlayTTSStop");
    }

    void __stdcall onReaderInitialized()
    {
        //		Log.d(TAG, "onReaderInitialized");
    }
    void __stdcall onSettingsApplied()
    {
        //Log.d(TAG, "onSettingsApplied");
    }
    void __stdcall onContentLoaded()
    {
        //Log.d(TAG, "onContentLoaded");

    }
    void __stdcall onPageLoaded()
    {
        //Log.d(TAG, "onPageLoaded()");

    }

};

// COM IDocHostUIHandler for Trident Browser Control
class MyDocHostUIHandler : public IDocHostUIHandler
{
private:
    int ref_count;
    CMyObject disp;
public:
    MyDocHostUIHandler() { ref_count = 0; };

    //   IDocHostUIHandler   Method  
    STDMETHOD(ShowContextMenu)(
        /*   [in]   */   DWORD   dwID,
        /*   [in]   */   POINT   __RPC_FAR   *ppt,
        /*   [in]   */   IUnknown   __RPC_FAR   *pcmdtReserved,
        /*   [in]   */   IDispatch   __RPC_FAR   *pdispReserved)
    {
        return   E_NOTIMPL;
    }

    STDMETHODIMP_(ULONG) AddRef(){
        return 1;
        //return ++ref_count;
    }

    STDMETHODIMP_(ULONG) Release(){
        return 1;
        //return --ref_count;
    }

    STDMETHODIMP QueryInterface(REFIID iid, LPVOID* ppvObj){
        return   E_NOTIMPL;
        if (iid == IID_IUnknown || iid == IID_IDocHostUIHandler) {
            *ppvObj = this;
            return S_OK;
        }
        return S_FALSE;
    }

    STDMETHOD(GetHostInfo)(
        /*   [out][in]   */   DOCHOSTUIINFO   __RPC_FAR   *pInfo)
    {
        return   E_NOTIMPL;
        pInfo->cbSize = sizeof(DOCHOSTUIINFO);
        pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_SCROLL_NO;
        pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
        return S_OK;
    }

    STDMETHOD(ShowUI)(
        /*   [in]   */   DWORD   dwID,
        /*   [in]   */   IOleInPlaceActiveObject   __RPC_FAR   *pActiveObject,
        /*   [in]   */   IOleCommandTarget   __RPC_FAR   *pCommandTarget,
        /*   [in]   */   IOleInPlaceFrame   __RPC_FAR   *pFrame,
        /*   [in]   */   IOleInPlaceUIWindow   __RPC_FAR   *pDoc){
        return   E_NOTIMPL;
    }

    STDMETHOD(HideUI)(void){ return   E_NOTIMPL; }

    STDMETHOD(UpdateUI)(void){ return   E_NOTIMPL; }

    STDMETHOD(EnableModeless)(
        /*   [in]   */   BOOL   fEnable){
        return   E_NOTIMPL;
    }

    STDMETHOD(OnDocWindowActivate)(
        /*   [in]   */   BOOL   fActivate){
        return   E_NOTIMPL;
    }

    STDMETHOD(OnFrameWindowActivate)(
        /*   [in]   */   BOOL   fActivate){
        return   E_NOTIMPL;
    }

    STDMETHOD(ResizeBorder)(
        /*   [in]   */   LPCRECT   prcBorder,
        /*   [in]   */   IOleInPlaceUIWindow   __RPC_FAR   *pUIWindow,
        /*   [in]   */   BOOL   fRameWindow){
        return   E_NOTIMPL;
    }

    STDMETHOD(TranslateAccelerator)(
        /*   [in]   */   LPMSG   lpMsg,
        /*   [in]   */   const   GUID   __RPC_FAR   *pguidCmdGroup,
        /*   [in]   */   DWORD   nCmdID){
        return   E_NOTIMPL;
    }

    STDMETHOD(GetOptionKeyPath)(
        /*   [out]   */   LPOLESTR   __RPC_FAR   *pchKey,
        /*   [in]   */   DWORD   dw){
        return   E_NOTIMPL;
    }

    STDMETHOD(GetDropTarget)(
        /*   [in]   */   IDropTarget   __RPC_FAR   *pDropTarget,
        /*   [out]   */   IDropTarget   __RPC_FAR   *__RPC_FAR   *ppDropTarget){
        return   E_NOTIMPL;
    }

    STDMETHOD(GetExternal)(
        /*   [out]   */   IDispatch   __RPC_FAR   *__RPC_FAR   *ppDispatch){
        //return   E_NOTIMPL;

        *ppDispatch = &disp;
        return   S_OK;
        return   E_NOTIMPL;
    }

    STDMETHOD(TranslateUrl)(
        /*   [in]   */   DWORD   dwTranslate,
        /*   [in]   */   OLECHAR   __RPC_FAR   *pchURLIn,
        /*   [out]   */   OLECHAR   __RPC_FAR   *__RPC_FAR   *ppchURLOut){
        return   E_NOTIMPL;
    }

    STDMETHOD(FilterDataObject)(
        /*   [in]   */   IDataObject   __RPC_FAR   *pDO,
        /*   [out]   */   IDataObject   __RPC_FAR   *__RPC_FAR   *ppDORet){
        return   E_NOTIMPL;
    }
};
