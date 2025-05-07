#include "firmware_updater.h"
#include "stm32f4xx.h"



extern char firmware_buffer[MAX_FIRMWARE_SIZE];
extern uint32_t firmware_size;

#define EMPTY_MEM	0xFFFFFFFF

typedef void (*func_ptr)(void);

void jump_to_app(uint32_t addr_value)
{
	uint32_t app_start_address;
	func_ptr jump_to_app;

	/*Disable and re-enable AHB bus*/
	RCC->AHB1RSTR = 0xFFFFFFFF;
	RCC->AHB1RSTR = 0x00000000;

    // Reset SysTick
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

    if((*(uint32_t *)addr_value) != EMPTY_MEM)
    {
    	printf("Staring application.....\r\r");

    	app_start_address =  *(uint32_t *)(addr_value + 4);
    	jump_to_app  =   (func_ptr)app_start_address;

    	__set_MSP(*(uint32_t *)addr_value);


    	jump_to_app();

    }
}


/**
  * @brief Extracts the Content-Length value from the HTTP response buffer.
  *
  * @param recv_buffer The buffer containing the HTTP response.
  * @return int32_t The Content-Length value, or -1 if not found or invalid.
  */
 int32_t extract_content_length( char *recv_buffer)
 {
     const char *content_length_str = "Content-Length: ";
     const char *end_of_header_str = "\r\n";

     // Find the position of "Content-Length: " in the receive buffer
     const char *start = strstr(recv_buffer, content_length_str);
     if (start == NULL) {
         printf("Content-Length header not found\r\n");
         return -1;  // Content-Length not found
     }

     // Move the pointer to the start of the content length value
     start += strlen(content_length_str);

     // Find the end of the content length value (terminated by "\r\n")
     const char *end = strstr(start, end_of_header_str);
     if (end == NULL) {
         printf("End of Content-Length header not found\r\n");
         return -1;  // End of Content-Length not found
     }

     // Extract the substring that represents the Content-Length value
     char content_length_value[16];  // Assuming the Content-Length value is not too large
     size_t length = end - start;
     if (length >= sizeof(content_length_value)) {
         printf("Content-Length value too long\r\n");
         return -1;  // Length too long
     }

     // Copy the content length value to a temporary buffer
     strncpy(content_length_value, start, length);
     content_length_value[length] = '\0';  // Null-terminate the string

     // Convert the Content-Length value to an integer
     int32_t content_length = atoi(content_length_value);
     if (content_length <= 0) {
         printf("Invalid Content-Length value\r\n");
         return -1;  // Invalid Content-Length value
     }

     return content_length;
 }


 int get_firmware_binary(char *source_buffer, char *substring, char *dest_buffer, int bytes_to_copy)
 {
     // Find the location of the substring in the source buffer
     const char *substring_position = strstr(source_buffer, substring);

     // If the substring is not found, return an error code
     if (substring_position == NULL) {
         printf("Substring not found\r\n");
         return -1; // Substring not found
     }

     // Move the pointer past the substring to get to the data after it
     const char *data_start = substring_position + strlen(substring);

     size_t data_length = bytes_to_copy;

     printf("Data length to copy: %d\r\n", data_length);

     // Copy the data from data_start to the destination buffer
     memcpy(dest_buffer, data_start, data_length);



     return 0; // Success
 }

 void firmware_update(void)
 {
	  flash_write_data_byte(NEW_FIRMWARE_START_ADDRESS, (uint8_t *)firmware_buffer,firmware_size);
 }


