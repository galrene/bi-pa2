#include "CSaver.h"
CSaver::CSaver ( void )
: m_Path ( defaultSaveLocation ) {}

CSaver::CSaver ( fs::path p )
: m_Path ( p ) {}


bool CSaver::createDirectory ( const string & dirName ) {
    fs::path tmpPath = m_Path;
    tmpPath.append ( dirName );
    if ( fs::exists ( tmpPath ) && fs::is_directory ( tmpPath ) && fs::is_empty ( tmpPath ) )
        return true;
    if ( ! fs::exists ( tmpPath ) ) {
        fs::create_directory ( tmpPath );
        return true;
    }
    tmpPath += "_";
    for ( size_t i = 1; i < SIZE_MAX; i++ ) {
        tmpPath += i + '0';
        if ( ! fs::exists ( tmpPath ) ) {
            fs::create_directory ( tmpPath );
            break;
        }
        string clearNumber = tmpPath.generic_string(); clearNumber.pop_back();
        tmpPath = clearNumber;
    }
    if ( ! fs::exists ( tmpPath ) ) {
        cerr << "Unable to create savefile directory" << endl;
        return false;
    }
    // dirName = tmpPath.filename();
    return true;
}

// template < typename C >
bool CSaver::save ( deque<shared_ptr<CCard>> & elements, const string & dirName ) {
    try {
        if ( ! createDirectory ( dirName ) )
            return false;
    }
    catch ( const exception & e ) {
        cerr << e.what() << '\n';
        return false;
    }
    m_Path.append ( dirName );
    for ( const auto & x : elements ) {
        string fileName = x->getHeader();
        fileName += ".ini";
        m_Path.append ( fileName );
        ofstream ofs ( m_Path );
        if ( ! ofs.good() ) {
            cerr << "Failed to access file " << m_Path << endl;
            continue;
        }
        x->dumpInfo ( ofs );
        m_Path = m_Path.parent_path();
    }
    m_Path = m_Path.parent_path();
    return true;
}
