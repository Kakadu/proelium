#include <ctime>
#include <stdlib.h>
#include <cmath>

using namespace std;
// srand(time(0)); - Эту строчку нужно поместить в главную программу - инициализация генератора случайных чисел.

bool is_unit_alive (int type) {
	

// type = 1 орудие адн
// type = 2 танк

// Возвращает 1 если юнит выжил и 0 если погиб

	if (type == 1)
		if (rand()%5 == 2)
			return 0;
		else
			return 1;
	else if (type == 2)
		if (rand()%3 == 1)
			return 0;
		else
			return 1;
	else
		return 0;
} 
