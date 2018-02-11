/****************************************************************************
read.c
Author: Jiajie He <jiajiehe@stanford.edu>
****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

/*----------------------------- Module Defines ----------------------------*/
// EDID is accessible via the I²C-bus at address 0x50
#define I2C_ADDR 0x50
// EDID 1.4 is 128-byte long, whereas EDID 1.3 can be longer (possibly with additional descriptors)
#define edid_LENGTH 256

/*----------------------------- Module Functions ----------------------------*/
static bool read_i2c(int ADAPTER_NR, uint8_t * edid);
static void interpret_edid(uint8_t * edid);

/*----------------------------- Main Function ----------------------------*/
int main (int argc, char *argv[])
{
    if (argc != 2){
        printf("Please specify i2c bus\n");
        return 1;
    }

    int ADAPTER_NR = atoi(argv[1]);

    uint8_t edid[edid_LENGTH];
    
    if (read_i2c(ADAPTER_NR, edid)){
        interpret_edid(edid);
    }

    return 0;
}

/*----------------------------- Module Function Defines ----------------------------*/

/****************************************************************************
 Function
     read_i2c
 Parameters
     int ADAPTER_NR, I²C-bus where monitor is connected 
     uint8_t * edid, the empty array whose length is 256 and will be updated with 
                     EDID data via this function
 Returns
     bool true, if the I²C data is successfully accessed
     or bool false, if the I²C data is unsuccessfully accessed
 Description
     Store EDID data into a given array
 Notes
 Author
     Jiajie He, 2/16/18
****************************************************************************/

bool read_i2c(int ADAPTER_NR, uint8_t * edid){
    int file;
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", ADAPTER_NR);
    puts(filename);
    file = open(filename, O_RDWR);
    if (file < 0){
        printf("Error opening i2c bus %d\n", ADAPTER_NR);
        return false;
    }
    printf("Success opening i2c bus %d\n", ADAPTER_NR);

    if (ioctl(file, I2C_SLAVE, I2C_ADDR) < 0) {
        printf("Error accessing device of address %#x\n", I2C_ADDR);
        return false;
    }
    printf("Success accessing device of address %#x\n", I2C_ADDR);
    
    if (read(file, edid, edid_LENGTH) != edid_LENGTH){
        printf("Error reading data out of device of address %#x\n", I2C_ADDR);
        return false;
    }
    printf("Now printing data:\n");
    
    //Uncomment the following to print out the EDID data
    /*
    for (int i = 0; i < edid_LENGTH; i++){
        printf("0x%02X ", edid[i]);
    }
    */
    return true;
}



/****************************************************************************
 Function
     interpret_edid
 Parameters
     uint8_t * edid, the array containing 256-byte EDID information
 Returns
     nothing
 Description
     Based on the EDID information, extract Manufacturer ID, Week and Year,
     Serial Number, and EDID Version and Revision.
 Notes
 Author
     Jiajie He, 2/16/18
****************************************************************************/

void interpret_edid(uint8_t * edid){
    // Manufacturer ID corresponds to bytes 8-9
    printf("\nManufacturer ID: ");
    uint16_t manufacturer_id = edid[8]<<8 | edid[9];
    //printf("0x%04X ", manufacturer_id);
    // Bit 14-10 determines the 5 LSB of ASCII of the first letter
    char letter_1 = (char) ( 0x40 | ((manufacturer_id & 0x7C00) >> 10 ));
    printf("%c", letter_1);
    // Bit 9-5 determines the 5 LSB of ASCII of the second letter
    char letter_2 = (char) ( 0x40 | ((manufacturer_id & 0x3E0) >> 5 ));
    printf("%c", letter_2);
    // Bit 4-0 determines the 5 LSB of ASCII of the third letter
    char letter_3 = (char) ( 0x40 | (manufacturer_id & 0x1F));
    printf("%c", letter_3);

    // Week corresponds to byte 16
    printf("\nWeek: ");
    uint8_t week = edid[16];
    printf("%d", week);

    // Year corresponds to byte 17
    printf("\nYear: ");
    int year = 1990 + edid[17];
    printf("%d", year);

    // Serial Number corresponds to byte 12-15
    printf("\nSerial Number:");
    uint32_t serial_number = edid[12] << 24 | edid[13] << 16 | edid[14] << 8 | edid[15];
    printf("%u", serial_number);

    // Version corresponds to byte 18
    // Revision corresponds to byte 19
    printf("\nEDID Version and Revision:");
    uint8_t version = edid[18];
    uint8_t revision = edid[19];
    printf("%u.%u", version, revision);
}

