#include "stream.hpp"
#include <fstream>
using namespace std;

namespace JCONER {

FileIStream::FileIStream(std::string filename)
    :_filename(filename)
{
    _fin.open(_filename.c_str());
    if (_fin) {
        LOG_ERROR("File doen't exist! [%s]", filename.c_str());        
    }
}

}
