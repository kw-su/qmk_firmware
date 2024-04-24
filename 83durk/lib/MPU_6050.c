#include <stdint.h>
#include "MPU_6050.h"
#include "i2c_master.h"



#define MPU6050_ADDR (0x68 << 1)
#define MPU6050_AX (0x3B)
#define MPU6050_AY (0x3D)
#define MPU6050_AZ (0x3F)
#define MPU6050_TP (0x41)
#define MPU6050_GX (0x43)
#define MPU6050_GY (0x45)
#define MPU6050_GZ (0x47)


static void MPU6050_init(void);
static void MPU6050_read(void);

int16_t ax,ay,az;//加速度
int16_t tp;
int16_t gx,gy,gz;//ジャイロ
uint16_t timeout = 20;
uint8_t regaddr = 0x3B;
uint8_t battery = 0x6B;

static void MPU6050_init(void){
    uint8_t set_data = 0;
    i2c_init();
    i2c_writeReg(MPU6050_ADDR,battery,&set_data,1,timeout);
}

static void MPU6050_read(void){
    uint8_t data[14];
    i2c_init();
    i2c_readReg(MPU6050_ADDR,regaddr,data,sizeof(data),timeout);
    ax = (data[0] << 8) | data[1];
    ay = (data[2] << 8) | data[3];
    az = (data[4] << 8) | data[5];
    tp = (data[6] << 8) | data[7];
    gx = (data[8] << 8) | data[9];
    gy = (data[10] << 8) | data[11];
    gz = (data[11] << 8) | data[13];
    return;
}

void MPU6050_call_init(void){
    MPU6050_init();
    return;
}
void MPU6050_coal_read(void){
    MPU6050_read();
    return;
}

void MPU6050_data (int16_t *get_data){
    get_data[0] = ax;
    get_data[1] = ay;
    get_data[2] = az;
    get_data[3] = tp;
    get_data[4] = gx;
    get_data[5] = gy;
    get_data[6] = gz;
    return;
}