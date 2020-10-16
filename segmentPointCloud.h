#ifndef _writeFeature_H__
#define _writeFeature_H__

#include <kvs/PolygonObject>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <kvs/ColorMap>

enum WritingDataType {
    Ascii = 0,
    Binary = 1
};

const float NORMAL[3] = { 0.0, 0.0, 0.0 };
const int COLOR[3]    = { 0, 255, 255 };

void writeFeatureColor( kvs::PolygonObject *ply,
                        std::vector<float> &ft,
                        double th,
                        char* filename,
                        WritingDataType type = Ascii )
{
  size_t num     = ply->numberOfVertices();
  bool hasNormal = false, hasColor = false;

  if( num == ply->numberOfNormals() ) hasNormal = true;
  if( num == ply->numberOfColors() ) hasColor   = true;

  kvs::ValueArray<kvs::Real32> coords  = ply->coords();
  kvs::ValueArray<kvs::Real32> normals = ply->normals();
  kvs::ValueArray<kvs::UInt8> colors   = ply->colors();


  std::ofstream fout( filename );
  if( type == Ascii ) {
    fout << "#/SPBR_ASCII_Data"      << std::endl;
    fout << "#/RepeatLevel 1"        << std::endl;
    fout << "#/BGColorRGBByte 0 0 0" << std::endl;
    fout << "#/ImageResolution 1024" << std::endl;
    fout << "#/Shading 0"            << std::endl;
    fout << "#/EndHeader"            << std::endl;
  }

  for(int i=0; i<num; i++ ) {

    float x = coords[3*i];
    float y = coords[3*i+1];
    float z = coords[3*i+2];

    float nx = NORMAL[0];
    float ny = NORMAL[1];
    float nz = NORMAL[2];

    int r = COLOR[0];
    int g = COLOR[1];
    int b = COLOR[2];

    if( hasNormal ) {
      nx = normals[ 3*i ];
      ny = normals[ 3*i+1 ];
      nz = normals[ 3*i+2 ];
    }

    if( hasColor ) {

      // Color Edge
      if( ft[i] > th ) {
        r = (int)255;
        g = (int)0;
        b = (int)0;
      } else {
        // r = (int)255;
        // g = (int)255;
        // b = (int)255;

        r = (int)colors[ 3*i ];
        g = (int)colors[ 3*i+1 ];
        b = (int)colors[ 3*i+2 ];
      }
    }
    if( type == Binary ) {
      fout.write( (char*)&x, sizeof(float) );
      fout.write( (char*)&y, sizeof(float) );
      fout.write( (char*)&z, sizeof(float) );

      fout.write( (char*)&nx, sizeof(float) );
      fout.write( (char*)&ny, sizeof(float) );
      fout.write( (char*)&nz, sizeof(float) );

      unsigned char cl = (unsigned char)r;
      fout.write( (char*)&cl, sizeof(unsigned char) );
      cl = (unsigned char)g;
      fout.write( (char*)&cl, sizeof(unsigned char) );
      cl = (unsigned char)b;
      fout.write( (char*)&cl, sizeof(unsigned char) );
    }
    else {
      if(ft[i] <= th) {
        fout  << x << " " << y << " " << z << " "
              << nx << " " << ny << " " << nz << " "
              << r << " " << g << " " << b
              << std::endl;
      }
    }
  }

  fout.close();
}

#endif