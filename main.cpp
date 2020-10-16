#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "importPointClouds.h"
#include "writeFeatureColor.h"

#include <kvs/PolygonObject>
#include <kvs/PointObject>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/Camera>
#include <kvs/PointRenderer>
#include <kvs/Coordinate>

const char OUT_FILE[] = "SPBR_DATA/output.spbr";

int main( int argc, char** argv ) {
    char outSPBRfile[128];
    strcpy( outSPBRfile, OUT_FILE );

    if ( argc != 3 ) {
        std::cerr << "USAGE: $ ./vfp [input.xyz] [output.spbr]" << std::endl;
        exit(1);
    } else {
        strcpy( outSPBRfile, argv[2] );
    }

    ImportPointClouds *ply = new ImportPointClouds( argv[1] );
    ply->updateMinMaxCoords();
    std::cout << "\nPLY Min, Max Coords:" << std::endl;
    std::cout << "Min : " << ply->minObjectCoord() << std::endl;
    std::cout << "Max : " << ply->maxObjectCoord() << std::endl;
    std::cout << "Number of points: " << ply->numberOfVertices() << std::endl;

    std::vector<float> ft = ply->featureData();

    double th;

    std::cout << "Input Feature Threshold : ";
    std::cin >> th;

    WritingDataType type = Ascii;
    writeFeatureColor( ply, ft, th, outSPBRfile, type );

    kvs::PointObject* object = new kvs::PointObject( *ply );
    object->setSize( 1 );
    object->updateMinMaxCoords();

    // Exec. SPBR
    std::string out_feature_spbr( outSPBRfile );
    std::string EXEC("spbr ");
    EXEC += out_feature_spbr;
    system( EXEC.c_str() );

    return 0;
}