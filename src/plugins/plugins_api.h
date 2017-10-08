#ifndef PLUGINS_API_H
#define PLUGINS_API_H


#include <stdint.h>

//==============  Jag plugins api description ===================================
//
//    Custom shared libraries should provide 2 functions:
//
// #ifdef __cplusplus
// extern "C" {
// #endif
//
// int jag_data_source_match(const wchar_t *file_name);
// struct jag_data_source_api* jag_load_data_source(const wchar_t *file_name);
//
// #ifdef __cplusplus
// }
// #endif
//
//
//
//     jag_data_source_match - function determines if this shared library can process and
//  load file "file_name" (on failure 0 is returned, on success - nonzero).
//
//     jag_load_data_source - loads data from "file_name" data source and returns pointer
//  to the jag_data_source_api object (see description below).
//
//==================================================================================






#define JAG_PARAM_DESCR_SIZE 128
#define JAG_PARAM_NAME_SIZE 64
#define JAG_PARAM_UNITS_SIZE 64
#define JAG_MATCH_FUNCTION_NAME "jag_data_source_match"
#define JAG_LOAD_FUNCTION_NAME "jag_load_data_source"


#ifdef __cplusplus
extern "C" {
#endif


struct jag_data_source_api;

struct jag_param_description
{
    wchar_t description[JAG_PARAM_NAME_SIZE];
    wchar_t name[JAG_PARAM_NAME_SIZE];
    wchar_t units[JAG_PARAM_UNITS_SIZE];
    uint32_t dimension; // if parameter is scalar than dimension = 1,
    // if parameter is a vector than dimension is a size of this vector,
    // if parameter is 2D, 3D ... vector you should linearize it and
    // represent as 1D vector.
};

struct jag_data_source_api
{
    // p_impl - pointer to the implementation specific data
    void *p_impl;

    // get_number_of_parameters - returns number of parameters written in the data source
    uint32_t (*get_number_of_parameters)(const struct jag_data_source_api *data_source);

    // get_parameter_description - returns description of the parameter with "primary_index" index
    // primary_index is in range [0; result of "get_number_of_parameters" - 1].
    void (*get_parameter_description)(const struct jag_data_source_api *data_source,
                                      uint32_t primary_index,
                                      struct jag_param_description *description);

    // get_parameter_size - returns number of data points in "secondary_index"
    // coordinate of the parameter with "primary_index" index (if this parameter
    // has only one dimension than "secondary_index" should be ignored). For non scalar
    // parameters secondary_index is in range [0, "dimension in jag_param_description structure" - 1].
    uint64_t (*get_parameter_size)(const struct jag_data_source_api *data_source,
                                   uint32_t primary_index,
                                   uint32_t secondary_index);

    // get_parameter - returns data for the "secondary_index"
    // coordinate of the parameter with "primary_index" index (if this parameter
    // has only one dimension than secondary_index should be ignored).
    uint64_t (*get_parameter)(const struct jag_data_source_api *data_source,
                              uint32_t primary_index,
                              uint32_t secondary_index,
                              double *x_data,
                              double *y_data,
                              size_t data_sz);

    // get_data_source_name - returns name of the data source from which this jag_data_source_api
    // was loaded
    const wchar_t* (*get_data_source_name)(const struct jag_data_source_api *data_source);

    // loading_progress - returns loading progress. Before loading progress == 0.
    // When loading of all data is finished progress == 100. Jag will not invoke
    // get_parameter function if progress != 100.
    int (*loading_progress)(const struct jag_data_source_api *data_source);

    // is_valid - returns true if all data were loaded successfully, if errors during
    // loading were detected false is returned
    int (*is_valid)(const struct jag_data_source_api *data_source);

    // delete_data_source - this function should release all resources allocated for loaded data
    void (*delete_data_source)(struct jag_data_source_api *data_source);
};



#ifdef __cplusplus
}
#endif



// NOTE: get_number_of_parameters, get_parameter_description can be invoked before loading_progress == 100.
// Also get_number_of_parameters or get_parameter_description can be invoked simultaneously with loading_progress
// from different threads. So, if you need synchronization, you should ensure it. If at the moment of invocation
// of these functions number of parameters or their descriptions are not known, execution of these functions should be blocked
// and wait until this information will be read from the data source and only then correct result should be returned.
//
// get_parameter_size, get_data_source_name, is_valid, delete_data_source - will be invoked by jag only if loading_progress == 100.




// NOTE: Jag designed in assumption that all parameters in the data source,
// are represented as functions of some argument (this parameter can be time or something like that):
// Yni = fn(xi) (thats why pointer to the function "get_parameter" in jag_data_source_api
// structure returns 2 data vectors x_data and y_data). For 2 different parameters parameter1 and parameter1
// these arguments can be different, so "get_parameter" can return different "x_data" for parameters with
// different primary_index. Also, keep in mind that data in x_data should be in ascending order. Adjacent points in x_data can be equal.
//
// EXAMPLE: Suppose you wrote a plugin to load "*.csv" files and you have a file:
//
// some_data.csv:
//
// time, parameter1, parameter2
// 0, 10, 20
// 100, 20, 21
// 200, 30, 31
// 300, 40, 21
// 400, 50, 20
//
// Then for example your plugin can be implemented so that:
// get_number_of_parameters - returns 2 (for parameter1, parameter2, each parameter has dimension = 1) (parameter1 and parameter2 are considered
// as functions of argument time)
// get_parameter_size - returns 5 (because each parameter has 5 points to show on the plot)
// get_parameter - returns for parameter1(primary_index = 0) x_data = {0, 100, 200, 300, 400}, y_data = {10, 20, 30, 40, 50},
//                         for parameter1(primary_index = 1) x_data = {0, 100, 200, 300, 400}, y_data = {20, 21, 31, 21, 20},
//
// Also you can write another implementation. For example:
// get_number_of_parameters - returns 2 (for time, parameter1, parameter2, each parameter has dimension = 1) (parameter1, parameter2, time are considered
// as functions of argument number of line in the origin data source file)
// get_parameter_size - returns 5 (because each parameter has 5 points to show on the plot)
// get_parameter - returns for      time (primary_index = 0) x_data = {1, 2, 3, 4, 5}, y_data = {0, 100, 200, 300, 400},
//                         for parameter1(primary_index = 1) x_data = {1, 2, 3, 4, 5}, y_data = {10, 20, 30, 40, 50},
//                         for parameter1(primary_index = 2) x_data = {1, 2, 3, 4, 5}, y_data = {20, 21, 31, 21, 20},



#endif // PLUGINS_API_H
