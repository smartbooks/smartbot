
#define P0 (*((unsigned char *)0x80))
#define P1 (*((unsigned char *)0x90))
#define P2 (*((unsigned char *)0xa0))
#define P3 (*((unsigned char *)0xb0))


void main()
{
	unsigned char cnt = 0;

	while(1){

		if(cnt < 1){
			P1 = 0;
		}else{
			P1 = 0xff;
		}

		cnt++;


		if(cnt >= 10){
			cnt=0;
		}
	
	}

}
