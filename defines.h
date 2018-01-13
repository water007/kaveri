
#ifndef __DEFINES_H
#define __DEFINES_H

#define CONFIG_FILE_PATH "./configs.xml"

#define STR_VALUE(arg)      #arg

//Command struct [0xAA len CMD SUBCMD WRITE/READ payload[Write] 0x55] (len excluding footer)
//Response [0xAA Single/Multi Code[Single]/Len[Multi] 0x55]
//Response [0xAA Payload[Multi] 0x55]
#define HEADER 0xAA
#define READ_SET   0x00
#define WRITE_SET !READ
#define FOOTER 0x55

enum commands{
    GET_FILE = 0x00,
    IMGSENSOR = 0x02,
    SIP,
    WIFI_ST,
    WIFI_AP,
    BLE,
    AUDIO,
    OTHR_SEN,
};

enum img_sub{
    IMG_RES,
    MOTION_DET,
};

enum wifi_sub{
    SSID,
    PWD,
    MODE,
};

enum ble_sub{
    FREQ,
};

enum audio_sub{
    LEVEL,
};

enum sen_sub{
    ALS,
    SDCARD,
    LED,
    GAS,
    PIR
};

enum misc_sub{
    BATTERY
};

enum sip_sub{
    USERNAME,
    PASSWORD,
};

struct response{
    unsigned char header;
    int len_code;
    unsigned char footer;
}__attribute__((packed));

#endif