#ifndef __ST7789_H__
#define __ST7789_H__

#define ST7789_MADCTL_MY  0x80  // ????
#define ST7789_MADCTL_MX  0x40  // ????
#define ST7789_MADCTL_MV  0x20  // ????
#define ST7789_MADCTL_ML  0x10  // ????
#define ST7789_MADCTL_RGB 0x00  // RGB????
#define ST7789_MADCTL_BGR 0x08  // BGR????

#define ST7789_DATA_ROTATION 	(ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_ML)

#define ST7789_NOP 0x00     // No operation 无操作
#define ST7789_SWRESET 0x01 // Software reset 软件复位
#define ST7789_RDDID 0x04   // Read display ID 读取显示器ID
#define ST7789_COLMOD_RGB444 0x03 // RGB444 color mode RGB444颜色模式,12位色
#define ST7789_COLMOD_RGB565 0x05 // RGB565 color mode RGB565颜色模式,16位色
#define ST7789_COLMOD_RGB666 0x06 // RGB666 color mode RGB666颜色模式,18位色

#define ST7789_RDDST 0x09   // Read display status 读取显示器状态
#define ST7789_RDDPM 0x0A  // Read display power mode 读取显示器电源模式
#define ST7789_RDDMADCTL 0x0B // Read display memory access control 读取显示
#define ST7789_RDDCOLMOD 0x0C // Read display pixel format 读取显示器像素
#define ST7789_RDDIM 0x0D   // Read display image mode 读取显示器图像
#define ST7789_RDDSM 0x0E   // Read display signal mode 读取显示器信号

#define ST7789_SLPIN 0x10  // sleep in 睡眠模式
#define ST7789_SLPOUT 0x11 // out of sleep mode 退出睡眠模式
#define ST7789_PTLON 0x12  // partial display mode 局部显示模式
#define ST7789_NORON 0x13  // normal display mode 正常显示模式
#define ST7789_INVOFF 0x20  // Display inversion off 关闭显示反转
#define ST7789_INVON 0x21   // Display inversion on 打开显示反转
#define ST7789_GAMSET 0x26 // Gamma curve selected 选择伽马曲线
#define ST7789_DISPOFF 0x28 // display off 关闭显示
#define ST7789_DISPON 0x29  // display on 打开显示
#define ST7789_CASET 0x2A   // column address set 列地址设置
#define ST7789_RASET 0x2B   // row address set 行地址设置
#define ST7789_RAMWR 0x2C   // write to RAM 写入RAM
#define ST7789_RAMRD 0x2E   // read from RAM 读取RAM

#define ST7789_PTLAR 0x30  // partial area 局部区域开始/结束地址
#define ST7789_TEOFF 0x34  // Tearing effect line on/off 撕裂效果行关闭
#define ST7789_TEON 0x35   // Tearing effect line on/off 撕裂效果行打开
#define ST7789_MADCTL 0x36 // Memory Data Access Control 内存访问控制 
// Bit	名称	功能
//  7	MY	行地址顺序(垂直刷新方向):   0=从上到下(默认)  1=从下到上(垂直翻转)
//  6	MX	列地址顺序(水平刷新方向):   0=从左到右(默认)  1=从右到左(水平翻转)
//  5	MV	行列交换(X/Y 交换):        0=正常模式(默认)  1= 交换X/Y(旋转90°或270°)
//  4	ML	垂直刷新顺序((（LCD驱动内部使用，通常设为 0)
//  3	RGB	RGB/BGR顺序:              0 = RGB((（默认)  1 = BGR
//  2	MH	水平刷新顺序((（LCD驱动内部使用，通常设为 0)
// 1-0	-	保留位((（设为 00)
#define ST7789_IDMOFF 0x38 // idle mode off 空闲模式关闭
#define ST7789_IDMON 0x39  // idle mode on 空闲模式打开
#define ST7789_COLMOD 0x3A // set color mode 配置屏幕的颜色模式，决定每个像素的数据位数(RGB格式)和传输方式,
#define ST7789_FRMCTR1 0xB1 // Frame rate ctrl 正常模式/全彩模式帧率控制1
#define ST7789_FRMCTR2 0xB2 // Frame rate ctrl 待机模式/8位模式帧率控制2
#define ST7789_FRMCTR3 0xB3 // Frame rate ctrl 部分模式/全彩模式帧率控制3
#define ST7789_INVCTR 0xB4  // Display inversion control 显示反转控制
// Bit	  名称	            功能	                               推荐值
// 7-4	   -        保留位（通常设为 0）                            0000
//  3	  DIV	    数据反转模式,0:正常（默认）     1:反转	         0（默认）
//  2	  EOR	    行扫描顺序,0:正常（从上到下）1:反转（从下到上）	  0（默认）
//  1	  COL	    列扫描顺序,0:正常（从左到右）1:反转（从右到左）	  0（默认）
//  0	   -	    保留位（通常设为 0）	                        0
#define ST7789_DISSET5 0xB6 // Display function set 5 显示功能设置5
#define ST7789_EMSet 0xB7 // Entry Mode Set 入口模式设置,设置 GRAM（显存）的读写顺序
/**
 * Bit	名称                    	功能
    7	DFM	                Data Format Mode    0:65K色（RGB565)    1:262K色（RGB666）
    6	BGR	                颜色顺序             0:RGB（默认）       1:BGR（红蓝交换）
    5	HWM 	            水平写入模式         0:从左到右（默认）   1:从右到左（水平翻转）
    4	VWM	                垂直写入模式         0 = 从上到下（默认） 1:从下到上（垂直翻转）
    3	ORG	                显存地址顺序         0 = 正常（默认）     1 = 反向（某些特殊模式）
    2	ID1	                扫描方向控制（与ID0组合使用）
    1	ID0	                扫描方向控制（与ID1组合使用）
    0	TRI	                保留位（通常设为0）

*  ID1	ID0	    扫描方向	                典型用途
    0	 0	 从左到右，从上到下（默认）	    0° 正常显示
    0	 1	 从右到左，从上到下	             水平翻转
    1	 0	 从左到右，从下到上	             垂直翻转
    1	 1	 从右到左，从下到上	            180° 旋转
 **/
#define ST7789_PUMPR 0xB8 // Pump ratio 设置泵的比率
#define ST7789_PWCTR1 0xC0 // Power control 1 电源控制1
#define ST7789_PWCTR2 0xC1 // Power control 2 电源控制2
#define ST7789_PWCTR3 0xC2 // Power control 3 电源控制3
#define ST7789_PWCTR4 0xC3 // Power control 4 电源控制4
#define ST7789_PWCTR5 0xC4 // Power control 5 电源控制5
#define ST7789_VMCTR1 0xC5 // VCOM control 1 VCOM控制1

#define ST7789_RDID1 0xDA // Read ID 1 读取ID1
#define ST7789_RDID2 0xDB // Read ID 2 读取ID2
#define ST7789_RDID3 0xDC // Read ID 3 读取ID3
#define ST7789_RDID4 0xDD // Read ID 4 读取ID4

#define ST7789_PWCTR6 0xFC // Power control 6 电源控制6

#define ST7789_GMCTRP1 0xE0 // 
#define ST7789_GMCTRN1 0xE1 // Gamma

#endif // ST7789_H