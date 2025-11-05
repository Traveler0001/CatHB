#include "string.h"
#include "math.h"
#include "stdlib.h"
#include "mid_lcd.h"
#include "hardware_list.h"

#if 1

/**
* @brief 
* 
* @param xsta 
* @param ysta 
* @param xend 
* @param yend 
* @param color 
*/

// /******************************************************************************
//       ����˵������ָ��λ�û���
//       ������ݣ�x,y ��������
//                 color ������?
//       ����ֵ��  ��
// ******************************************************************************/


#if LVGL
void LCD_DrawPoint_LVGL(const lv_area_t *area, uint8_t *px_map)
{
    drv_LcdAddressSet(area->x1, area->y1, area->x2, area->y2);
    drv_LcdWRData8(*px_map);
}
void LCD_Fill_LVGL(const lv_area_t *area, uint8_t *px_map)
{
    // CubeMX生成的SPI DMA句柄（在main.c或spi.c中定义）
    extern SPI_HandleTypeDef hspi1;
    extern DMA_HandleTypeDef hdma_spi1_tx; // SPI发送DMA句柄
                                           // extern DMA_HandleTypeDef hdma_spi1_rx;  // SPI接收DMA句柄

    uint16_t xsta = area->x1, xend = area->x2, ysta = area->y1, yend = area->y2;
    uint32_t windowarea = (xend - xsta + 1) * (yend - ysta + 1); // 窗口面积
    LCD_Address_Set(xsta, ysta, xend, yend);

    uint16_t buff_size = 0xFFFF;
    // uint32_t remaining = strlen(px_map); // 总字节数
    uint32_t remaining = windowarea*2; // 总字节数

    LCD_DC_Set(); // 写数据
    LCD_CS_Clr(); // 片选

    while (remaining > 0) {
        // 计算本次填充的字节数
        // if (HAL_DMA_GetState(&hdma_spi1_tx) != HAL_DMA_STATE_READY) {
            uint16_t fill_size = remaining > buff_size ? buff_size : remaining;

            // 发送数据
            HAL_SPI_Transmit_DMA(&hspi1, px_map, fill_size);
            while (HAL_DMA_GetState(&hdma_spi1_tx) != HAL_DMA_STATE_READY) 
                ;
            
            // HAL_SPI_Transmit(&hspi1, px_map, fill_size, HAL_MAX_DELAY);
            remaining -= fill_size;
        // }
    }
    LCD_CS_Set(); // 片选
}
#else
#if BASECOMPONENTS
HAL_StatusTypeDef mid_LcdDrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    drv_LcdAddressSet(&x, &y, &x, &y); 
    drv_LcdWRData8(color);
    return HAL_OK;
}

HAL_StatusTypeDef mid_LcdFill(uint16_t *xsta, uint16_t *ysta, uint16_t *xend, uint16_t *yend, uint16_t *color)
{
    drv_LcdAddressSet(xsta, ysta, xend, yend);

    uint32_t pixel_num = 0;
    pixel_num          = (*xend - *xsta + 1) * (*yend - *ysta + 1);
    uint8_t color_high = (uint8_t)(*color >> 8);
    uint8_t color_low  = (uint8_t)*color;

    // 使用较小的缓冲区，避免栈溢出
    uint8_t buff[512];                  // 512字节缓冲区，可根据实际情况调整
    uint32_t remaining = pixel_num * 2; // 总字节数
    while (remaining > 0) {
        // 计算本次填充的字节数
        uint16_t fill_size = (remaining > sizeof(buff)) ? sizeof(buff) : remaining;
        // 确保是2的倍数，保持颜色数据完整性
        fill_size = (fill_size / 2) * 2;

        // 填充缓冲区
        for (uint16_t i = 0; i < fill_size; i += 2) {
            buff[i]     = color_high;
            buff[i + 1] = color_low;
        }

        // 发送数据
        drv_LcdWRDatas(buff, fill_size);
        remaining -= fill_size;
    }
    return HAL_OK; 
}

#endif // BASECOMPONENTS

#if EXTENDEDCOMPONENTS
#define M_PI        3.14
#define delay_ms(X) HAL_Delay(X)
#define delay_ms(X) delay_cycles(((CPUCLK_FREQ / 1000) * (X)))

/******************************************************************************
      ����˵��������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   �ߵ���ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; // ������������
    delta_y = y2 - y1;
    uRow    = x1; // �����������?
    uCol    = y1;
    if (delta_x > 0)
        incx = 1; // ���õ�������
    else if (delta_x == 0)
        incx = 0; // ��ֱ��
    else {
        incx    = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; // ˮƽ��
    else {
        incy    = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x; // ѡȡ��������������
    else
        distance = delta_y;
    for (t = 0; t < distance + 1; t++) {
        LCD_DrawPoint(uRow, uCol, color); // ����
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
}

/**
 * @brief ������
 *
 * @param x1 ��ʼx����
 * @param y1 ��ʼy����
 * @param x2 ��ֹx����
 * @param y2 ��ֹy����
 * @param color ���ε���ɫ
 */
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}

/******************************************************************************
      ����˵������Բ
      ������ݣ�x0,y0   Բ������
                r       �뾶
                color   Բ����ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawCircle(uint16_t x0, uint16_t y0, unsigned char r, uint16_t color)
{
    int a, b;
    a = 0;
    b = r;
    while (a <= b) {
        LCD_DrawPoint(x0 - b, y0 - a, color); // 3
        LCD_DrawPoint(x0 + b, y0 - a, color); // 0
        LCD_DrawPoint(x0 - a, y0 + b, color); // 1
        LCD_DrawPoint(x0 - a, y0 - b, color); // 2
        LCD_DrawPoint(x0 + b, y0 + a, color); // 4
        LCD_DrawPoint(x0 + a, y0 - b, color); // 5
        LCD_DrawPoint(x0 + a, y0 + b, color); // 6
        LCD_DrawPoint(x0 - b, y0 + a, color); // 7
        a++;
        if ((a * a + b * b) > (r * r)) // �ж�Ҫ���ĵ��Ƿ���?
        {
            b--;
        }
    }
}

void LCD_DrawFullCircle(uint16_t x0, uint16_t y0, unsigned char r, uint16_t color)
{
    int a, b;
    a = 0;
    b = r;
    while (a <= b) {
        LCD_DrawLine(x0 - b, y0 - a, x0 + b, y0 - a, color); // 3
        LCD_DrawLine(x0 - a, y0 + b, x0 + a, y0 + b, color); // 1
        LCD_DrawLine(x0 - a, y0 - b, x0 + a, y0 - b, color); // 2
        LCD_DrawLine(x0 - b, y0 + a, x0 + b, y0 + a, color); // 4
        a++;
        if ((a * a + b * b) > (r * r)) // �ж�Ҫ���ĵ��Ƿ���?
        {
            b--;
        }
    }
}

/**
 * @brief ��ָ��λ�û�һ����
 *
 * @param x ���x����
 * @param y ���y����
 * @param a ������ʼ�Ƕ�
 * @param b ������ֹ�Ƕ�
 * @param r �뾶
 * @param c ������ɫ
 */
#include <math.h>

void LCD_Drawarc(int x, int y, int a, int b, unsigned int r, unsigned int c)
{
    int x_tp, y_tp;
    int d; // Decision variable ����:���߱���

    // Start with the top and bottom rows of the circle ����:��Բ�Ķ����͵ײ��п�ʼ
    for (x_tp = 0; x_tp <= r; x_tp++) {
        // Calculate the corresponding y values ����:������Ӧ��yֵ
        y_tp = (int)sqrt(r * r - x_tp * x_tp);

        // Draw the horizontal lines from the circle edge to the center ����:��Բ�ı�Ե�����Ļ���ˮƽ��
        for (int i = x_tp; i >= -x_tp; i--) {
            LCD_DrawPoint(x + i, y - y_tp, c);
            LCD_DrawPoint(x + i, y + y_tp, c);
        }
    }

    // Now fill the rest of the circle ����:�������Բ�����ಿ��?
    d = 3 - 2 * r;
    while (x_tp > y_tp) {
        if (d < 0) {
            d += 4 * x_tp + 6;
        } else {
            d += 4 * (x_tp - y_tp) + 10;
            y_tp++;
        }
        x_tp--;

        // Draw the horizontal lines from the circle edge to the center ����:��Բ�ı�Ե�����Ļ���ˮƽ��
        for (int i = -x_tp; i <= x_tp; i++) {
            LCD_DrawPoint(x + i, y - y_tp, c);
            LCD_DrawPoint(x + i, y + y_tp, c);
        }

        // Draw the vertical lines from the circle edge to the center ����:��Բ�ı�Ե�����Ļ��ƴ�ֱ��
        for (int i = -y_tp; i <= y_tp; i++) {
            LCD_DrawPoint(x + x_tp, y + i, c);
            LCD_DrawPoint(x - x_tp, y + i, c);
        }
    }
}

/**
 * @brief ��ָ��λ�û�һ��Բ�Ǿ���
 *
 * @param xsta ��ʼ����x
 * @param ysta ��ʼ����y
 * @param xend ��ֹ����x
 * @param yend ��ֹ����y
 * @param color ������ɫ
 */
void LCD_ArcRect(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color)
{

    int r = 4;
    // ��������
    LCD_DrawLine(xsta + r, ysta, xend - r, ysta, color);
    LCD_DrawLine(xsta, ysta + r, xsta, yend - r, color);
    LCD_DrawLine(xsta + r, yend, xend - r, yend, color);
    LCD_DrawLine(xend, ysta + r, xend, yend - r, color);

    // HAL_Delay(1000);

    // LCD_DrawFullCircle(xsta + r, ysta + r, r, color);
    // LCD_DrawFullCircle(xend - r, ysta + r, r, color);
    // LCD_DrawFullCircle(xsta + r, yend - r, r, color);
    // LCD_DrawFullCircle(xend - r, yend - r, r, color);

    // �ٻ��ĸ�Բ��
    LCD_Drawarc(xsta + r, ysta + r, 90, 180, r, color);  // ����
    LCD_Drawarc(xend - r, ysta + r, 0, 90, r, color);    // ����
    LCD_Drawarc(xsta + r, yend - r, 180, 270, r, color); // ����
    LCD_Drawarc(xend - r, yend - r, 270, 360, r, color); // ����

    // �����ʵ�ľ���?
    LCD_Fill(xsta + r, ysta, xend - r, ysta + r, color); // ��
    // LCD_Fill(xsta + r, ysta + r, xend - r, yend - r, color); // ��
    LCD_Fill(xsta, ysta + r, xend, yend - r, color);     // ��
    LCD_Fill(xsta + r, yend - r, xend - r, yend, color); // ��
    // LCD_Fill(xsta, ysta + r, xsta + r, yend - r, color); // ��
    // LCD_Fill(xend - r, ysta + r, xend, yend - r, color); // ��
}

/**
 * @brief ���ƴ�ֱ��
 *
 * @param x ���x����
 * @param y1 ���y����
 * @param y2 �յ�y����
 * @param color �ߵ���ɫ
 */
void LCD_DrawVerrticalLine(int x, int y1, int y2, unsigned int color)
{
    int i;
    LCD_Address_Set(x, y1, x, y1 + y2);
    for (i = y1; i <= y1 + y2; i++) {
        LCD_WR_DATA(color);
    }
}

/******************************************************************************
      ����˵������ʾ�����ַ�
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ���ַ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChar(uint16_t x, uint16_t y, unsigned char num, uint16_t fc, uint16_t bc, unsigned char sizey, unsigned char mode)
{
    unsigned char temp, sizex, t, m = 0;
    uint16_t i, TypefaceNum; // һ���ַ���ռ�ֽڴ�С
    uint16_t x0 = x;
    sizex       = sizey / 2;
    TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
    num         = num - ' ';                             // �õ�ƫ�ƺ���?
    LCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1); // ���ù��λ��?
    for (i = 0; i < TypefaceNum; i++) {
        if (sizey == 12)
            temp = ascii_1206[num][i]; // ����6x12����
        else if (sizey == 16)
            temp = ascii_1608[num][i]; // ����8x16����
        else if (sizey == 24)
            temp = ascii_2412[num][i]; // ����12x24����
        else if (sizey == 32)
            temp = ascii_3216[num][i]; // ����16x32����
        else
            return;
        for (t = 0; t < 8; t++) {
            if (!mode) // �ǵ���ģʽ
            {
                if (temp & (0x01 << t))
                    LCD_WR_DATA(fc);
                else
                    LCD_WR_DATA(bc);
                m++; // x�����ص������?
                if (m % sizex == 0) {
                    m = 0;
                    break;
                }
            } else // ����ģʽ
            {
                if (temp & (0x01 << t))
                    LCD_DrawPoint(x, y, fc); // ��һ����
                x++;
                if ((x - x0) == sizex) {
                    x = x0;
                    y++;
                    break;
                }
            }
        }
    }
}

/******************************************************************************
      ����˵������ʾ�ַ���
      ������ݣ�x,y��ʾ����
                *p Ҫ��ʾ���ַ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowString(uint16_t x, uint16_t y, const unsigned char *p, uint16_t fc, uint16_t bc, unsigned char sizey, unsigned char mode)
{
    while (*p != '\0') {
        LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
        x += sizey / 2;
        p++;
    }
}

/**
 * @brief ��ʾ��Ӣ���ַ���
 *
 * @param x ��ʾx����
 * @param y ��ʾy����
 * @param s Ҫ��ʾ�ĺ��ִ�
 * @param fc �ֵ���ɫ
 * @param bc �ֵı���ɫ
 * @param sizey �ֺ� ��ѡ 16 24 32
 * @param mode 0�ǵ���ģʽ  1����ģʽ
 */
void LCD_ShowChinese(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    while (*s != 0) {
        if (*s >= 128) {
            if (sizey == 12)
                LCD_ShowChinese12x12(x, y, s, fc, bc, sizey, mode);
            else if (sizey == 16)
                LCD_ShowChinese16x16(x, y, s, fc, bc, sizey, mode);
            else if (sizey == 24)
                LCD_ShowChinese24x24(x, y, s, fc, bc, sizey, mode);
            else if (sizey == 32)
                LCD_ShowChinese32x32(x, y, s, fc, bc, sizey, mode);
            else
                return;
            s += 2;
            x += sizey;
        } else {
            LCD_ShowChar(x, y, *s, fc, bc, sizey, mode);
            s += 1;
            x += (sizey / 2);
        }
    }
}

/**
 * @brief ��ʾ����12x12����
 *
 * @param x ���x����
 * @param y ���y����
 * @param s Ҫ��ʾ�ĺ���
 * @param fc �ֵ���ɫ
 * @param bc �ֵı���ɫ
 * @param sizey �ֺ�
 * @param mode 0�ǵ���ģʽ  1����ģʽ
 */
void LCD_ShowChinese12x12(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char i, j, m = 0;
    unsigned int k;
    unsigned int HZnum;       // ������Ŀ
    unsigned int TypefaceNum; // һ���ַ���ռ�ֽڴ�С
    unsigned int x0 = x;
    TypefaceNum     = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;

    HZnum = sizeof(tfont12) / sizeof(typFNT_GB12); // ͳ�ƺ�����Ŀ
    for (k = 0; k < HZnum; k++) {
        if ((tfont12[k].Index[0] == *(s)) && (tfont12[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // �ǵ��ӷ�ʽ
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j))
                            LCD_WR_DATA(fc);
                        else
                            LCD_WR_DATA(bc);
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else // ���ӷ�ʽ
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j))
                            LCD_DrawPoint(x, y, fc); // ��һ����
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // ���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��?
    }
}

/**
 * @brief ��ʾ����16x16����
 *
 * @param x ���x����
 * @param y ���y����
 * @param s Ҫ��ʾ�ĺ���
 * @param fc �ֵ���ɫ
 * @param bc �ֵı���ɫ
 * @param sizey �ֺ�
 * @param mode 0�ǵ���ģʽ  1����ģʽ
 */
void LCD_ShowChinese16x16(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char i, j, m = 0;
    unsigned int k;
    unsigned int HZnum;       // ������Ŀ
    unsigned int TypefaceNum; // һ���ַ���ռ�ֽڴ�С
    unsigned int x0 = x;
    TypefaceNum     = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey; // 32
    HZnum           = sizeof(tfont16) / sizeof(typFNT_GB16);       // ͳ�ƺ�����Ŀ
    for (k = 0; k < HZnum; k++) {
        if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // �ǵ��ӷ�ʽ
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j))
                            LCD_WR_DATA(fc);
                        else
                            LCD_WR_DATA(bc);
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else // ���ӷ�ʽ
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j))
                            LCD_DrawPoint(x, y, fc); // ��һ����
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // ���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��?
    }
}

/**
 * @brief ��ʾ����24x24����
 *
 * @param x ���x����
 * @param y ���y����
 * @param s Ҫ��ʾ�ĺ���
 * @param fc �ֵ���ɫ
 * @param bc �ֵı���ɫ
 * @param sizey �ֺ�
 * @param mode 0�ǵ���ģʽ  1����ģʽ
 */
void LCD_ShowChinese24x24(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char i, j, m = 0;
    unsigned int k;
    unsigned int HZnum;       // ������Ŀ
    unsigned int TypefaceNum; // һ���ַ���ռ�ֽڴ�С
    unsigned int x0 = x;
    TypefaceNum     = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum           = sizeof(tfont24) / sizeof(typFNT_GB24); // ͳ�ƺ�����Ŀ
    for (k = 0; k < HZnum; k++) {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // �ǵ��ӷ�ʽ
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j))
                            LCD_WR_DATA(fc);
                        else
                            LCD_WR_DATA(bc);
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else // ���ӷ�ʽ
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j))
                            LCD_DrawPoint(x, y, fc); // ��һ����
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // ���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��?
    }
}

/**
 * @brief ��ʾ����32x32����
 *
 * @param x ���x����
 * @param y ���y����
 * @param s Ҫ��ʾ�ĺ���
 * @param fc �ֵ���ɫ
 * @param bc �ֵı���ɫ
 * @param sizey �ֺ�
 * @param mode 0�ǵ���ģʽ  1����ģʽ
 */
void LCD_ShowChinese32x32(unsigned int x, unsigned int y, unsigned char *s, unsigned int fc, unsigned int bc, unsigned char sizey, unsigned char mode)
{
    unsigned char i, j, m = 0;
    unsigned int k;
    unsigned int HZnum;       // ������Ŀ
    unsigned int TypefaceNum; // һ���ַ���ռ�ֽڴ�С
    unsigned int x0 = x;
    TypefaceNum     = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum           = sizeof(tfont32) / sizeof(typFNT_GB32); // ͳ�ƺ�����Ŀ
    for (k = 0; k < HZnum; k++) {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1))) {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++) {
                for (j = 0; j < 8; j++) {
                    if (!mode) // �ǵ��ӷ�ʽ
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j))
                            LCD_WR_DATA(fc);
                        else
                            LCD_WR_DATA(bc);
                        m++;
                        if (m % sizey == 0) {
                            m = 0;
                            break;
                        }
                    } else // ���ӷ�ʽ
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j))
                            LCD_DrawPoint(x, y, fc); // ��һ����
                        x++;
                        if ((x - x0) == sizey) {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // ���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��?
    }
}

/******************************************************************************
      ����˵������ʾ����
      ������ݣ�m������nָ��
      ����ֵ��  ��
******************************************************************************/
uint32_t mypow(uint8_t m, uint8_t n)
{
    uint16_t result = 1;
    while (n--)
        result *= m;
    return result;
}

/******************************************************************************
      ����˵������ʾ��������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ��������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, unsigned char len, uint16_t fc, uint16_t bc, unsigned char sizey)
{
    unsigned char t, temp;
    unsigned char enshow = 0;
    unsigned char sizex  = sizey / 2;
    for (t = 0; t < len; t++) {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
                continue;
            } else
                enshow = 1;
        }
        LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

/******************************************************************************
      ����˵������ʾ��λС������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾС������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, unsigned char len, uint16_t fc, uint16_t bc, unsigned char sizey)
{
    unsigned char t, temp, sizex;
    uint16_t num1;
    sizex = sizey / 2;
    num1  = num * 100;
    for (t = 0; t < len; t++) {
        temp = (num1 / mypow(10, len - t - 1)) % 10;
        if (t == (len - 2)) {
            LCD_ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
            t++;
            len += 1;
        }
        LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

// 坝六进制坑逝uint8_t数组
void LCD_ShowUint8ArrayToHex(uint16_t x, uint16_t y, const uint8_t *arr, uint16_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
    char hexstr[6];
    for (uint16_t i = 0; i < len; i++) {
        snprintf(hexstr, 4, " %02X", arr[i]);
        LCD_ShowString(x + i * 3 * sizey / 2, y, (uint8_t *)hexstr, fc, bc, sizey, 0);
        // LCD_ShowChar()
    }
}

/**
 * @brief ��ʾͼƬ
 *
 * @param x ���x����
 * @param y ���y����
 * @param length ͼƬ����
 * @param width ͼƬ����
 * @param pic ͼƬ����
 */
void LCD_ShowPicture(unsigned int x, unsigned int y, unsigned int length, unsigned int width, const unsigned char pic[])
{
    unsigned int i, j;
    uint32_t k = 0;
    LCD_Address_Set(x, y, x + length - 1, y + width - 1);
    for (i = 0; i < length; i++) {
        for (j = 0; j < width; j++) {
            LCD_WR_DATA8(pic[k * 2]);
            LCD_WR_DATA8(pic[k * 2 + 1]);
            k++;
        }
    }
}
#endif // EXTENDEDCOMPONENTS

#endif // LVGL

HAL_StatusTypeDef mid_LcdTest(void)
{
    uint16_t x0     = (rand() % 320);
    uint16_t x1     = (rand() % 320);
    uint16_t y0     = (rand() % 240);
    uint16_t y1     = (rand() % 240);
    uint16_t color = rand();
    if (x0 > x1) {
        uint16_t temp = x0;
        x0           = x1;
        x1           = temp;
    }
    if (y0 > y1) {
        uint16_t temp = y0;
        y0           = y1;
        y1           = temp;
    }

    // HAL_UART_Transmit(&huart1, str, sizeof(str), HAL_MAX_DELAY); //
    // printf( "drawing rectangle at %d %d %d %d in color %04X\n", x0, y0, x1, y1, color);
    return mid_LcdFill(&x0, &y0, &x1, &y1, &color);
}

#endif