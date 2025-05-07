#ifndef _FIRMWARE_UPDATER_H
#define _FIRMWARE_UPDATER_H

#include <stdio.h>
#include <stdint.h>

#define MAX_FIRMWARE_SIZE		7000
#define NEW_FIRMWARE_START_ADDRESS		0x0800C000  //SECTOR 3

 int32_t extract_content_length( char *recv_buffer);

 int get_firmware_binary(char *source_buffer, char *substring, char *dest_buffer, int bytes_to_copy);
 void firmware_update(void);
 void jump_to_app(uint32_t addr_value);

#endif
