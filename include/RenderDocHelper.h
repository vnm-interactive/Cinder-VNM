#pragma once

#include "renderdoc_app.h"

struct RenderDocHelper
{
    RENDERDOC_API_1_4_1* rdc = nullptr;

    bool setup()
    {
        rdc = GetRenderDocApi();
        return rdc != nullptr;
    }

    void startCapture()
    {
        if (rdc)
            rdc->StartFrameCapture(NULL, NULL);
    }

    void endCapture()
    {
        if (rdc)
            rdc->EndFrameCapture(NULL, NULL);
    }

    static RENDERDOC_API_1_4_1* GetRenderDocApi()
    {
        RENDERDOC_API_1_4_1* rdoc = nullptr;
#if 1
        HINSTANCE module = LoadLibraryA("C:/Program Files/RenderDoc/renderdoc.dll");
#else
        HMODULE module = GetModuleHandleA("renderdoc.dll");
#endif
        if (module == NULL)
        {
            return nullptr;
        }

        pRENDERDOC_GetAPI getApi = nullptr;
        getApi = (pRENDERDOC_GetAPI)GetProcAddress(module, "RENDERDOC_GetAPI");

        if (getApi == nullptr)
        {
            return nullptr;
        }

        if (getApi(eRENDERDOC_API_Version_1_4_1, (void**)&rdoc) != 1)
        {
            return nullptr;
        }

        return rdoc;
    }
};
