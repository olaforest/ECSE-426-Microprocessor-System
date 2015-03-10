#ifndef _KEYPAD_
#define _KEYPAD_

void keypad_init(void);
uint8_t read_cols(void);
int get_key(char * key);
uint8_t get_keys(void);

#endif
