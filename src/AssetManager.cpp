#include "../include/AssetManager.h"
#include "cinder/app/App.h"
#include "cinder/ImageIo.h"
#include "cinder/Function.h"
#include "cinder/Utilities.h"
#include "cinder/ObjLoader.h"
#include "cinder/Log.h"
#include "cinder/Function.h"
#include <map>

using namespace std;
using namespace ci;
using namespace app;

namespace
{
    template <typename T> 
    T& getAssetResource(const string& relativeName, function<T(const string&, const string&)> loadFunc, const string& relativeNameB = "")
    {
        typedef map<string, T> MapType;
        static MapType sMap;
        static T nullResource;
        auto it = sMap.find(relativeName+relativeNameB);
        if (it != sMap.end())
        {
            return it->second;
        }
        CI_LOG_V( "Loading: " << relativeName << " " << relativeNameB);

        try
        {
            auto aPath = getAssetPath("") / relativeName;
            auto bPath = getAssetPath("") / relativeNameB;
            
            auto resource = loadFunc(aPath.string(), bPath.string());
            return sMap[relativeName+relativeNameB] = resource;
        }
        catch (Exception& e)
        {
            CI_LOG_EXCEPTION( "getAssetResource", e);
            return nullResource;
        }
    }
}

namespace am
{
    static SurfaceRef loadSurface(const string& absoluteName, const string&)
    {
        auto source = loadImage(absoluteName);
        return Surface::create(source);
    }

    SurfaceRef& surface(const string& relativeName)
    {
        return getAssetResource<SurfaceRef>(relativeName, loadSurface);
    }

    template <typename T>
    shared_ptr<T>& texture(const string& relativeName, const typename T::Format& format)
    {
        auto loadTexture = [&format](const string& absoluteName, const string&) -> shared_ptr<T>
        {
            auto ext = fs::path(absoluteName).extension();
#if !defined( CINDER_GL_ES ) || defined( CINDER_GL_ANGLE )
            if (ext == ".dds")
            {
                auto source = DataSourcePath::create(absoluteName);
                return T::createFromDds(source, format);
            }
#endif
            if (ext == ".ktx")
            {
                auto source = DataSourcePath::create(absoluteName);
                return T::createFromKtx(source, format);
            }
            auto source = loadImage(absoluteName);
            return T::create(source, format);
        };
        return getAssetResource<shared_ptr<T>>(relativeName, loadTexture);
    }

    //gl::Texture1dRef& texture1d(const std::string& relativeName, const gl::Texture1d::Format& format)
    //{
    //    return texture<gl::Texture1d>(relativeName, format);
    //}

    gl::Texture2dRef& texture2d(const std::string& relativeName, const gl::Texture2d::Format& format)
    {
        return texture<gl::Texture2d>(relativeName, format);
    }

    gl::TextureCubeMapRef& textureCubeMap(const std::string& relativeName, const gl::TextureCubeMap::Format& format)
    {
        return texture<gl::TextureCubeMap>(relativeName, format);
    }

    static TriMeshRef loadTriMesh(const string& absoluteName, const string&)
    {
        auto source = DataSourcePath::create(absoluteName);
        auto ext = fs::path(absoluteName).extension();
        TriMeshRef mesh;
        
        if (ext == ".obj")
        {
            ObjLoader loader( source );
            mesh = TriMesh::create(loader);
        }
        else if (ext == ".msh")
        {
            mesh = TriMesh::create();
            mesh->read(source);
        }
        else
        {
            CI_LOG_W( "Unsupported mesh format: " << absoluteName );
            return nullptr;
        }
        
        if( !mesh->hasNormals() ) {
            mesh->recalculateNormals();
        }
        
        if( ! mesh->hasTangents() ) {
            mesh->recalculateTangents();
        }
        
        return mesh;
    }

    TriMeshRef& triMesh(const string& relativeName)
    {
        return getAssetResource<TriMeshRef>(relativeName, loadTriMesh);
    }

    static gl::VboMeshRef loadVboMesh(const string& absoluteName, const string&)
    {
        gl::VboMesh::Layout layout;
        auto triMesh = loadTriMesh(absoluteName, "");
        return gl::VboMesh::create(*triMesh);
    }

    gl::VboMeshRef& vboMesh(const string& relativeName)
    {
        return getAssetResource<gl::VboMeshRef>(relativeName, loadVboMesh);
    }

    static gl::GlslProgRef loadGlslProg(const string& vsAbsoluteName, const string& fsAbsoluteName)
    {
        gl::GlslProg::Format format;
#if defined( CINDER_GL_ES )
        format.version(300); // es 3.0
#else
        format.version(150); // gl 3.2
#endif
        format.vertex(DataSourcePath::create(vsAbsoluteName));
        format.fragment(DataSourcePath::create(fsAbsoluteName));
                
        return gl::GlslProg::create(format);
    }

    gl::GlslProgRef& glslProg(const string& vsFileName, const string& fsFileName)
    {
        return getAssetResource<gl::GlslProgRef>(vsFileName, loadGlslProg, fsFileName);
    }

    static string loadStr(const string& absoluteName, const string&)
    {
        return loadString(DataSourcePath::create(absoluteName));
    }

    string& str(const string& relativeName)
    {
        return getAssetResource<string>(relativeName, loadStr);
    }

    static vector<string> loadPaths(const string& absoluteFolderName, const string&, bool isLongMode)
    {
        vector<string> files;
        fs::directory_iterator kEnd;
        for (fs::directory_iterator it(absoluteFolderName); it != kEnd; ++it)
        {
            if (fs::is_regular_file(*it) && it->path().extension() != ".db" 
                && it->path().extension() != ".DS_Store")
            {
#ifdef _DEBUG
                //console() << it->path() << endl;
#endif
                files.push_back(isLongMode ?
                                it->path().string() :
                                it->path().filename().string());
            }
        }
        return files;
    }

    vector<string> longPaths(const string& relativeFolderName)
    {
        return getAssetResource<vector<string>>(relativeFolderName, bind(loadPaths, placeholders::_1, placeholders::_2, true));
    }
    
    vector<string> shortPaths(const string& relativeFolderName)
    {
        return getAssetResource<vector<string>>(relativeFolderName, bind(loadPaths, placeholders::_1, placeholders::_2, false));
    }
    
}
