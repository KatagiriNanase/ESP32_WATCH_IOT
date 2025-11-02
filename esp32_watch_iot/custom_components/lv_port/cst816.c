#include "cst816.h" 

#define TOUCH_OFFSET_Y 0
// #define REVERSE 1

static i2c_master_bus_handle_t i2c_handle;
static i2c_master_dev_handle_t cst816_handle;
CST816_Info	CST816_Instance;
/*
*********************************************************************************************************
*	函 数 名: CST816_GPIO_Init
*	功能说明: CST816 GPIO口初始化
*	形    参：none
*	返 回 值: none
*********************************************************************************************************
*/
void CST816_GPIO_Init(void)
{
	/* 初始化i2c bus */

	i2c_master_bus_config_t i2c_cfg = {
		.clk_source = I2C_CLK_SRC_DEFAULT,
		.i2c_port = I2C_NUM_0,
		.scl_io_num = I2C_SCL,
		.sda_io_num = I2C_SDA,
		.glitch_ignore_cnt = 7,
		.intr_priority = 0
	};
	ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_cfg, &i2c_handle));

	/* 挂载cst816 */

	i2c_device_config_t dev_cfg = {
		.dev_addr_length = I2C_ADDR_BIT_7,
		.device_address = DEVICE_ADDR,
		.scl_speed_hz = 400 * 1000,
	};

	ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_handle, &dev_cfg, &cst816_handle));

}

/*
*********************************************************************************************************
*	函 数 名: CST816_Init
*	功能说明: CST816初始化
*	形    参：none
*	返 回 值: none
*********************************************************************************************************
*/
void CST816_Init(void)
{
	CST816_GPIO_Init();
	CST816_Config_AutoSleepTime(5);
}

#if IICSOFT
/*
*********************************************************************************************************
*	函 数 名: CST816_IIC_ReadREG
*	功能说明: 读取触摸屏单个寄存器的数据
*	形    参：reg：寄存器地址
*	返 回 值: 返回寄存器存储的数据
*********************************************************************************************************
*/
uint8_t CST816_IIC_ReadREG(uint8_t addr)
{
	return IIC_Read_One_Byte(&CST816_dev, Device_Addr, addr);
}

/*
*********************************************************************************************************
*	函 数 名: CST816_IIC_WriteREG
*	功能说明: 向触摸屏的寄存器写入数据
*	形    参：addr：寄存器地址
*						dat:	写入的数据
*	返 回 值: 返回寄存器存储的数据
*********************************************************************************************************
*/
void CST816_IIC_WriteREG(uint8_t addr, uint8_t dat)
{
	IIC_Write_One_Byte(&CST816_dev, Device_Addr, addr, dat);
}

#else

/*
*********************************************************************************************************
*	函 数 名: CST816_IIC_ReadREG
*	功能说明: 读取触摸屏单个寄存器的数据
*	形    参：reg：寄存器地址
*	返 回 值: 返回寄存器存储的数据
*********************************************************************************************************
*/
uint8_t CST816_IIC_ReadREG(uint8_t addr)
{
	uint8_t ret;
	ESP_ERROR_CHECK(i2c_master_transmit_receive(cst816_handle, &addr, 1, &ret, 1, 1000));
	return ret;
}

/*
*********************************************************************************************************
*	函 数 名: CST816_IIC_WriteREG
*	功能说明: 向触摸屏的寄存器写入数据
*	形    参：addr：寄存器地址
*						dat:	写入的数据
*	返 回 值: 返回寄存器存储的数据
*********************************************************************************************************
*/
void CST816_IIC_WriteREG(uint8_t addr, uint8_t dat)
{
	uint8_t data[2] = { addr,dat };
	i2c_master_transmit(cst816_handle, data, 2, 1000);
}

#endif
/*
*********************************************************************************************************
*	函 数 名: TOUCH_RESET
*	功能说明: 触摸屏复位
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
// void CST816_RESET(void)
// {
// 	// TOUCH_RST_0;
// 	// HAL_Delay(10);
// 	// TOUCH_RST_1;
// 	// HAL_Delay(100);
// }
#if IICSOFT 
/*
*********************************************************************************************************
*	函 数 名: TOUCH_READ_X
*	功能说明: 读取触摸屏在触摸时的坐标值
*	形    参：无
*	返 回 值: 无 （数据存储在CST816_Instance结构体中）
*********************************************************************************************************
*/
void CST816_Get_XY_AXIS(void)
{
	uint8_t DAT[4];
	IIC_Read_Multi_Byte(&CST816_dev, Device_Addr, XposH, 4, DAT);
	CST816_Instance.X_Pos = ((DAT[0] & 0x0F) << 8) | DAT[1];//(temp[0]&0X0F)<<4|
	CST816_Instance.Y_Pos = ((DAT[2] & 0x0F) << 8) | DAT[3] + TOUCH_OFFSET_Y;//(temp[2]&0X0F)<<4|
#if REVERSE
	CST816_Instance.X_Pos = 239 - CST816_Instance.X_Pos;
	CST816_Instance.Y_Pos = 279 - CST816_Instance.Y_Pos;
#endif
}
#else
/*
*********************************************************************************************************
*	函 数 名: TOUCH_READ_X
*	功能说明: 读取触摸屏在触摸时的坐标值
*	形    参：无
*	返 回 值: 无 （数据存储在CST816_Instance结构体中）
*********************************************************************************************************
*/
void CST816_Get_XY_AXIS(void)
{
	uint8_t DAT[4];
	uint8_t reg_addr = XposH;
	// IIC_Read_Multi_Byte(&hi2c1, Device_Addr, XposH, 4, DAT);
	i2c_master_transmit_receive(cst816_handle, &reg_addr, 1, DAT, 4, 1000);
	CST816_Instance.X_Pos = ((DAT[0] & 0x0F) << 8) | DAT[1];//(temp[0]&0X0F)<<4|
	CST816_Instance.Y_Pos = (((DAT[2] & 0x0F) << 8) | DAT[3]) + TOUCH_OFFSET_Y;//(temp[2]&0X0F)<<4|
#if REVERSE
	CST816_Instance.X_Pos = 239 - CST816_Instance.X_Pos;
	CST816_Instance.Y_Pos = 299 - CST816_Instance.Y_Pos;
#endif
}
#endif


/** 读取坐标值
 * @param  x x坐标
 * @param  y y坐标
 * @param state 松手状态 0,松手 1按下
 * @return 无
*/
void cst816t_read(int16_t* x, int16_t* y, int* state)
{
	uint8_t DAT[4];
	uint8_t touch_pnt_cnt = 0;        // Number of detected touch points
	static int16_t last_x = 0;  // 12bit pixel value
	static int16_t last_y = 0;  // 12bit pixel value
	// i2c_read(CST816T_ADDR, 0x02, 1, &touch_pnt_cnt);
	uint8_t reg_addr = FingerNum;
	ESP_ERROR_CHECK(i2c_master_transmit_receive(cst816_handle, &reg_addr, 1, &touch_pnt_cnt, 1, 1000));
	if (touch_pnt_cnt != 1)
	{    // ignore no touch & multi touch
		*x = last_x;
		*y = last_y;
		*state = 0;
		return;
	}

	//读取X，Y坐标
	reg_addr = XposH;
	ESP_ERROR_CHECK(i2c_master_transmit_receive(cst816_handle, &reg_addr, 1, DAT, 4, 1000));

	int16_t current_x = ((DAT[0] & 0x0F) << 8) | (DAT[1] & 0xFF);
	int16_t current_y = ((DAT[2] & 0x0F) << 8) | (DAT[3] & 0xFF);

	if (last_x != current_x || current_y != last_y)
	{
		last_x = current_x;
		last_y = current_y;
		//ESP_LOGI(TAG,"touch x:%d,y:%d",last_x,last_y);
	}


	if (last_x >= LCD_H_RES)
		last_x = LCD_H_RES - 1;
	if (last_y >= LCD_V_RES)
		last_y = LCD_V_RES - 1;

	*x = last_x;
	*y = last_y;
	*state = 1;
}


/*
*********************************************************************************************************
*	函 数 名: CST816_Get_FingerNum
*	功能说明: 读取触摸屏的手指触摸个数,0xFF为睡眠
*	形    参：无
*	返 回 值: 返回芯片ID
*********************************************************************************************************
*/
uint8_t CST816_Get_FingerNum(void)
{
	return CST816_IIC_ReadREG(FingerNum);
}


/*
*********************************************************************************************************
*	函 数 名: CST816_Get_ChipID
*	功能说明: 读取触摸屏的芯片ID
*	形    参：无
*	返 回 值: 返回芯片ID
*********************************************************************************************************
*/
uint8_t CST816_Get_ChipID(void)
{
	return CST816_IIC_ReadREG(ChipID);
}


/*
*********************************************************************************************************
*	函 数 名: CST816_Config_MotionMask
*	功能说明: 使能连续动作（连续左右滑动，连续上下滑动，双击）
*	形    参：mode：模式(5种)
*	返 回 值: 无
*	注    意：使能连续动作会增加响应时间
*********************************************************************************************************
*/
void CST816_Config_MotionMask(uint8_t mode)
{
	CST816_IIC_WriteREG(MotionMask, mode);
}


/*
*********************************************************************************************************
*	函 数 名: CST816_Config_AutoSleepTime
*	功能说明: 规定time内无触摸，自动进入低功耗模式
*	形    参：time：时间(s)
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816_Config_AutoSleepTime(uint8_t time)
{
	CST816_IIC_WriteREG(AutoSleepTime, time);
}

/*
*********************************************************************************************************
*	函 数 名: CST816_Sleep
*	功能说明: 进入睡眠，无触摸唤醒功能
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816_Sleep(void)
{
	CST816_IIC_WriteREG(SleepMode, 0x03);
}

/*
*********************************************************************************************************
*	函 数 名: CST816_Wakeup
*	功能说明: 唤醒
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
// void CST816_Wakeup(void)
// {
// 	CST816_RESET();
// }

/*
*********************************************************************************************************
*	函 数 名: CST816_Config_MotionSlAngle
*	功能说明: 手势检测滑动分区角度控制。Angle=tan(c)*10 c为以x轴正方向为基准的角度。
*	形    参：x_right_y_up_angle：角度值
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816_Config_MotionSlAngle(uint8_t x_right_y_up_angle)
{
	CST816_IIC_WriteREG(MotionSlAngle, x_right_y_up_angle);
}


/*
*********************************************************************************************************
*	函 数 名: CST816_Config_NorScanPer
*	功能说明: 正常快速检测周期配置函数。
*						此值会影响到LpAutoWakeTime和AutoSleepTime。
*						单位10ms，可选值：1～30。默认值为1。
*	形    参：Period：周期值
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816_Config_NorScanPer(uint8_t Period)
{
	if (Period >= 30)
		Period = 30;
	CST816_IIC_WriteREG(NorScanPer, Period);
}


/*
*********************************************************************************************************
*	函 数 名: CST816_Config_IrqPluseWidth
*	功能说明: 中断低脉冲输出宽度配置函数
*	形    参：Period：周期值
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816_Config_IrqPluseWidth(uint8_t Width)
{
	if (Width >= 200)
		Width = 200;
	CST816_IIC_WriteREG(IrqPluseWidth, Width);
}


/*
*********************************************************************************************************
*	函 数 名: CST816_Config_NorScanPer
*	功能说明: 低功耗扫描唤醒门限配置函数。越小越灵敏。默认值48
*	形    参：TH：门限值
*	返 回 值: 无
*********************************************************************************************************
*/
void CST816_Config_LpScanTH(uint8_t TH)
{
	CST816_IIC_WriteREG(LpScanTH, TH);
}