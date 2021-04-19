#include <sys/types.h>

unsigned short my_chksm(unsigned short *p, int num)
{
	register long sum;
	u_int16_t oddbyte;
	register u_int16_t ret;

	sum = 0;
	while (num > 1) {
		sum += *p++;
		num -= 2;
	}

	if (num == 1) {
		oddbyte = 0;
		*((u_int8_t *) & oddbyte) = *(u_int8_t *) p;
		sum += oddbyte;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	ret = ~sum;

	return (ret);
}
 kprabhakar5441@gmail. 

  com 