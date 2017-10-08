#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <unistd.h>

#include "../src/plugins/plugins_api.h"
#include <pthread.h>


#define MAYBE_UNUSED(x)  ((void)(x))
#define FILE_NAME_SIZE 256
//#define JAG_PARAM_DESCR_SIZE 128

#ifdef __cplusplus
extern "C" {
#endif



// NOTE: this is an example of jag plugin implementation.


struct implementation 
{
    wchar_t file_name[FILE_NAME_SIZE];
    pthread_mutex_t data_ready_mutex;
    pthread_mutex_t progress_mutex;
    int loading_progress;
};


static uint32_t func_get_number_of_parameters(const struct jag_data_source_api *data_source)
{
    // fake ensuring that parameter descriptions are loaded
    while (1) {
        struct implementation *impl = (struct implementation*)data_source->p_impl;
        pthread_mutex_lock(&impl->progress_mutex);
        int progress = impl->loading_progress;
        pthread_mutex_unlock(&impl->progress_mutex);
        if (progress > 10)
  	    break;
    }

    return 10;
}

static void func_get_parameter_description(const struct jag_data_source_api *data_source,
                                    uint32_t primary_index,
                                    struct jag_param_description *description)
{
    // fake ensuring that parameter descriptions are loaded
    while (1) {
        struct implementation *impl = (struct implementation*)data_source->p_impl;
        pthread_mutex_lock(&impl->progress_mutex);
        int progress = impl->loading_progress;
        pthread_mutex_unlock(&impl->progress_mutex);
        if (progress > 10)
  	    break;
    }

    // setting some dummy data desctiptions
    (void)primary_index;
    description->dimension = 1;
    if (primary_index % 2) {
    	wcscpy(description->name, L"some_name");
	wcscpy(description->description, L"description");
	wcscpy(description->units, L"m/s"); 
    } else {
	wcscpy(description->name, L"another_name");
	wcscpy(description->description, L"русское описание");
	wcscpy(description->units, L"kg"); 
    }
}



static uint64_t func_get_parameter_size(const struct jag_data_source_api *data_source,
                                 uint32_t primary_index,
                                 uint32_t secondary_index)
{
    // fake ensuring that data are loaded
    struct implementation *impl = (struct implementation*)data_source->p_impl;
    pthread_mutex_lock(&impl->data_ready_mutex);
    pthread_mutex_unlock(&impl->data_ready_mutex);

    (void)primary_index;
    (void)secondary_index;
    return 100;
}

static uint64_t func_get_parameter(const struct jag_data_source_api *data_source,
                            uint32_t primary_index,
                            uint32_t secondary_index,
                            double *x_data,
                            double *y_data,
                            size_t data_sz)
{
    // fake ensuring that data are loaded
    struct implementation *impl = (struct implementation*)data_source->p_impl;
    pthread_mutex_lock(&impl->data_ready_mutex);
    pthread_mutex_unlock(&impl->data_ready_mutex);


    // imitating some data
    (void)primary_index;
    (void)secondary_index;
    assert(data_sz >= 100);
    int i = 0;
    for (i = 0; i < 100; ++i) {
        x_data[i] = i;
        y_data[i] = i % (primary_index + 2);
    }
    return 100;
}



static const wchar_t* func_get_data_source_name(const struct jag_data_source_api *data_source)
{
    // fake ensuring that data are loaded
    struct implementation *impl = (struct implementation*)data_source->p_impl;
    pthread_mutex_lock(&impl->data_ready_mutex);
    pthread_mutex_unlock(&impl->data_ready_mutex);

    return impl->file_name;
}

static int func_loading_progress(const struct jag_data_source_api *data_source)
{
    struct implementation *impl = (struct implementation*)data_source->p_impl;

    pthread_mutex_lock(&impl->progress_mutex);
    int progress = impl->loading_progress;
    pthread_mutex_unlock(&impl->progress_mutex);

    return progress;
}



static int func_is_valid(const struct jag_data_source_api *data_source)
{
    // fake ensuring that data are loaded
    struct implementation *impl = (struct implementation*)data_source->p_impl;
    pthread_mutex_lock(&impl->data_ready_mutex);
    pthread_mutex_unlock(&impl->data_ready_mutex);

    // dummy implementation, always return true
    (void)data_source;
    return 1;
}

static void func_delete_data_source(struct jag_data_source_api *data_source)
{
    // fake ensuring that data are loaded
    struct implementation *impl = (struct implementation*)data_source->p_impl;
    pthread_mutex_lock(&impl->data_ready_mutex);
    pthread_mutex_unlock(&impl->data_ready_mutex);

    free(data_source->p_impl);
    free(data_source);
}






static void* loading_imitation(void *arg)
{
    struct jag_data_source_api *data_source = (struct jag_data_source_api*)arg;
    struct implementation *impl = (struct implementation*)data_source->p_impl;

    // imitating data source loading process
    int i;
    for (i = 1; i <= 10; ++i) {
        pthread_mutex_lock(&impl->progress_mutex);
        impl->loading_progress = i * 10;
        pthread_mutex_unlock(&impl->progress_mutex);
        sleep(3); 
    }

    pthread_mutex_unlock(&impl->data_ready_mutex);

    return NULL;
}


int jag_data_source_match(const wchar_t *file_name)
{
    // dummy implementation can load all data source that contain "dummy" frase in file_name
    return wcsstr(file_name, L"dummy") != NULL; 
}



struct jag_data_source_api* jag_load_data_source(const wchar_t *file_name)
{
    struct jag_data_source_api *result = (struct jag_data_source_api *)malloc(sizeof(struct jag_data_source_api));
    result->get_number_of_parameters = func_get_number_of_parameters;
    result->get_parameter_description = func_get_parameter_description;
    result->get_parameter_size = func_get_parameter_size;
    result->get_parameter = func_get_parameter;

    result->get_data_source_name = func_get_data_source_name;
    result->loading_progress = func_loading_progress;
    result->is_valid = func_is_valid;
    result->delete_data_source = func_delete_data_source;

    struct implementation *impl = (struct implementation*)malloc(sizeof(struct implementation));
    pthread_mutex_init(&impl->data_ready_mutex, NULL);
    pthread_mutex_init(&impl->progress_mutex, NULL);
    wcsncpy(impl->file_name, file_name, FILE_NAME_SIZE - 1);
    impl->loading_progress = 0;

    pthread_mutex_lock(&impl->data_ready_mutex);
    result->p_impl = impl;

    pthread_t thread;
    pthread_create(&thread, NULL, &loading_imitation, result);
    pthread_detach(thread);
    
    return result;
}


#ifdef __cplusplus
}
#endif
