#ifndef __TFT_LCD_H
#define __TFT_LCD_H

// #define LVGL 1

#include "main.h"

// #if LVGL
// #include "lvgl.h"
// #endif

// // ������ɫ
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40      // ��ɫ
#define BRRED 0XFC07      // �غ�ɫ
#define GRAY 0X8430       // ��ɫ
#define DARKBLUE 0X01CF   // ����ɫ
#define LIGHTBLUE 0X7D7C  // ǳ��ɫ
#define GRAYBLUE 0X5458   // ����ɫ
#define LIGHTGREEN 0X841F // ǳ��ɫ
#define LGRAY 0XC618      // ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE 0XA651  // ǳ����ɫ(�м����ɫ)
#define LBBLUE 0X2B12     // ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

#define screen_center_x (LCD_W / 2) // ��Ļ����X = ��Ļx/2
#define screen_center_y (LCD_H / 2) // ��Ļ����Y = ��Ļy/2



// #define screen_center_x (LCD_W / 2) // ��Ļ����X = ��Ļx/2
// #define screen_center_y (LCD_H / 2) // ��Ļ����Y = ��Ļy/2

/******* ����UI *******/
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);                                 // ��ָ��λ�û�һ����

#if LVGL
void LCD_Fill_LVGL(const lv_area_t * area, uint8_t * px_map);                                 // LVGL专用填充函数
void LCD_DrawPoint_LVGL(const lv_area_t * area, uint8_t * px_map);
#else
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);  // ָ�����������ɫ
// void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);      // ��ָ��λ�û�һ����
// void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color); // ��ָ��λ�û�һ������
// void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);                   // ��ָ��λ�û�һ��Բ
// void LCD_Drawarc(int x, int y, int a, int b, unsigned int r, unsigned int c);               // ��ָ��λ�û�һ����
// void LCD_ArcRect(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color);
// void LCD_DrawVerrticalLine(int x, int y1, int y2, unsigned int color);

// void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);        // ��ʾһ���ַ�
// void LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode); // ��ʾ�ַ���
// void LCD_ShowChinese12x12(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);
// void LCD_ShowChinese16x16(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);
// void LCD_ShowChinese24x24(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);
// void LCD_ShowChinese32x32(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);
// void LCD_ShowChinese(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode);

// uint32_t mypow(uint8_t m, uint8_t n);                                                                            // ����
// void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey); // ��ʾ��������
// void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey); // ��ʾ��λС������
// void LCD_ShowUint8ArrayToHex(uint16_t x, uint16_t y, const uint8_t *arr, uint16_t len, uint16_t fc, uint16_t bc, uint8_t sizey);
// void LCD_ShowPicture(unsigned int x, unsigned int y, unsigned int length, unsigned int width, const unsigned char pic[]);

#endif // LVGL

// void lcd_test(void);

#endif