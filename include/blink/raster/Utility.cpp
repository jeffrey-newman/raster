//
//  Utility.cpp
//  CCCALM
//
//  Created by a1091793 on 15/05/2016.
//
//

#include <stdio.h>
#include "utility.h"
#include "cpl_conv.h"

namespace blink {
    namespace raster {
        namespace detail {
            
            GDALDataset* create_standard_gdaldataset(
                                                     const boost::filesystem::path& path, int rows, int cols
                                                     , GDALDataType datatype, int nBands)
            {
                GDALAllRegister();
                
                GDALDriverManager* m = GetGDALDriverManager();
                GDALDriver* driver = m->GetDriverByName("GTiff");
                
                char **papszOptions = NULL;
                papszOptions = CSLSetNameValue(papszOptions, "BLOCKXSIZE", "256");
                papszOptions = CSLSetNameValue(papszOptions, "BLOCKYSIZE", "256");
                papszOptions = CSLSetNameValue(papszOptions, "TILED", "YES");
                papszOptions = CSLSetNameValue(papszOptions, "COMPRESS", "LZW");
                papszOptions = CSLSetNameValue(papszOptions, "BIGTIFF", "YES");
                
                CPLSetConfigOption( "GDAL_TIFF_INTERNAL_MASK", "NO" );
                CPLSetConfigOption( "TIFF_USE_OVR", "YES" );

                
                GDALDataset* dataset = driver->Create(path.string().c_str(), cols, rows
                                                      , nBands, datatype, papszOptions);
                return dataset;
            }
            
            GDALDataset* create_standard_gdaldataset_from_model(
                                                                const boost::filesystem::path& path, const GDALDataset* model
                                                                , GDALDataType datatype, int nBands)
            {
                int rows = const_cast<GDALDataset*>(model)->GetRasterYSize();
                int cols = const_cast<GDALDataset*>(model)->GetRasterXSize();
                
                GDALDataset* dataset = create_standard_gdaldataset(path, rows, cols
                                                                   , datatype, nBands);
                
                if (dataset == NULL) return NULL;
                
                double gt_data[6];
                double* geotransform = gt_data;
                CPLErr error_status
                = const_cast<GDALDataset*>(model)->GetGeoTransform(geotransform);
                dataset->SetGeoTransform(geotransform);
                dataset->SetProjection(
                                       const_cast<GDALDataset*>(model)->GetProjectionRef());
                return dataset;
            }
            
            boost::filesystem::path get_temp_tiff_path()
            {
                boost::filesystem::path temp_path
                = boost::filesystem::temp_directory_path();
                boost::filesystem::path unique_temp_path_model
                = temp_path /= "%%%%-%%%%-%%%%-%%%%.tif";
                return unique_path(unique_temp_path_model);
            }
        }
        
        template<typename T>
        gdal_raster<T> open_gdal_raster(const boost::filesystem::path& path
                                        , GDALAccess access, int band)
        {
            return detail::gdal_makers::open_gdal_raster<T>(path, access, band);
        }
        
        template
        gdal_raster<double> open_gdal_raster(const boost::filesystem::path& path
                                             , GDALAccess access, int band);
        
        template
        gdal_raster<int> open_gdal_raster(const boost::filesystem::path& path
                                             , GDALAccess access, int band);
        
        template
        gdal_raster<float> open_gdal_raster(const boost::filesystem::path& path
                                          , GDALAccess access, int band);
        
        template<typename T>
        gdal_raster<T> create_gdal_raster(const boost::filesystem::path& path
                                          , int rows, int cols, GDALDataType datatype)
        {
            return detail::gdal_makers::create_gdal_raster<T>(path, rows, cols, datatype);
        }
        
        template
        gdal_raster<int> create_gdal_raster(const boost::filesystem::path& path
                                            , int rows, int cols, GDALDataType datatype);
        
        template
        gdal_raster<float> create_gdal_raster(const boost::filesystem::path& path
                                            , int rows, int cols, GDALDataType datatype);
        
        template
        gdal_raster<double> create_gdal_raster(const boost::filesystem::path& path
                                            , int rows, int cols, GDALDataType datatype);
        

        
        template<typename T, typename U>
        gdal_raster<T> create_gdal_raster_from_model(
                                                     const boost::filesystem::path& path, const gdal_raster<U>& model,
                                                     GDALDataType datatype)
        {
            return detail::gdal_makers::create_gdal_raster_from_model<T>(path, model,
                                                                         datatype);
        }
        
        template
        gdal_raster<int> create_gdal_raster_from_model(
                                                     const boost::filesystem::path& path, const gdal_raster<int>& model,
                                                       GDALDataType datatype);
        template
        gdal_raster<double> create_gdal_raster_from_model(
                                                       const boost::filesystem::path& path, const gdal_raster<double>& model,
                                                       GDALDataType datatype);
        
        template
        gdal_raster<double> create_gdal_raster_from_model(
                                                          const boost::filesystem::path& path, const gdal_raster<int>& model,
                                                          GDALDataType datatype);
        
        template
        gdal_raster<float> create_gdal_raster_from_model(
                                                          const boost::filesystem::path& path, const gdal_raster<float>& model,
                                                          GDALDataType datatype);
        
        template<typename T>
        gdal_raster<T> create_temp_gdal_raster(int rows, int cols, GDALDataType datatype)
        {
            return detail::gdal_makers::create_temp_gdal_raster<T>(rows, cols, datatype);
        }
        
        template<typename T, typename U>
        gdal_raster<T> create_temp_gdal_raster_from_model(const gdal_raster<U>& model, GDALDataType datatype)
        {
            return detail::gdal_makers::create_temp_gdal_raster_from_model<T>(model,
                                                                              datatype);
        }

       template
        gdal_raster<double> create_temp_gdal_raster_from_model(const gdal_raster<double>& model, GDALDataType datatype);

        template
        gdal_raster<int> create_temp_gdal_raster_from_model(const gdal_raster<double>& model, GDALDataType datatype);

        template
        gdal_raster<float> create_temp_gdal_raster_from_model(const gdal_raster<double>& model, GDALDataType datatype);

        template
        gdal_raster<double> create_temp_gdal_raster_from_model(const gdal_raster<int>& model, GDALDataType datatype);

        template
        gdal_raster<int> create_temp_gdal_raster_from_model(const gdal_raster<int>& model, GDALDataType datatype);

        template
        gdal_raster<float> create_temp_gdal_raster_from_model(const gdal_raster<int>& model, GDALDataType datatype);

        template
        gdal_raster<double> create_temp_gdal_raster_from_model(const gdal_raster<float>& model, GDALDataType datatype);

        template
        gdal_raster<int> create_temp_gdal_raster_from_model(const gdal_raster<float>& model, GDALDataType datatype);

        template
        gdal_raster<float> create_temp_gdal_raster_from_model(const gdal_raster<float>& model, GDALDataType datatype);
        
        template<typename T>
        boost::shared_ptr<gdal_raster<T> > open_gdal_rasterSPtr(const boost::filesystem::path& path
                                                                , GDALAccess access, int band)
        {
            return detail::gdal_makers::open_gdal_rasterSPtr<T>(path, access, band);
        }
        
        template
        boost::shared_ptr<gdal_raster<double> > open_gdal_rasterSPtr(const boost::filesystem::path& path
                                                 , GDALAccess access, int band);
        
        template
        boost::shared_ptr<gdal_raster<int> > open_gdal_rasterSPtr(const boost::filesystem::path& path
                                              , GDALAccess access, int band);
        
        template
        boost::shared_ptr<gdal_raster<float> > open_gdal_rasterSPtr(const boost::filesystem::path& path
                                                , GDALAccess access, int band);
        
        template<typename T>
        boost::shared_ptr<gdal_raster<T> > create_gdal_rasterSPtr(const boost::filesystem::path& path
                                                                  , int rows, int cols, GDALDataType datatype)
        {
            return detail::gdal_makers::create_gdal_rasterSPtr<T>(path, rows, cols, datatype);
        }
        
        template
        boost::shared_ptr<gdal_raster<int> > create_gdal_rasterSPtr(const boost::filesystem::path& path
                                                                    , int rows, int cols, GDALDataType datatype);
        
        template
        boost::shared_ptr<gdal_raster<float> > create_gdal_rasterSPtr(const boost::filesystem::path& path
                                                                    , int rows, int cols, GDALDataType datatype);
        
        template
        boost::shared_ptr<gdal_raster<double> > create_gdal_rasterSPtr(const boost::filesystem::path& path
                                                                      , int rows, int cols, GDALDataType datatype);
        
        
        
        template<typename T, typename U>
        boost::shared_ptr<gdal_raster<T> > create_gdal_rasterSPtr_from_model(
                                                                             const boost::filesystem::path& path, const gdal_raster<U>& model,
                                                                             GDALDataType datatype)
        {
            return detail::gdal_makers::create_gdal_rasterSPtr_from_model<T>(path, model,
                                                                             datatype);
        }
        
        template
        boost::shared_ptr<gdal_raster<int> > create_gdal_rasterSPtr_from_model(
                                                           const boost::filesystem::path& path, const gdal_raster<int>& model,
                                                           GDALDataType datatype);
        template
        boost::shared_ptr<gdal_raster<double> > create_gdal_rasterSPtr_from_model(
                                                              const boost::filesystem::path& path, const gdal_raster<double>& model,
                                                              GDALDataType datatype);
        
        template
        boost::shared_ptr<gdal_raster<double> > create_gdal_rasterSPtr_from_model(
                                                              const boost::filesystem::path& path, const gdal_raster<int>& model,
                                                              GDALDataType datatype);
        
        template
        boost::shared_ptr<gdal_raster<float> > create_gdal_rasterSPtr_from_model(
                                                             const boost::filesystem::path& path, const gdal_raster<float>& model,
                                                             GDALDataType datatype);
        
        template
        boost::shared_ptr<gdal_raster<int32_t> > create_gdal_rasterSPtr_from_model(
                                                                                 const boost::filesystem::path& path, const gdal_raster<float>& model,
                                                                                 GDALDataType datatype);
        
        template<typename T>
        boost::shared_ptr<gdal_raster<T> > create_temp_gdal_rasterSPtr(int rows, int cols, GDALDataType datatype)
        {
            return detail::gdal_makers::create_temp_gdal_rasterSPtr<T>(rows, cols, datatype);
        }
        
        template<typename T, typename U>
        boost::shared_ptr<gdal_raster<T> > create_temp_gdal_rasterSPtr_from_model(const gdal_raster<U>& model, GDALDataType datatype)
        {
            return detail::gdal_makers::create_temp_gdal_rasterSPtr_from_model<T>(model,
                                                                                  datatype);
        }
        
    }
}