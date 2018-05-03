// Amazon FPGA Hardware Development Kit
//
// Copyright 2016 Amazon.com, Inc. or its affiliates. All Rights Reserved.
//
// Licensed under the Amazon Software License (the "License"). You may not use
// this file except in compliance with the License. A copy of the License is
// located at
//
//    http://aws.amazon.com/asl/
//
// or in the "license" file accompanying this file. This file is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, express or
// implied. See the License for the specific language governing permissions and
// limitations under the License.


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

//C++ headers
#include <cstdint>
#include <iostream>
//#include <cstring>
#include <fstream>
//#include <cstdlib>
#include <time.h>
#include <sys/time.h>

#include <fpga_pci.h>
#include <fpga_mgmt.h>
#include <utils/lcd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./inference_net/stb_image.h"
#include "./inference_net/weight_bias_one_dim.h"
#include "./inference_net/config.h"
#include "./inference_net/inference_func.h"
#include "./inference_net/acc_bdport_config.h"

using namespace std;

/* use the stdout logger for printing debug information  */
const struct logger *logger = &logger_stdout;

int main(int argc, char **argv) {
    int rc;
    int slot_id;

    /* initialize the fpga_pci library so we could have access to FPGA PCIe from this applications */
    rc = fpga_pci_init();
    fail_on(rc, out, "Unable to initialize the fpga_pci library");

    /* This demo works with single FPGA slot, we pick slot #0 as it works for both f1.2xl and f1.16xl */
    slot_id = 0;

    rc = check_afi_ready(slot_id);
    fail_on(rc, out, "AFI not ready");
    
    /* Accessing the CL registers via AppPF BAR0, which maps to sh_cl_ocl_ AXI-Lite bus between AWS FPGA Shell and the CL*/
    printf("\n");

    printf("===== AXI CDMA Example =====\n");	
    rc = peek_poke_example(slot_id, FPGA_APP_PF, APP_PF_BAR1);
    fail_on(rc, out, "peek-poke example failed");
  
    return rc; 
   
out:
    return 1;
}

/*
 * An example to attach to an arbitrary slot, pf, and bar with register access.
 */
int peek_poke_example(int slot_id, int pf_id, int bar_id) {
    int rc;
    int rc_0;
    int rc_4;
    int rc_sda;
    uint32_t value;

    int loop_var;
    int loop_var_1;
    int loop_var_2;

    int data_1[1024]={0};
    data_type in_data[3*32*32];
    data_type in_data_part1_3D[3][20][20];
    data_type in_data_part2_3D[3][20][20];
    data_type in_data_part3_3D[3][20][20];
    data_type in_data_part4_3D[3][20][20];
    data_type in_data_part1[3*20*20];
    data_type in_data_part2[3*20*20];
    data_type in_data_part3[3*20*20];
    data_type in_data_part4[3*20*20];
    data_type  out_temp_full[8192];
    data_type  out_temp_1[4096];
    data_type  out_temp_2[4096];
    data_type  out_temp_3[4096];
    data_type  out_temp_4[4096];
    data_type  out_temp_5[4096];
    data_type  out_temp_6[4096];
    data_type  out_temp_7[4096];
    data_type  out_temp_8[4096];
    data_type  out_temp_1_1[4096];
    data_type  out_temp_2_1[4096];
    data_type  out_temp_3_1[4096];
    data_type  out_temp_4_1[4096];
    data_type  out_temp_5_1[4096];
    data_type  out_temp_6_1[4096];
    data_type  out_temp_7_1[4096];
    data_type  out_temp_8_1[4096];
    data_type  fc_3_out[10];
    data_type_w conv_weight[2400+25600+51200+10240];
    data_type_w conv_weight_1[1]={1};
    data_type_w conv_weight_2[2400+25600+51200+10240];
    data_type_w conv_bias[32+32+64+10];
    data_type_w conv_bias_1[32+32+64+10];
    float conv_1_weight2D[2400];
    float conv_1_weight_cdma[32768];
    float conv_1_bias2D[32];
    float conv_2_weight2D[25600];
    float conv_2_bias2D[32];
    float conv_3_weight2D[51200];
    float conv_3_bias2D[64];
    float fc_1_weight2D[10240];
    float fc_1_bias2D[10];
    data_type_w weight_temp_1[3][32][5][5];
    data_type_w weight_temp_2[32][32][5][5];
    data_type_w weight_temp_3[32][64][5][5];
    data_type_w weight_temp_4[64][10][4][4];
    data_type_w conv_weight_mem_port_0_0[500];
    data_type_w conv_weight_mem_port_0_1[500];
    data_type_w conv_weight_mem_port_0_2[500];
    data_type_w conv_weight_mem_port_0_3[500];
    data_type_w conv_weight_mem_port_0_4[500];
    data_type_w conv_weight_mem_port_0_5[500];
    data_type_w conv_weight_mem_port_0_6[500];
    data_type_w conv_weight_mem_port_0_7[500];
    data_type_w conv_weight_mem_port_0_8[500];
    data_type_w conv_weight_mem_port_0_9[500];
    data_type_w conv_weight_mem_port_0_10[500];
    data_type_w conv_weight_mem_port_0_11[500];
    data_type_w conv_weight_mem_port_0_12[500];
    data_type_w conv_weight_mem_port_0_13[500];
    data_type_w conv_weight_mem_port_0_14[500];
    data_type_w conv_weight_mem_port_0_15[500];
    data_type_w conv_weight_mem_port_0_16[500];
    data_type_w conv_weight_mem_port_0_17[500];
    data_type_w conv_weight_mem_port_0_18[500];
    data_type_w conv_weight_mem_port_0_19[500];
    data_type_w conv_weight_mem_port_0_20[500];
    data_type_w conv_weight_mem_port_0_21[500];
    data_type_w conv_weight_mem_port_0_22[500];
    data_type_w conv_weight_mem_port_0_23[500];
    data_type_w conv_weight_mem_port_0_24[500];
    data_type_w conv_weight_mem_port_0_25[500];
    data_type_w conv_weight_mem_port_0_26[500];
    data_type_w conv_weight_mem_port_0_27[500];
    data_type_w conv_weight_mem_port_0_28[500];
    data_type_w conv_weight_mem_port_0_29[500];
    data_type_w conv_weight_mem_port_0_30[500];
    data_type_w conv_weight_mem_port_0_31[500];
    data_type_w conv_weight_mem_port_1_0[500];
    data_type_w conv_weight_mem_port_1_1[500];
    data_type_w conv_weight_mem_port_1_2[500];
    data_type_w conv_weight_mem_port_1_3[500];
    data_type_w conv_weight_mem_port_1_4[500];
    data_type_w conv_weight_mem_port_1_5[500];
    data_type_w conv_weight_mem_port_1_6[500];
    data_type_w conv_weight_mem_port_1_7[500];
    data_type_w conv_weight_mem_port_1_8[500];
    data_type_w conv_weight_mem_port_1_9[500];
    data_type_w conv_weight_mem_port_1_10[500];
    data_type_w conv_weight_mem_port_1_11[500];
    data_type_w conv_weight_mem_port_1_12[500];
    data_type_w conv_weight_mem_port_1_13[500];
    data_type_w conv_weight_mem_port_1_14[500];
    data_type_w conv_weight_mem_port_1_15[500];
    data_type_w conv_weight_mem_port_1_16[500];
    data_type_w conv_weight_mem_port_1_17[500];
    data_type_w conv_weight_mem_port_1_18[500];
    data_type_w conv_weight_mem_port_1_19[500];
    data_type_w conv_weight_mem_port_1_20[500];
    data_type_w conv_weight_mem_port_1_21[500];
    data_type_w conv_weight_mem_port_1_22[500];
    data_type_w conv_weight_mem_port_1_23[500];
    data_type_w conv_weight_mem_port_1_24[500];
    data_type_w conv_weight_mem_port_1_25[500];
    data_type_w conv_weight_mem_port_1_26[500];
    data_type_w conv_weight_mem_port_1_27[500];
    data_type_w conv_weight_mem_port_1_28[500];
    data_type_w conv_weight_mem_port_1_29[500];
    data_type_w conv_weight_mem_port_1_30[500];
    data_type_w conv_weight_mem_port_1_31[500];
    data_type_w conv_weight_mem_port_2_0[500];
    data_type_w conv_weight_mem_port_2_1[500];
    data_type_w conv_weight_mem_port_2_2[500];
    data_type_w conv_weight_mem_port_2_3[500];
    data_type_w conv_weight_mem_port_2_4[500];
    data_type_w conv_weight_mem_port_2_5[500];
    data_type_w conv_weight_mem_port_2_6[500];
    data_type_w conv_weight_mem_port_2_7[500];
    data_type_w conv_weight_mem_port_2_8[500];
    data_type_w conv_weight_mem_port_2_9[500];
    data_type_w conv_weight_mem_port_2_10[500];
    data_type_w conv_weight_mem_port_2_11[500];
    data_type_w conv_weight_mem_port_2_12[500];
    data_type_w conv_weight_mem_port_2_13[500];
    data_type_w conv_weight_mem_port_2_14[500];
    data_type_w conv_weight_mem_port_2_15[500];
    data_type_w conv_weight_mem_port_2_16[500];
    data_type_w conv_weight_mem_port_2_17[500];
    data_type_w conv_weight_mem_port_2_18[500];
    data_type_w conv_weight_mem_port_2_19[500];
    data_type_w conv_weight_mem_port_2_20[500];
    data_type_w conv_weight_mem_port_2_21[500];
    data_type_w conv_weight_mem_port_2_22[500];
    data_type_w conv_weight_mem_port_2_23[500];
    data_type_w conv_weight_mem_port_2_24[500];
    data_type_w conv_weight_mem_port_2_25[500];
    data_type_w conv_weight_mem_port_2_26[500];
    data_type_w conv_weight_mem_port_2_27[500];
    data_type_w conv_weight_mem_port_2_28[500];
    data_type_w conv_weight_mem_port_2_29[500];
    data_type_w conv_weight_mem_port_2_30[500];
    data_type_w conv_weight_mem_port_2_31[500];
    data_type_w conv_weight_mem_port_3_0[500];
    data_type_w conv_weight_mem_port_3_1[500];
    data_type_w conv_weight_mem_port_3_2[500];
    data_type_w conv_weight_mem_port_3_3[500];
    data_type_w conv_weight_mem_port_3_4[500];
    data_type_w conv_weight_mem_port_3_5[500];
    data_type_w conv_weight_mem_port_3_6[500];
    data_type_w conv_weight_mem_port_3_7[500];
    data_type_w conv_weight_mem_port_3_8[500];
    data_type_w conv_weight_mem_port_3_9[500];
    data_type_w conv_weight_mem_port_3_10[500];
    data_type_w conv_weight_mem_port_3_11[500];
    data_type_w conv_weight_mem_port_3_12[500];
    data_type_w conv_weight_mem_port_3_13[500];
    data_type_w conv_weight_mem_port_3_14[500];
    data_type_w conv_weight_mem_port_3_15[500];
    data_type_w conv_weight_mem_port_3_16[500];
    data_type_w conv_weight_mem_port_3_17[500];
    data_type_w conv_weight_mem_port_3_18[500];
    data_type_w conv_weight_mem_port_3_19[500];
    data_type_w conv_weight_mem_port_3_20[500];
    data_type_w conv_weight_mem_port_3_21[500];
    data_type_w conv_weight_mem_port_3_22[500];
    data_type_w conv_weight_mem_port_3_23[500];
    data_type_w conv_weight_mem_port_3_24[500];
    data_type_w conv_weight_mem_port_3_25[500];
    data_type_w conv_weight_mem_port_3_26[500];
    data_type_w conv_weight_mem_port_3_27[500];
    data_type_w conv_weight_mem_port_3_28[500];
    data_type_w conv_weight_mem_port_3_29[500];
    data_type_w conv_weight_mem_port_3_30[500];
    data_type_w conv_weight_mem_port_3_31[500];
    data_type_w conv_weight_mem_port_4_0[500];
    data_type_w conv_weight_mem_port_4_1[500];
    data_type_w conv_weight_mem_port_4_2[500];
    data_type_w conv_weight_mem_port_4_3[500];
    data_type_w conv_weight_mem_port_4_4[500];
    data_type_w conv_weight_mem_port_4_5[500];
    data_type_w conv_weight_mem_port_4_6[500];
    data_type_w conv_weight_mem_port_4_7[500];
    data_type_w conv_weight_mem_port_4_8[500];
    data_type_w conv_weight_mem_port_4_9[500];
    data_type_w conv_weight_mem_port_4_10[500];
    data_type_w conv_weight_mem_port_4_11[500];
    data_type_w conv_weight_mem_port_4_12[500];
    data_type_w conv_weight_mem_port_4_13[500];
    data_type_w conv_weight_mem_port_4_14[500];
    data_type_w conv_weight_mem_port_4_15[500];
    data_type_w conv_weight_mem_port_4_16[500];
    data_type_w conv_weight_mem_port_4_17[500];
    data_type_w conv_weight_mem_port_4_18[500];
    data_type_w conv_weight_mem_port_4_19[500];
    data_type_w conv_weight_mem_port_4_20[500];
    data_type_w conv_weight_mem_port_4_21[500];
    data_type_w conv_weight_mem_port_4_22[500];
    data_type_w conv_weight_mem_port_4_23[500];
    data_type_w conv_weight_mem_port_4_24[500];
    data_type_w conv_weight_mem_port_4_25[500];
    data_type_w conv_weight_mem_port_4_26[500];
    data_type_w conv_weight_mem_port_4_27[500];
    data_type_w conv_weight_mem_port_4_28[500];
    data_type_w conv_weight_mem_port_4_29[500];
    data_type_w conv_weight_mem_port_4_30[500];
    data_type_w conv_weight_mem_port_4_31[500];
    data_type_w conv_weight_mem_port_5_0[500];
    data_type_w conv_weight_mem_port_5_1[500];
    data_type_w conv_weight_mem_port_5_2[500];
    data_type_w conv_weight_mem_port_5_3[500];
    data_type_w conv_weight_mem_port_5_4[500];
    data_type_w conv_weight_mem_port_5_5[500];
    data_type_w conv_weight_mem_port_5_6[500];
    data_type_w conv_weight_mem_port_5_7[500];
    data_type_w conv_weight_mem_port_5_8[500];
    data_type_w conv_weight_mem_port_5_9[500];
    data_type_w conv_weight_mem_port_5_10[500];
    data_type_w conv_weight_mem_port_5_11[500];
    data_type_w conv_weight_mem_port_5_12[500];
    data_type_w conv_weight_mem_port_5_13[500];
    data_type_w conv_weight_mem_port_5_14[500];
    data_type_w conv_weight_mem_port_5_15[500];
    data_type_w conv_weight_mem_port_5_16[500];
    data_type_w conv_weight_mem_port_5_17[500];
    data_type_w conv_weight_mem_port_5_18[500];
    data_type_w conv_weight_mem_port_5_19[500];
    data_type_w conv_weight_mem_port_5_20[500];
    data_type_w conv_weight_mem_port_5_21[500];
    data_type_w conv_weight_mem_port_5_22[500];
    data_type_w conv_weight_mem_port_5_23[500];
    data_type_w conv_weight_mem_port_5_24[500];
    data_type_w conv_weight_mem_port_5_25[500];
    data_type_w conv_weight_mem_port_5_26[500];
    data_type_w conv_weight_mem_port_5_27[500];
    data_type_w conv_weight_mem_port_5_28[500];
    data_type_w conv_weight_mem_port_5_29[500];
    data_type_w conv_weight_mem_port_5_30[500];
    data_type_w conv_weight_mem_port_5_31[500];
    data_type_w conv_weight_mem_port_6_0[500];
    data_type_w conv_weight_mem_port_6_1[500];
    data_type_w conv_weight_mem_port_6_2[500];
    data_type_w conv_weight_mem_port_6_3[500];
    data_type_w conv_weight_mem_port_6_4[500];
    data_type_w conv_weight_mem_port_6_5[500];
    data_type_w conv_weight_mem_port_6_6[500];
    data_type_w conv_weight_mem_port_6_7[500];
    data_type_w conv_weight_mem_port_6_8[500];
    data_type_w conv_weight_mem_port_6_9[500];
    data_type_w conv_weight_mem_port_6_10[500];
    data_type_w conv_weight_mem_port_6_11[500];
    data_type_w conv_weight_mem_port_6_12[500];
    data_type_w conv_weight_mem_port_6_13[500];
    data_type_w conv_weight_mem_port_6_14[500];
    data_type_w conv_weight_mem_port_6_15[500];
    data_type_w conv_weight_mem_port_6_16[500];
    data_type_w conv_weight_mem_port_6_17[500];
    data_type_w conv_weight_mem_port_6_18[500];
    data_type_w conv_weight_mem_port_6_19[500];
    data_type_w conv_weight_mem_port_6_20[500];
    data_type_w conv_weight_mem_port_6_21[500];
    data_type_w conv_weight_mem_port_6_22[500];
    data_type_w conv_weight_mem_port_6_23[500];
    data_type_w conv_weight_mem_port_6_24[500];
    data_type_w conv_weight_mem_port_6_25[500];
    data_type_w conv_weight_mem_port_6_26[500];
    data_type_w conv_weight_mem_port_6_27[500];
    data_type_w conv_weight_mem_port_6_28[500];
    data_type_w conv_weight_mem_port_6_29[500];
    data_type_w conv_weight_mem_port_6_30[500];
    data_type_w conv_weight_mem_port_6_31[500];
    data_type_w conv_weight_mem_port_7_0[500];
    data_type_w conv_weight_mem_port_7_1[500];
    data_type_w conv_weight_mem_port_7_2[500];
    data_type_w conv_weight_mem_port_7_3[500];
    data_type_w conv_weight_mem_port_7_4[500];
    data_type_w conv_weight_mem_port_7_5[500];
    data_type_w conv_weight_mem_port_7_6[500];
    data_type_w conv_weight_mem_port_7_7[500];
    data_type_w conv_weight_mem_port_7_8[500];
    data_type_w conv_weight_mem_port_7_9[500];
    data_type_w conv_weight_mem_port_7_10[500];
    data_type_w conv_weight_mem_port_7_11[500];
    data_type_w conv_weight_mem_port_7_12[500];
    data_type_w conv_weight_mem_port_7_13[500];
    data_type_w conv_weight_mem_port_7_14[500];
    data_type_w conv_weight_mem_port_7_15[500];
    data_type_w conv_weight_mem_port_7_16[500];
    data_type_w conv_weight_mem_port_7_17[500];
    data_type_w conv_weight_mem_port_7_18[500];
    data_type_w conv_weight_mem_port_7_19[500];
    data_type_w conv_weight_mem_port_7_20[500];
    data_type_w conv_weight_mem_port_7_21[500];
    data_type_w conv_weight_mem_port_7_22[500];
    data_type_w conv_weight_mem_port_7_23[500];
    data_type_w conv_weight_mem_port_7_24[500];
    data_type_w conv_weight_mem_port_7_25[500];
    data_type_w conv_weight_mem_port_7_26[500];
    data_type_w conv_weight_mem_port_7_27[500];
    data_type_w conv_weight_mem_port_7_28[500];
    data_type_w conv_weight_mem_port_7_29[500];
    data_type_w conv_weight_mem_port_7_30[500];
    data_type_w conv_weight_mem_port_7_31[500];
    
    int ctrl_param_1[2] = {1, 0};
    int ctrl_param_2[2] = {0, 1};
    int acc_param_conv_1[16] = {1, 0, 0, 0, 5, 0, 0, 3, 1, 0, 0, 0, 0, 0, 1, 0};
    int acc_param_conv_2[16] = {32, 5, 32, 16, 16, 16, 16, 1, 2, 1, 1024, 32, 1024, 0, 1, 1};
    int acc_param_conv_3[16] = {32, 5, 64, 8, 8, 8, 8, 1, 2, 1, 4224, 64, 0, 0, 1, 1};
    int acc_param_conv_4[16] = {64, 4, 10, 4, 4, 1, 1, 4, 0, 0, 10624, 128, 0, 0, 1, 1};
    int pool_engine_param_in[16] = {2, 0, 0, 0, 3, 16, 16, 16, 16, 0, 0, 0, 0, 0, 0, 0};
    int acc_param_pool_1[9] = {32, 32, 16, 3, 16, 16, 2, 0, 0};
    int acc_param_pool_2[9] = {16, 16, 32, 3, 8, 8, 2, 0, 0};
    int acc_param_pool_3[9] = {8, 8, 64, 3, 4, 4, 2, 0, 0};

    int w;
    int h;
    int channels;
    int size;
    const unsigned char * data ;
    const unsigned char * image_orig ;
    int in_number_conv = 0;
    int in_number_fc = 0;
    int in_number_pooling = 0;
    int conv_weight_num=0;
    int conv_bias_num=0;

    string image_dir = "./netInput/50000.png";
    const char* weight_src = "./netInput/net_weights_cifar.txt";
    ifstream ifs1("./netInput/net_mean.txt");
    float f;
    float channel_mean[3] = { 0 };
    float in_data_3D_channel_swap[3][32][32] = { 0 };
    float in_data_3D[3][32][32] = { 0 };
    float in_data_3D_padding[3][36][36] = { 0 };
    string str1;
    string y1 = "[";
    string y2 = "]";
    int index = 0;
    int in_data_size=0;
    std::ofstream indata;
    std::ofstream outdata;
    std::ofstream weightdata;
    std::ofstream test_output;

    //int i,j,k;
    int count = 0;

    //time mreasurement variable define
    struct timeval start,end;
    unsigned long diff;
    //XConv
    XInference_net InstancePtr_1;
    InstancePtr_1.ctrl_bus_baseaddress = XINFERENCE_IP_CRTL_BUS_ADDR_1;
    InstancePtr_1.IsReady = 0x01;
    //XPool
    XInference_net InstancePtr_2;
    InstancePtr_2.ctrl_bus_baseaddress = XINFERENCE_IP_CRTL_BUS_ADDR_2;
    InstancePtr_2.IsReady = 0x01;
    uint32_t ip_status;
    /* pci_bar_handle_t is a handler for an address space exposed by one PCI BAR on one of the PCI PFs of the FPGA */

    pci_bar_handle_t pci_bar_handle = PCI_BAR_HANDLE_INIT;
    pci_bar_handle_t pci_bar_handle_0 = PCI_BAR_HANDLE_INIT;
    pci_bar_handle_t pci_bar_handle_4 = PCI_BAR_HANDLE_INIT;

    pci_bar_handle_t pci_bar_handle_sda = PCI_BAR_HANDLE_INIT;

    cout << "test point 1" << endl;

    /* attach to the fpga, with a pci_bar_handle out param
     * To attach to multiple slots or BARs, call this function multiple times,
     * saving the pci_bar_handle to specify which address space to interact with in
     * other API calls.
     * This function accepts the slot_id, physical function, and bar number
     */
    rc = fpga_pci_attach(slot_id, pf_id, bar_id, 0, &pci_bar_handle);
    fail_on(rc, out, "Unable to attach to the AFI on slot id %d", slot_id);

    rc_0 = fpga_pci_attach(slot_id, pf_id, 0, 0, &pci_bar_handle_0);
    fail_on(rc_0, out, "Unable to attach to the AFI on slot id %d", slot_id);

    rc_4 = fpga_pci_attach(slot_id, pf_id, 4, 0, &pci_bar_handle_4);
    fail_on(rc_4, out, "Unable to attach to the AFI on slot id %d", slot_id);

    rc_sda = fpga_pci_attach(slot_id, FPGA_MGMT_PF, MGMT_PF_BAR4, 0, &pci_bar_handle_sda);
    fail_on(rc_sda, out, "Unable to attach to the AFI on slot id %d", slot_id);


//--------------------------input image data initialization----------------//
    if (!ifs1) {
        cout << "mean data file not found !" << endl;
        getchar();
    }
    while (ifs1 >> str1) {
        int p1 = str1.find(y1, 0);
        if (p1 >= 0) {
            str1.erase(p1, y1.length());
        }
        int p2 = str1.find(y2, 0);
        if (p2 >= 0) {
            str1.erase(p2, y2.length());
        }
        f = atof(str1.c_str());
        channel_mean[index] = f;
        index++;
    }
    ifs1.close();
    data = loadfile(image_dir, size);
    image_orig = stbi_load_from_memory(data, size, &w, &h, &channels, 3);
    for (loop_var = 0; loop_var < 3; loop_var++) {
        for (loop_var_1 = loop_var; loop_var_1 < w*h*3; loop_var_1 += 3) {
            in_data_3D_channel_swap[2 - loop_var][loop_var_1 / (w * 3)][(loop_var_1 % (w * 3) - loop_var) / 3] = (float)image_orig[loop_var_1]; //range:0--255
        }
    }
    for (loop_var = 0; loop_var < 3; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 32; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 32; loop_var_2++) {
                in_data_3D_channel_swap[loop_var][loop_var_1][loop_var_2] /= 255;// range:0--1
            }
        }
    }
    for (loop_var = 0; loop_var < 3; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 32; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 32; loop_var_2++) {
                in_data_3D[loop_var][loop_var_1][loop_var_2] = in_data_3D_channel_swap[loop_var][loop_var_1][loop_var_2] * 255 - channel_mean[loop_var];
            }
        }
    }
    //add padding for input
    for (loop_var = 0; loop_var < 3; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 36; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 36; loop_var_2++) {
                in_data_3D_padding[loop_var][loop_var_1][loop_var_2] = 0;
            }
        }
    }
    for (loop_var = 0; loop_var < 3; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 32; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 32; loop_var_2++) {
                in_data_3D_padding[loop_var][loop_var_1+2][loop_var_2+2] = (data_type)in_data_3D[loop_var][loop_var_1][loop_var_2];
            }
        }
    }
    //divide input into 4 parts
    for (loop_var = 0; loop_var < 3; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 20; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 20; loop_var_2++) {
                in_data_part1_3D[loop_var][loop_var_1][loop_var_2] = (data_type)in_data_3D_padding[loop_var][loop_var_1][loop_var_2];
            }
        }
    }
    for (loop_var = 0; loop_var < 3; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 20; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 20; loop_var_2++) {
                in_data_part2_3D[loop_var][loop_var_1][loop_var_2] = (data_type)in_data_3D_padding[loop_var][loop_var_1][loop_var_2+16];
            }
        }
    }
    for (loop_var = 0; loop_var < 3; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 20; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 20; loop_var_2++) {
                in_data_part3_3D[loop_var][loop_var_1][loop_var_2] = (data_type)in_data_3D_padding[loop_var][loop_var_1+16][loop_var_2];
            }
        }
    }
    for (loop_var = 0; loop_var < 3; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 20; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 20; loop_var_2++) {
                in_data_part4_3D[loop_var][loop_var_1][loop_var_2] = (data_type)in_data_3D_padding[loop_var][loop_var_1+16][loop_var_2+16];
            }
        }
    }
    //4 parts input 3D to 1D
    for (loop_var = 0; loop_var < 3; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 20; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 20; loop_var_2++) {
                in_data_part1[in_data_size] = (data_type)in_data_part1_3D[loop_var][loop_var_1][loop_var_2];
                in_data_size++;
            }
        }
    }
    in_data_size=0;
    for (loop_var = 0; loop_var < 3; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 20; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 20; loop_var_2++) {
                in_data_part2[in_data_size] = (data_type)in_data_part2_3D[loop_var][loop_var_1][loop_var_2];
                in_data_size++;
            }
        }
    }
    in_data_size=0;
    for (loop_var = 0; loop_var < 3; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 20; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 20; loop_var_2++) {
                in_data_part3[in_data_size] = (data_type)in_data_part3_3D[loop_var][loop_var_1][loop_var_2];
                in_data_size++;
            }
        }
    }
    in_data_size=0;
    for (loop_var = 0; loop_var < 3; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 20; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 20; loop_var_2++) {
                in_data_part4[in_data_size] = (data_type)in_data_part4_3D[loop_var][loop_var_1][loop_var_2];
                in_data_size++;
            }
        }
    }
    in_data_size=0;
    for (loop_var = 0; loop_var < 3; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 32; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 32; loop_var_2++) {
                in_data[in_data_size] = (data_type)in_data_3D[loop_var][loop_var_1][loop_var_2];
                in_data_size++;
            }
        }
    }
    indata.open("./netOutput/in_data.txt", ios::app);
    for (loop_var = 0; loop_var < 3; loop_var++) {
        indata <<"indata:"<< endl;
        for (loop_var_1 = 0; loop_var_1 < 20; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 20; loop_var_2++) {
                indata << in_data_part2_3D[loop_var][loop_var_1][loop_var_2] << " ";
            }
            indata << endl;
        }
        indata << endl;
    }
    indata << endl;
    indata.close();
    cout << endl;

//----------------------input weight data initialization ------------------//
    // Prepare weights and bias for conv layer 1
    memset(conv_1_weight2D, 0, 2400 * sizeof(float));
    load_weight_conv(
        weight_src, 
        conv_1_weight2D,
        weight_bias_record,
        nn_channel_size_conv, 
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    memset(conv_1_bias2D, 0, 32 * sizeof(float));
    load_bias_conv(
        weight_src, 
        conv_1_bias2D,
        weight_bias_record,
        nn_channel_size_conv, 
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    for (int i = 0; i < 32; i++) {
        conv_bias[conv_bias_num] = (data_type_w)conv_1_bias2D[i];
        conv_bias_num++;
    }
    in_number_conv++;

    // Prepare weights and bias for conv layer 2
    memset(conv_2_weight2D, 0, 25600 * sizeof(float));
    load_weight_conv(
        weight_src, 
        conv_2_weight2D,
        weight_bias_record,
        nn_channel_size_conv, 
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    memset(conv_2_bias2D, 0, 32 * sizeof(float));
    load_bias_conv(
        weight_src, 
        conv_2_bias2D,
        weight_bias_record,
        nn_channel_size_conv, 
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    for (int i = 0; i < 32; i++) {
        conv_bias[conv_bias_num] = (data_type_w)conv_2_bias2D[i];
        conv_bias_num++;
    }
    in_number_conv++;

    // Prepare weights and bias for conv layer 3
    memset(conv_3_weight2D, 0, 51200 * sizeof(float));
    load_weight_conv(
        weight_src, 
        conv_3_weight2D,
        weight_bias_record,
        nn_channel_size_conv, 
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    memset(conv_3_bias2D, 0, 64 * sizeof(float));
    load_bias_conv(
        weight_src, 
        conv_3_bias2D,
        weight_bias_record,
        nn_channel_size_conv, 
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    for (int i = 0; i < 64; i++) {
        conv_bias[conv_bias_num] = (data_type_w)conv_3_bias2D[i];
        conv_bias_num++;
    }
    in_number_conv++;

    // Prepare weights and bias for fc layer 1
    memset(fc_1_weight2D, 0, 10240 * sizeof(float));
    load_weight_fc(
        weight_src, 
        fc_1_weight2D,
        weight_bias_record,
        nn_channel_size_fc, 
        nn_in_number_fc,
        nn_out_number_fc,
        in_number_fc);
    memset(fc_1_bias2D, 0, 10 * sizeof(float));
    load_bias_fc(
        weight_src, 
        fc_1_bias2D,
        weight_bias_record,
        nn_channel_size_fc, 
        nn_in_number_fc,
        nn_out_number_fc,
        in_number_fc);
    for (int i = 0; i < 10; i++) {
        conv_bias[conv_bias_num] = (data_type_w)fc_1_bias2D[i];
        conv_bias_num++;
    }
    in_number_fc++;
    //conv_1
    for (int i = 0; i < 32; i++) {
        for(int j = 0; j < 3; j++){
            for(int k1 = 0; k1 < 5; k1++){
                for(int k2 = 0; k2 < 5; k2++){
                    weight_temp_1[j][i][k1][k2] = (data_type_w)conv_1_weight2D[i*3*5*5 + j*5*5 + k1*5 + k2];
                }
            }
        }
    }
    for(int k1=0; k1 <5; k1++){
        for(int k2=0; k2<5; k2++){
            conv_weight_mem_port_0_0[k1*5 + k2] = weight_temp_1[0][0][k1][k2];
            conv_weight_mem_port_0_1[k1*5 + k2] = weight_temp_1[0][1][k1][k2];
            conv_weight_mem_port_0_2[k1*5 + k2] = weight_temp_1[0][2][k1][k2];
            conv_weight_mem_port_0_3[k1*5 + k2] = weight_temp_1[0][3][k1][k2];
            conv_weight_mem_port_0_4[k1*5 + k2] = weight_temp_1[0][4][k1][k2];
            conv_weight_mem_port_0_5[k1*5 + k2] = weight_temp_1[0][5][k1][k2];
            conv_weight_mem_port_0_6[k1*5 + k2] = weight_temp_1[0][6][k1][k2];
            conv_weight_mem_port_0_7[k1*5 + k2] = weight_temp_1[0][7][k1][k2];
            conv_weight_mem_port_0_8[k1*5 + k2] = weight_temp_1[0][8][k1][k2];
            conv_weight_mem_port_0_9[k1*5 + k2] = weight_temp_1[0][9][k1][k2];
            conv_weight_mem_port_0_10[k1*5 + k2] = weight_temp_1[0][10][k1][k2];
            conv_weight_mem_port_0_11[k1*5 + k2] = weight_temp_1[0][11][k1][k2];
            conv_weight_mem_port_0_12[k1*5 + k2] = weight_temp_1[0][12][k1][k2];
            conv_weight_mem_port_0_13[k1*5 + k2] = weight_temp_1[0][13][k1][k2];
            conv_weight_mem_port_0_14[k1*5 + k2] = weight_temp_1[0][14][k1][k2];
            conv_weight_mem_port_0_15[k1*5 + k2] = weight_temp_1[0][15][k1][k2];
            conv_weight_mem_port_0_16[k1*5 + k2] = weight_temp_1[0][16][k1][k2];
            conv_weight_mem_port_0_17[k1*5 + k2] = weight_temp_1[0][17][k1][k2];
            conv_weight_mem_port_0_18[k1*5 + k2] = weight_temp_1[0][18][k1][k2];
            conv_weight_mem_port_0_19[k1*5 + k2] = weight_temp_1[0][19][k1][k2];
            conv_weight_mem_port_0_20[k1*5 + k2] = weight_temp_1[0][20][k1][k2];
            conv_weight_mem_port_0_21[k1*5 + k2] = weight_temp_1[0][21][k1][k2];
            conv_weight_mem_port_0_22[k1*5 + k2] = weight_temp_1[0][22][k1][k2];
            conv_weight_mem_port_0_23[k1*5 + k2] = weight_temp_1[0][23][k1][k2];
            conv_weight_mem_port_0_24[k1*5 + k2] = weight_temp_1[0][24][k1][k2];
            conv_weight_mem_port_0_25[k1*5 + k2] = weight_temp_1[0][25][k1][k2];
            conv_weight_mem_port_0_26[k1*5 + k2] = weight_temp_1[0][26][k1][k2];
            conv_weight_mem_port_0_27[k1*5 + k2] = weight_temp_1[0][27][k1][k2];
            conv_weight_mem_port_0_28[k1*5 + k2] = weight_temp_1[0][28][k1][k2];
            conv_weight_mem_port_0_29[k1*5 + k2] = weight_temp_1[0][29][k1][k2];
            conv_weight_mem_port_0_30[k1*5 + k2] = weight_temp_1[0][30][k1][k2];
            conv_weight_mem_port_0_31[k1*5 + k2] = weight_temp_1[0][31][k1][k2];

            conv_weight_mem_port_1_0[k1*5 + k2] = weight_temp_1[1][0][k1][k2];
            conv_weight_mem_port_1_1[k1*5 + k2] = weight_temp_1[1][1][k1][k2];
            conv_weight_mem_port_1_2[k1*5 + k2] = weight_temp_1[1][2][k1][k2];
            conv_weight_mem_port_1_3[k1*5 + k2] = weight_temp_1[1][3][k1][k2];
            conv_weight_mem_port_1_4[k1*5 + k2] = weight_temp_1[1][4][k1][k2];
            conv_weight_mem_port_1_5[k1*5 + k2] = weight_temp_1[1][5][k1][k2];
            conv_weight_mem_port_1_6[k1*5 + k2] = weight_temp_1[1][6][k1][k2];
            conv_weight_mem_port_1_7[k1*5 + k2] = weight_temp_1[1][7][k1][k2];
            conv_weight_mem_port_1_8[k1*5 + k2] = weight_temp_1[1][8][k1][k2];
            conv_weight_mem_port_1_9[k1*5 + k2] = weight_temp_1[1][9][k1][k2];
            conv_weight_mem_port_1_10[k1*5 + k2] = weight_temp_1[1][10][k1][k2];
            conv_weight_mem_port_1_11[k1*5 + k2] = weight_temp_1[1][11][k1][k2];
            conv_weight_mem_port_1_12[k1*5 + k2] = weight_temp_1[1][12][k1][k2];
            conv_weight_mem_port_1_13[k1*5 + k2] = weight_temp_1[1][13][k1][k2];
            conv_weight_mem_port_1_14[k1*5 + k2] = weight_temp_1[1][14][k1][k2];
            conv_weight_mem_port_1_15[k1*5 + k2] = weight_temp_1[1][15][k1][k2];
            conv_weight_mem_port_1_16[k1*5 + k2] = weight_temp_1[1][16][k1][k2];
            conv_weight_mem_port_1_17[k1*5 + k2] = weight_temp_1[1][17][k1][k2];
            conv_weight_mem_port_1_18[k1*5 + k2] = weight_temp_1[1][18][k1][k2];
            conv_weight_mem_port_1_19[k1*5 + k2] = weight_temp_1[1][19][k1][k2];
            conv_weight_mem_port_1_20[k1*5 + k2] = weight_temp_1[1][20][k1][k2];
            conv_weight_mem_port_1_21[k1*5 + k2] = weight_temp_1[1][21][k1][k2];
            conv_weight_mem_port_1_22[k1*5 + k2] = weight_temp_1[1][22][k1][k2];
            conv_weight_mem_port_1_23[k1*5 + k2] = weight_temp_1[1][23][k1][k2];
            conv_weight_mem_port_1_24[k1*5 + k2] = weight_temp_1[1][24][k1][k2];
            conv_weight_mem_port_1_25[k1*5 + k2] = weight_temp_1[1][25][k1][k2];
            conv_weight_mem_port_1_26[k1*5 + k2] = weight_temp_1[1][26][k1][k2];
            conv_weight_mem_port_1_27[k1*5 + k2] = weight_temp_1[1][27][k1][k2];
            conv_weight_mem_port_1_28[k1*5 + k2] = weight_temp_1[1][28][k1][k2];
            conv_weight_mem_port_1_29[k1*5 + k2] = weight_temp_1[1][29][k1][k2];
            conv_weight_mem_port_1_30[k1*5 + k2] = weight_temp_1[1][30][k1][k2];
            conv_weight_mem_port_1_31[k1*5 + k2] = weight_temp_1[1][31][k1][k2];

            conv_weight_mem_port_2_0[k1*5 + k2] = weight_temp_1[2][0][k1][k2];
            conv_weight_mem_port_2_1[k1*5 + k2] = weight_temp_1[2][1][k1][k2];
            conv_weight_mem_port_2_2[k1*5 + k2] = weight_temp_1[2][2][k1][k2];
            conv_weight_mem_port_2_3[k1*5 + k2] = weight_temp_1[2][3][k1][k2];
            conv_weight_mem_port_2_4[k1*5 + k2] = weight_temp_1[2][4][k1][k2];
            conv_weight_mem_port_2_5[k1*5 + k2] = weight_temp_1[2][5][k1][k2];
            conv_weight_mem_port_2_6[k1*5 + k2] = weight_temp_1[2][6][k1][k2];
            conv_weight_mem_port_2_7[k1*5 + k2] = weight_temp_1[2][7][k1][k2];
            conv_weight_mem_port_2_8[k1*5 + k2] = weight_temp_1[2][8][k1][k2];
            conv_weight_mem_port_2_9[k1*5 + k2] = weight_temp_1[2][9][k1][k2];
            conv_weight_mem_port_2_10[k1*5 + k2] = weight_temp_1[2][10][k1][k2];
            conv_weight_mem_port_2_11[k1*5 + k2] = weight_temp_1[2][11][k1][k2];
            conv_weight_mem_port_2_12[k1*5 + k2] = weight_temp_1[2][12][k1][k2];
            conv_weight_mem_port_2_13[k1*5 + k2] = weight_temp_1[2][13][k1][k2];
            conv_weight_mem_port_2_14[k1*5 + k2] = weight_temp_1[2][14][k1][k2];
            conv_weight_mem_port_2_15[k1*5 + k2] = weight_temp_1[2][15][k1][k2];
            conv_weight_mem_port_2_16[k1*5 + k2] = weight_temp_1[2][16][k1][k2];
            conv_weight_mem_port_2_17[k1*5 + k2] = weight_temp_1[2][17][k1][k2];
            conv_weight_mem_port_2_18[k1*5 + k2] = weight_temp_1[2][18][k1][k2];
            conv_weight_mem_port_2_19[k1*5 + k2] = weight_temp_1[2][19][k1][k2];
            conv_weight_mem_port_2_20[k1*5 + k2] = weight_temp_1[2][20][k1][k2];
            conv_weight_mem_port_2_21[k1*5 + k2] = weight_temp_1[2][21][k1][k2];
            conv_weight_mem_port_2_22[k1*5 + k2] = weight_temp_1[2][22][k1][k2];
            conv_weight_mem_port_2_23[k1*5 + k2] = weight_temp_1[2][23][k1][k2];
            conv_weight_mem_port_2_24[k1*5 + k2] = weight_temp_1[2][24][k1][k2];
            conv_weight_mem_port_2_25[k1*5 + k2] = weight_temp_1[2][25][k1][k2];
            conv_weight_mem_port_2_26[k1*5 + k2] = weight_temp_1[2][26][k1][k2];
            conv_weight_mem_port_2_27[k1*5 + k2] = weight_temp_1[2][27][k1][k2];
            conv_weight_mem_port_2_28[k1*5 + k2] = weight_temp_1[2][28][k1][k2];
            conv_weight_mem_port_2_29[k1*5 + k2] = weight_temp_1[2][29][k1][k2];
            conv_weight_mem_port_2_30[k1*5 + k2] = weight_temp_1[2][30][k1][k2];
            conv_weight_mem_port_2_31[k1*5 + k2] = weight_temp_1[2][31][k1][k2];
        }
    }
    //conv_2
    for (int i = 0; i < 32; i++) {
        for(int j = 0; j < 32; j++){
            for(int k1 = 0; k1 < 5; k1++){
                for(int k2 = 0; k2 < 5; k2++){
                    weight_temp_2[j][i][k1][k2] = (data_type_w)conv_2_weight2D[i*32*5*5 + j*5*5 + k1*5 + k2];
                }
            }
        }
    }
    for(int k=0; k <4; k++){
        for(int k1=0; k1 <5; k1++){
            for(int k2=0; k2<5; k2++){
                conv_weight_mem_port_0_0[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][0][k1][k2];
                conv_weight_mem_port_0_1[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][1][k1][k2];
                conv_weight_mem_port_0_2[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][2][k1][k2];
                conv_weight_mem_port_0_3[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][3][k1][k2];
                conv_weight_mem_port_0_4[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][4][k1][k2];
                conv_weight_mem_port_0_5[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][5][k1][k2];
                conv_weight_mem_port_0_6[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][6][k1][k2];
                conv_weight_mem_port_0_7[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][7][k1][k2];
                conv_weight_mem_port_0_8[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][8][k1][k2];
                conv_weight_mem_port_0_9[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][9][k1][k2];
                conv_weight_mem_port_0_10[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][10][k1][k2];
                conv_weight_mem_port_0_11[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][11][k1][k2];
                conv_weight_mem_port_0_12[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][12][k1][k2];
                conv_weight_mem_port_0_13[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][13][k1][k2];
                conv_weight_mem_port_0_14[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][14][k1][k2];
                conv_weight_mem_port_0_15[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][15][k1][k2];
                conv_weight_mem_port_0_16[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][16][k1][k2];
                conv_weight_mem_port_0_17[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][17][k1][k2];
                conv_weight_mem_port_0_18[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][18][k1][k2];
                conv_weight_mem_port_0_19[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][19][k1][k2];
                conv_weight_mem_port_0_20[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][20][k1][k2];
                conv_weight_mem_port_0_21[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][21][k1][k2];
                conv_weight_mem_port_0_22[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][22][k1][k2];
                conv_weight_mem_port_0_23[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][23][k1][k2];
                conv_weight_mem_port_0_24[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][24][k1][k2];
                conv_weight_mem_port_0_25[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][25][k1][k2];
                conv_weight_mem_port_0_26[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][26][k1][k2];
                conv_weight_mem_port_0_27[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][27][k1][k2];
                conv_weight_mem_port_0_28[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][28][k1][k2];
                conv_weight_mem_port_0_29[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][29][k1][k2];
                conv_weight_mem_port_0_30[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][30][k1][k2];
                conv_weight_mem_port_0_31[25 + k*25 + k1*5 + k2] = weight_temp_2[0+8*k][31][k1][k2];

                conv_weight_mem_port_1_0[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][0][k1][k2];
                conv_weight_mem_port_1_1[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][1][k1][k2];
                conv_weight_mem_port_1_2[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][2][k1][k2];
                conv_weight_mem_port_1_3[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][3][k1][k2];
                conv_weight_mem_port_1_4[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][4][k1][k2];
                conv_weight_mem_port_1_5[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][5][k1][k2];
                conv_weight_mem_port_1_6[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][6][k1][k2];
                conv_weight_mem_port_1_7[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][7][k1][k2];
                conv_weight_mem_port_1_8[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][8][k1][k2];
                conv_weight_mem_port_1_9[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][9][k1][k2];
                conv_weight_mem_port_1_10[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][10][k1][k2];
                conv_weight_mem_port_1_11[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][11][k1][k2];
                conv_weight_mem_port_1_12[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][12][k1][k2];
                conv_weight_mem_port_1_13[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][13][k1][k2];
                conv_weight_mem_port_1_14[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][14][k1][k2];
                conv_weight_mem_port_1_15[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][15][k1][k2];
                conv_weight_mem_port_1_16[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][16][k1][k2];
                conv_weight_mem_port_1_17[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][17][k1][k2];
                conv_weight_mem_port_1_18[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][18][k1][k2];
                conv_weight_mem_port_1_19[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][19][k1][k2];
                conv_weight_mem_port_1_20[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][20][k1][k2];
                conv_weight_mem_port_1_21[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][21][k1][k2];
                conv_weight_mem_port_1_22[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][22][k1][k2];
                conv_weight_mem_port_1_23[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][23][k1][k2];
                conv_weight_mem_port_1_24[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][24][k1][k2];
                conv_weight_mem_port_1_25[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][25][k1][k2];
                conv_weight_mem_port_1_26[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][26][k1][k2];
                conv_weight_mem_port_1_27[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][27][k1][k2];
                conv_weight_mem_port_1_28[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][28][k1][k2];
                conv_weight_mem_port_1_29[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][29][k1][k2];
                conv_weight_mem_port_1_30[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][30][k1][k2];
                conv_weight_mem_port_1_31[25 + k*25 + k1*5 + k2] = weight_temp_2[1+8*k][31][k1][k2];

                conv_weight_mem_port_2_0[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][0][k1][k2];
                conv_weight_mem_port_2_1[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][1][k1][k2];
                conv_weight_mem_port_2_2[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][2][k1][k2];
                conv_weight_mem_port_2_3[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][3][k1][k2];
                conv_weight_mem_port_2_4[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][4][k1][k2];
                conv_weight_mem_port_2_5[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][5][k1][k2];
                conv_weight_mem_port_2_6[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][6][k1][k2];
                conv_weight_mem_port_2_7[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][7][k1][k2];
                conv_weight_mem_port_2_8[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][8][k1][k2];
                conv_weight_mem_port_2_9[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][9][k1][k2];
                conv_weight_mem_port_2_10[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][10][k1][k2];
                conv_weight_mem_port_2_11[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][11][k1][k2];
                conv_weight_mem_port_2_12[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][12][k1][k2];
                conv_weight_mem_port_2_13[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][13][k1][k2];
                conv_weight_mem_port_2_14[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][14][k1][k2];
                conv_weight_mem_port_2_15[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][15][k1][k2];
                conv_weight_mem_port_2_16[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][16][k1][k2];
                conv_weight_mem_port_2_17[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][17][k1][k2];
                conv_weight_mem_port_2_18[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][18][k1][k2];
                conv_weight_mem_port_2_19[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][19][k1][k2];
                conv_weight_mem_port_2_20[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][20][k1][k2];
                conv_weight_mem_port_2_21[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][21][k1][k2];
                conv_weight_mem_port_2_22[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][22][k1][k2];
                conv_weight_mem_port_2_23[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][23][k1][k2];
                conv_weight_mem_port_2_24[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][24][k1][k2];
                conv_weight_mem_port_2_25[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][25][k1][k2];
                conv_weight_mem_port_2_26[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][26][k1][k2];
                conv_weight_mem_port_2_27[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][27][k1][k2];
                conv_weight_mem_port_2_28[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][28][k1][k2];
                conv_weight_mem_port_2_29[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][29][k1][k2];
                conv_weight_mem_port_2_30[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][30][k1][k2];
                conv_weight_mem_port_2_31[25 + k*25 + k1*5 + k2] = weight_temp_2[2+8*k][31][k1][k2];
                
                conv_weight_mem_port_3_0[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][0][k1][k2];
                conv_weight_mem_port_3_1[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][1][k1][k2];
                conv_weight_mem_port_3_2[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][2][k1][k2];
                conv_weight_mem_port_3_3[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][3][k1][k2];
                conv_weight_mem_port_3_4[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][4][k1][k2];
                conv_weight_mem_port_3_5[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][5][k1][k2];
                conv_weight_mem_port_3_6[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][6][k1][k2];
                conv_weight_mem_port_3_7[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][7][k1][k2];
                conv_weight_mem_port_3_8[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][8][k1][k2];
                conv_weight_mem_port_3_9[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][9][k1][k2];
                conv_weight_mem_port_3_10[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][10][k1][k2];
                conv_weight_mem_port_3_11[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][11][k1][k2];
                conv_weight_mem_port_3_12[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][12][k1][k2];
                conv_weight_mem_port_3_13[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][13][k1][k2];
                conv_weight_mem_port_3_14[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][14][k1][k2];
                conv_weight_mem_port_3_15[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][15][k1][k2];
                conv_weight_mem_port_3_16[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][16][k1][k2];
                conv_weight_mem_port_3_17[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][17][k1][k2];
                conv_weight_mem_port_3_18[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][18][k1][k2];
                conv_weight_mem_port_3_19[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][19][k1][k2];
                conv_weight_mem_port_3_20[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][20][k1][k2];
                conv_weight_mem_port_3_21[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][21][k1][k2];
                conv_weight_mem_port_3_22[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][22][k1][k2];
                conv_weight_mem_port_3_23[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][23][k1][k2];
                conv_weight_mem_port_3_24[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][24][k1][k2];
                conv_weight_mem_port_3_25[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][25][k1][k2];
                conv_weight_mem_port_3_26[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][26][k1][k2];
                conv_weight_mem_port_3_27[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][27][k1][k2];
                conv_weight_mem_port_3_28[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][28][k1][k2];
                conv_weight_mem_port_3_29[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][29][k1][k2];
                conv_weight_mem_port_3_30[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][30][k1][k2];
                conv_weight_mem_port_3_31[25 + k*25 + k1*5 + k2] = weight_temp_2[3+8*k][31][k1][k2];

                conv_weight_mem_port_4_0[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][0][k1][k2];
                conv_weight_mem_port_4_1[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][1][k1][k2];
                conv_weight_mem_port_4_2[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][2][k1][k2];
                conv_weight_mem_port_4_3[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][3][k1][k2];
                conv_weight_mem_port_4_4[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][4][k1][k2];
                conv_weight_mem_port_4_5[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][5][k1][k2];
                conv_weight_mem_port_4_6[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][6][k1][k2];
                conv_weight_mem_port_4_7[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][7][k1][k2];
                conv_weight_mem_port_4_8[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][8][k1][k2];
                conv_weight_mem_port_4_9[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][9][k1][k2];
                conv_weight_mem_port_4_10[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][10][k1][k2];
                conv_weight_mem_port_4_11[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][11][k1][k2];
                conv_weight_mem_port_4_12[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][12][k1][k2];
                conv_weight_mem_port_4_13[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][13][k1][k2];
                conv_weight_mem_port_4_14[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][14][k1][k2];
                conv_weight_mem_port_4_15[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][15][k1][k2];
                conv_weight_mem_port_4_16[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][16][k1][k2];
                conv_weight_mem_port_4_17[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][17][k1][k2];
                conv_weight_mem_port_4_18[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][18][k1][k2];
                conv_weight_mem_port_4_19[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][19][k1][k2];
                conv_weight_mem_port_4_20[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][20][k1][k2];
                conv_weight_mem_port_4_21[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][21][k1][k2];
                conv_weight_mem_port_4_22[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][22][k1][k2];
                conv_weight_mem_port_4_23[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][23][k1][k2];
                conv_weight_mem_port_4_24[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][24][k1][k2];
                conv_weight_mem_port_4_25[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][25][k1][k2];
                conv_weight_mem_port_4_26[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][26][k1][k2];
                conv_weight_mem_port_4_27[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][27][k1][k2];
                conv_weight_mem_port_4_28[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][28][k1][k2];
                conv_weight_mem_port_4_29[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][29][k1][k2];
                conv_weight_mem_port_4_30[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][30][k1][k2];
                conv_weight_mem_port_4_31[25 + k*25 + k1*5 + k2] = weight_temp_2[4+8*k][31][k1][k2];

                conv_weight_mem_port_5_0[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][0][k1][k2];
                conv_weight_mem_port_5_1[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][1][k1][k2];
                conv_weight_mem_port_5_2[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][2][k1][k2];
                conv_weight_mem_port_5_3[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][3][k1][k2];
                conv_weight_mem_port_5_4[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][4][k1][k2];
                conv_weight_mem_port_5_5[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][5][k1][k2];
                conv_weight_mem_port_5_6[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][6][k1][k2];
                conv_weight_mem_port_5_7[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][7][k1][k2];
                conv_weight_mem_port_5_8[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][8][k1][k2];
                conv_weight_mem_port_5_9[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][9][k1][k2];
                conv_weight_mem_port_5_10[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][10][k1][k2];
                conv_weight_mem_port_5_11[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][11][k1][k2];
                conv_weight_mem_port_5_12[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][12][k1][k2];
                conv_weight_mem_port_5_13[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][13][k1][k2];
                conv_weight_mem_port_5_14[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][14][k1][k2];
                conv_weight_mem_port_5_15[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][15][k1][k2];
                conv_weight_mem_port_5_16[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][16][k1][k2];
                conv_weight_mem_port_5_17[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][17][k1][k2];
                conv_weight_mem_port_5_18[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][18][k1][k2];
                conv_weight_mem_port_5_19[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][19][k1][k2];
                conv_weight_mem_port_5_20[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][20][k1][k2];
                conv_weight_mem_port_5_21[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][21][k1][k2];
                conv_weight_mem_port_5_22[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][22][k1][k2];
                conv_weight_mem_port_5_23[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][23][k1][k2];
                conv_weight_mem_port_5_24[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][24][k1][k2];
                conv_weight_mem_port_5_25[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][25][k1][k2];
                conv_weight_mem_port_5_26[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][26][k1][k2];
                conv_weight_mem_port_5_27[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][27][k1][k2];
                conv_weight_mem_port_5_28[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][28][k1][k2];
                conv_weight_mem_port_5_29[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][29][k1][k2];
                conv_weight_mem_port_5_30[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][30][k1][k2];
                conv_weight_mem_port_5_31[25 + k*25 + k1*5 + k2] = weight_temp_2[5+8*k][31][k1][k2];

                conv_weight_mem_port_6_0[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][0][k1][k2];
                conv_weight_mem_port_6_1[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][1][k1][k2];
                conv_weight_mem_port_6_2[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][2][k1][k2];
                conv_weight_mem_port_6_3[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][3][k1][k2];
                conv_weight_mem_port_6_4[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][4][k1][k2];
                conv_weight_mem_port_6_5[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][5][k1][k2];
                conv_weight_mem_port_6_6[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][6][k1][k2];
                conv_weight_mem_port_6_7[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][7][k1][k2];
                conv_weight_mem_port_6_8[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][8][k1][k2];
                conv_weight_mem_port_6_9[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][9][k1][k2];
                conv_weight_mem_port_6_10[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][10][k1][k2];
                conv_weight_mem_port_6_11[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][11][k1][k2];
                conv_weight_mem_port_6_12[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][12][k1][k2];
                conv_weight_mem_port_6_13[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][13][k1][k2];
                conv_weight_mem_port_6_14[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][14][k1][k2];
                conv_weight_mem_port_6_15[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][15][k1][k2];
                conv_weight_mem_port_6_16[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][16][k1][k2];
                conv_weight_mem_port_6_17[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][17][k1][k2];
                conv_weight_mem_port_6_18[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][18][k1][k2];
                conv_weight_mem_port_6_19[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][19][k1][k2];
                conv_weight_mem_port_6_20[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][20][k1][k2];
                conv_weight_mem_port_6_21[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][21][k1][k2];
                conv_weight_mem_port_6_22[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][22][k1][k2];
                conv_weight_mem_port_6_23[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][23][k1][k2];
                conv_weight_mem_port_6_24[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][24][k1][k2];
                conv_weight_mem_port_6_25[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][25][k1][k2];
                conv_weight_mem_port_6_26[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][26][k1][k2];
                conv_weight_mem_port_6_27[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][27][k1][k2];
                conv_weight_mem_port_6_28[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][28][k1][k2];
                conv_weight_mem_port_6_29[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][29][k1][k2];
                conv_weight_mem_port_6_30[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][30][k1][k2];
                conv_weight_mem_port_6_31[25 + k*25 + k1*5 + k2] = weight_temp_2[6+8*k][31][k1][k2];

                conv_weight_mem_port_7_0[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][0][k1][k2];
                conv_weight_mem_port_7_1[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][1][k1][k2];
                conv_weight_mem_port_7_2[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][2][k1][k2];
                conv_weight_mem_port_7_3[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][3][k1][k2];
                conv_weight_mem_port_7_4[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][4][k1][k2];
                conv_weight_mem_port_7_5[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][5][k1][k2];
                conv_weight_mem_port_7_6[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][6][k1][k2];
                conv_weight_mem_port_7_7[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][7][k1][k2];
                conv_weight_mem_port_7_8[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][8][k1][k2];
                conv_weight_mem_port_7_9[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][9][k1][k2];
                conv_weight_mem_port_7_10[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][10][k1][k2];
                conv_weight_mem_port_7_11[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][11][k1][k2];
                conv_weight_mem_port_7_12[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][12][k1][k2];
                conv_weight_mem_port_7_13[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][13][k1][k2];
                conv_weight_mem_port_7_14[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][14][k1][k2];
                conv_weight_mem_port_7_15[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][15][k1][k2];
                conv_weight_mem_port_7_16[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][16][k1][k2];
                conv_weight_mem_port_7_17[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][17][k1][k2];
                conv_weight_mem_port_7_18[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][18][k1][k2];
                conv_weight_mem_port_7_19[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][19][k1][k2];
                conv_weight_mem_port_7_20[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][20][k1][k2];
                conv_weight_mem_port_7_21[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][21][k1][k2];
                conv_weight_mem_port_7_22[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][22][k1][k2];
                conv_weight_mem_port_7_23[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][23][k1][k2];
                conv_weight_mem_port_7_24[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][24][k1][k2];
                conv_weight_mem_port_7_25[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][25][k1][k2];
                conv_weight_mem_port_7_26[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][26][k1][k2];
                conv_weight_mem_port_7_27[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][27][k1][k2];
                conv_weight_mem_port_7_28[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][28][k1][k2];
                conv_weight_mem_port_7_29[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][29][k1][k2];
                conv_weight_mem_port_7_30[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][30][k1][k2];
                conv_weight_mem_port_7_31[25 + k*25 + k1*5 + k2] = weight_temp_2[7+8*k][31][k1][k2];
            }
        }
    }
    //conv_3
    for (int i = 0; i < 64; i++) {
        for(int j = 0; j < 32; j++){
            for(int k1 = 0; k1 < 5; k1++){
                for(int k2 = 0; k2 < 5; k2++){
                    weight_temp_3[j][i][k1][k2] = (data_type_w)conv_3_weight2D[i*32*5*5 + j*5*5 + k1*5 + k2];
                }
            }
        }
    }
    for(int j=0; j<2; j++){
        for(int k=0; k <4; k++){
            for(int k1=0; k1 <5; k1++){
                for(int k2=0; k2<5; k2++){
                    conv_weight_mem_port_0_0[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][0+32*j][k1][k2];
                    conv_weight_mem_port_0_1[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][1+32*j][k1][k2];
                    conv_weight_mem_port_0_2[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][2+32*j][k1][k2];
                    conv_weight_mem_port_0_3[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][3+32*j][k1][k2];
                    conv_weight_mem_port_0_4[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][4+32*j][k1][k2];
                    conv_weight_mem_port_0_5[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][5+32*j][k1][k2];
                    conv_weight_mem_port_0_6[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][6+32*j][k1][k2];
                    conv_weight_mem_port_0_7[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][7+32*j][k1][k2];
                    conv_weight_mem_port_0_8[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][8+32*j][k1][k2];
                    conv_weight_mem_port_0_9[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][9+32*j][k1][k2];
                    conv_weight_mem_port_0_10[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][10+32*j][k1][k2];
                    conv_weight_mem_port_0_11[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][11+32*j][k1][k2];
                    conv_weight_mem_port_0_12[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][12+32*j][k1][k2];
                    conv_weight_mem_port_0_13[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][13+32*j][k1][k2];
                    conv_weight_mem_port_0_14[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][14+32*j][k1][k2];
                    conv_weight_mem_port_0_15[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][15+32*j][k1][k2];
                    conv_weight_mem_port_0_16[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][16+32*j][k1][k2];
                    conv_weight_mem_port_0_17[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][17+32*j][k1][k2];
                    conv_weight_mem_port_0_18[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][18+32*j][k1][k2];
                    conv_weight_mem_port_0_19[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][19+32*j][k1][k2];
                    conv_weight_mem_port_0_20[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][20+32*j][k1][k2];
                    conv_weight_mem_port_0_21[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][21+32*j][k1][k2];
                    conv_weight_mem_port_0_22[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][22+32*j][k1][k2];
                    conv_weight_mem_port_0_23[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][23+32*j][k1][k2];
                    conv_weight_mem_port_0_24[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][24+32*j][k1][k2];
                    conv_weight_mem_port_0_25[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][25+32*j][k1][k2];
                    conv_weight_mem_port_0_26[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][26+32*j][k1][k2];
                    conv_weight_mem_port_0_27[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][27+32*j][k1][k2];
                    conv_weight_mem_port_0_28[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][28+32*j][k1][k2];
                    conv_weight_mem_port_0_29[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][29+32*j][k1][k2];
                    conv_weight_mem_port_0_30[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][30+32*j][k1][k2];
                    conv_weight_mem_port_0_31[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[0+8*k][31+32*j][k1][k2];

                    conv_weight_mem_port_1_0[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][0+32*j][k1][k2];
                    conv_weight_mem_port_1_1[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][1+32*j][k1][k2];
                    conv_weight_mem_port_1_2[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][2+32*j][k1][k2];
                    conv_weight_mem_port_1_3[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][3+32*j][k1][k2];
                    conv_weight_mem_port_1_4[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][4+32*j][k1][k2];
                    conv_weight_mem_port_1_5[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][5+32*j][k1][k2];
                    conv_weight_mem_port_1_6[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][6+32*j][k1][k2];
                    conv_weight_mem_port_1_7[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][7+32*j][k1][k2];
                    conv_weight_mem_port_1_8[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][8+32*j][k1][k2];
                    conv_weight_mem_port_1_9[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][9+32*j][k1][k2];
                    conv_weight_mem_port_1_10[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][10+32*j][k1][k2];
                    conv_weight_mem_port_1_11[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][11+32*j][k1][k2];
                    conv_weight_mem_port_1_12[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][12+32*j][k1][k2];
                    conv_weight_mem_port_1_13[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][13+32*j][k1][k2];
                    conv_weight_mem_port_1_14[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][14+32*j][k1][k2];
                    conv_weight_mem_port_1_15[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][15+32*j][k1][k2];
                    conv_weight_mem_port_1_16[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][16+32*j][k1][k2];
                    conv_weight_mem_port_1_17[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][17+32*j][k1][k2];
                    conv_weight_mem_port_1_18[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][18+32*j][k1][k2];
                    conv_weight_mem_port_1_19[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][19+32*j][k1][k2];
                    conv_weight_mem_port_1_20[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][20+32*j][k1][k2];
                    conv_weight_mem_port_1_21[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][21+32*j][k1][k2];
                    conv_weight_mem_port_1_22[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][22+32*j][k1][k2];
                    conv_weight_mem_port_1_23[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][23+32*j][k1][k2];
                    conv_weight_mem_port_1_24[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][24+32*j][k1][k2];
                    conv_weight_mem_port_1_25[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][25+32*j][k1][k2];
                    conv_weight_mem_port_1_26[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][26+32*j][k1][k2];
                    conv_weight_mem_port_1_27[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][27+32*j][k1][k2];
                    conv_weight_mem_port_1_28[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][28+32*j][k1][k2];
                    conv_weight_mem_port_1_29[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][29+32*j][k1][k2];
                    conv_weight_mem_port_1_30[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][30+32*j][k1][k2];
                    conv_weight_mem_port_1_31[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[1+8*k][31+32*j][k1][k2];

                    conv_weight_mem_port_2_0[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][0+32*j][k1][k2];
                    conv_weight_mem_port_2_1[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][1+32*j][k1][k2];
                    conv_weight_mem_port_2_2[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][2+32*j][k1][k2];
                    conv_weight_mem_port_2_3[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][3+32*j][k1][k2];
                    conv_weight_mem_port_2_4[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][4+32*j][k1][k2];
                    conv_weight_mem_port_2_5[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][5+32*j][k1][k2];
                    conv_weight_mem_port_2_6[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][6+32*j][k1][k2];
                    conv_weight_mem_port_2_7[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][7+32*j][k1][k2];
                    conv_weight_mem_port_2_8[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][8+32*j][k1][k2];
                    conv_weight_mem_port_2_9[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][9+32*j][k1][k2];
                    conv_weight_mem_port_2_10[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][10+32*j][k1][k2];
                    conv_weight_mem_port_2_11[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][11+32*j][k1][k2];
                    conv_weight_mem_port_2_12[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][12+32*j][k1][k2];
                    conv_weight_mem_port_2_13[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][13+32*j][k1][k2];
                    conv_weight_mem_port_2_14[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][14+32*j][k1][k2];
                    conv_weight_mem_port_2_15[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][15+32*j][k1][k2];
                    conv_weight_mem_port_2_16[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][16+32*j][k1][k2];
                    conv_weight_mem_port_2_17[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][17+32*j][k1][k2];
                    conv_weight_mem_port_2_18[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][18+32*j][k1][k2];
                    conv_weight_mem_port_2_19[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][19+32*j][k1][k2];
                    conv_weight_mem_port_2_20[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][20+32*j][k1][k2];
                    conv_weight_mem_port_2_21[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][21+32*j][k1][k2];
                    conv_weight_mem_port_2_22[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][22+32*j][k1][k2];
                    conv_weight_mem_port_2_23[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][23+32*j][k1][k2];
                    conv_weight_mem_port_2_24[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][24+32*j][k1][k2];
                    conv_weight_mem_port_2_25[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][25+32*j][k1][k2];
                    conv_weight_mem_port_2_26[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][26+32*j][k1][k2];
                    conv_weight_mem_port_2_27[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][27+32*j][k1][k2];
                    conv_weight_mem_port_2_28[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][28+32*j][k1][k2];
                    conv_weight_mem_port_2_29[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][29+32*j][k1][k2];
                    conv_weight_mem_port_2_30[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][30+32*j][k1][k2];
                    conv_weight_mem_port_2_31[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[2+8*k][31+32*j][k1][k2];
                    
                    conv_weight_mem_port_3_0[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][0+32*j][k1][k2];
                    conv_weight_mem_port_3_1[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][1+32*j][k1][k2];
                    conv_weight_mem_port_3_2[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][2+32*j][k1][k2];
                    conv_weight_mem_port_3_3[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][3+32*j][k1][k2];
                    conv_weight_mem_port_3_4[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][4+32*j][k1][k2];
                    conv_weight_mem_port_3_5[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][5+32*j][k1][k2];
                    conv_weight_mem_port_3_6[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][6+32*j][k1][k2];
                    conv_weight_mem_port_3_7[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][7+32*j][k1][k2];
                    conv_weight_mem_port_3_8[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][8+32*j][k1][k2];
                    conv_weight_mem_port_3_9[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][9+32*j][k1][k2];
                    conv_weight_mem_port_3_10[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][10+32*j][k1][k2];
                    conv_weight_mem_port_3_11[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][11+32*j][k1][k2];
                    conv_weight_mem_port_3_12[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][12+32*j][k1][k2];
                    conv_weight_mem_port_3_13[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][13+32*j][k1][k2];
                    conv_weight_mem_port_3_14[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][14+32*j][k1][k2];
                    conv_weight_mem_port_3_15[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][15+32*j][k1][k2];
                    conv_weight_mem_port_3_16[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][16+32*j][k1][k2];
                    conv_weight_mem_port_3_17[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][17+32*j][k1][k2];
                    conv_weight_mem_port_3_18[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][18+32*j][k1][k2];
                    conv_weight_mem_port_3_19[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][19+32*j][k1][k2];
                    conv_weight_mem_port_3_20[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][20+32*j][k1][k2];
                    conv_weight_mem_port_3_21[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][21+32*j][k1][k2];
                    conv_weight_mem_port_3_22[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][22+32*j][k1][k2];
                    conv_weight_mem_port_3_23[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][23+32*j][k1][k2];
                    conv_weight_mem_port_3_24[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][24+32*j][k1][k2];
                    conv_weight_mem_port_3_25[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][25+32*j][k1][k2];
                    conv_weight_mem_port_3_26[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][26+32*j][k1][k2];
                    conv_weight_mem_port_3_27[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][27+32*j][k1][k2];
                    conv_weight_mem_port_3_28[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][28+32*j][k1][k2];
                    conv_weight_mem_port_3_29[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][29+32*j][k1][k2];
                    conv_weight_mem_port_3_30[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][30+32*j][k1][k2];
                    conv_weight_mem_port_3_31[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[3+8*k][31+32*j][k1][k2];

                    conv_weight_mem_port_4_0[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][0+32*j][k1][k2];
                    conv_weight_mem_port_4_1[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][1+32*j][k1][k2];
                    conv_weight_mem_port_4_2[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][2+32*j][k1][k2];
                    conv_weight_mem_port_4_3[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][3+32*j][k1][k2];
                    conv_weight_mem_port_4_4[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][4+32*j][k1][k2];
                    conv_weight_mem_port_4_5[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][5+32*j][k1][k2];
                    conv_weight_mem_port_4_6[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][6+32*j][k1][k2];
                    conv_weight_mem_port_4_7[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][7+32*j][k1][k2];
                    conv_weight_mem_port_4_8[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][8+32*j][k1][k2];
                    conv_weight_mem_port_4_9[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][9+32*j][k1][k2];
                    conv_weight_mem_port_4_10[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][10+32*j][k1][k2];
                    conv_weight_mem_port_4_11[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][11+32*j][k1][k2];
                    conv_weight_mem_port_4_12[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][12+32*j][k1][k2];
                    conv_weight_mem_port_4_13[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][13+32*j][k1][k2];
                    conv_weight_mem_port_4_14[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][14+32*j][k1][k2];
                    conv_weight_mem_port_4_15[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][15+32*j][k1][k2];
                    conv_weight_mem_port_4_16[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][16+32*j][k1][k2];
                    conv_weight_mem_port_4_17[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][17+32*j][k1][k2];
                    conv_weight_mem_port_4_18[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][18+32*j][k1][k2];
                    conv_weight_mem_port_4_19[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][19+32*j][k1][k2];
                    conv_weight_mem_port_4_20[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][20+32*j][k1][k2];
                    conv_weight_mem_port_4_21[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][21+32*j][k1][k2];
                    conv_weight_mem_port_4_22[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][22+32*j][k1][k2];
                    conv_weight_mem_port_4_23[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][23+32*j][k1][k2];
                    conv_weight_mem_port_4_24[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][24+32*j][k1][k2];
                    conv_weight_mem_port_4_25[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][25+32*j][k1][k2];
                    conv_weight_mem_port_4_26[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][26+32*j][k1][k2];
                    conv_weight_mem_port_4_27[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][27+32*j][k1][k2];
                    conv_weight_mem_port_4_28[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][28+32*j][k1][k2];
                    conv_weight_mem_port_4_29[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][29+32*j][k1][k2];
                    conv_weight_mem_port_4_30[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][30+32*j][k1][k2];
                    conv_weight_mem_port_4_31[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[4+8*k][31+32*j][k1][k2];

                    conv_weight_mem_port_5_0[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][0+32*j][k1][k2];
                    conv_weight_mem_port_5_1[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][1+32*j][k1][k2];
                    conv_weight_mem_port_5_2[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][2+32*j][k1][k2];
                    conv_weight_mem_port_5_3[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][3+32*j][k1][k2];
                    conv_weight_mem_port_5_4[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][4+32*j][k1][k2];
                    conv_weight_mem_port_5_5[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][5+32*j][k1][k2];
                    conv_weight_mem_port_5_6[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][6+32*j][k1][k2];
                    conv_weight_mem_port_5_7[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][7+32*j][k1][k2];
                    conv_weight_mem_port_5_8[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][8+32*j][k1][k2];
                    conv_weight_mem_port_5_9[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][9+32*j][k1][k2];
                    conv_weight_mem_port_5_10[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][10+32*j][k1][k2];
                    conv_weight_mem_port_5_11[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][11+32*j][k1][k2];
                    conv_weight_mem_port_5_12[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][12+32*j][k1][k2];
                    conv_weight_mem_port_5_13[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][13+32*j][k1][k2];
                    conv_weight_mem_port_5_14[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][14+32*j][k1][k2];
                    conv_weight_mem_port_5_15[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][15+32*j][k1][k2];
                    conv_weight_mem_port_5_16[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][16+32*j][k1][k2];
                    conv_weight_mem_port_5_17[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][17+32*j][k1][k2];
                    conv_weight_mem_port_5_18[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][18+32*j][k1][k2];
                    conv_weight_mem_port_5_19[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][19+32*j][k1][k2];
                    conv_weight_mem_port_5_20[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][20+32*j][k1][k2];
                    conv_weight_mem_port_5_21[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][21+32*j][k1][k2];
                    conv_weight_mem_port_5_22[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][22+32*j][k1][k2];
                    conv_weight_mem_port_5_23[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][23+32*j][k1][k2];
                    conv_weight_mem_port_5_24[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][24+32*j][k1][k2];
                    conv_weight_mem_port_5_25[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][25+32*j][k1][k2];
                    conv_weight_mem_port_5_26[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][26+32*j][k1][k2];
                    conv_weight_mem_port_5_27[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][27+32*j][k1][k2];
                    conv_weight_mem_port_5_28[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][28+32*j][k1][k2];
                    conv_weight_mem_port_5_29[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][29+32*j][k1][k2];
                    conv_weight_mem_port_5_30[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][30+32*j][k1][k2];
                    conv_weight_mem_port_5_31[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[5+8*k][31+32*j][k1][k2];

                    conv_weight_mem_port_6_0[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][0+32*j][k1][k2];
                    conv_weight_mem_port_6_1[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][1+32*j][k1][k2];
                    conv_weight_mem_port_6_2[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][2+32*j][k1][k2];
                    conv_weight_mem_port_6_3[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][3+32*j][k1][k2];
                    conv_weight_mem_port_6_4[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][4+32*j][k1][k2];
                    conv_weight_mem_port_6_5[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][5+32*j][k1][k2];
                    conv_weight_mem_port_6_6[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][6+32*j][k1][k2];
                    conv_weight_mem_port_6_7[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][7+32*j][k1][k2];
                    conv_weight_mem_port_6_8[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][8+32*j][k1][k2];
                    conv_weight_mem_port_6_9[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][9+32*j][k1][k2];
                    conv_weight_mem_port_6_10[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][10+32*j][k1][k2];
                    conv_weight_mem_port_6_11[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][11+32*j][k1][k2];
                    conv_weight_mem_port_6_12[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][12+32*j][k1][k2];
                    conv_weight_mem_port_6_13[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][13+32*j][k1][k2];
                    conv_weight_mem_port_6_14[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][14+32*j][k1][k2];
                    conv_weight_mem_port_6_15[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][15+32*j][k1][k2];
                    conv_weight_mem_port_6_16[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][16+32*j][k1][k2];
                    conv_weight_mem_port_6_17[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][17+32*j][k1][k2];
                    conv_weight_mem_port_6_18[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][18+32*j][k1][k2];
                    conv_weight_mem_port_6_19[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][19+32*j][k1][k2];
                    conv_weight_mem_port_6_20[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][20+32*j][k1][k2];
                    conv_weight_mem_port_6_21[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][21+32*j][k1][k2];
                    conv_weight_mem_port_6_22[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][22+32*j][k1][k2];
                    conv_weight_mem_port_6_23[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][23+32*j][k1][k2];
                    conv_weight_mem_port_6_24[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][24+32*j][k1][k2];
                    conv_weight_mem_port_6_25[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][25+32*j][k1][k2];
                    conv_weight_mem_port_6_26[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][26+32*j][k1][k2];
                    conv_weight_mem_port_6_27[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][27+32*j][k1][k2];
                    conv_weight_mem_port_6_28[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][28+32*j][k1][k2];
                    conv_weight_mem_port_6_29[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][29+32*j][k1][k2];
                    conv_weight_mem_port_6_30[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][30+32*j][k1][k2];
                    conv_weight_mem_port_6_31[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[6+8*k][31+32*j][k1][k2];

                    conv_weight_mem_port_7_0[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][0+32*j][k1][k2];
                    conv_weight_mem_port_7_1[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][1+32*j][k1][k2];
                    conv_weight_mem_port_7_2[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][2+32*j][k1][k2];
                    conv_weight_mem_port_7_3[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][3+32*j][k1][k2];
                    conv_weight_mem_port_7_4[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][4+32*j][k1][k2];
                    conv_weight_mem_port_7_5[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][5+32*j][k1][k2];
                    conv_weight_mem_port_7_6[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][6+32*j][k1][k2];
                    conv_weight_mem_port_7_7[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][7+32*j][k1][k2];
                    conv_weight_mem_port_7_8[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][8+32*j][k1][k2];
                    conv_weight_mem_port_7_9[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][9+32*j][k1][k2];
                    conv_weight_mem_port_7_10[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][10+32*j][k1][k2];
                    conv_weight_mem_port_7_11[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][11+32*j][k1][k2];
                    conv_weight_mem_port_7_12[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][12+32*j][k1][k2];
                    conv_weight_mem_port_7_13[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][13+32*j][k1][k2];
                    conv_weight_mem_port_7_14[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][14+32*j][k1][k2];
                    conv_weight_mem_port_7_15[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][15+32*j][k1][k2];
                    conv_weight_mem_port_7_16[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][16+32*j][k1][k2];
                    conv_weight_mem_port_7_17[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][17+32*j][k1][k2];
                    conv_weight_mem_port_7_18[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][18+32*j][k1][k2];
                    conv_weight_mem_port_7_19[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][19+32*j][k1][k2];
                    conv_weight_mem_port_7_20[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][20+32*j][k1][k2];
                    conv_weight_mem_port_7_21[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][21+32*j][k1][k2];
                    conv_weight_mem_port_7_22[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][22+32*j][k1][k2];
                    conv_weight_mem_port_7_23[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][23+32*j][k1][k2];
                    conv_weight_mem_port_7_24[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][24+32*j][k1][k2];
                    conv_weight_mem_port_7_25[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][25+32*j][k1][k2];
                    conv_weight_mem_port_7_26[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][26+32*j][k1][k2];
                    conv_weight_mem_port_7_27[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][27+32*j][k1][k2];
                    conv_weight_mem_port_7_28[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][28+32*j][k1][k2];
                    conv_weight_mem_port_7_29[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][29+32*j][k1][k2];
                    conv_weight_mem_port_7_30[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][30+32*j][k1][k2];
                    conv_weight_mem_port_7_31[25 + 100 + j*100 + k*25 + k1*5 + k2] = weight_temp_3[7+8*k][31+32*j][k1][k2];
                }
            }
        }
    }
    //fc_1
    for (int i = 0; i < 10; i++) {
        for(int j = 0; j < 64; j++){
            for(int k1 = 0; k1 < 4; k1++){
                for(int k2 = 0; k2 < 4; k2++){
                    weight_temp_4[j][i][k1][k2] = (data_type_w)fc_1_weight2D[i*64*4*4 + j*4*4 + k1*4 + k2];
                }
            }
        }
    }
    for(int k=0; k <8; k++){
        for(int k1=0; k1 <4; k1++){
            for(int k2=0; k2<4; k2++){
                    conv_weight_mem_port_0_0[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[0+8*k][0][k1][k2];
                    conv_weight_mem_port_0_1[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[0+8*k][1][k1][k2];
                    conv_weight_mem_port_0_2[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[0+8*k][2][k1][k2];
                    conv_weight_mem_port_0_3[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[0+8*k][3][k1][k2];
                    conv_weight_mem_port_0_4[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[0+8*k][4][k1][k2];
                    conv_weight_mem_port_0_5[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[0+8*k][5][k1][k2];
                    conv_weight_mem_port_0_6[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[0+8*k][6][k1][k2];
                    conv_weight_mem_port_0_7[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[0+8*k][7][k1][k2];
                    conv_weight_mem_port_0_8[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[0+8*k][8][k1][k2];
                    conv_weight_mem_port_0_9[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[0+8*k][9][k1][k2];

                    conv_weight_mem_port_1_0[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[1+8*k][0][k1][k2];
                    conv_weight_mem_port_1_1[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[1+8*k][1][k1][k2];
                    conv_weight_mem_port_1_2[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[1+8*k][2][k1][k2];
                    conv_weight_mem_port_1_3[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[1+8*k][3][k1][k2];
                    conv_weight_mem_port_1_4[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[1+8*k][4][k1][k2];
                    conv_weight_mem_port_1_5[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[1+8*k][5][k1][k2];
                    conv_weight_mem_port_1_6[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[1+8*k][6][k1][k2];
                    conv_weight_mem_port_1_7[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[1+8*k][7][k1][k2];
                    conv_weight_mem_port_1_8[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[1+8*k][8][k1][k2];
                    conv_weight_mem_port_1_9[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[1+8*k][9][k1][k2];

                    conv_weight_mem_port_2_0[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[2+8*k][0][k1][k2];
                    conv_weight_mem_port_2_1[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[2+8*k][1][k1][k2];
                    conv_weight_mem_port_2_2[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[2+8*k][2][k1][k2];
                    conv_weight_mem_port_2_3[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[2+8*k][3][k1][k2];
                    conv_weight_mem_port_2_4[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[2+8*k][4][k1][k2];
                    conv_weight_mem_port_2_5[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[2+8*k][5][k1][k2];
                    conv_weight_mem_port_2_6[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[2+8*k][6][k1][k2];
                    conv_weight_mem_port_2_7[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[2+8*k][7][k1][k2];
                    conv_weight_mem_port_2_8[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[2+8*k][8][k1][k2];
                    conv_weight_mem_port_2_9[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[2+8*k][9][k1][k2];
                    
                    conv_weight_mem_port_3_0[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[3+8*k][0][k1][k2];
                    conv_weight_mem_port_3_1[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[3+8*k][1][k1][k2];
                    conv_weight_mem_port_3_2[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[3+8*k][2][k1][k2];
                    conv_weight_mem_port_3_3[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[3+8*k][3][k1][k2];
                    conv_weight_mem_port_3_4[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[3+8*k][4][k1][k2];
                    conv_weight_mem_port_3_5[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[3+8*k][5][k1][k2];
                    conv_weight_mem_port_3_6[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[3+8*k][6][k1][k2];
                    conv_weight_mem_port_3_7[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[3+8*k][7][k1][k2];
                    conv_weight_mem_port_3_8[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[3+8*k][8][k1][k2];
                    conv_weight_mem_port_3_9[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[3+8*k][9][k1][k2];

                    conv_weight_mem_port_4_0[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[4+8*k][0][k1][k2];
                    conv_weight_mem_port_4_1[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[4+8*k][1][k1][k2];
                    conv_weight_mem_port_4_2[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[4+8*k][2][k1][k2];
                    conv_weight_mem_port_4_3[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[4+8*k][3][k1][k2];
                    conv_weight_mem_port_4_4[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[4+8*k][4][k1][k2];
                    conv_weight_mem_port_4_5[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[4+8*k][5][k1][k2];
                    conv_weight_mem_port_4_6[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[4+8*k][6][k1][k2];
                    conv_weight_mem_port_4_7[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[4+8*k][7][k1][k2];
                    conv_weight_mem_port_4_8[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[4+8*k][8][k1][k2];
                    conv_weight_mem_port_4_9[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[4+8*k][9][k1][k2];

                    conv_weight_mem_port_5_0[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[5+8*k][0][k1][k2];
                    conv_weight_mem_port_5_1[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[5+8*k][1][k1][k2];
                    conv_weight_mem_port_5_2[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[5+8*k][2][k1][k2];
                    conv_weight_mem_port_5_3[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[5+8*k][3][k1][k2];
                    conv_weight_mem_port_5_4[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[5+8*k][4][k1][k2];
                    conv_weight_mem_port_5_5[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[5+8*k][5][k1][k2];
                    conv_weight_mem_port_5_6[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[5+8*k][6][k1][k2];
                    conv_weight_mem_port_5_7[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[5+8*k][7][k1][k2];
                    conv_weight_mem_port_5_8[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[5+8*k][8][k1][k2];
                    conv_weight_mem_port_5_9[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[5+8*k][9][k1][k2];

                    conv_weight_mem_port_6_0[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[6+8*k][0][k1][k2];
                    conv_weight_mem_port_6_1[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[6+8*k][1][k1][k2];
                    conv_weight_mem_port_6_2[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[6+8*k][2][k1][k2];
                    conv_weight_mem_port_6_3[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[6+8*k][3][k1][k2];
                    conv_weight_mem_port_6_4[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[6+8*k][4][k1][k2];
                    conv_weight_mem_port_6_5[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[6+8*k][5][k1][k2];
                    conv_weight_mem_port_6_6[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[6+8*k][6][k1][k2];
                    conv_weight_mem_port_6_7[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[6+8*k][7][k1][k2];
                    conv_weight_mem_port_6_8[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[6+8*k][8][k1][k2];
                    conv_weight_mem_port_6_9[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[6+8*k][9][k1][k2];

                    conv_weight_mem_port_7_0[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[7+8*k][0][k1][k2];
                    conv_weight_mem_port_7_1[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[7+8*k][1][k1][k2];
                    conv_weight_mem_port_7_2[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[7+8*k][2][k1][k2];
                    conv_weight_mem_port_7_3[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[7+8*k][3][k1][k2];
                    conv_weight_mem_port_7_4[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[7+8*k][4][k1][k2];
                    conv_weight_mem_port_7_5[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[7+8*k][5][k1][k2];
                    conv_weight_mem_port_7_6[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[7+8*k][6][k1][k2];
                    conv_weight_mem_port_7_7[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[7+8*k][7][k1][k2];
                    conv_weight_mem_port_7_8[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[7+8*k][8][k1][k2];
                    conv_weight_mem_port_7_9[25 + 100 + 200 + k*16 + k1*4 + k2] = weight_temp_4[7+8*k][9][k1][k2];
            }
        }
    }
    //weight_cdma
    for ( loop_var = 0; loop_var < 3; loop_var++ ){
        for (loop_var_1 = 0; loop_var_1 < 32; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 25; loop_var_2++) {
                conv_1_weight_cdma[loop_var*32768+loop_var_1*1024+loop_var_2]=conv_1_weight2D[loop_var*800+loop_var_1*25+loop_var_2];
            }
        }
    }

    //write data to DDR_SH_ADDR
    //Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR, conv_weight, 2400+25600+51200+10240);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_0, conv_weight_mem_port_0_0, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_1, conv_weight_mem_port_0_1, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_2, conv_weight_mem_port_0_2, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_3, conv_weight_mem_port_0_3, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_4, conv_weight_mem_port_0_4, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_5, conv_weight_mem_port_0_5, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_6, conv_weight_mem_port_0_6, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_7, conv_weight_mem_port_0_7, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_8, conv_weight_mem_port_0_8, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_9, conv_weight_mem_port_0_9, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_10, conv_weight_mem_port_0_10, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_11, conv_weight_mem_port_0_11, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_12, conv_weight_mem_port_0_12, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_13, conv_weight_mem_port_0_13, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_14, conv_weight_mem_port_0_14, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_15, conv_weight_mem_port_0_15, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_16, conv_weight_mem_port_0_16, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_17, conv_weight_mem_port_0_17, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_18, conv_weight_mem_port_0_18, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_19, conv_weight_mem_port_0_19, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_20, conv_weight_mem_port_0_20, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_21, conv_weight_mem_port_0_21, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_22, conv_weight_mem_port_0_22, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_23, conv_weight_mem_port_0_23, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_24, conv_weight_mem_port_0_24, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_25, conv_weight_mem_port_0_25, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_26, conv_weight_mem_port_0_26, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_27, conv_weight_mem_port_0_27, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_28, conv_weight_mem_port_0_28, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_29, conv_weight_mem_port_0_29, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_30, conv_weight_mem_port_0_30, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_0_31, conv_weight_mem_port_0_31, 453);

    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_0, conv_weight_mem_port_1_0, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_1, conv_weight_mem_port_1_1, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_2, conv_weight_mem_port_1_2, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_3, conv_weight_mem_port_1_3, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_4, conv_weight_mem_port_1_4, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_5, conv_weight_mem_port_1_5, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_6, conv_weight_mem_port_1_6, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_7, conv_weight_mem_port_1_7, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_8, conv_weight_mem_port_1_8, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_9, conv_weight_mem_port_1_9, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_10, conv_weight_mem_port_1_10, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_11, conv_weight_mem_port_1_11, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_12, conv_weight_mem_port_1_12, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_13, conv_weight_mem_port_1_13, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_14, conv_weight_mem_port_1_14, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_15, conv_weight_mem_port_1_15, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_16, conv_weight_mem_port_1_16, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_17, conv_weight_mem_port_1_17, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_18, conv_weight_mem_port_1_18, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_19, conv_weight_mem_port_1_19, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_20, conv_weight_mem_port_1_20, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_21, conv_weight_mem_port_1_21, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_22, conv_weight_mem_port_1_22, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_23, conv_weight_mem_port_1_23, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_24, conv_weight_mem_port_1_24, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_25, conv_weight_mem_port_1_25, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_26, conv_weight_mem_port_1_26, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_27, conv_weight_mem_port_1_27, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_28, conv_weight_mem_port_1_28, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_29, conv_weight_mem_port_1_29, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_30, conv_weight_mem_port_1_30, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_1_31, conv_weight_mem_port_1_31, 453);

    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_0, conv_weight_mem_port_2_0, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_1, conv_weight_mem_port_2_1, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_2, conv_weight_mem_port_2_2, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_3, conv_weight_mem_port_2_3, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_4, conv_weight_mem_port_2_4, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_5, conv_weight_mem_port_2_5, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_6, conv_weight_mem_port_2_6, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_7, conv_weight_mem_port_2_7, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_8, conv_weight_mem_port_2_8, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_9, conv_weight_mem_port_2_9, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_10, conv_weight_mem_port_2_10, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_11, conv_weight_mem_port_2_11, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_12, conv_weight_mem_port_2_12, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_13, conv_weight_mem_port_2_13, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_14, conv_weight_mem_port_2_14, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_15, conv_weight_mem_port_2_15, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_16, conv_weight_mem_port_2_16, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_17, conv_weight_mem_port_2_17, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_18, conv_weight_mem_port_2_18, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_19, conv_weight_mem_port_2_19, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_20, conv_weight_mem_port_2_20, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_21, conv_weight_mem_port_2_21, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_22, conv_weight_mem_port_2_22, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_23, conv_weight_mem_port_2_23, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_24, conv_weight_mem_port_2_24, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_25, conv_weight_mem_port_2_25, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_26, conv_weight_mem_port_2_26, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_27, conv_weight_mem_port_2_27, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_28, conv_weight_mem_port_2_28, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_29, conv_weight_mem_port_2_29, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_30, conv_weight_mem_port_2_30, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_2_31, conv_weight_mem_port_2_31, 453);

    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_0, conv_weight_mem_port_3_0, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_1, conv_weight_mem_port_3_1, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_2, conv_weight_mem_port_3_2, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_3, conv_weight_mem_port_3_3, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_4, conv_weight_mem_port_3_4, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_5, conv_weight_mem_port_3_5, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_6, conv_weight_mem_port_3_6, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_7, conv_weight_mem_port_3_7, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_8, conv_weight_mem_port_3_8, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_9, conv_weight_mem_port_3_9, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_10, conv_weight_mem_port_3_10, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_11, conv_weight_mem_port_3_11, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_12, conv_weight_mem_port_3_12, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_13, conv_weight_mem_port_3_13, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_14, conv_weight_mem_port_3_14, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_15, conv_weight_mem_port_3_15, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_16, conv_weight_mem_port_3_16, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_17, conv_weight_mem_port_3_17, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_18, conv_weight_mem_port_3_18, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_19, conv_weight_mem_port_3_19, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_20, conv_weight_mem_port_3_20, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_21, conv_weight_mem_port_3_21, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_22, conv_weight_mem_port_3_22, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_23, conv_weight_mem_port_3_23, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_24, conv_weight_mem_port_3_24, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_25, conv_weight_mem_port_3_25, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_26, conv_weight_mem_port_3_26, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_27, conv_weight_mem_port_3_27, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_28, conv_weight_mem_port_3_28, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_29, conv_weight_mem_port_3_29, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_30, conv_weight_mem_port_3_30, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_3_31, conv_weight_mem_port_3_31, 453);

    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_0, conv_weight_mem_port_4_0, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_1, conv_weight_mem_port_4_1, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_2, conv_weight_mem_port_4_2, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_3, conv_weight_mem_port_4_3, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_4, conv_weight_mem_port_4_4, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_5, conv_weight_mem_port_4_5, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_6, conv_weight_mem_port_4_6, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_7, conv_weight_mem_port_4_7, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_8, conv_weight_mem_port_4_8, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_9, conv_weight_mem_port_4_9, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_10, conv_weight_mem_port_4_10, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_11, conv_weight_mem_port_4_11, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_12, conv_weight_mem_port_4_12, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_13, conv_weight_mem_port_4_13, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_14, conv_weight_mem_port_4_14, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_15, conv_weight_mem_port_4_15, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_16, conv_weight_mem_port_4_16, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_17, conv_weight_mem_port_4_17, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_18, conv_weight_mem_port_4_18, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_19, conv_weight_mem_port_4_19, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_20, conv_weight_mem_port_4_20, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_21, conv_weight_mem_port_4_21, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_22, conv_weight_mem_port_4_22, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_23, conv_weight_mem_port_4_23, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_24, conv_weight_mem_port_4_24, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_25, conv_weight_mem_port_4_25, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_26, conv_weight_mem_port_4_26, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_27, conv_weight_mem_port_4_27, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_28, conv_weight_mem_port_4_28, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_29, conv_weight_mem_port_4_29, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_30, conv_weight_mem_port_4_30, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_4_31, conv_weight_mem_port_4_31, 453);

    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_0, conv_weight_mem_port_5_0, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_1, conv_weight_mem_port_5_1, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_2, conv_weight_mem_port_5_2, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_3, conv_weight_mem_port_5_3, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_4, conv_weight_mem_port_5_4, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_5, conv_weight_mem_port_5_5, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_6, conv_weight_mem_port_5_6, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_7, conv_weight_mem_port_5_7, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_8, conv_weight_mem_port_5_8, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_9, conv_weight_mem_port_5_9, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_10, conv_weight_mem_port_5_10, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_11, conv_weight_mem_port_5_11, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_12, conv_weight_mem_port_5_12, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_13, conv_weight_mem_port_5_13, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_14, conv_weight_mem_port_5_14, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_15, conv_weight_mem_port_5_15, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_16, conv_weight_mem_port_5_16, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_17, conv_weight_mem_port_5_17, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_18, conv_weight_mem_port_5_18, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_19, conv_weight_mem_port_5_19, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_20, conv_weight_mem_port_5_20, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_21, conv_weight_mem_port_5_21, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_22, conv_weight_mem_port_5_22, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_23, conv_weight_mem_port_5_23, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_24, conv_weight_mem_port_5_24, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_25, conv_weight_mem_port_5_25, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_26, conv_weight_mem_port_5_26, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_27, conv_weight_mem_port_5_27, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_28, conv_weight_mem_port_5_28, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_29, conv_weight_mem_port_5_29, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_30, conv_weight_mem_port_5_30, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_5_31, conv_weight_mem_port_5_31, 453);

    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_0, conv_weight_mem_port_6_0, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_1, conv_weight_mem_port_6_1, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_2, conv_weight_mem_port_6_2, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_3, conv_weight_mem_port_6_3, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_4, conv_weight_mem_port_6_4, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_5, conv_weight_mem_port_6_5, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_6, conv_weight_mem_port_6_6, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_7, conv_weight_mem_port_6_7, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_8, conv_weight_mem_port_6_8, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_9, conv_weight_mem_port_6_9, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_10, conv_weight_mem_port_6_10, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_11, conv_weight_mem_port_6_11, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_12, conv_weight_mem_port_6_12, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_13, conv_weight_mem_port_6_13, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_14, conv_weight_mem_port_6_14, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_15, conv_weight_mem_port_6_15, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_16, conv_weight_mem_port_6_16, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_17, conv_weight_mem_port_6_17, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_18, conv_weight_mem_port_6_18, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_19, conv_weight_mem_port_6_19, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_20, conv_weight_mem_port_6_20, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_21, conv_weight_mem_port_6_21, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_22, conv_weight_mem_port_6_22, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_23, conv_weight_mem_port_6_23, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_24, conv_weight_mem_port_6_24, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_25, conv_weight_mem_port_6_25, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_26, conv_weight_mem_port_6_26, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_27, conv_weight_mem_port_6_27, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_28, conv_weight_mem_port_6_28, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_29, conv_weight_mem_port_6_29, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_30, conv_weight_mem_port_6_30, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_6_31, conv_weight_mem_port_6_31, 453);

    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_0, conv_weight_mem_port_7_0, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_1, conv_weight_mem_port_7_1, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_2, conv_weight_mem_port_7_2, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_3, conv_weight_mem_port_7_3, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_4, conv_weight_mem_port_7_4, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_5, conv_weight_mem_port_7_5, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_6, conv_weight_mem_port_7_6, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_7, conv_weight_mem_port_7_7, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_8, conv_weight_mem_port_7_8, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_9, conv_weight_mem_port_7_9, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_10, conv_weight_mem_port_7_10, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_11, conv_weight_mem_port_7_11, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_12, conv_weight_mem_port_7_12, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_13, conv_weight_mem_port_7_13, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_14, conv_weight_mem_port_7_14, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_15, conv_weight_mem_port_7_15, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_16, conv_weight_mem_port_7_16, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_17, conv_weight_mem_port_7_17, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_18, conv_weight_mem_port_7_18, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_19, conv_weight_mem_port_7_19, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_20, conv_weight_mem_port_7_20, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_21, conv_weight_mem_port_7_21, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_22, conv_weight_mem_port_7_22, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_23, conv_weight_mem_port_7_23, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_24, conv_weight_mem_port_7_24, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_25, conv_weight_mem_port_7_25, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_26, conv_weight_mem_port_7_26, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_27, conv_weight_mem_port_7_27, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_28, conv_weight_mem_port_7_28, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_29, conv_weight_mem_port_7_29, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_30, conv_weight_mem_port_7_30, 453);
    Fill_Bram(pci_bar_handle_4, DDR_SH_ADDR_7_31, conv_weight_mem_port_7_31, 453);
    Fill_Bram(pci_bar_handle_4, DDR_B_ADDR_1, conv_bias, 32+32+64+10);
    Fill_Bram(pci_bar_handle_4, DDR_A_ADDR_1, in_data_part1, 3*20*20);
    Fill_Bram(pci_bar_handle_4, DDR_A_ADDR_2, in_data_part2, 3*20*20);
    Fill_Bram(pci_bar_handle_4, DDR_A_ADDR_3, in_data_part3, 3*20*20);
    Fill_Bram(pci_bar_handle_4, DDR_A_ADDR_4, in_data_part4, 3*20*20);
    
    printf("Finished writing to SH_DDR data\n");

//----------------------conv_1 layer -----------------------//
    //conv_1_1
    gettimeofday(&start,0);
    //input
    Fill_Bram(pci_bar_handle_4, BUF_OUT_0_0, in_data_part1, 400);
    Fill_Bram(pci_bar_handle_4, BUF_OUT_0_1, &in_data_part1[400], 400);
    Fill_Bram(pci_bar_handle_4, BUF_OUT_0_2, &in_data_part1[800], 400);
    //set_cdma(pci_bar_handle,0x02000000,0x0000000C,0x04000000,0x00000000,0x00000640);
    //set_cdma(pci_bar_handle,0x02000000+0x00000640,0x0000000C,0x04010000,0x00000000,0x00000640);
    //set_cdma(pci_bar_handle,0x02000000+0x00000C80,0x0000000C,0x04020000,0x00000000,0x00000640);
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "cdma time = " << diff << "  us" << endl;
    gettimeofday(&start,0);
    //addr test
    /*Fill_Bram(pci_bar_handle_4, BUF_OUT_1_0, in_data_part4, 1024);
    //set_cdma(pci_bar_handle,0x02000000,0x0000000C,0x00047000,0x00000000,0x00001000);
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_0, out_temp_1, 1024);
    Fill_param(pci_bar_handle_4, ACC_PARAMS_1, acc_param_conv_1, 1024);
    Read_param(pci_bar_handle_4, ACC_PARAMS_1, data_1, 1024);
    outdata.open("./netOutput/out_temp_data.txt", ios::app);
    outdata <<"conv_output:"<< endl;
    for(loop_var = 0; loop_var < 1024; loop_var++){
        outdata << out_temp_1[loop_var] << "  ";
    }
    outdata << endl;    
    outdata.close();*/
    //weight
    /*for (loop_var_1 = 0; loop_var_1 < 3; loop_var_1++) {
        for (loop_var_2 = 0; loop_var_2 < 32; loop_var_2++) {
            set_cdma(pci_bar_handle,0x01000000+loop_var_1*100000+loop_var_2*1000,0x0000000E,0x02000000+loop_var_1*20000+loop_var_2*1000,0x00000000,0x00000800);
        }
    }*/
    /*set_cdma(pci_bar_handle,0x01000000,0x0000000E,0x02000000,0x00000000,0x00000800);
    set_cdma(pci_bar_handle,0x01001000,0x0000000E,0x02001000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01002000,0x0000000E,0x02002000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01003000,0x0000000E,0x02003000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01004000,0x0000000E,0x02004000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01005000,0x0000000E,0x02005000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01006000,0x0000000E,0x02006000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01007000,0x0000000E,0x02007000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01008000,0x0000000E,0x02008000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01009000,0x0000000E,0x02009000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0100A000,0x0000000E,0x0200A000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0100B000,0x0000000E,0x0200B000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0100C000,0x0000000E,0x0200C000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0100D000,0x0000000E,0x0200D000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0100E000,0x0000000E,0x0200E000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0100F000,0x0000000E,0x0200F000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01010000,0x0000000E,0x02010000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01011000,0x0000000E,0x02011000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01012000,0x0000000E,0x02012000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01013000,0x0000000E,0x02013000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01014000,0x0000000E,0x02014000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01015000,0x0000000E,0x02015000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01016000,0x0000000E,0x02016000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01017000,0x0000000E,0x02017000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01018000,0x0000000E,0x02018000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01019000,0x0000000E,0x02019000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0101A000,0x0000000E,0x0201A000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0101B000,0x0000000E,0x0201B000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0101C000,0x0000000E,0x0201C000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0101D000,0x0000000E,0x0201D000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0101E000,0x0000000E,0x0201E000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0101F000,0x0000000E,0x0201F000,0x00000000,0x00000714);

    set_cdma(pci_bar_handle,0x01100000,0x0000000E,0x02020000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01101000,0x0000000E,0x02021000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01102000,0x0000000E,0x02022000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01103000,0x0000000E,0x02023000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01104000,0x0000000E,0x02024000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01105000,0x0000000E,0x02025000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01106000,0x0000000E,0x02026000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01107000,0x0000000E,0x02027000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01108000,0x0000000E,0x02028000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01109000,0x0000000E,0x02029000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0110A000,0x0000000E,0x0202A000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0110B000,0x0000000E,0x0202B000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0110C000,0x0000000E,0x0202C000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0110D000,0x0000000E,0x0202D000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0110E000,0x0000000E,0x0202E000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0110F000,0x0000000E,0x0202F000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01110000,0x0000000E,0x02030000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01111000,0x0000000E,0x02031000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01112000,0x0000000E,0x02032000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01113000,0x0000000E,0x02033000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01114000,0x0000000E,0x02034000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01115000,0x0000000E,0x02035000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01116000,0x0000000E,0x02036000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01117000,0x0000000E,0x02037000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01118000,0x0000000E,0x02038000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01119000,0x0000000E,0x02039000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0111A000,0x0000000E,0x0203A000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0111B000,0x0000000E,0x0203B000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0111C000,0x0000000E,0x0203C000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0111D000,0x0000000E,0x0203D000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0111E000,0x0000000E,0x0203E000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0111F000,0x0000000E,0x0203F000,0x00000000,0x00000714);

    set_cdma(pci_bar_handle,0x01200000,0x0000000E,0x02040000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01201000,0x0000000E,0x02041000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01202000,0x0000000E,0x02042000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01203000,0x0000000E,0x02043000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01204000,0x0000000E,0x02044000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01205000,0x0000000E,0x02045000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01206000,0x0000000E,0x02046000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01207000,0x0000000E,0x02047000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01208000,0x0000000E,0x02048000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01209000,0x0000000E,0x02049000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0120A000,0x0000000E,0x0204A000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0120B000,0x0000000E,0x0204B000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0120C000,0x0000000E,0x0204C000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0120D000,0x0000000E,0x0204D000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0120E000,0x0000000E,0x0204E000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0120F000,0x0000000E,0x0204F000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01210000,0x0000000E,0x02050000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01211000,0x0000000E,0x02051000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01212000,0x0000000E,0x02052000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01213000,0x0000000E,0x02053000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01214000,0x0000000E,0x02054000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01215000,0x0000000E,0x02055000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01216000,0x0000000E,0x02056000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01217000,0x0000000E,0x02057000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01218000,0x0000000E,0x02058000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01219000,0x0000000E,0x02059000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0121A000,0x0000000E,0x0205A000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0121B000,0x0000000E,0x0205B000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0121C000,0x0000000E,0x0205C000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0121D000,0x0000000E,0x0205D000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0121E000,0x0000000E,0x0205E000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0121F000,0x0000000E,0x0205F000,0x00000000,0x00000714);

    /*set_cdma(pci_bar_handle,0x01300000,0x0000000E,0x02060000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01301000,0x0000000E,0x02061000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01302000,0x0000000E,0x02062000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01303000,0x0000000E,0x02063000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01304000,0x0000000E,0x02064000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01305000,0x0000000E,0x02065000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01306000,0x0000000E,0x02066000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01307000,0x0000000E,0x02067000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01308000,0x0000000E,0x02068000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01309000,0x0000000E,0x02069000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0130A000,0x0000000E,0x0206A000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0130B000,0x0000000E,0x0206B000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0130C000,0x0000000E,0x0206C000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0130D000,0x0000000E,0x0206D000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0130E000,0x0000000E,0x0206E000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0130F000,0x0000000E,0x0206F000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01310000,0x0000000E,0x02070000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01311000,0x0000000E,0x02071000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01312000,0x0000000E,0x02072000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01313000,0x0000000E,0x02073000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01314000,0x0000000E,0x02074000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01315000,0x0000000E,0x02075000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01316000,0x0000000E,0x02076000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01317000,0x0000000E,0x02077000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01318000,0x0000000E,0x02078000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01319000,0x0000000E,0x02079000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0131A000,0x0000000E,0x0207A000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0131B000,0x0000000E,0x0207B000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0131C000,0x0000000E,0x0207C000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0131D000,0x0000000E,0x0207D000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0131E000,0x0000000E,0x0207E000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0131F000,0x0000000E,0x0207F000,0x00000000,0x00000714);

    set_cdma(pci_bar_handle,0x01400000,0x0000000E,0x02080000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01401000,0x0000000E,0x02081000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01402000,0x0000000E,0x02082000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01403000,0x0000000E,0x02083000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01404000,0x0000000E,0x02084000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01405000,0x0000000E,0x02085000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01406000,0x0000000E,0x02086000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01407000,0x0000000E,0x02087000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01408000,0x0000000E,0x02088000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01409000,0x0000000E,0x02089000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0140A000,0x0000000E,0x0208A000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0140B000,0x0000000E,0x0208B000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0140C000,0x0000000E,0x0208C000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0140D000,0x0000000E,0x0208D000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0140E000,0x0000000E,0x0208E000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0140F000,0x0000000E,0x0208F000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01410000,0x0000000E,0x02090000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01411000,0x0000000E,0x02091000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01412000,0x0000000E,0x02092000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01413000,0x0000000E,0x02093000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01414000,0x0000000E,0x02094000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01415000,0x0000000E,0x02095000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01416000,0x0000000E,0x02096000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01417000,0x0000000E,0x02097000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01418000,0x0000000E,0x02098000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01419000,0x0000000E,0x02099000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0141A000,0x0000000E,0x0209A000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0141B000,0x0000000E,0x0209B000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0141C000,0x0000000E,0x0209C000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0141D000,0x0000000E,0x0209D000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0141E000,0x0000000E,0x0209E000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0141F000,0x0000000E,0x0209F000,0x00000000,0x00000714);

    set_cdma(pci_bar_handle,0x01500000,0x0000000E,0x020A0000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01501000,0x0000000E,0x020A1000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01502000,0x0000000E,0x020A2000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01503000,0x0000000E,0x020A3000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01504000,0x0000000E,0x020A4000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01505000,0x0000000E,0x020A5000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01506000,0x0000000E,0x020A6000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01507000,0x0000000E,0x020A7000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01508000,0x0000000E,0x020A8000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01509000,0x0000000E,0x020A9000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0150A000,0x0000000E,0x020AA000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0150B000,0x0000000E,0x020AB000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0150C000,0x0000000E,0x020AC000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0150D000,0x0000000E,0x020AD000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0150E000,0x0000000E,0x020AE000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0150F000,0x0000000E,0x020AF000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01510000,0x0000000E,0x020B0000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01511000,0x0000000E,0x020B1000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01512000,0x0000000E,0x020B2000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01513000,0x0000000E,0x020B3000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01514000,0x0000000E,0x020B4000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01515000,0x0000000E,0x020B5000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01516000,0x0000000E,0x020B6000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01517000,0x0000000E,0x020B7000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01518000,0x0000000E,0x020B8000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01519000,0x0000000E,0x020B9000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0151A000,0x0000000E,0x020BA000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0151B000,0x0000000E,0x020BB000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0151C000,0x0000000E,0x020BC000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0151D000,0x0000000E,0x020BD000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0151E000,0x0000000E,0x020BE000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0151F000,0x0000000E,0x020BF000,0x00000000,0x00000714);

    set_cdma(pci_bar_handle,0x01600000,0x0000000E,0x020C0000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01601000,0x0000000E,0x020C1000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01602000,0x0000000E,0x020C2000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01603000,0x0000000E,0x020C3000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01604000,0x0000000E,0x020C4000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01605000,0x0000000E,0x020C5000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01606000,0x0000000E,0x020C6000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01607000,0x0000000E,0x020C7000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01608000,0x0000000E,0x020C8000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01609000,0x0000000E,0x020C9000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0160A000,0x0000000E,0x020CA000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0160B000,0x0000000E,0x020CB000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0160C000,0x0000000E,0x020CC000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0160D000,0x0000000E,0x020CD000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0160E000,0x0000000E,0x020CE000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0160F000,0x0000000E,0x020CF000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01610000,0x0000000E,0x020D0000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01611000,0x0000000E,0x020D1000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01612000,0x0000000E,0x020D2000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01613000,0x0000000E,0x020D3000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01614000,0x0000000E,0x020D4000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01615000,0x0000000E,0x020D5000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01616000,0x0000000E,0x020D6000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01617000,0x0000000E,0x020D7000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01618000,0x0000000E,0x020D8000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01619000,0x0000000E,0x020D9000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0161A000,0x0000000E,0x020DA000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0161B000,0x0000000E,0x020DB000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0161C000,0x0000000E,0x020DC000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0161D000,0x0000000E,0x020DD000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0161E000,0x0000000E,0x020DE000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0161F000,0x0000000E,0x020DF000,0x00000000,0x00000714);

    set_cdma(pci_bar_handle,0x01700000,0x0000000E,0x020E0000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01701000,0x0000000E,0x020E1000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01702000,0x0000000E,0x020E2000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01703000,0x0000000E,0x020E3000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01704000,0x0000000E,0x020E4000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01705000,0x0000000E,0x020E5000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01706000,0x0000000E,0x020E6000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01707000,0x0000000E,0x020E7000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01708000,0x0000000E,0x020E8000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01709000,0x0000000E,0x020E9000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0170A000,0x0000000E,0x020EA000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0170B000,0x0000000E,0x020EB000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0170C000,0x0000000E,0x020EC000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0170D000,0x0000000E,0x020ED000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0170E000,0x0000000E,0x020EE000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0170F000,0x0000000E,0x020EF000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01710000,0x0000000E,0x020F0000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01711000,0x0000000E,0x020F1000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01712000,0x0000000E,0x020F2000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01713000,0x0000000E,0x020F3000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01714000,0x0000000E,0x020F4000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01715000,0x0000000E,0x020F5000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01716000,0x0000000E,0x020F6000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01717000,0x0000000E,0x020F7000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01718000,0x0000000E,0x020F8000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x01719000,0x0000000E,0x020F9000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0171A000,0x0000000E,0x020FA000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0171B000,0x0000000E,0x020FB000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0171C000,0x0000000E,0x020FC000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0171D000,0x0000000E,0x020FD000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0171E000,0x0000000E,0x020FE000,0x00000000,0x00000714);
    set_cdma(pci_bar_handle,0x0171F000,0x0000000E,0x020FF000,0x00000000,0x00000714);*/
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_0, conv_1_weight2D, 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_1, &conv_1_weight2D[25], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_2, &conv_1_weight2D[50], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_3, &conv_1_weight2D[75], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_4, &conv_1_weight2D[100], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_5, &conv_1_weight2D[125], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_6, &conv_1_weight2D[150], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_7, &conv_1_weight2D[175], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_8, &conv_1_weight2D[200], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_9, &conv_1_weight2D[225], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_10, &conv_1_weight2D[250], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_11, &conv_1_weight2D[275], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_12, &conv_1_weight2D[300], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_13, &conv_1_weight2D[325], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_14, &conv_1_weight2D[350], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_15, &conv_1_weight2D[375], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_16, &conv_1_weight2D[400], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_17, &conv_1_weight2D[425], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_18, &conv_1_weight2D[450], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_19, &conv_1_weight2D[475], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_20, &conv_1_weight2D[500], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_21, &conv_1_weight2D[525], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_22, &conv_1_weight2D[550], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_23, &conv_1_weight2D[575], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_24, &conv_1_weight2D[600], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_25, &conv_1_weight2D[625], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_26, &conv_1_weight2D[650], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_27, &conv_1_weight2D[675], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_28, &conv_1_weight2D[700], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_29, &conv_1_weight2D[725], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_30, &conv_1_weight2D[750], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_31, &conv_1_weight2D[775], 25);

    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_0, &conv_1_weight2D[800], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_1, &conv_1_weight2D[825], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_2, &conv_1_weight2D[850], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_3, &conv_1_weight2D[875], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_4, &conv_1_weight2D[900], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_5, &conv_1_weight2D[925], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_6, &conv_1_weight2D[950], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_7, &conv_1_weight2D[975], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_8, &conv_1_weight2D[1000], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_9, &conv_1_weight2D[1025], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_10, &conv_1_weight2D[1050], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_11, &conv_1_weight2D[1075], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_12, &conv_1_weight2D[1100], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_13, &conv_1_weight2D[1125], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_14, &conv_1_weight2D[1150], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_15, &conv_1_weight2D[1175], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_16, &conv_1_weight2D[1200], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_17, &conv_1_weight2D[1225], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_18, &conv_1_weight2D[1250], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_19, &conv_1_weight2D[1275], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_20, &conv_1_weight2D[1300], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_21, &conv_1_weight2D[1325], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_22, &conv_1_weight2D[1350], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_23, &conv_1_weight2D[1375], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_24, &conv_1_weight2D[1400], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_25, &conv_1_weight2D[1425], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_26, &conv_1_weight2D[1450], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_27, &conv_1_weight2D[1475], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_28, &conv_1_weight2D[1500], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_29, &conv_1_weight2D[1525], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_30, &conv_1_weight2D[1550], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_1_31, &conv_1_weight2D[1575], 25);

    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_0, &conv_1_weight2D[1600], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_1, &conv_1_weight2D[1625], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_2, &conv_1_weight2D[1650], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_3, &conv_1_weight2D[1675], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_4, &conv_1_weight2D[1700], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_5, &conv_1_weight2D[1725], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_6, &conv_1_weight2D[1750], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_7, &conv_1_weight2D[1775], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_8, &conv_1_weight2D[1800], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_9, &conv_1_weight2D[1825], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_10, &conv_1_weight2D[1850], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_11, &conv_1_weight2D[1875], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_12, &conv_1_weight2D[1900], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_13, &conv_1_weight2D[1925], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_14, &conv_1_weight2D[1950], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_15, &conv_1_weight2D[1975], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_16, &conv_1_weight2D[2000], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_17, &conv_1_weight2D[2025], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_18, &conv_1_weight2D[2050], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_19, &conv_1_weight2D[2075], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_20, &conv_1_weight2D[2100], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_21, &conv_1_weight2D[2125], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_22, &conv_1_weight2D[2150], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_23, &conv_1_weight2D[2175], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_24, &conv_1_weight2D[2200], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_25, &conv_1_weight2D[2225], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_26, &conv_1_weight2D[2250], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_27, &conv_1_weight2D[2275], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_28, &conv_1_weight2D[2300], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_29, &conv_1_weight2D[2325], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_30, &conv_1_weight2D[2350], 25);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_2_31, &conv_1_weight2D[2375], 25);
    //set_cdma(pci_bar_handle,0x01000000,0x0000000E,0x00060000,0x00000000,0x00060000);
    Fill_param(pci_bar_handle_4, ACC_PARAMS_0, acc_param_conv_1, 16);
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "conv1_1 data loading time = " << diff << "  us" << endl;

    Fill_Bram(pci_bar_handle_4, CONV_B_BRAM_PCIS, conv_1_bias2D, 32);
    //set_cdma(pci_bar_handle,0x01000000,0x0000000E,0x00080000,0x00000000,0x00000640);
    gettimeofday(&start,0);  
    XInference_net_Start(pci_bar_handle, &InstancePtr_1);
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr_1)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "conv1_1 processing time = " << diff << "  us" << endl;
    //Read_Bram(pci_bar_handle_4, 0x00080000, out_temp_2, 40960);
    //Read_Bram(pci_bar_handle_4, 0x00240000, out_temp_2, 1024);
    //Read_Bram(pci_bar_handle_4, 0x0003F000, out_temp_2, 1024);
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_0, out_temp_2, 256);
    outdata.open("./netOutput/out_temp_data.txt", ios::app);
    outdata <<"conv_output:"<< endl;
    for(loop_var = 0; loop_var < 256; loop_var++){
        outdata << out_temp_2[loop_var] << "  ";
    }
    outdata << endl;    
    outdata.close();
    //conv_1_2
    /*gettimeofday(&start,0);  
    //input
    //Fill_Bram(pci_bar_handle_4, BUF_OUT_0_0, in_data_part2, 400);
    //Fill_Bram(pci_bar_handle_4, BUF_OUT_0_1, &in_data_part2[400], 400);
    //Fill_Bram(pci_bar_handle_4, BUF_OUT_0_2, &in_data_part2[800], 400);
    set_cdma(pci_bar_handle,0x02100000,0x0000000C,0x04000000,0x00000000,0x00000640);
    set_cdma(pci_bar_handle,0x02100000+0x00000640,0x0000000C,0x04010000,0x00000000,0x00000640);
    set_cdma(pci_bar_handle,0x02100000+0x00000C80,0x0000000C,0x04020000,0x00000000,0x00000640);
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "conv1_2 data loading time = " << diff << "  us" << endl;
    gettimeofday(&start,0);  
    XInference_net_Start(pci_bar_handle, &InstancePtr_1);
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr_1)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "conv1_2 processing time = " << diff << "  us" << endl;
    //conv_1_3
    gettimeofday(&start,0);  
    //input
    //Fill_Bram(pci_bar_handle_4, BUF_OUT_0_0, in_data_part3, 400);
    //Fill_Bram(pci_bar_handle_4, BUF_OUT_0_1, &in_data_part3[400], 400);
    //Fill_Bram(pci_bar_handle_4, BUF_OUT_0_2, &in_data_part3[800], 400);
    set_cdma(pci_bar_handle,0x02200000,0x0000000C,0x04000000,0x00000000,0x00000640);
    set_cdma(pci_bar_handle,0x02200000+0x00000640,0x0000000C,0x04010000,0x00000000,0x00000640);
    set_cdma(pci_bar_handle,0x02200000+0x00000C80,0x0000000C,0x04020000,0x00000000,0x00000640);
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "conv1_3 data loading time = " << diff << "  us" << endl;
    gettimeofday(&start,0);  
    XInference_net_Start(pci_bar_handle, &InstancePtr_1);
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr_1)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "conv1_3 processing time = " << diff << "  us" << endl;
    //conv_1_4
    gettimeofday(&start,0);  
    //input
    //Fill_Bram(pci_bar_handle_4, BUF_OUT_0_0, in_data_part4, 400);
    //Fill_Bram(pci_bar_handle_4, BUF_OUT_0_1, &in_data_part4[400], 400);
    //Fill_Bram(pci_bar_handle_4, BUF_OUT_0_2, &in_data_part4[800], 400);
    set_cdma(pci_bar_handle,0x02300000,0x0000000C,0x04000000,0x00000000,0x00000640);
    set_cdma(pci_bar_handle,0x02300000+0x00000640,0x0000000C,0x04010000,0x00000000,0x00000640);
    set_cdma(pci_bar_handle,0x02300000+0x00000C80,0x0000000C,0x04020000,0x00000000,0x00000640);
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "conv1_4 data loading time = " << diff << "  us" << endl;
    gettimeofday(&start,0);  
    XInference_net_Start(pci_bar_handle, &InstancePtr_1);
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr_1)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "conv1_4 processing time = " << diff << "  us" << endl;
    gettimeofday(&start,0);  
    set_cdma(pci_bar_handle,0x01000000,0x0000000E,0x00060000,0x00000000,0x00100000);
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "conv1_4 processing time = " << diff << "  us" << endl;*/
//------------------------------------------------------------------------------------------
    printf("\n");
    printf("Reading and verifying DDR_B Dst Buffer 1KB\n");

out:
    /* clean up */
    if (pci_bar_handle >= 0) {
        rc = fpga_pci_detach(pci_bar_handle);
        if (rc) {
            printf("Failure while detaching from the fpga.\n");
        }
    }

    if (pci_bar_handle_0 >= 0) {
        rc_0 = fpga_pci_detach(pci_bar_handle_0);
        if (rc_0) {
            printf("Failure while detaching from the fpga.\n");
        }
    }

    if (pci_bar_handle_4 >= 0) {
        rc_4 = fpga_pci_detach(pci_bar_handle_4);
        if (rc_4) {
            printf("Failure while detaching from the fpga.\n");
        }
    }


    if (pci_bar_handle_sda >= 0) {
        rc_sda = fpga_pci_detach(pci_bar_handle_sda);
        if (rc_sda) {
            printf("Failure while detaching from the fpga.\n");
        }
    }

    /* if there is an error code, exit with status 1 */
    return (rc != 0 ? 1 : 0);
}
