#include <stdio.h>
#include <stdlib.h>

#include "defines.h"

int parse_cmd(const unsigned char *buf, char **resp, int *out_len){
    unsigned int *len = (unsigned int*)&buf[1]; //len excluding header & footer
    if(buf[0] == 0xAA && buf[4 + *len + 1] == 0x55){
        char cmd = buf[5], sub_cmd = buf[6];
        switch(cmd){//Command
        case GET_FILE:{
                printf("inside get file\n");    
                FILE *fp = fopen(CONFIG_FILE_PATH, "rb");
                if(fp == NULL){
                    return -1;
                }
                fseek(fp, 0, SEEK_END);
                *out_len = ftell(fp);
                fseek(fp, 0, SEEK_SET);
                char* rd_ptr = (char*)malloc(sizeof(char) * *out_len);
                fread(rd_ptr, sizeof(char), *out_len, fp);
                *resp = rd_ptr;
                fclose(fp);
            }
            break;
        case IMGSENSOR:
            switch(sub_cmd){
                case IMG_RES:
                    update_node("main", "img", "res", "320", 1, "test");
                    break;
                case MOTION_DET:
                    break;
                default:
                    break; 
            }
            break;
        case SIP:
            switch(sub_cmd){
                case USERNAME:
                    break;
                case PASSWORD:
                    break;
                default:
                    break;
            }
            break;
        case WIFI_ST:
            switch(sub_cmd){
                case SSID:
                    break;
                case PWD:
                    break;
                default:
                    break;
            }
            break;
        case WIFI_AP:
            switch(sub_cmd){
                case SSID:
                    break;
                case PWD:
                    break;
                default:
                    break;
            }
            break;
        case BLE:
            break;
        case AUDIO:
            break;
        case OTHR_SEN:
            switch(sub_cmd){
                case ALS:
                    break;
                case SDCARD:
                    break;
                case LED:
                    break;
                case GAS:
                    break;
                case PIR:
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
        }
    }else{
        return -1;
    }
    return 0;
}