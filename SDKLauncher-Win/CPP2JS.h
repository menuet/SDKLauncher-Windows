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

#define _USE_READIUM 1
#include <atlbase.h>
#include "explorer.h"
#include <MsHTML.h>
#include <afxwin.h>
#include <string>

#include <ePub3\initialization.h>
#include <ePub3\archive.h>
#include <ePub3\xml\io.h>
#include <ePub3\filter_manager_impl.h>
#include <ePub3\media-overlays_smil_model.h>

using namespace ePub3;

#include <ePub3\utilities\byte_stream.h>
#include <ePub3\container.h>
#include <ePub3\nav_table.h>

#include "json.h"


/******************************************************************************
 *  class ViewerSettings
 *
 */
class ViewerSettings
{
public:

    bool    mIsSyntheticSpread;
    int     mFontSize;
    int     mColumnGap;

    ViewerSettings(bool isSyntheticSpread, int fontSize, int columnGap)
    {
        mIsSyntheticSpread = isSyntheticSpread;
        mFontSize = fontSize;
        mColumnGap = columnGap;
    }


    bool isSyntheticSpread()
    {
        return mIsSyntheticSpread;
    }

    int getFontSize()
    {
        return mFontSize;
    }

    int getColumnGap()
    {
        return mColumnGap;
    }

    json::Object toJSON() {
        json::Object json;
        json["isSyntheticSpread"] = mIsSyntheticSpread;
        json["fontSize"] = mFontSize;
        json["columnGap"] = mColumnGap;

        return json;
    }

    std::string toString() {
        return "";
        //return std::string(std::string("ViewerSettings [isSyntheticSpread=") + mIsSyntheticSpread + std::string(", fontSize=") + mFontSize + ", columnGap=" + mColumnGap + "]";
    }

};

/******************************************************************************
 *  struct OpenPageRequest
 *
 */
struct OpenPageRequest
{
    std::string		idref;
    int				spineItemPageIndex;
    std::string		elementCfi;
    std::string		contentRefUrl;
    std::string		sourceFileHref;
    std::string		elementId;

public:
    OpenPageRequest(std::string idref_, int spineItemPageIndex_, std::string elementCfi_, std::string contentRefUrl_, std::string sourceFileHref_, std::string elementId_) :
        idref(idref_), spineItemPageIndex(spineItemPageIndex_), elementCfi(elementCfi_), contentRefUrl(contentRefUrl_), sourceFileHref(sourceFileHref_), elementId(elementId_) {
	}

    static OpenPageRequest fromIdref(std::string idref) {
        return OpenPageRequest(idref, 0, "", "", "", "");
    }

    static OpenPageRequest fromIdrefAndIndex(std::string idref, int spineItemPageIndex) {
        return OpenPageRequest(idref, spineItemPageIndex, "", "", "", "");
    }

    static OpenPageRequest fromIdrefAndCfi(std::string idref, std::string elementCfi) {
        return OpenPageRequest(idref, 0, elementCfi, "", "", "");
    }

    static OpenPageRequest fromContentUrl(std::string contentRefUrl, std::string sourceFileHref) {
        return OpenPageRequest("", 0, "", contentRefUrl, sourceFileHref, "");
    }

    static OpenPageRequest fromElementId(std::string idref, std::string elementId){
        return OpenPageRequest(idref, 0, "", "", "", elementId);
    }

    json::Object toJSON()
    {
        json::Object json;
        json["idref"] = idref;
        json["spineItemPageIndex"] = spineItemPageIndex;
        json["elementCfi"] = elementCfi;
        json["contentRefUrl"] = contentRefUrl;
        json["sourceFileHref"] = sourceFileHref;
        json["elementId"] = elementId;

        return json;
    }
};

/******************************************************************************
 *  struct TOCEntry
 *
 */
struct TOCEntry
{
    TOCEntry(){}
    TOCEntry(std::string name_, std::string uri_) :sTOCName(name_), sURI(uri_){}

    std::string             sTOCName;
    std::string             sURI;
    std::vector<TOCEntry>   arrChildren;
};

/******************************************************************************
 *  class ReadiumJSAPI
 * 
 *  This provides the wrappers for the JS functionality, translating from C++ to JS
 */
class ReadiumJSApi
{
    PackagePtr		pkg;	

public:
	ReadiumJSApi(CExplorer*pWebBrowser_);
	ReadiumJSApi();
	~ReadiumJSApi()	{}

    bool		    getByteResp(std::string sURI, BYTE** bytes, ULONGLONG* pSize);
    void	        initReadiumSDK();

    std::list<std::pair<std::string, std::string> >     getSpineList();

    void		    digInto(const NavigationList& lst, TOCEntry& rOut);
    void		    getTOCList(TOCEntry &rOut);
    void		    openEPub3(std::string path);
    std::string     getBookTitle();
    std::string     getBasePath();
    void		    loadJSOnReady(std::string jScript);
    void		    loadJS(std::string jScript);

public:
    CExplorer	    *WebBrowser;

    void			bookmarkCurrentPage();
    void			openPageLeft();
    void			openPageRight();
    json::Array		getJSON(shared_ptr<const ePub3::SMILData::Sequence> seqBody);
    json::Object	getPackageJSON(PackagePtr pckg);
    void			openBook(PackagePtr pkg, ViewerSettings viewerSettings, OpenPageRequest openPageRequestData);
    void			updateSettings(ViewerSettings viewerSettings);
    void			openContentUrl(std::string href, std::string baseUrl);
    void            openSpineItemPage(std::string idRef, int page);
    void            openSpineItemElementCfi(std::string idRef, std::string elementCfi);
    void            nextMediaOverlay();
    void            previousMediaOverlay();
    bool            bMediaOverlayToggled;	// TODO: this state should be in sync
    void            toggleMediaOverlay();
    void            turnMediaOverlayOff();
};
