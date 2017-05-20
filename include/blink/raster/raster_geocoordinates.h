//
// Created by a1091793 on 8/2/17.
//

#ifndef AGGREGATE_MAP_2V2_RASTER_GEOCOORDINATES_H_H
#define AGGREGATE_MAP_2V2_RASTER_GEOCOORDINATES_H_H

#include <blink/raster/gdal_raster.h>

template <typename T>
class GeoTransform {

private:
    double geoTransform[6];
    double geoInvTransform[6];

public:
    GeoTransform(blink::raster::gdal_raster<T> & raster)
    {
        const_cast<GDALDataset*>(raster.get_gdal_dataset())->GetGeoTransform(geoTransform);
        GDALInvGeoTransform(geoTransform, geoInvTransform);

    }

    blink::raster::coordinate_2d
    getRasterPosition(GeoPosition pos)
    {

        double geo_x, geo_y;
        double pixel, line;

        geo_x = pos.geo_x;
        geo_y = pos.geo_y;

        GDALApplyGeoTransform(&geoInvTransform[0], pos.geo_x, pos.geo_y, &pixel, &line);
        return (blink::raster::coordinate_2d(line, pixel));

    }
    
    GeoPosition
    getGeoCoordinate(blink::raster::coordinate_2d coord)
    {
        double geo_x, geo_y;
        double pixel, line;

        pixel = double(coord.col);
        line = double(coord.row);

        GDALApplyGeoTransform(&geoTransform[0], pixel, line, &geo_x, &geo_y);
        return (GeoPosition(geo_x, geo_y));
    }
};

class GeoPosition
{
public:
    typedef double index_type;
    typedef GeoPosition this_type;

    // default constructor
    GeoPosition(index_type x = 0, index_type y = 0) : geo_y(y), geo_x(x)
    {

    }

    GeoPosition(std::initializer_list<double> numbers)
    {
        int k = 0;
        for(auto i = numbers.begin(); i != numbers.end(); i++)
        {
            if(k == 0) geo_y = *i;
            if(k == 1) geo_x = *i;
            k++;
        }
    }

    this_type& operator+=(const this_type& that)
    {
        geo_y += that.geo_y;
        geo_x += that.geo_x;
        return *this;
    }

    this_type& operator-=(const this_type& that)
    {
        geo_y -= that.geo_y;
        geo_x -= that.geo_x;
        return *this;
    }

    template<class T>
    this_type& operator*=(const T& factor)
    {
        geo_y *= factor;
        geo_x *= factor;
        return *this;
    }

    template<class T>
    this_type& operator/=(const T& factor)
    {
        geo_y /= factor;
        geo_x /= factor;
        return *this;
    }

    this_type operator+(const this_type& that) const
    {
        this_type copy(*this);
        return  copy += that;
    }

    this_type operator-(const this_type& that) const
    {
        this_type copy(*this);
        return copy -= that;
    }

    template<class T>
    this_type operator*(const T& factor) const
    {
        return this_type(*this) *= factor;
    }

    template<class T>
    this_type operator/(const T& factor) const
    {
        return this_type(*this) /= factor;
    }

    bool operator==(const this_type& that) const
    {
        return geo_y == that.geo_y && geo_x == that.geo_x;
    }

    bool operator!=(const this_type& that) const
    {
        return geo_y != that.geo_y || geo_x != that.geo_x;
    }

    index_type geo_y;
    index_type geo_x;

private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & geo_y;
        ar & geo_x;
    }
};

GeoPosition
getGeoPosition(const coordinate_2d & location, const double * adfGeoTransform)
{
    GeoPosition position; //(row, col)
    int & line = location.first;
    int & pixel = location.second;
    position.col = adfGeoTransform[0] + pixel*adfGeoTransform[1] + line*adfGeoTransform[2];
    position.row = adfGeoTransform[3] + pixel*adfGeoTransform[4] + line*adfGeoTransform[5];
    return (position);
}

coordinate_2d
getRasterRowColumn(const GeoPosition & location, const GeoTransform transform)
{
    RasterCoordinates coordinates; //(row, col)
    coordinates.first = std::floor((location.first - transform.y_origin) / transform.y_line_space);
    coordinates.second = std::floor((location.second - transform.x_origin) / transform.pixel_width);
    return (coordinates);
}

#endif //AGGREGATE_MAP_2V2_RASTER_GEOCOORDINATES_H_H
