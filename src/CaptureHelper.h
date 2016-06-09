#pragma once

#include "cinder/Capture.h"
#include "cinder/Log.h"
#include "cinder/ip/flip.h"

using namespace ci;
using namespace ci::app;

struct CaptureHelper
{
    Surface         surface;
    gl::TextureRef  texture;
    ivec2           size;
    bool            flip;
    
    bool isReady()
    {
        return texture != nullptr;
    }
    
    bool checkNewFrame()
    {
        bool ret = hasNewFrame;
        hasNewFrame = false;
        return ret;
    }
    
    void setup(int32_t width = 640, int32_t height = 480, const Capture::DeviceRef device = Capture::DeviceRef())
    {
        try
        {
            capture = Capture::create(width, height, device);
            capture->start();
            hasNewFrame = false;
            flip = false;
            
            auto updateFn = [this]
            {
                if (capture && capture->checkNewFrame())
                {
                    hasNewFrame = true;
                    surface = *capture->getSurface();
                    if (flip) ip::flipHorizontal(&surface);

                    size = surface.getSize();
                    
                    if (!texture)
                    {
                        // Capture images come back as top-down, and it's more efficient to keep them that way
                        gl::Texture::Format format;
//                        format.loadTopDown();
                        texture = gl::Texture::create( surface, format);
                    }
                    else
                    {
                        texture->update( surface );
                    }
                }
            };
            App::get()->getSignalUpdate().connect(updateFn);
            
        }
        catch ( ci::Exception &exc )
        {
            CI_LOG_EXCEPTION( "Failed to init capture ", exc );
        }
    }
    
    static void printDevices()
    {
        for ( const auto &device : Capture::getDevices() )
        {
            console() << "Device: " << device->getName() << " "
#if defined( CINDER_COCOA_TOUCH )
            << ( device->isFrontFacing() ? "Front" : "Rear" ) << "-facing"
#endif
            << std::endl;
        }
    }
    
private:
    
    CaptureRef  capture;
    bool        hasNewFrame;

};
