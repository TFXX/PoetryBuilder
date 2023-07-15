#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#define LENGTH 7


/**
	@brief 随机生成一个汉字
**/
uint16_t get_a_character()
{

	// 内码 = 区位码 + 0xA0
	uint16_t c = 0XA1B0;
	
	// 设置随机数种子
	static bool isInitRand = false;
	if(!isInitRand)
	{
		srand((unsigned)time(NULL));
		isInitRand = !isInitRand;
	}

	uint16_t byte1 = 0, byte2 = 0;
	do{
		// 第一个字节，WORD的低8位
		// 区位码[16,55] 对应 [0xB0,0xD7]
		int a = rand();
		byte1 = a % 0X28;
	
		// 第二个字节，WORD的高八位
		// 区位码[1, 94] 对应 [0xA1, 0xFE]
		a = rand();
		byte2 = a % 0X5E;
	}while(byte1 == 0x27 && byte2 > 0x58);
	c += byte1 + (byte2 << 8);
	return c;
}

bool make_poetry(char *poetry, size_t poetrySize)
{
	if(poetrySize > sizeof(uint16_t) * 56 + 1)
	{
		// 数组空间不足
		return false;
	}

	uint16_t *s = (uint16_t *)poetry;

	size_t characterNum = (poetrySize - 1) / sizeof(uint16_t);

	for(size_t i = 0; i < characterNum; i++)
	{
		s[i] = get_a_character();
	}

	return true;
}

void print_poetry(char *poetry, size_t poetrySize)
{
	size_t l = LENGTH * sizeof(uint16_t);
	int a = 0;
	for(size_t i = 0; i < poetrySize - sizeof(uint16_t); i += l)
	{
		char *pBegin = poetry + i;

		size_t endIndex = i + l;
		char temp = poetry[endIndex];
		poetry[endIndex] = '\0';
		printf("%s%s", pBegin, (a++ % 2)?"。\n":"，");
		poetry[endIndex] = temp;
	}
}

int main(int argc, char **argv)
{
	char poetry[LENGTH * sizeof(uint16_t) * 8 + 1] = { 0 };

	make_poetry(poetry, LENGTH * sizeof(uint16_t) * 8 + 1);
	
	print_poetry(poetry, LENGTH * sizeof(uint16_t) * 8 + 1);
	return 0;
}