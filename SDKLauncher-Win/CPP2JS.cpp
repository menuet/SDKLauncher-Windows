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

#include "stdafx.h"
#include "CPP2JS.h"
#include <ePub3/utilities/error_handler.h>
#include <ePub3/content_module_manager.h>

//CCriticalSection g_cs;

void ReadiumJSApi::digInto(const NavigationList& list, TOCEntry& rOut)
{
    std::string sBaseUrl = getBasePath();	// fast fix
    int iTrimLen = sBaseUrl.length();

    for (NavigationList::const_iterator v = list.begin(); v != list.end(); ++v)
    {
        NavigationPoint* pt = dynamic_cast<NavigationPoint*>(v->get());
        std::string sTitle = pt->Title().c_str();
        std::string sPath = pt->AbsolutePath(pkg).c_str();
        if (0 == sPath.find(sBaseUrl))
        {
            sPath = sPath.substr(iTrimLen, sPath.length());
        }
        rOut.arrChildren.push_back(TOCEntry(sTitle, sPath));
        
        const NavigationList& list = pt->Children();
        digInto(list, (rOut.arrChildren.back()) );
    }
}

void ReadiumJSApi::getTOCList(TOCEntry &rOut)
{
    if (!pkg)
        return;

    rOut = TOCEntry("TOC", "");

    shared_ptr<NavigationTable> toc = pkg->TableOfContents();
    const NavigationList& list = toc->Children();

    digInto(list, rOut);
}

std::list<std::pair<std::string, std::string>> ReadiumJSApi::getSpineList()
{

    std::list<std::pair<std::string, std::string> > ret;
    if (!pkg)
        return ret;
    
    size_t                  idx = 0;
    shared_ptr<SpineItem>   pSpineItem = pkg->SpineItemAt(idx++);

    while (pSpineItem != 0)
    {
        json::Object curItem;
        shared_ptr<ManifestItem>    manifestItem = pSpineItem->ManifestItem();
 
        if (manifestItem)
        {
            ret.push_back(std::make_pair(manifestItem->BaseHref().c_str(), manifestItem->BaseHref().c_str()));
        }

        pSpineItem = pkg->SpineItemAt(idx++);   //[dict setObject : _idref forKey : @"idref"];
    }


    return ret;
}

std::string ReadiumJSApi::getBasePath()
{
    if (pkg)
    {
        return pkg->BasePath().c_str();
    }
    return "";
}

std::string ReadiumJSApi::getBookTitle()
{
    if (pkg)
    {
        pkg->BasePath();
        return pkg->Title().c_str();
    }
    return "";
}


static bool m_ignoreRemainingErrors = false;

void ReadiumJSApi::openEPub3(std::string path)	
{
    m_ignoreRemainingErrors = false;

    try{
        DWORD beginTimeMs = GetTickCount();
        ContainerPtr container = Container::OpenContainer(path);
        DWORD containerOpened = GetTickCount();
        
        if (container)
        {
            pkg = container->DefaultPackage();
            if (pkg == 0)
            {
                auto pkgs = container->Packages();
                if (pkgs.size() <= 0)
                {
					MessageBox(NULL, _T("ReadiumSDK: No packages found !"), _T("Error"), MB_OK);
                    return;
                }
                pkg = pkgs[0];
            }

            // by requesting an empty URL we effectively request the first spine item, usually the title page
            OpenPageRequest req = OpenPageRequest("", 0, "", "", "", "");

            ViewerSettings set(true, 100, 20);
            
            DWORD openBookStartedMs = GetTickCount();
            openBook(pkg, set, req);
            DWORD EndTimeTimeMs = GetTickCount();
        }
    }
    //catch (ePub3::epub_spec_error err)
    //{

    //}
    catch (...)
    {
        throw;
    }
    
}

bool ReadiumJSApi::getByteResp(std::string sURI, BYTE** bytes, ULONGLONG* pSize)
{
    if (!pkg) {
        return false;
    }
    
	std::string str = sURI.substr(1);

    if (str.length() == 0) 
		return false;

    unique_ptr<ByteStream> stream = pkg->ReadStreamForRelativePath(str);
    if (stream)
    {
        ByteStream::size_type bytesAvailable = stream->BytesAvailable();
        if (bytesAvailable > 0)
        {
            char* buf = new char[bytesAvailable];
            memset(buf, 0, bytesAvailable);
            stream->ReadBytes(buf, bytesAvailable);
            *pSize = bytesAvailable;
            *bytes = (BYTE*)buf;
            return true;
        }
    }
    return false;
}

json::Array ReadiumJSApi::getJSON(shared_ptr<const ePub3::SMILData::Sequence> seqBody)
{
    json::Array ret;
    //TODO: populateChildren
    const shared_vector<const ePub3::SMILData::TimeContainer>::size_type k = seqBody->GetChildrenCount();
    shared_vector<const SMILData::TimeContainer>::size_type i = 0;
    for (; i < k; ++i)
    {
        shared_ptr<const SMILData::TimeContainer> timeContainer = seqBody->GetChild(i);
        json::Object obj;

        if (timeContainer->IsParallel())
        {
            auto para = std::dynamic_pointer_cast<const ePub3::SMILData::Parallel>(timeContainer);
            auto audio = para->Audio();
            auto text = para->Text();
            json::Array ret1;
            if (audio)
            {
                json::Object obj1;

                {
                    std::ostringstream stringStream;
                    stringStream << audio->ClipEndMilliseconds() / 1000.;
                    std::string copyOfStr = stringStream.str();

                    obj1["clipEnd"] = copyOfStr;
                }

                obj1["nodeType"] = std::string("audio");
                obj1["src"] = std::string(audio->SrcFile().c_str());	//"audio\/mobydick_001_002_melville.mp4",

                {
                    std::ostringstream stringStream;
                    stringStream << audio->ClipBeginMilliseconds() / 1000.;
                    std::string copyOfStr = stringStream.str();

                    obj1["clipBegin"] = copyOfStr;
                }
                ret1.push_back(obj1);
            }
            if (text)
            {
                json::Object obj2;

                obj2["srcFile"] = std::string(text->SrcFile().c_str());				//"chapter_002.xhtml",
                obj2["srcFragmentId"] = std::string(text->SrcFragmentId().c_str());	//"c02h01",
                obj2["nodeType"] = std::string("text");
                obj2["src"] = std::string(text->SrcFile().c_str()) + "#" + std::string(text->SrcFragmentId().c_str());    // "chapter_002.xhtml#c02h01"

                ret1.push_back(obj2);
            }
            obj["children"] = ret1;
            obj["nodeType"] = std::string("par");
        }
        else if (timeContainer->IsSequence())
        {
            auto sequence = std::dynamic_pointer_cast<const ePub3::SMILData::Sequence>(timeContainer);
            obj["textref"] = std::string(sequence->TextRefFile().c_str());
            //obj["textref"] =
            json::Array children = getJSON(sequence);
            obj["children"] = children;
            obj["nodeType"] = std::string("seq");
        }

        //obj["nodeType"] = std::string(timeContainer->Type().c_str());
        obj["epubtype"] = std::string("");

        //"textref" : "",
        //Sequence
        //timeContainer->Name().c_str();
        //"textref" : "",
        //"epubtype" : ""
        //"children" : []
        //qDebug() << obj;
        ret.push_back(obj);
    }
    return ret;
}
json::Object ReadiumJSApi::getPackageJSON(PackagePtr pckg)
{
    json::Object obj;

    // Level 0
    {
        obj["rootUrl"] = std::string("/");			//[dict setObject : @"/" forKey:@"rootUrl"];
        obj["rendition_layout"] = std::string("");	//[dict setObject : _rendition_layout forKey : @"rendition_layout"];
        obj["rendition_flow"] = std::string("");	//[dict setObject : _rendition_flow forKey : @"rendition_flow"];

        json::Object spine;

        json::Array spineItems;

        size_t idx = 0;
        shared_ptr<SpineItem>   pSpineItem = pckg->SpineItemAt(idx++);
        while (pSpineItem != 0)
        {
            json::Object curItem;
            shared_ptr<ManifestItem>    manifestItem = pSpineItem->ManifestItem();
            if (manifestItem)
            {
                 curItem["href"] = std::string(manifestItem->BaseHref().c_str());	//[dict setObject : _href forKey : @"href"];
            }
            else
                curItem["href"] = std::string("");


            curItem["idref"] = std::string(pSpineItem->Idref().c_str());	//[dict setObject : _idref forKey : @"idref"];
            curItem["media_type"] = std::string(pSpineItem->ManifestItem()->MediaType().c_str());	//[dict setObject : _idref forKey : @"idref"];

            //pSpineItem->Spread()
            //enum class PageSpread
            //{
            //	Automatic,              ///< No value specified by the author.
            //	Left,                   ///< This is the left page of a spread.
            //	Right,                  ///< This is the right page of a spread.
            //	Center,                 ///< This is a double-width page, spread across both left & right.
            //};


            curItem["page_spread"] = std::string("");       //[dict setObject : _page_spread forKey : @"page_spread"];
            curItem["rendition_layout"] = std::string("");  //[dict setObject : _rendition_layout forKey : @"rendition_layout"];
            curItem["rendition_spread"] = std::string("");  //[dict setObject : _rendition_spread forKey : @"rendition_spread"];
            curItem["rendition_flow"] = std::string("");    //[dict setObject : _rendition_flow forKey : @"rendition_flow"];
            curItem["media_overlay_id"] = std::string("");  //[dict setObject : _media_overlay_id forKey : @"media_overlay_id"];
            spineItems.push_back(curItem);

            pSpineItem = pckg->SpineItemAt(idx++);
        }

        spine["items"] = spineItems;
        spine["direction"] = std::string("default"); //[dict setObject : _direction forKey : @"direction"];
        obj["spine"] = spine;                        //[dict setObject : [_spine toDictionary] forKey : @"spine"];


        json::Object media_overlay;

        {
            std::shared_ptr<MediaOverlaysSmilModel>   smilModel = pckg->MediaOverlaysSmilModel();
            std::vector<std::shared_ptr<SMILData>>::size_type n = smilModel->GetSmilCount();
            json::Array smil_models;
            std::vector<std::shared_ptr<SMILData>>::size_type i = 0;
            for (i = 0; i < n; ++i)
            {
                std::shared_ptr<SMILData> curSmil = smilModel->GetSmil(i);
                json::Object smilModel;

                if (curSmil->XhtmlSpineItem())
                    smilModel["spineItemId"] = std::string(curSmil->XhtmlSpineItem()->Idref().c_str());
                else
                    smilModel["spineItemId"] = std::string("");

                //smilModel["id"]

                std::ostringstream stringStream;
                stringStream << curSmil->DurationMilliseconds_Calculated() / 1000.;
                std::string copyOfStr = stringStream.str();

                smilModel["duration"] = copyOfStr;

                if (curSmil->SmilManifestItem())
                {
                    smilModel["id"] = std::string(curSmil->SmilManifestItem()->Identifier().c_str());
                    smilModel["href"] = std::string(curSmil->SmilManifestItem()->Href().c_str());
                }
                else
                {
                    smilModel["id"] = std::string("");
                    smilModel["href"] = std::string("fake.smil");
                }

                smilModel["smilVersion"] = std::string("3.0");
                //[dict setObject : self.children forKey : @"children"];


                shared_ptr<const ePub3::SMILData::Sequence> seqBody = curSmil->Body();
                json::Array arrChildren = getJSON(seqBody);
                smilModel["children"] = arrChildren;
                smil_models.push_back(smilModel);
            }

            /*
            [dict setObject : smilDictionaries forKey : @"smil_models"];
            {
            for (DNSmilModel *mo in _smilModels) {
            [smilDictionaries addObject : [mo toDictionary]];
            }
            [
            [dict setObject : self.id forKey : @"id"];
            [dict setObject : self.spineItemId forKey : @"spineItemId"];
            [dict setObject : self.href forKey : @"href"];
            [dict setObject : self.smilVersion forKey : @"smilVersion"];

            [dict setObject : self.duration forKey : @"duration"];

            [dict setObject : self.children forKey : @"children"];
            ]
            }*/
            media_overlay["smil_models"] = smil_models;

            json::Array skippables;
            json::Array escapables;

            smilModel = pckg->MediaOverlaysSmilModel();
            
            if (smilModel)
            {
                std::vector<string>::size_type nCount = smilModel->GetSkippablesCount();
                for (std::vector<string>::size_type i = 0; i < nCount; ++i)
                {
                    string sSkippable = smilModel->GetSkippable(i);
                    skippables.push_back(sSkippable.c_str());
                }

                nCount = smilModel->GetEscapablesCount();
                for (std::vector<string>::size_type i = 0; i < nCount; ++i)
                {
                    string sEsc = smilModel->GetEscapable(i);
                    escapables.push_back(sEsc.c_str());
                }

            }
            media_overlay["skippables"] = skippables;//[dict setObject : self.skippables forKey : @"skippables"];
            media_overlay["escapables"] = escapables;//[dict setObject : self.escapables forKey : @"escapables"];
            media_overlay["duration"] = std::string(pckg->MediaOverlays_DurationTotal().c_str()); //[dict setObject : self.duration forKey : @"duration"]; = 1403.5
            media_overlay["narrator"] = std::string(pckg->MediaOverlays_Narrator().c_str());//[dict setObject : self.narrator forKey : @"narrator"];
            media_overlay["activeClass"] = std::string(pckg->MediaOverlays_ActiveClass().c_str());//[dict setObject : self.activeClass forKey : @"activeClass"];, "activeClass" : "-epub-media-overlay-active",
            media_overlay["playbackActiveClass"] = std::string(pckg->MediaOverlays_PlaybackActiveClass().c_str());//[dict setObject : self.playbackActiveClass forKey : @"playbackActiveClass"];
        }
        obj["media_overlay"] = media_overlay; //[dict setObject : [_mediaOverlay toDictionary] forKey : @"media_overlay"];

        return obj;
    }
		
    return obj;
}

void ReadiumJSApi::openBook(PackagePtr pckg, ViewerSettings viewerSettings, OpenPageRequest openPageRequestData)
{
    json::Object openBookData;
    try {
        openBookData["package"] = getPackageJSON(pckg);
        openBookData["settings"] = viewerSettings.toJSON();
        openBookData["openPageRequest"] = openPageRequestData.toJSON();
    }
    catch (...)
    {
        //	Log.e(TAG, "" + e.getMessage(), e);
    }
    //catch (JSONException e) {
    //	Log.e(TAG, "" + e.getMessage(), e);
    //}

    std::string str = json::Serialize(openBookData);
    loadJSOnReady("ReadiumSDK.reader.openBook(" + str + ");");
}

void ReadiumJSApi::updateSettings(ViewerSettings viewerSettings)
{
    try {
        std::string str = json::Serialize(viewerSettings.toJSON());
        loadJSOnReady(std::string("ReadiumSDK.reader.updateSettings(") + str + ");");
    }
    //catch (JSONException e) {
    //	Log.e(TAG, "" + e.getMessage(), e);
    //}
    catch (...)
    {
        //	Log.e(TAG, "" + e.getMessage(), e);
    }
}

void ReadiumJSApi::openContentUrl(std::string href, std::string baseUrl)
{
    loadJSOnReady("ReadiumSDK.reader.openContentUrl(\"" + href + "\", \"" + baseUrl + "\");");
}

void ReadiumJSApi::openSpineItemPage(std::string idRef, int page)
{
    loadJSOnReady("ReadiumSDK.reader.openSpineItemPage(\"" + idRef + "\", " + std::to_string(page) + ");");
}

void ReadiumJSApi::openSpineItemElementCfi(std::string idRef, std::string elementCfi)
{
    loadJSOnReady("ReadiumSDK.reader.openSpineItemElementCfi(\"" + idRef + "\",\"" + elementCfi + "\");");
}

void ReadiumJSApi::nextMediaOverlay()
{
    loadJSOnReady("ReadiumSDK.reader.nextMediaOverlay();");
}

void ReadiumJSApi::previousMediaOverlay()
{
    loadJSOnReady("ReadiumSDK.reader.previousMediaOverlay();");
}

void ReadiumJSApi::toggleMediaOverlay()
{
    bMediaOverlayToggled = (bMediaOverlayToggled) ? false : true;
    loadJSOnReady("ReadiumSDK.reader.toggleMediaOverlay();");
}

void ReadiumJSApi::turnMediaOverlayOff()
{
    if (bMediaOverlayToggled)
        toggleMediaOverlay();
}

void ReadiumJSApi::bookmarkCurrentPage() 
{
    loadJS("window.LauncherUI.getBookmarkData(ReadiumSDK.reader.bookmarkCurrentPage());");
}

void ReadiumJSApi::openPageLeft() 
{
    loadJS("ReadiumSDK.reader.openPageLeft();");
}

void ReadiumJSApi::openPageRight()
{
    loadJS("ReadiumSDK.reader.openPageRight();");
}


void ReadiumJSApi::loadJS(std::string jScript)
{
    if (WebBrowser)
    {
        CComPtr<IDispatch> pDispDoc = WebBrowser->get_Document();
        CComQIPtr<IHTMLDocument2> pHtmlDoc(pDispDoc);
        if (pHtmlDoc)
        {
            CComPtr<IHTMLWindow2>    pMainWin2;
            pHtmlDoc->get_parentWindow(&pMainWin2);

            if (pMainWin2)
            {
                CComVariant    vtRv(0);
                CComBSTR bsCode = jScript.c_str() /*L"alert (\" Hi !\");"*/, bsLang = L"JavaScript";
                HRESULT hr = pMainWin2->execScript(bsCode, bsLang, &vtRv);

                if (!SUCCEEDED(hr))
                {
                    //AfxMessageBox(L"Error executing script");
                }
            }
        }
    }
}

void ReadiumJSApi::loadJSOnReady(std::string jScript)
{
    loadJS("$(document).ready(function () {" + jScript + "});");
}


void ReadiumJSApi::initReadiumSDK()
{
    //using namespace	ePub3;

    ePub3::ErrorHandlerFn launcherErrorHandler = LauncherErrorHandler;
    ePub3::SetErrorHandler(launcherErrorHandler);
    
    ePub3::InitializeSdk();
    ePub3::PopulateFilterManager();
    //ePub3::ContentModuleManager::Instance();
}

ReadiumJSApi::ReadiumJSApi(CExplorer*pWebBrowser_):WebBrowser(pWebBrowser_), bMediaOverlayToggled(false)
{
}

ReadiumJSApi::ReadiumJSApi():WebBrowser(0), bMediaOverlayToggled(false)
{
}

/**
 *  External function to handle errors in the EPUB3 library
 */
bool LauncherErrorHandler(const ePub3::error_details& err)
{
    const char * msg = err.message();
    wprintf(L"%s\n", msg);

    if (err.is_spec_error())
    {
        switch (err.severity())
        {
        case ePub3::ViolationSeverity::Critical:
        case ePub3::ViolationSeverity::Major: {

            if (!m_ignoreRemainingErrors)
            {
                int len = strlen(msg) + 1;
                wchar_t *w_msg = new wchar_t[len];
                memset(w_msg, 0, len);
                MultiByteToWideChar(CP_ACP, NULL, msg, -1, w_msg, len);

                int msgboxID = MessageBox(
                    NULL,
                    (LPCWSTR)w_msg,
                    (LPCWSTR)L"EPUB warning",
                    MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON1
                    );

                switch (msgboxID)
                {
                case IDCANCEL:
                    m_ignoreRemainingErrors = true;
                    break;
                case IDOK:
                    break;
                default:
                    break;
                }
            }

            break;
        }
        default:
            break;
        }
    }

    return true;
}
