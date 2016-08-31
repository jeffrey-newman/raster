#ifndef PNG_PLOTTER
#define PNG_PLOTTER

#include <boost/filesystem.hpp>
#include "pngwriter.h"
#include <blink/raster/gdal_raster.h>
#include <tuple>

namespace blink {

namespace raster {

void printRaster2PNG(gdal_raster<int> & raster, std::vector<std::tuple<int, int, int> > & colour_scheme, boost::filesystem::path & file_path)
{

    pngwriter image(raster.nCols(), raster.nRows(), 1.0, file_path.c_str());
    for (int i = 0; i < raster.nCols(); ++i)
    {
        for (int j = 0; j < raster.nRows(); ++j)
        {
            int class_val = raster.get(coordinate_2d(j,i));
            std::tuple<int, int, int> & colour = colour_scheme[class_val];
            image.plot(i+1,raster.nRows() - j,std::get<0>(colour)/ 255.0, std::get<1>(colour)/255.0, std::get<2>(colour)/255.0);
        }

    }
    image.close();

}


}

}

#endif
