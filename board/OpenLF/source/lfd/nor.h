#ifndef __LFD_NOR_H__

#include <stdlib.h>

enum nor_fields {
	NOR_FIELD_BACKLIGHT,
};

int nor_open(void);
int nor_get_field(enum nor_fields field, void *dest, size_t size);

#endif /* __LFD_NOR_H__ */
