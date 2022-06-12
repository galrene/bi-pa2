#include "CSaver.h"

bool CSaver::createDirectory ( fs::path & dirPath ) const {
    fs::path tmpPath = dirPath;
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
    dirPath = tmpPath;
    return true;
}

bool CSaver::saveCards ( const map<string,shared_ptr<CCard>> & cards, fs::path & dirPath ) const {
    try {
        if ( ! createDirectory ( dirPath ) )
            return false;
    }
    catch ( const exception & e ) {
        cerr << e.what() << '\n';
        return false;
    }
    for ( const auto & [ cardName, cardDefinition ] : cards ) {
        string fileName = cardDefinition->getHeader();
        fileName += ".ini";
        dirPath.append ( fileName );
        ofstream ofs ( dirPath );
        if ( ! ofs.good() ) {
            cerr << "Failed to access file " << dirPath << endl;
            dirPath = dirPath.parent_path();
            continue;
        }
        cardDefinition->dumpInfo ( ofs );
        dirPath = dirPath.parent_path();
    }
    return true;
}
