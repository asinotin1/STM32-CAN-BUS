    #include "MPU6050.h"
    float AX, AY, AZ;
    float GX, GY, GZ;
    void MPU6050_Init(void)
    {
        uint8_t check;
        uint8_t data;
        HAL_I2C_Mem_Read(&hi2c1, 0xD0, WHO_AM_I_REG, I2C_MEMADD_SIZE_8BIT, &check, 1, 1000);
        if(check == 104) 
        {
            // đánh thức sensor bằng cách ghi 0 vào thanh ghi PWR_MGMT_1
            data = 0;
            HAL_I2C_Mem_Write(&hi2c1, 0xD0, PWR_MGMT_1_REG, 1, &data, 1, 1000);
            
            //cau hinh toc do lay mau = 1kHz, ghi 7 vao thanh ghi SMPLRT_DIV
            data = 0x07;
            HAL_I2C_Mem_Write(&hi2c1, 0xD0, SMPLRT_DIV_REG, 1, &data, 1, 1000);
        
        
            // cấu hình phạm vi lấy  con quay = 250 độ/giây, ghi 0 vào thanh ghi GYRO_CONFIG
            data = 0x00;
            HAL_I2C_Mem_Write(&hi2c1, 0xD0, GYRO_CONFIG_REG, 1, &data, 1, 1000);
            //cấu hình phạm vi lấy gia tốc = 2g, ghi 0 vào thanh ghi ACCEL_CONFIG 
            data = 0x00;
            HAL_I2C_Mem_Write(&hi2c1, 0xD0, ACCEL_CONFIG_REG, 1, &data, 1, 1000);
        }
    
        
    }
    void MPU6050_read_accel(void)
    {
        uint8_t Data[6];
        int16_t Accel_X_RAW = 0;
        int16_t Accel_Y_RAW = 0;
        int16_t Accel_Z_RAW = 0;

        HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, Data, 6, 1000);
        Accel_X_RAW = (int16_t)(Data[0] << 8 | Data[1]);
        Accel_Y_RAW = (int16_t)(Data[2] << 8 | Data[3]);
        Accel_Z_RAW = (int16_t)(Data[4] << 8 | Data[5]);
        AX = Accel_X_RAW/16384.0;
        AY = Accel_Y_RAW/16384.0;
        AZ = Accel_Z_RAW/16384.0;
    }
    void MPU6050_read_gyro(void)
    {
        uint8_t Data[6];
        int16_t Gyro_X_RAW = 0;
        int16_t Gyro_Y_RAW = 0;
        int16_t Gyro_Z_RAW = 0;

        HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, GYRO_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, Data, 6, 1000);
        Gyro_X_RAW = (int16_t)(Data[0] << 8 | Data[1]);
        Gyro_Y_RAW = (int16_t)(Data[2] << 8 | Data[3]);
        Gyro_Z_RAW = (int16_t)(Data[4] << 8 | Data[5]);
        GX = Gyro_X_RAW/131.0;
        GY = Gyro_Y_RAW/131.0;
        GZ = Gyro_Z_RAW/131.0;
    }
float MPU6050_vibrate()
{
    return sqrt(AX * AX + AY * AY + AZ * AZ);
}
