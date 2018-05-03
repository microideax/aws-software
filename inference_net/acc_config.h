
#ifndef _ACC_CONFIG_H_
#define _ACC_CONFIG_H_

/* Constants determined by the CL */
/* a set of register offsets; this CL has only one */
/* these register addresses should match the addresses in */
/* /aws-fpga/hdk/cl/examples/common/cl_common_defines.vh */

#define HELLO_WORLD_REG_ADDR UINT64_C(0x0)

#define DDR_SH_ADDR UINT64_C(0xE01000000)
#define DDR_B_ADDR UINT64_C(0xD02000000)
#define DDR_A_ADDR UINT64_C(0xC02000000)

#define HELLO_WORLD_REG_ADDR_CONTROL UINT64_C(0x00)
#define HELLO_WORLD_REG_ADDR_STATUS UINT64_C(0x04)

#define HELLO_WORLD_REG_ADDR_SRC_MSB_ADDR UINT64_C(0x1C)
#define HELLO_WORLD_REG_ADDR_DST_MSB_ADDR UINT64_C(0x24)

#define HELLO_WORLD_REG_ADDR_SRC_ADDR UINT64_C(0x18)
#define HELLO_WORLD_REG_ADDR_DST_ADDR UINT64_C(0x20)
#define HELLO_WORLD_REG_ADDR_BYTES UINT64_C(0x28)


//BRAM PCIS address offset
// conv weight port -- S00_AXI --- S00_AXI_7 --- 32*8 bram controller with 4k(484 available) bram address
#define CONV_W_BRAM_PCIS UINT64_C(0x00040000)
// conv bias port -- S_AXI (4k)
#define CONV_B_BRAM_PCIS UINT64_C(0x00180000)
// temp out 0 1 port -- S00_AXI_8 --- [0x20_0000, 0x4_0000) --- 8 bram controller with 32k address/per
#define BUF_OUT_0 UINT64_C(0x00200000)
// temp out 1 1 port -- S00_AXI_9 - S00_AXI_10 ---- 32*1 bram controller with 4k bram address
#define BUF_OUT_1 UINT64_C(0x00140000)
// ctrl_cmd_in_port -- axi_bram_ctrl_4 (4k)
//#define CTRL_PARAMS UINT64_C(0x00C8000000)
// conv_param_in_port -- S_AXI_1 (4k)
#define ACC_PARAMS_0 UINT64_C(0x00181000)
//// pool_param_in_port -- S_AXI_2 (4k)
#define ACC_PARAMS_1 UINT64_C(0x00182000)


#endif

