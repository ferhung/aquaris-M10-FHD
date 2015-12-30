/*****************************************************************************
 *
 * Filename:
 * ---------
 *    mt6582_auxadc.c
 *
 * Project:
 * --------
 *   Android_Software
 *
 * Description:
 * ------------
 *   This Module defines functions of mt6582 AUXADC
 *
 * Author:
 * -------
 * Zhong Wang
 *
 ****************************************************************************/

#include <linux/init.h>		/* For init/exit macros */
#include <linux/module.h>	/* For MODULE_ marcros  */
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/kthread.h>
#include <linux/clk.h>

#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <mach/mt_gpt.h>
#include <mach/sync_write.h>

#include <linux/of_address.h>

#include "mt_auxadc_sw.h"
#include "mt_auxadc_hw.h"

//extern struct clk *clk;

struct clk *clk;
extern void __iomem * auxadc_base;
//unsigned long apmixedsys_base;



#define DRV_ClearBits(addr, data)     {\
   kal_uint16 temp;\
   temp = DRV_Reg(addr);\
   temp &=  ~(data);\
   mt_reg_sync_writew(temp, addr);\
}

#define DRV_SetBits(addr, data)     {\
   kal_uint16 temp;\
   temp = DRV_Reg(addr);\
   temp |= (data);\
   mt_reg_sync_writew(temp, addr);\
}

#define DRV_SetData(addr, bitmask, value)     {\
   kal_uint16 temp;\
   temp = (~(bitmask)) & DRV_Reg(addr);\
   temp |= (value);\
   mt_reg_sync_writew(temp, addr);\
}

#define AUXADC_DRV_ClearBits16(addr, data)           DRV_ClearBits(addr, data)
#define AUXADC_DRV_SetBits16(addr, data)             DRV_SetBits(addr, data)
#define AUXADC_DRV_WriteReg16(addr, data)            mt_reg_sync_writew(data, addr)
#define AUXADC_DRV_ReadReg16(addr)                   DRV_Reg(addr)
#define AUXADC_DRV_SetData16(addr, bitmask, value)   DRV_SetData(addr, bitmask, value)

#define AUXADC_DVT_DELAYMACRO(u4Num)                                     \
{                                                                        \
    unsigned int u4Count = 0;                                           \
    for (u4Count = 0; u4Count < u4Num; u4Count++);                      \
}

#define AUXADC_CLR_BITS(BS, REG)     {\
   kal_uint32 temp;\
   temp = DRV_Reg32(REG);\
   temp &=  ~(BS);\
   mt65xx_reg_sync_writel(temp, REG);\
}

#define AUXADC_SET_BITS(BS, REG)     {\
   kal_uint32 temp;\
   temp = DRV_Reg32(REG);\
   temp |= (BS);\
   mt65xx_reg_sync_writel(temp, REG);\
}

#define VOLTAGE_FULL_RANGE  1500	/* VA voltage */
#define AUXADC_PRECISE      4096	/* 12 bits */

/*****************************************************************************
 * Integrate with NVRAM
****************************************************************************/
/* use efuse cali */
#if 0
static kal_uint32 g_adc_ge;
static kal_uint32 g_adc_oe;
/* static kal_uint32 g_o_vts = 0; */
static kal_uint32 g_o_vbg;
/* static kal_uint32 g_degc_cali = 0; */
static kal_uint32 g_adc_cali_en;
/* static kal_uint32 g_o_vts_abb = 0; */
/* static kal_int32 g_o_slope = 0; */
/* static kal_uint32 g_o_slope_sign = 0; */
/* static kal_uint32 g_id = 0; */
static kal_uint32 g_y_vbg;	/* defaul 1967 if cali_en=0 */
#endif
static DEFINE_MUTEX(mutex_get_cali_value);
static int adc_auto_set;
static kal_bool g_auxadc_suspended = KAL_FALSE;

#if 0
static u16 mt_tpd_read_adc(u16 pos)
{
	AUXADC_DRV_SetBits16((volatile u16 *)AUXADC_TP_ADDR, pos);
	AUXADC_DRV_SetBits16((volatile u16 *)AUXADC_TP_CON0, 0x01);
	while (0x01 & AUXADC_DRV_ReadReg16((volatile u16 *)AUXADC_TP_CON0)) {;
	}			/* wait for write finish */
	return AUXADC_DRV_ReadReg16((volatile u16 *)AUXADC_TP_DATA0);
}

static void mt_auxadc_disable_penirq(void)
{
	/* Turn off PENIRQ detection circuit */
	AUXADC_DRV_SetBits16((volatile u16 *)AUXADC_TP_CMD, 1);
	/* run once touch function */
	mt_tpd_read_adc(TP_CMD_ADDR_X);
}
#endif

/* step1 check con2 if auxadc is busy */
/* step2 clear bit */
/* step3  read channle and make sure old ready bit ==0 */
/* step4 set bit  to trigger sample */
/* step5  read channle and make sure  ready bit ==1 */
/* step6 read data */

int IMM_auxadc_GetOneChannelValue(int dwChannel, int data[4], int *rawdata)
{
	unsigned int channel[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int idle_count = 0;
	int data_ready_count = 0;

	if (g_auxadc_suspended == KAL_TRUE)
		mt_auxadc_hal_resume();
	mutex_lock(&mutex_get_cali_value);
#if 0
	if (enable_clock(MT_PDN_PERI_AUXADC, "AUXADC")) {
		printk("hwEnableClock AUXADC failed.");
	}
#endif
//	if (dwChannel == PAD_AUX_XP)
//		mt_auxadc_disable_penirq();
	/* step1 check con2 if auxadc is busy */
	while ((*(volatile u16 *)AUXADC_CON2) & 0x01) {
		printk("[adc_api]: wait for module idle\n");
		msleep(10);
		idle_count++;
		if (idle_count > 300) {
			/* wait for idle time out */
			printk("[adc_api]: wait for auxadc idle time out\n");
			mutex_unlock(&mutex_get_cali_value);
			return -1;
		}
	}
	/* step2 clear bit */
	if (0 == adc_auto_set) {
		/* clear bit */
		AUXADC_DRV_ClearBits16((volatile u16 *)AUXADC_CON1, (1 << dwChannel));
	}

	/* step3  read channle and make sure old ready bit ==0 */
	while ((*(volatile u16 *)(AUXADC_DAT0 + dwChannel * 0x04)) & (1 << 12)) {
		printk("[adc_api]: wait for channel[%d] ready bit clear\n", dwChannel);
		msleep(10);
		data_ready_count++;
		if (data_ready_count > 300) {
			/* wait for idle time out */
			printk("[adc_api]: wait for channel[%d] ready bit clear time out\n",
			       dwChannel);
			mutex_unlock(&mutex_get_cali_value);
			return -2;
		}
	}

	/* step4 set bit  to trigger sample */
	if (0 == adc_auto_set) {
		AUXADC_DRV_SetBits16((volatile u16 *)AUXADC_CON1, (1 << dwChannel));
	}
	/* step5  read channle and make sure  ready bit ==1 */
	udelay(25);		/* we must dealay here for hw sample cahnnel data */
	data_ready_count = 0;
	while (0 == ((*(volatile u16 *)(AUXADC_DAT0 + dwChannel * 0x04)) & (1 << 12))) {
		printk("[adc_api]: wait for channel[%d] ready bit ==1\n", dwChannel);
		msleep(10);
		data_ready_count++;

		if (data_ready_count > 300) {
			/* wait for idle time out */
			printk("[adc_api]: wait for channel[%d] data ready time out\n", dwChannel);
			mutex_unlock(&mutex_get_cali_value);
			return -3;
		}
	}
	/* step6 read data */

	channel[dwChannel] = (*(volatile u16 *)(AUXADC_DAT0 + dwChannel * 0x04)) & 0x0FFF;
	if (NULL != rawdata) {
		*rawdata = channel[dwChannel];
	}
	/* printk("[adc_api: imm mode raw data => channel[%d] = %d\n",dwChannel, channel[dwChannel]); */
	/* printk("[adc_api]: imm mode => channel[%d] = %d.%02d\n", dwChannel, (channel[dwChannel] * 150 / AUXADC_PRECISE / 100), ((channel[dwChannel] * 150 / AUXADC_PRECISE) % 100)); */
	data[0] = (channel[dwChannel] * 150 / AUXADC_PRECISE / 100);
	data[1] = ((channel[dwChannel] * 150 / AUXADC_PRECISE) % 100);

#if 0
	if (disable_clock(MT_PDN_PERI_AUXADC, "AUXADC")) {
		printk("hwEnableClock AUXADC failed.");
	}
#endif
	mutex_unlock(&mutex_get_cali_value);

	return 0;

}

/* 1v == 1000000 uv */
/* this function voltage Unit is uv */
int IMM_auxadc_GetOneChannelValue_Cali(int Channel, int *voltage)
{
	int ret = 0, data[4], rawvalue;
	u_int64_t temp_vol;

	ret = IMM_auxadc_GetOneChannelValue(Channel, data, &rawvalue);
	if (ret) {
		printk("[adc_api]:IMM_auxadc_GetOneChannelValue_Cali  get raw value error %d\n",
		       ret);
		return -1;
	}
	temp_vol = (u_int64_t) rawvalue * 1500000 / AUXADC_PRECISE;
	*voltage = temp_vol;
	/* printk("[adc_api]:IMM_auxadc_GetOneChannelValue_Cali  voltage= %d uv\n",*voltage); */
	return 0;

}

#if 0
static int IMM_auxadc_get_evrage_data(int times, int Channel)
{
	int ret = 0, data[4], i, ret_value = 0, ret_temp = 0;

	i = times;
	while (i--) {
		ret_value = IMM_auxadc_GetOneChannelValue(Channel, data, &ret_temp);
		ret += ret_temp;
		printk("[auxadc_get_data(channel%d)]: ret_temp=%d\n", Channel, ret_temp);
	}

	ret = ret / times;
	return ret;
}
#endif

static void mt_auxadc_cal_prepare(void)
{
	/* mt6582 no voltage calibration */
}

void mt_auxadc_hal_init(struct platform_device *dev)
{

//	AUXADC_DRV_SetBits16((volatile u16 *)AUXADC_CON_RTP, 1);	/* disable RTP */

#ifdef CONFIG_OF
		int ret = 0;
		struct class_device *class_dev = NULL;
		struct device_node *node = NULL;	
		node = of_find_compatible_node(NULL, NULL, "mediatek,AUXADC");
		if(node) {
			/* iomap registers */	
			auxadc_base = (unsigned long)of_iomap(node, 0);
		}
		else {
			printk("[ADC] get auxadc base fail!\n");
			return ret;
		}
		
		
#if 0
			node = of_find_compatible_node(NULL, NULL, "mediatek,mt8173-apmixedsys");
			if(node) {
				/* iomap registers */	
				apmixedsys_base = (unsigned long)of_iomap(node, 0);
			}
			else {
				printk("[ADC] get apmixedsys base fail!\n");
				return ret;
			}
#endif 
	
//		clk = devm_clk_get(&dev->dev, "auxadc_clk");

#endif

     AUXADC_DRV_SetBits16((volatile u16 *)AUXADC_MISC, 1 << 14); /* power on ADC */
     mt_auxadc_cal_prepare();



}

void mt_auxadc_hal_suspend(void)
{
    clk_disable_unprepare(clk);
    g_auxadc_suspended = KAL_TRUE;

#if 0
	printk("******** MT auxadc driver suspend!! ********\n" );
	#ifndef CONFIG_MTK_FPGA	  
	if(disable_clock(MT_PDN_PERI_AUXADC,"AUXADC"))    
		printk("hwEnableClock AUXADC failed.");
	#endif	
#endif

}

void mt_auxadc_hal_resume(void)
{

	if (g_auxadc_suspended == KAL_FALSE)
		return;
	if (clk_prepare_enable(clk)) {
		printk("hwEnableClock AUXADC again!!!.");
		if (clk_prepare_enable(clk)) {
			printk("hwEnableClock AUXADC failed.");
		}

	}

#if 0

	printk("******** MT auxadc driver resume!! ********\n" );
	#ifndef CONFIG_MTK_FPGA	i
	if(enable_clock(MT_PDN_PERI_AUXADC,"AUXADC"))	  
		printk("hwEnableClock AUXADC failed!!!.");
	#endif
#endif	

	AUXADC_DRV_SetBits16((volatile u16 *)AUXADC_MISC, 1 << 14);	/* power on ADC */
//	AUXADC_DRV_SetBits16((volatile u16 *)AUXADC_CON_RTP, 1);	/* disable RTP */
	g_auxadc_suspended = KAL_FALSE;
}

int mt_auxadc_dump_register(char *buf)
{
	printk("[auxadc]: AUXADC_CON0=%x\n", *(volatile u16 *)AUXADC_CON0);
	printk("[auxadc]: AUXADC_CON1=%x\n", *(volatile u16 *)AUXADC_CON1);
	printk("[auxadc]: AUXADC_CON2=%x\n", *(volatile u16 *)AUXADC_CON2);

	return sprintf(buf, "AUXADC_CON0:%x\n AUXADC_CON1:%x\n AUXADC_CON2:%x\n",
		       *(volatile u16 *)AUXADC_CON0, *(volatile u16 *)AUXADC_CON1,
		       *(volatile u16 *)AUXADC_CON2);
}

/*
The unit of volt is mv; The unit 0f period and debounce is ms; tFlag is 1 for high trigger,and 0 for low trigger;
*/
void mt_auxadc_enableBackgroundDection(u16 channel, u16 volt, u16 period, u16 debounce, u16 tFlag)
{
	/* Set background detection period, Set 0 to disable background detection. */
	*(volatile u16 *)AUXADC_DET_PERIOD = period * 32;

	/* Set the Debounce Time */
	*(volatile u16 *)AUXADC_DET_DEBT = debounce * 32;

	/* set the voltage for background detection(bit 15 set higher or lower) */
	*(volatile u16 *)AUXADC_DET_VOLT = (u16) ((unsigned int)(4096 * volt) / 1500);

	if (tFlag == 1) {
		*(volatile u16 *)AUXADC_DET_VOLT |= 1 << 15;
	} else {
		*(volatile u16 *)AUXADC_DET_VOLT &= ~(1 << 15);
	}
	/* Set the Channel */
	*(volatile u16 *)AUXADC_DET_SEL = channel;

}

void mt_auxadc_disableBackgroundDection(u16 channel)
{
	/* Set the Channel */
	*(volatile u16 *)AUXADC_DET_SEL = channel;

	/* Set 0 to disable background detection. */
	*(volatile u16 *)AUXADC_DET_PERIOD = 0;
}

u16 mt_auxadc_getCurrentChannel(void)
{
	return *(volatile u16 *)AUXADC_DET_SEL;
}

u16 mt_auxadc_getCurrentTrigger(void)
{
	return (*(volatile u16 *)AUXADC_DET_VOLT >> 15) & (u16) 1;
}
