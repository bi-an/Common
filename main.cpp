#define _CRT_SECURE_NO_DEPRECATE
#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<string>
#include "game.h"

int main()
{
	char ch;
	ch = 'y';
	while (ch == 'y' || ch == 'Y')
	{
		Game game;
		game.Play();
		cout << "\n\nDo you want to play again<y, n>? ";
		cin >> ch;
	}

	return 0;
}