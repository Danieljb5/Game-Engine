#pragma once

#include <platform.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

#ifdef LINUX
#include <dlfcn.h>

namespace detail
{
    void* load_library(const char* library, bool required = false)
    {
        std::string lib_name = "lib";
        lib_name += library;
        lib_name += ".so";
        void* lib = dlopen(lib_name.c_str(), RTLD_NOW);
        auto error = dlerror();
        if(error)
        {
            std::cout << error << "\n";
            if(!required)
            {
                return nullptr;
            }
            std::cout << "could not load required library, exiting\n";
            exit(1);
        }
        return lib;
    }

    void unload_library(void* library)
    {
        dlclose(library);
        library = nullptr;
    }
}

namespace cl
{
    template <typename _Callable>
    _Callable load_library_function(void* library, const char* function)
    {
        _Callable func = (_Callable)dlsym(library, function);
        auto error = dlerror();
        if(error)
        {
            std::cout << error << "\n";
            exit(1);
        }
        return func;
    }
}

#endif

#ifdef WINDOWS
#include <windows.h>

namespace detail
{
    void* load_library(const char* library)
    {
        std::string lib_name = library;
        lib_name += ".dll";
        LPCSTR lib = lib_name.c_str();
        HINSTANCE hGetProcIDDLL = LoadLibrary(lib);
        if(!hGetProcIDDLL)
        {
            std::cout << lib_name << ": cannot open shared object file: No such file or directory\n";
            return nullptr;
        }
        return hGetProcIDDLL;
    }

    void unload_library(void* library)
    {
        FreeLibrary((HINSTANCE)library);
        library = nullptr;
    }
}

namespace cl
{
    template <typename _Callable>
    _Callable load_library_function(void* library, const char* function)
    {
        _Callable func = (_Callable)GetProcAddress((HINSTANCE)library, function);
        if(!func)
        {
            std::cout << "error loading function '" << function << "'\n";
            exit(1);
        }
        return func;
    }
}

#endif

class LibraryManager
{
public:
    LibraryManager()
    {

    }

    ~LibraryManager()
    {
        for(auto it = libraries.begin(); it != libraries.end(); it++)
        {
            detail::unload_library(it->second);
        }
        libraries.clear();
    }

    void* get_library(const char* name)
    {
        if(!libraries.count(name))
        {
            void* lib = detail::load_library(name);
            if(lib)
            {
                libraries[name] = lib;
            }
            else
            {
                return nullptr;
            }
        }
        return libraries[name];
    }

private:
    std::unordered_map<std::string, void*> libraries;
};

#define LIBRARY(_name) \
    extern void (*init)(LibraryManager*); \
    namespace detail \
    { \
        LibraryManager* libManager; \
        void* library; \
        void load_lib(LibraryManager* libManager) \
        { \
            if(!library) \
            { \
                library = libManager->get_library(#_name); \
                if(!library) \
                { \
                    std::cout << "[" << #_name << "] library not available, functionality disabled\n"; \
                } \
                else \
                { \
                    init(libManager); \
                } \
            } \
            else \
            { \
                std::cout << "[" << #_name << "] error: library already loaded\n"; \
            } \
        } \
        void init_stub(LibraryManager* libManager) \
        { \
            if(library) \
            { \
                init = load_library_function<void (*)(LibraryManager*)>(library, "init"); \
                return init(libManager); \
            } \
            return; \
        } \
        void* get_library() \
        { \
            return library; \
        } \
    } \
    void (*init)(LibraryManager*) = detail::init_stub;

#define STUB(_name, _return) \
    extern _return (*_name)(); \
    namespace detail \
    { \
        _return _name##_stub() \
        { \
            if(library) \
            { \
                _name = load_library_function<_return (*)()>(library, #_name); \
                return _name (); \
            } \
            return; \
        } \
    } \
    _return (*_name)() = detail::_name##_stub;

#define STUB_RET(_name, _return) \
    extern _return (*_name)(); \
    namespace detail \
    { \
        _return _name##_stub() \
        { \
            if(library) \
            { \
                _name = load_library_function<_return (*)()>(library, #_name); \
                return _name (); \
            } \
            return {}; \
        } \
    } \
    _return (*_name)() = detail::_name##_stub;

#define STUB_RET_PTR(_name, _return) \
    extern _return (*_name)(); \
    namespace detail \
    { \
        _return _name##_stub() \
        { \
            if(library) \
            { \
                _name = load_library_function<_return (*)()>(library, #_name); \
                return _name (); \
            } \
            return nullptr; \
        } \
    } \
    _return (*_name)() = detail::_name##_stub;

#define STUB_A(_name, _return, _args...) \
    extern _return (*_name)(_args); \
    namespace detail \
    { \
        _return _name##_stub(_args) \
        { \
            if(library) \
            { \
                _name = load_library_function<_return (*)(_args)>(library, #_name);

// return _name##(_args);

#define STUB_B(_name, _return, _args...) \
            } \
            return; \
        } \
    } \
    _return (*_name)(_args) = detail::_name##_stub;

#define STUB_B_RET(_name, _return, _args...) \
            } \
            return {}; \
        } \
    } \
    _return (*_name)(_args) = detail::_name##_stub;

#define STUB_B_RET_PTR(_name, _return, _args...) \
            } \
            return nullptr; \
        } \
    } \
    _return (*_name)(_args) = detail::_name##_stub;
