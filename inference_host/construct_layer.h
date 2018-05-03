//
// Created by yaochen on 9/4/18.
//

/*TODO: complete an initial conv_pool_layer with existing small models
 *TODO: add the simple scheduling method with this function
 */

#ifndef _CONSTRUCT_LAYER_H_
#define _CONSTRUCT_LAYER_H_

#include "../inference_net_hls/acc_dim_config.h"
#include "acc_mem_config.h"
#include "inference_func.h"

int conv_pool_layer(int * param, data_type_w * in_weight, data_type * in_data, data_type_o out_data);
int conv_layer(int * param, data_type_w * in_weight, data_type * in_data, data_type_o out_data);
int pool_layer(int * param, data_type_w * in_weight, data_type * in_data, data_type_o out_data);
int fc_layer(int * param, data_type_w * in_weight, data_type * in_data, data_type_o out_data);


int conv_pool_layer(int * param, data_type_w * in_weight, data_type * in_data, data_type_o out_data) {

    // iterations of accelerator calls, the scheduling should be implemented here
    // this function is similar to the function in construct_layer.h
    // the layer call models could be added in this header file as well


    return 0;

}

#endif