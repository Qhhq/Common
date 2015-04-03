#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/iterator/filter_iterator.hpp>

#include <ltdl.h>

namespace fs = boost::filesystem;


namespace
{

typedef std::remove_pointer<lt_dlhandle>::type handle_type;

struct lt_dlclose_action
{
    void operator()(handle_type *plugin) const
    {
        lt_dlclose(plugin);
    }
};

struct Validate
{
    bool operator()(const fs::path &entry) const
    {
        return fs::is_regular_file(entry)
               && fs::extension(entry) == ".so";
    }
};

}

struct Application
{
    std::string name;
};

typedef Application (*getApplication)();

void Open(const std::string &libraryName)
{
    // clear errors
    lt_dlerror();

    lt_dlhandle module = 0;
    lt_dladvise advise;
    int32_t ret = lt_dlinit();;

    if (0 != ret)
    {
        throw std::runtime_error(std::string("Could not intialize dynamic loading:") + lt_dlerror());
    }
    ret = lt_dladvise_init(&advise);
    if (0 != ret)
    {
        throw std::runtime_error(std::string("Could not intialize dynamic loading:") + lt_dlerror());
    }

    ret = lt_dladvise_global(&advise);
    if (0 != ret)
    {
        throw std::runtime_error(std::string("Could not intialize dynamic loading of global symbols:") + lt_dlerror());
    }

    module = lt_dlopenadvise(libraryName.c_str(), advise);
    lt_dladvise_destroy(&advise);

    if(!module)
    {
        throw std::runtime_error(std::string("Unable to load '" + libraryName + "' - " + lt_dlerror()));
    }

    void *symbolAddress = lt_dlsym(module, "Name");

    if(!symbolAddress)
    {
        throw std::runtime_error(lt_dlerror());
        //std::cout << "name: " << value.name << std::endl;
    }
}

void Load(const std::string &pathStr)
{
    const fs::path path(pathStr);

    if(!fs::exists(path) ||
       !fs::is_directory(path))
    {
        throw std::runtime_error(std::string("Directory '' does not exist or is not a directory."));
    }
    typedef boost::filter_iterator<Validate, fs::directory_iterator> directoryIterator;

    directoryIterator it = directoryIterator(Validate(), fs::directory_iterator(path));
    directoryIterator end;

    while(it != end)
    {
        Open(it->path().string());
        ++it;
    }
}


int main(int /*argc*/, char */*argv*/[])
{
    try
    {
        std::string path("/home/anya/sputnik/simple/123");
        Load(path);
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }


    return 0;
}
