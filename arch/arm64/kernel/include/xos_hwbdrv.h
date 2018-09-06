/* xos_hwbdrv.h COPYRIGHT FUJITSU LIMITED 2017 */

#ifndef _VHBM_SYS_H_
#define _VHBM_SYS_H_

#include <xos_sys_common.h>

#define VHBM_BARRIER_CTRL_EL1    sys_reg(3,0,11,12,0)
#define VHBM_BARRIER_BST_BIT_EL1 sys_reg(3,0,11,12,4)
#define VHBM_BARRIER_INIT_SYNC_BB0_EL1 sys_reg(3,0,15,13,0)
#define VHBM_BARRIER_INIT_SYNC_BB1_EL1 sys_reg(3,0,15,13,1)
#define VHBM_BARRIER_INIT_SYNC_BB2_EL1 sys_reg(3,0,15,13,2)
#define VHBM_BARRIER_INIT_SYNC_BB3_EL1 sys_reg(3,0,15,13,3)
#define VHBM_BARRIER_INIT_SYNC_BB4_EL1 sys_reg(3,0,15,13,4)
#define VHBM_BARRIER_INIT_SYNC_BB5_EL1 sys_reg(3,0,15,13,5)
#define VHBM_BARRIER_ASSIGN_SYNC_W0_EL1	sys_reg(3,0,15,15,0)
#define VHBM_BARRIER_ASSIGN_SYNC_W1_EL1	sys_reg(3,0,15,15,1)
#define VHBM_BARRIER_ASSIGN_SYNC_W2_EL1	sys_reg(3,0,15,15,2)
#define VHBM_BARRIER_ASSIGN_SYNC_W3_EL1	sys_reg(3,0,15,15,3)

#define VHBM_BARRIER_BST_SYNC_W0_EL0 sys_reg(3,3,15,15,0)
#define VHBM_BARRIER_BST_SYNC_W1_EL0 sys_reg(3,3,15,15,1)
#define VHBM_BARRIER_BST_SYNC_W2_EL0 sys_reg(3,3,15,15,2)
#define VHBM_BARRIER_BST_SYNC_W3_EL0 sys_reg(3,3,15,15,3)
#define VHBM_BARRIER_LBSY_SYNC_W0_EL0 sys_reg(3,3,15,15,0)
#define VHBM_BARRIER_LBSY_SYNC_W1_EL0 sys_reg(3,3,15,15,1)
#define VHBM_BARRIER_LBSY_SYNC_W2_EL0 sys_reg(3,3,15,15,2)
#define VHBM_BARRIER_LBSY_SYNC_W3_EL0 sys_reg(3,3,15,15,3)

#define VHBM_BARRIER_INIT_SYNC_BB(num)  (VHBM_BARRIER_INIT_SYNC_BB ##num_EL1)
#define VHBM_BARRIER_ASSIGN_SYNC_W(num) (VHBM_BARRIER_ASSIGN_SYNC_W##num_EL1)
#define VHBM_BARRIER_LBSY_SYNC_W(num)   (VHBM_BARRIER_LBSY_SYNC_W##num_EL0)
#define VHBM_BARRIER_BST_SYNC_W(num)    (VHBM_BARRIER_BST_SYNC_W##num_EL0)

#define VHBM_BARRIER_CTRL_EL1_EL1AE_ENABLE	(1UL << 63)
#define VHBM_BARRIER_CTRL_EL1_EL0AE_ENABLE	(1UL << 62)
#define VHBM_BARRIER_INIT_SYNC_BB_EL1_BST_MASK	(0x1fffUL)
#define VHBM_BARRIER_INIT_SYNC_BB_EL1_LBSY_MASK	(1UL << 20)
#define VHBM_BARRIER_INIT_SYNC_BB_EL1_LBSY_SHIFT (20)
#define VHBM_BARRIER_INIT_SYNC_BB_EL1_BST_MASK_MASK	(0x1fffUL << 32)
#define VHBM_BARRIER_INIT_SYNC_BB_EL1_BST_MASK_SHIFT	(32)
#define VHBM_BARRIER_INIT_ASSIGN_SYNC_W_EL1_VALID_ENABLE	(1UL << 63)
#define VHBM_BARRIER_INIT_ASSIGN_SYNC_W_EL1_BB_NUM_MASK	(0x7UL)
#define VHBM_BARRIER_BST_BIT_EL1_BANK_MASK	(3)
#define VHBM_BARRIER_BST_BIT_EL1_BANK_SHIFT	(4)
#define VHBM_BARRIER_BST_BIT_EL1_BST_BIT_MASK	(0xfUL)
#define VHBM_BARRIER_BST_SYNC_W_EL0_VALUE_MASK	(0x1UL)
#define VHBM_BARRIER_LBSY_SYNC_W_EL0_VALUE_MASK	(0x1UL)

#define VHBM_BARRIER_BLADE_0_NUM (0)
#define VHBM_BARRIER_BLADE_1_NUM (1)
#define VHBM_BARRIER_BLADE_2_NUM (2)
#define VHBM_BARRIER_BLADE_3_NUM (3)
#define VHBM_BARRIER_BLADE_4_NUM (4)
#define VHBM_BARRIER_BLADE_5_NUM (5)

#define VHBM_BARRIER_WINDOW_0_NUM (0)
#define VHBM_BARRIER_WINDOW_1_NUM (1)
#define VHBM_BARRIER_WINDOW_2_NUM (2)
#define VHBM_BARRIER_WINDOW_3_NUM (3)

#define VHBM_BW_UNSPECIFIED (0xFF)
#define VHBM_BB_UNSPECIFIED (0xFF)
#define VHBM_CMG_UNSPECIFIED (0xFF)

#define VHBM_NODE_MONOPOLY_JOB (0)
#define VHBM_NODE_SHARE_JOB (1)

void vhbm_barrier_registers_init(void);

#endif /* #ifndef _VHBM_SYS_H_ */