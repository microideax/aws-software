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
#include <fcntl.h>
#include <errno.h>
#include <poll.h>

//C++ headers
#include <cstdint>
#include <iostream>
//#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include <fpga_pci.h>
#include <fpga_mgmt.h>
#include <utils/lcd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../inference_net_hls/stb_image.h"
#include "../inference_net_hls/weight_bias_one_dim.h"
#include "../inference_net_hls/acc_dim_config.h"
#include "../inference_host/inference_func.h"
#include "../inference_host/acc_mem_config.h"
#include "../inference_net_hls/softmax_one_dim.h"
#include "../inference_net_hls/predict_one_dim.h"
#include "../utils/cl_tsc.h"

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

    data_type in_data[32*32];
    data_type in_data_1[32*32];
    data_type in_data_2[32*32];
    data_type in_data_part1_3D[3][20][20];
    data_type in_data_part2_3D[3][20][20];
    data_type in_data_part3_3D[3][20][20];
    data_type in_data_part4_3D[3][20][20];
    data_type in_data_part1[3*20*20];
    data_type in_data_part2[3*20*20];
    data_type in_data_part3[3*20*20];
    data_type in_data_part4[3*20*20];
    data_type  out_data[28*28];
    data_type  out_temp_1[4704];
    data_type  out_temp_2[4704];
    data_type  out_temp_3[4704];
    data_type  out_temp_4[4704];
    data_type  out_temp_5[4704];
    data_type  out_temp_6[4704];
    data_type  out_temp_7[4704];
    data_type  out_temp_8[4704];
    data_type  out_temp_9[4704];
    data_type  out_temp_10[4704];
    data_type  out_temp_11[4704];
    data_type  out_temp_12[4704];
    data_type  out_temp_13[4704];
    data_type  out_temp_14[4704];
    data_type  out_temp_15[4704];
    data_type  out_temp_16[4704];
    data_type  out_temp_tmp_0[128*32];
    data_type  out_temp_tmp_1[128*32];
    data_type  out_temp_tmp_2[128*32];
    data_type  out_temp_tmp_3[128*32];
    data_type  out_temp_tmp_4[128*32];
    data_type  out_temp_tmp_5[128*32];
    data_type  out_temp_tmp_6[128*32];
    data_type  out_temp_tmp_7[128*32];
    data_type  out_temp_tmp_8[128*32];
    data_type  fc_3_out[10];
    data_type out_res[6*28*28];
    data_type_w conv_weight[6*5*5+2400+4000];
    data_type_w conv_weight_1[6*5*5+2400+4000];
    data_type_w conv_bias[6+16+10];
    data_type_w conv_bias_1[6+16+10];
    float conv_1_weight2D[150];
    float conv_1_bias2D[6];
    float conv_2_weight2D[2400];
    float conv_2_bias2D[16];
    float fc_1_weight2D[4000];
    float fc_1_bias2D[10];
    float conv_weight_tmp[8][32][1024];
    
    int ctrl_param_1[2] = {1, 0};
    int ctrl_param_2[2] = {0, 1};

    // param order = {n, k, m, Rin, Cin, Rout, Cout, S, P, act}
    int conv_param_1[16] = {1, 5, 6, 28, 28, 28, 28, 1, 2, 1, 0, 0, 0, 0, 1, 1};
    int pool_param_1[16] = {28, 28, 6, 2, 14, 14, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    int conv_param_2[16] = {6, 5, 16, 14, 14, 10, 10, 1, 0, 1, 0, 0, 0, 0, 1, 1};
    int pool_param_2[16] = {10, 10, 16, 2, 5, 5, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    int conv_param_3[16] = {16, 5, 10, 5, 5, 1, 1, 5, 0, 1, 0, 0, 0, 0, 1, 1};
    int pool_param_3[16] = {10, 10, 10, 2, 1, 1, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0};

    int acc_param_conv_1[16] = {1/*S*/, 0/*n*/, 0/*r*/, 0/*c*/, 5/*K*/, 0, 0, 1/*N*/, 1, 0, 0, 0, 0, 0, 0, 0};
    int acc_param_test[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int acc_param_conv_2[16] = {1/*S*/, 0/*n*/, 0/*r*/, 0/*c*/, 5/*K*/, 0, 0, 6/*N*/, 1, 0, 5, 6, 0, 0, 0, 0};
    int acc_param_conv_3[16] = {5/*S*/, 0/*n*/, 0/*r*/, 0/*c*/, 5/*K*/, 0, 0, 16/*N*/, 1, 0, 10, 22, 0, 0, 0, 0};
                             //C_in,R_in,N,K,C_out,R_out,stride,pad,act
    int acc_param_pool_1[16] = {2/*S*/, 0/*n*/, 0/*r*/, 0/*c*/, 2/*K*/, 28/*in_size*/, 28/*in_size*/, 0/*P*/, 16, 0, 0, 0, 0, 0, 0, 0};
    int acc_param_pool_2[16] = {2/*S*/, 0/*n*/, 0/*r*/, 0/*c*/, 2/*K*/, 10/*in_size*/, 10/*in_size*/, 0/*P*/, 16, 0, 0, 0, 0, 0, 0, 0};
    int acc_param_pool_3[16] = {0/*S*/, 0/*n*/, 0/*r*/, 0/*c*/, 0/*K*/, 1/*in_size*/, 1/*in_size*/, 0/*P*/, 0, 0, 0, 0, 0, 0, 0, 0};

    int feature_num1 = 0;
    int feature_num2 = 0;
    int feature_width1 = 0;
    int feature_width2 = 0;
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

    string image_dir = "./netInput/3.bmp";
    const char* weight_src = "./netInput/net_weights.txt";
    std::ofstream indata;
    std::ofstream outdata;
    std::ofstream weightdata;
    std::ofstream test_output;
    float in_data_3D_padding[1][32][32] = { 0 };
    int in_data_size=0;
    int w_r_offset = 0;
    int w_c_offset = 0;
    std::ofstream w_buf_t;

    //int i,j,k;
    int count = 0;

    cout << "test point 0" << endl;
    //time mreasurement variable define
    uint64_t start_time,end_time;
    struct timeval start,end;
    struct timeval start_net,end_net;
    unsigned long diff;
    XInference_net InstancePtr;
    InstancePtr.ctrl_bus_baseaddress = XINFERENCE_IP_CRTL_BUS_ADDR_1;
    InstancePtr.IsReady = 0x01;
    uint32_t ip_status;
    int fd, rc1;
    char device_file_name[256];
    float *write_buffer, *read_buffer;
    static const size_t buffer_size = 4096;
    int channel=0;
    read_buffer = NULL;
    write_buffer = NULL;
    fd = -1;

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

    rc1 = log_init("test_dram_dma");
    fail_on(rc1, out, "Unable to initialize the log.");
    rc1 = log_attach(logger, NULL, 0);
    fail_on(rc1, out, "%s", "Unable to attach to the log.");

    /* initialize the fpga_plat library */
    rc1 = fpga_mgmt_init();
    fail_on(rc1, out, "Unable to initialize the fpga_mgmt library");

    rc1 = sprintf(device_file_name, "/dev/edma%i_queue_0", slot_id);
    fail_on((rc1 = (rc1 < 0)? 1:0), out, "Unable to format device file name.");
    printf("device_file_name=%s\n", device_file_name);

    /* make sure the AFI is loaded and ready */
    rc1 = check_slot_config(slot_id);
    fail_on(rc1, out, "slot config is not correct");

    fd = open(device_file_name, O_RDWR);
    if(fd<0){
        printf("Cannot open device file %s.\nMaybe the EDMA "
               "driver isn't installed, isn't modified to attach to the PCI ID of "
               "your CL, or you're using a device file that doesn't exist?\n"
               "See the edma_install manual at <aws-fpga>/sdk/linux_kernel_drivers/edma/edma_install.md\n"
               "Remember that rescanning your FPGA can change the device file.\n"
               "To remove and re-add your edma driver and reset the device file mappings, run\n"
               "`sudo rmmod edma-drv && sudo insmod <aws-fpga>/sdk/linux_kernel_drivers/edma/edma-drv.ko`\n",
               device_file_name);
        fail_on((rc1 = (fd < 0)? 1:0), out, "unable to open DMA queue. ");
    }

    write_buffer = (float *)malloc(buffer_size);
    read_buffer = (float *)malloc(buffer_size);
    if (write_buffer == NULL || read_buffer == NULL) {
        rc1 = ENOMEM;
        goto out;
    }

    //rand_string(write_buffer, buffer_size);

    
    
//--------------------------input image data initialization----------------//
    data = loadfile(image_dir, size);
    image_orig = stbi_load_from_memory(data, size, &w, &h, &channels, 1);
    for (loop_var = 0; loop_var < 28*28; loop_var++) {
        in_data[loop_var] = (data_type)image_orig[loop_var];
    }
    //add padding for input
    for (loop_var = 0; loop_var < 1; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 32; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 32; loop_var_2++) {
                in_data_3D_padding[loop_var][loop_var_1][loop_var_2] = 0;
            }
        }
    }
    for (loop_var = 0; loop_var < 1; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 28; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 28; loop_var_2++) {
                in_data_3D_padding[loop_var][loop_var_1+2][loop_var_2+2] = (data_type)in_data[loop_var*28*28+loop_var_1*28+loop_var_2];
            }
        }
    }
    in_data_size=0;
    for (loop_var = 0; loop_var < 1; loop_var++) {
        for (loop_var_1 = 0; loop_var_1 < 32; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 32; loop_var_2++) {
                in_data[in_data_size] = (data_type)in_data_3D_padding[loop_var][loop_var_1][loop_var_2];
                in_data_size++;
            }
        }
    }
    for (loop_var = 0; loop_var < 32*32; loop_var++) {
        write_buffer[loop_var]=in_data[loop_var];
    }
    gettimeofday(&start, 0);
    set_pwrite(rc1, fd, write_buffer, buffer_size, 0x02000000);
    gettimeofday(&end, 0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    printf("pwrite time  = %d us\n", diff);

    rc1 = fsync(fd);
    fail_on((rc1 = (rc1 < 0)? errno:0), out, "call to fsync failed.");

    Read_Bram(pci_bar_handle_4, 0x02000000, in_data_1, 32*32);
    indata.open("./netOutput/in_data.txt", ios::app);
    for (loop_var = 0; loop_var < 1; loop_var++) {
        indata <<"indata:"<< endl;
        for (loop_var_1 = 0; loop_var_1 < 32; loop_var_1++) {
            for (loop_var_2 = 0; loop_var_2 < 32; loop_var_2++) {
                indata << in_data_1[loop_var_1*32+loop_var_2] << " ";
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
    memset(conv_1_weight2D, 0, 150 * sizeof(float));
    load_weight_conv(
        weight_src, 
        conv_1_weight2D,
        weight_bias_record,
        nn_channel_size_conv, 
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    //cout << "Loading conv weight 1 to memory space, starting at: " <<conv_weight_num << '\n';
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                conv_weight[conv_weight_num] = (data_type_w)conv_1_weight2D[i*5*5+j*5+k];
                conv_weight_num++;
            }
        }
    }
    memset(conv_1_bias2D, 0, 6 * sizeof(float));
    load_bias_conv(
        weight_src, 
        conv_1_bias2D,
        weight_bias_record,
        nn_channel_size_conv, 
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    //cout << "Loading conv bias 1 to memory space, starting at: " <<conv_bias_num << '\n';
    for (int i = 0; i < 6; i++) {
        conv_bias[conv_bias_num] = (data_type_w)conv_1_bias2D[i];
        conv_bias_num++;
    }
    in_number_conv++;

    // Prepare weights and bias for conv layer 2
    memset(conv_2_weight2D, 0, 2400 * sizeof(float));
    load_weight_conv(
        weight_src, 
        conv_2_weight2D,
        weight_bias_record,
        nn_channel_size_conv, 
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    //cout << "Loading conv weight 2 to memory space, starting at: " <<conv_weight_num << '\n';
    for (int i = 0; i < 2400; i++) {
        conv_weight[conv_weight_num] = (data_type_w)conv_2_weight2D[i];
        conv_weight_num++;
    }
    memset(conv_2_bias2D, 0, 16 * sizeof(float));
    load_bias_conv(
        weight_src, 
        conv_2_bias2D,
        weight_bias_record,
        nn_channel_size_conv, 
        nn_in_number_conv,
        nn_out_number_conv,
        in_number_conv);
    //cout << "Loading conv bias 2 to memory space, starting at: " <<conv_bias_num << '\n';
    for (int i = 0; i < 16; i++) {
        conv_bias[conv_bias_num] = (data_type_w)conv_2_bias2D[i];
        conv_bias_num++;
    }
    in_number_conv++;

    //cout<<"Finished loading conv weight into memory! Total: " <<conv_weight_num  << "... ... ..."<<endl;
    //cout<<"Finished loading conv bias into memory! Total: " <<conv_bias_num  << "... ... ..."<<endl;

    // Prepare weights and bias for fc layer 1
    memset(fc_1_weight2D, 0, 4000 * sizeof(float));
    load_weight_fc(
        weight_src, 
        fc_1_weight2D,
        weight_bias_record,
        nn_channel_size_fc, 
        nn_in_number_fc,
        nn_out_number_fc,
        in_number_fc);
    //cout << "Loading fc weight 1 to memory space, starting at: " <<conv_weight_num << '\n';
    for (int i = 0; i < 4000; i++) {
        conv_weight[conv_weight_num] = (data_type_w)fc_1_weight2D[i];
        conv_weight_num++;
    }
    memset(fc_1_bias2D, 0, 10 * sizeof(float));
    load_bias_fc(
        weight_src, 
        fc_1_bias2D,
        weight_bias_record,
        nn_channel_size_fc, 
        nn_in_number_fc,
        nn_out_number_fc,
        in_number_fc);
    //cout << "Loading fc bias 1 to memory space, starting at: " <<conv_bias_num << '\n';
    for (int i = 0; i < 10; i++) {
        conv_bias[conv_bias_num] = (data_type_w)fc_1_bias2D[i];
        conv_bias_num++;
    }
    in_number_fc++;

//---------------------conv weight bram ------------------------------------//
    //conv_1_w_load
//    w_buf_t_load(conv_weight_tmp, conv_1_weight2D, conv_param_1[10], conv_param_1[1], conv_param_1[0], conv_param_1[2], w_r_offset, w_c_offset);

    for (int i = 0; i < 1; i++ ) {
	for (int j = 0; j < 6; j++) {
	    for (int k = 0; k < 5; k++) {
		for (int l = 0; l < 5; l++) {
		    conv_weight_tmp[i][j][k*32 + l] = conv_1_weight2D[j*25 + k*5 +l];
		}
	    }
	}
    }

/*
    //conv_2_w_load
    w_c_offset = 5;
    acc_param_conv_2[10] = w_c_offset;
    w_buf_t_load(conv_weight_tmp, conv_2_weight2D, conv_param_2[10], conv_param_2[1], conv_param_2[0], conv_param_2[2], w_r_offset, w_c_offset);
    //fc_w_load
    w_c_offset = 10;
    acc_param_conv_3[10] = w_c_offset;
    w_buf_t_load(conv_weight_tmp, fc_1_weight2D, conv_param_3[10], conv_param_3[1], conv_param_3[0], conv_param_3[2], w_r_offset, w_c_offset);
*/
    w_buf_t.open("netOutput/weight.txt", ios::app);
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 32; j++) {
        w_buf_t <<"w_buf_data: "<<i<<"_"<<j<< endl;
        for(int k = 0; k < 32; k++){
          for(int l = 0; l < 32; l++){
            w_buf_t << conv_weight_tmp[i][j][k*32+l] << " ";
          }
          w_buf_t << endl;
        }
        w_buf_t << endl;
      }
      w_buf_t << endl;
    }
    w_buf_t.close();

    //write data to DDR_SH_ADDR
    //Fill weight data into the bram spaces
/*
    for(loop_var = 0; loop_var < 8; loop_var++){
        for(loop_var_1 = 0; loop_var_1 < 32; loop_var_1++){
            Fill_Bram(pci_bar_handle_4, 0xE01000000+0x00100000*loop_var+0x00001000*loop_var_1, conv_weight_tmp[loop_var][loop_var_1], 32*5);
        }  
    }
*/
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_0, conv_weight_tmp[0][0], 32*5);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_1, conv_weight_tmp[0][1], 32*5);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_2, conv_weight_tmp[0][2], 32*5);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_3, conv_weight_tmp[0][3], 32*5);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_4, conv_weight_tmp[0][4], 32*5);
    Fill_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_0_5, conv_weight_tmp[0][5], 32*5);

    //in_data
    Fill_Bram(pci_bar_handle_4, DDR_A_ADDR_1, in_data, 32*32);
    Fill_Bram(pci_bar_handle_4, DDR_B_ADDR, conv_bias, 6+16+10);
    
    printf("Finished writing to SH_DDR data\n");
    //nn_in_number_conv[in_number_conv]*nn_out_number_conv[in_number_conv]*nn_channel_size_conv[in_number_conv]*nn_channel_size_conv[in_number_conv]
//----------------------conv bias bram -------------------------------------//
    //nn_out_number_conv[in_number_conv]
    //Fill_Bram(pci_bar_handle_4, CONV_B_BRAM_PCIS, conv_bias, 6+16+10);
    feature_num1 = (Tn<conv_param_1[2]?Tn:conv_param_1[2]);
    feature_num2 = (Tn<conv_param_2[2]?Tn:conv_param_2[2]);
    feature_width1 = static_cast<int>(ceil(static_cast<float>((Tr<conv_param_1[5]?Tr:conv_param_1[5]) + 2 * pool_param_1[7] - pool_param_1[3]) / pool_param_1[6])) + 1;
    feature_width2 = static_cast<int>(ceil(static_cast<float>((Tr<conv_param_2[5]?Tr:conv_param_2[5]) + 2 * pool_param_2[7] - pool_param_2[3]) / pool_param_2[6])) + 1;
//----------------------input data buffer load------------------------------//
    gettimeofday(&start_net,0);
    //weight
    //load all weight
    /*for(loop_var_1 = 0; loop_var_1 < 16; loop_var_1++){
        set_cdma(pci_bar_handle,0x01000000+loop_var_1*0x00001000,0x0000000E,0x02200000+loop_var_1*0x00001000,0x00000000,0x00000280);
    }
    for(loop_var = 0; loop_var < 7; loop_var++){
        for(loop_var_1 = 0; loop_var_1 < 16; loop_var_1++){
            set_cdma(pci_bar_handle,0x01100000+loop_var*0x00100000+loop_var_1*0x00001000,0x0000000E,0x06020000+loop_var*0x00020000+loop_var_1*0x00001000,0x00000000,0x00000280);
        }  
    } 
    /*Read_Bram(pci_bar_handle_4, CONV_W_BRAM_PCIS_6_0, out_temp_1, 32*5);
    outdata.open("./netOutput/out_temp_data.txt", ios::app);
    outdata <<"conv_weight:"<< endl;
    for(loop_var = 0; loop_var < 5; loop_var++){
        for(loop_var_1 = 0; loop_var_1 < 32; loop_var_1++){
            outdata << out_temp_1[loop_var*32+loop_var_1] << "  ";
        }
        outdata << endl;   
    }
    outdata << endl;    
    outdata.close();*/
    //bias
    //Fill_Bram(pci_bar_handle_4, CONV_B_BRAM_PCIS, conv_bias, 6+16+10);
    /*Read_Bram(pci_bar_handle_4, CONV_B_BRAM_PCIS, conv_bias_1, 6+16+10);
    outdata.open("./netOutput/bias.txt", ios::app);
    outdata <<"conv_bias:"<< endl;
    for(loop_var = 0; loop_var < 32; loop_var++){
        outdata << conv_bias_1[loop_var] << "  ";  
    }
    outdata << endl;    
    outdata.close();*/
    gettimeofday(&start,0);
    start_time = cycles_to_microseconds(ticks());
    //input
    set_cdma(pci_bar_handle,0x02000000,0x0000000C,0x02000000,0x00000000,0x00001000);

    /*gettimeofday(&start,0);
    set_cdma(pci_bar_handle,0x02000000,0x0000000C,0x02000000,0x00000000,0x00001000);
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "CDMA processing time = " << diff << "  us" << endl;

    /*gettimeofday(&start,0);
    Fill_Bram(pci_bar_handle_4, BUF_IN_0_0, in_data_tmp, 1024);
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Fill_Bram processing time = " << diff << "  us" << endl;*/
    
//----------------------inference net ip status check -----------------------//    
    //1_1
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16);
    Fill_param(pci_bar_handle_4, POOL_CORE_PARAM, acc_param_pool_1, 16); 
    Fill_Bram(pci_bar_handle_4, CONV_B_BRAM_PCIS, conv_bias, 6+16+10);
    /*Read_param(pci_bar_handle_4, POOL_CORE_PARAM, acc_param_test, 16);
    Fill_Bram(pci_bar_handle_4, 0x02200000, conv_weight_tmp[0][0], 32*20);
    outdata.open("./netOutput/in_data.txt", ios::app);
    Fill_Bram(pci_bar_handle_4, 0x0403F000, in_data_tmp, 32*32);
    Read_Bram(pci_bar_handle_4, 0x0403F000, in_data_1, 32*32);
    outdata <<"in_data:"<< endl;
    for(loop_var = 0; loop_var < 32; loop_var++){
        for(loop_var_1 = 0; loop_var_1 < 32; loop_var_1++){
            outdata << in_data_1[loop_var*32+loop_var_1] << "  ";
        }
        outdata << endl;   
    }
    outdata << endl;
    outdata <<"param:"<< endl;
    for(loop_var = 0; loop_var < 16; loop_var++){
        outdata << acc_param_test[loop_var] << "  ";   
    }
    outdata << endl;    
    Fill_Bram(pci_bar_handle_4, CONV_B_BRAM_PCIS, conv_bias, 6+16+10);
    Read_Bram(pci_bar_handle_4, CONV_B_BRAM_PCIS, conv_bias_1, 6+16+10);
    outdata <<"bias:"<< endl;
    for(loop_var = 0; loop_var < 32; loop_var++){
        outdata << conv_bias_1[loop_var] << "  ";   
    }
    outdata << endl;  
    Read_Bram(pci_bar_handle_4, 0x02200000, in_data_1, 128*32);
    outdata <<"weight_data:"<< endl;
    for(loop_var = 0; loop_var < 32; loop_var++){
        for(loop_var_1 = 0; loop_var_1 < 128; loop_var_1++){
            outdata << in_data_1[loop_var*128+loop_var_1] << "  ";
        }
        outdata << endl;   
    }
    outdata << endl;    
    outdata.close();*/
    /*outdata.open("./netOutput/in_data.txt", ios::app);
    Read_Bram(pci_bar_handle_4, CONV_B_BRAM_PCIS, conv_bias_1, 6+16+10);
    outdata <<"bias:"<< endl;
    for(loop_var = 0; loop_var < 32; loop_var++){
        outdata << conv_bias_1[loop_var] << "  ";   
    }
    Read_Bram(pci_bar_handle_4, 0x02200000, in_data_1, 128*32);
    outdata <<"weight_data:"<< endl;
    for(loop_var = 0; loop_var < 32; loop_var++){
        for(loop_var_1 = 0; loop_var_1 < 128; loop_var_1++){
            outdata << in_data_1[loop_var*128+loop_var_1] << "  ";
        }
        outdata << endl;   
    }
    outdata << endl;    
    outdata.close();*/
    end_time = cycles_to_microseconds(ticks());
    cout << "input data loading time = " << end_time-start_time << "  us" << endl;
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "conv1_1 data loading time = " << diff << "  us" << endl;

    gettimeofday(&start,0);
    start_time = cycles_to_microseconds(ticks());
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    acc_param_conv_1[6]=8;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16);
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    end_time = cycles_to_microseconds(ticks());
    cout << "conv1_1 processing time = " << end_time-start_time << "  us" << endl;
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_1 processing time = " << diff << "  us" << endl;

    //1_2
    gettimeofday(&start,0);
    start_time = cycles_to_microseconds(ticks());
    
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    acc_param_conv_1[6]=16;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16);
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    end_time = cycles_to_microseconds(ticks());
    cout << "conv1_2 processing time = " << end_time-start_time << "  us" << endl;
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_2 processing time = " << diff << "  us" << endl;
    
    //1_3
    gettimeofday(&start,0);
    start_time = cycles_to_microseconds(ticks());
    
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    acc_param_conv_1[6]=24;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16); 
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    end_time = cycles_to_microseconds(ticks());
    cout << "conv1_3 processing time = " << end_time-start_time << "  us" << endl;
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_3 processing time = " << diff << "  us" << endl;
    
    //1_4
    gettimeofday(&start,0);
    start_time = cycles_to_microseconds(ticks());
    
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    end_time = cycles_to_microseconds(ticks());
    cout << "conv1_4 processing time = " << end_time-start_time << "  us" << endl;
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_4 processing time = " << diff << "  us" << endl;
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_1, in_data_1, 32*4);
    outdata.open("./netOutput/out_temp_data.txt", ios::app);
    outdata <<"out_data:"<< endl;
    for(loop_var = 0; loop_var < 4; loop_var++){
        for(loop_var_1 = 0; loop_var_1 < 32; loop_var_1++){
            outdata << in_data_1[loop_var*32+loop_var_1] << "  ";
        }
        outdata << endl;   
    }
    outdata << endl;    
    outdata.close();

    //1_5
    gettimeofday(&start,0);
    acc_param_conv_1[5]=8;
    acc_param_conv_1[6]=0;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16);
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_5 processing time = " << diff << "  us" << endl;
    
    //1_6
    gettimeofday(&start,0);
    acc_param_conv_1[6]=8;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16);
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_6 processing time = " << diff << "  us" << endl;
    
    //1_7
    gettimeofday(&start,0);
    acc_param_conv_1[6]=16;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16); 
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_7 processing time = " << diff << "  us" << endl;

    //1_8
    gettimeofday(&start,0);
    acc_param_conv_1[6]=24;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16);
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_8 processing time = " << diff << "  us" << endl;
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_1, in_data_1, 32*8);
    outdata.open("./netOutput/out_temp_data.txt", ios::app);
    outdata <<"out_data:"<< endl;
    for(loop_var = 0; loop_var < 8; loop_var++){
        for(loop_var_1 = 0; loop_var_1 < 32; loop_var_1++){
            outdata << in_data_1[loop_var*32+loop_var_1] << "  ";
        }
        outdata << endl;   
    }
    outdata << endl;    
    outdata.close();
    
    //1_9
    gettimeofday(&start,0);
    acc_param_conv_1[5]=16;
    acc_param_conv_1[6]=0;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16);
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_9 processing time = " << diff << "  us" << endl;
    
    //1_10
    gettimeofday(&start,0);
    acc_param_conv_1[6]=8;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16); 
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_10 processing time = " << diff << "  us" << endl;

    //1_11
    gettimeofday(&start,0);
    acc_param_conv_1[6]=16;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16);
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_11 processing time = " << diff << "  us" << endl;
    
    //1_12
    gettimeofday(&start,0);
    acc_param_conv_1[6]=24;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16);
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_12 processing time = " << diff << "  us" << endl;
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_1, in_data_1, 32*12);
    outdata.open("./netOutput/out_temp_data.txt", ios::app);
    outdata <<"out_data:"<< endl;
    for(loop_var = 0; loop_var < 12; loop_var++){
        for(loop_var_1 = 0; loop_var_1 < 32; loop_var_1++){
            outdata << in_data_1[loop_var*32+loop_var_1] << "  ";
        }
        outdata << endl;   
    }
    outdata << endl;    
    outdata.close();
    
    //1_13
    gettimeofday(&start,0);
    acc_param_conv_1[5]=24;
    acc_param_conv_1[6]=0;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16); 
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_13 processing time = " << diff << "  us" << endl;
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_1, in_data_1, 16*32);
    outdata.open("./netOutput/out_temp_data.txt", ios::app);
    outdata <<"out_data:"<< endl;
    for(loop_var = 0; loop_var < 16; loop_var++){
        for(loop_var_1 = 0; loop_var_1 < 32; loop_var_1++){
            outdata << in_data_1[loop_var*32+loop_var_1] << "  ";
        }
        outdata << endl;   
    }
    outdata << endl;    
    outdata.close();

/*    //1_14
    gettimeofday(&start,0);
    acc_param_conv_1[6]=8;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16);
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_14 processing time = " << diff << "  us" << endl;
    
    //1_15
    gettimeofday(&start,0);
    acc_param_conv_1[6]=16;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16);
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_15 processing time = " << diff << "  us" << endl;
    
    //1_16
    gettimeofday(&start,0);
    acc_param_conv_1[6]=24;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_1, 16); 
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_1_16 processing time = " << diff << "  us" << endl;

    Read_Bram(pci_bar_handle_4, BUF_OUT_1_1, in_data_1, 14*32);
    outdata.open("./netOutput/out_temp_data.txt", ios::app);
    outdata <<"out_data:"<< endl;
    for(loop_var = 0; loop_var < 14; loop_var++){
        for(loop_var_1 = 0; loop_var_1 < 32; loop_var_1++){
            outdata << in_data_1[loop_var*32+loop_var_1] << "  ";
        }
        outdata << endl;   
    }
    outdata << endl;    
    outdata.close();

    //----------------------conv_2 layer -----------------------// 
    //loading data
/*    gettimeofday(&start,0);
    start_time = cycles_to_microseconds(ticks());
    //load conv1 out_data
    //set_cdma(pci_bar_handle,0x02100000,0x00000000,0x02008000,0x00000000,0x00000200);
    for(loop_var = 0; loop_var < 6; loop_var++){
        set_cdma(pci_bar_handle,0x02100000+loop_var*0x00001000,0x00000000,0x02000000+loop_var*0x00001000,0x00000000,0x00000700);
    }   
    /*Read_Bram(pci_bar_handle_4, BUF_OUT_0_0, out_temp_7, 128*32);
    outdata.open("./netOutput/out_temp_data.txt", ios::app);
    outdata <<"conv_input:"<< endl;
    for(loop_var = 0; loop_var < 32; loop_var++){
        for(loop_var_1 = 0; loop_var_1 < 128; loop_var_1++){
            outdata << out_temp_7[loop_var*128+loop_var_1] << " ";
        }
        outdata << endl;   
    }
    outdata << endl;    
    outdata.close();*/
/*    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_2, 16);
    Fill_param(pci_bar_handle_4, POOL_CORE_PARAM, acc_param_pool_2, 16); 
    end_time = cycles_to_microseconds(ticks());
    cout << "conv2 loading time = " << end_time-start_time << "  us" << endl;
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "conv2 data loading time = " << diff << "  us" << endl;

    //----------------------inference net ip status check -----------------------//    
    //2_1
    gettimeofday(&start,0);
    start_time = cycles_to_microseconds(ticks());
    XInference_net_Start(pci_bar_handle, &InstancePtr);

    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    end_time = cycles_to_microseconds(ticks());
    cout << "conv2_1 processing time = " << end_time-start_time << "  us" << endl;
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer2_1 processing time = " << diff << "  us" << endl;

    //2_2
    gettimeofday(&start,0);
    start_time = cycles_to_microseconds(ticks());
    acc_param_conv_2[6]=8;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_2, 16); 
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    end_time = cycles_to_microseconds(ticks());
    cout << "conv2_2 processing time = " << end_time-start_time << "  us" << endl;
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_2_2 processing time = " << diff << "  us" << endl;

    //2_3
    gettimeofday(&start,0);
    acc_param_conv_2[6]=8;
    acc_param_conv_2[6]=0;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_2, 16); 
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_2_3 processing time = " << diff << "  us" << endl;

    //2_4
    gettimeofday(&start,0);
    acc_param_conv_2[6]=8;
    acc_param_conv_2[6]=8;
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_2, 16); 
    XInference_net_Start(pci_bar_handle, &InstancePtr);
    
    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Convolution layer_2_4 processing time = " << diff << "  us" << endl;

    Read_Bram(pci_bar_handle_4, BUF_OUT_1_0, in_data_1, 5*5);
    outdata.open("./netOutput/out_temp_data.txt", ios::app);
    outdata <<"out_data:"<< endl;
    for(loop_var = 0; loop_var < 5; loop_var++){
        for(loop_var_1 = 0; loop_var_1 < 5; loop_var_1++){
            outdata << in_data_1[loop_var*5+loop_var_1] << "  ";
        }
        outdata << endl;   
    }
    outdata << endl;    
    outdata.close();

    //----------------------fc layer -----------------------//  
    //fc1_1
    //loading fc1_1 in_data
    gettimeofday(&start,0);
    for(loop_var = 0; loop_var < 8; loop_var++){
        set_cdma(pci_bar_handle,0x02100000+loop_var*0x00001000,0x00000000,0x02000000+loop_var*0x00001000,0x00000000,0x00000500);
    }
    for(loop_var = 0; loop_var < 8; loop_var++){
        set_cdma(pci_bar_handle,0x02108000+loop_var*0x00001000,0x00000000,0x02008000+loop_var*0x00001000,0x00000000,0x00000500);
    }
    //cdma_cp(pci_bar_handle,feature_num2, feature_width2, 0x00040000, 0x04000000, 0x00000014);
    //cout<<feature_num2<<"!!!!"<<endl;
    //cout<<feature_width2<<"????"<<endl;
    /*for(loop_var = 0; loop_var < 8; loop_var++){
        for (loop_var_1 = 0; loop_var_1 < 5; loop_var_1++) {
            set_cdma(pci_bar_handle,0x00040000+loop_var*0x00001000+loop_var_1*0x00000080,0x00000000,0x04000000+loop_var*0x00010000+loop_var_1*0x00000200,0x00000000,0x00000014);
        }
    }*/
/*    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_3, 16);
    Fill_param(pci_bar_handle_4, POOL_CORE_PARAM, acc_param_pool_3, 16); 
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "fc_1_1 data loading time = " << diff << "  us" << endl;
    
    //----------------------inference net ip status check -----------------------//    
    //ip_status = XInference_net_ReadReg(pci_bar_handle, InstancePtr.ctrl_bus_baseaddress, XINFERENCE_NET_CRTL_BUS_ADDR_AP_CTRL);
    gettimeofday(&start,0);
    start_time = cycles_to_microseconds(ticks());
    XInference_net_Start(pci_bar_handle, &InstancePtr);

    //loading fc1_2 in_data
    //cdma_cp(pci_bar_handle,feature_num2, feature_width2, 0x00048000, 0x04000100, 0x00000014);
    /*for(loop_var = 0; loop_var < 8; loop_var++){
        for (loop_var_1 = 0; loop_var_1 < 5; loop_var_1++) {
            set_cdma(pci_bar_handle,0x00048000+loop_var*0x00001000+loop_var_1*0x00000080,0x00000000,0x04000100+loop_var*0x00010000+loop_var_1*0x00000200,0x00000000,0x00000014);
        }
    }*/
/*    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    end_time = cycles_to_microseconds(ticks());
    cout << "fc1_1 processing time = " << end_time-start_time << "  us" << endl;
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Fc layer1_1 processing time = " << diff << "  us" << endl;

    //fc1_2 
    //loading data
    gettimeofday(&start,0);
    acc_param_conv_3[1] = 8;//n
    acc_param_conv_3[10] = 15;//w_c_offset
    acc_param_conv_3[12] = 1;//in_buf_flag
    Fill_param(pci_bar_handle_4, CONV_CORE_PARAM, acc_param_conv_3, 16);
    Fill_param(pci_bar_handle_4, POOL_CORE_PARAM, acc_param_pool_3, 16); 
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "fc_1_2 data loading time = " << diff << "  us" << endl;

    gettimeofday(&start,0);
    XInference_net_Start(pci_bar_handle, &InstancePtr);

    while (!XInference_net_IsDone(pci_bar_handle, &InstancePtr)) {
        count++;
    }
    gettimeofday(&end,0);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    cout << "Fc layer1_2 processing time = " << diff << "  us" << endl;
    //cout << "IP is done at " << count << " attempts" << endl; 
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_0, out_temp_1, 1);
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_1, out_temp_2, 1);
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_2, out_temp_3, 1);
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_3, out_temp_4, 1);
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_4, out_temp_5, 1);
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_5, out_temp_6, 1);
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_6, out_temp_7, 1);
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_7, out_temp_8, 1);
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_8, out_temp_9, 1);
    Read_Bram(pci_bar_handle_4, BUF_OUT_1_9, out_temp_10, 1);

    fc_3_out[0]=(float)(out_temp_1[0]);
    fc_3_out[1]=(float)(out_temp_2[0]);
    fc_3_out[2]=(float)(out_temp_3[0]);
    fc_3_out[3]=(float)(out_temp_4[0]);
    fc_3_out[4]=(float)(out_temp_5[0]);
    fc_3_out[5]=(float)(out_temp_6[0]);
    fc_3_out[6]=(float)(out_temp_7[0]);
    fc_3_out[7]=(float)(out_temp_8[0]);
    fc_3_out[8]=(float)(out_temp_9[0]);
    fc_3_out[9]=(float)(out_temp_10[0]);
    for (loop_var = 0; loop_var < 10; loop_var++ ) {
        cout << fc_3_out[loop_var] << "  ";
    }
    cout << endl;
    softmax(fc_3_out, 10);
    predict(fc_3_out, 10);
    gettimeofday(&end_net,0);
    diff = 1000000*(end_net.tv_sec-start_net.tv_sec) + end_net.tv_usec-start_net.tv_usec;
    cout << "Lenet processing time = " << diff << "  us" << endl;
*/
//------------------------------------------------------------------------------------------
    printf("\n");
    printf("Reading and verifying DDR_B Dst Buffer 1KB\n");

    /*for ( loop_var = 0; loop_var < 256; loop_var++ ) {
        rc_4 = fpga_pci_peek(pci_bar_handle_4, (DDR_B_ADDR + loop_var*4), &value);
       fail_on(rc_4, out, "Unable to read read from the fpga !");
       //printf("register: 0x%x\n", value);
       if (value != loop_var)
        {
          printf("Data mismatch!");
        }
    }
    printf("\n");
    printf("CDMA Transfer Successful!\n");*/


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
