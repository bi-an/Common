#define _CRT_SECURE_NO_DEPRECATE

#pragma once

#include <memory.h>
#include <fstream>
#include <string>
//#include <conio.h>

#include "Error.h"
#include "card.h"

ostream &operator<<(ostream &o, const CCard &c);


class Game
{
public:
	Game() {}
	virtual~Game() {}

	void Initial();
	void Continue();
	void Play();

	bool ReadCache(string fGame, string fPlayer);
	void StoreGame();
	bool StoreGame(string fGame, string fPlayer);
	void ClearCache();

	void Help();

	friend class GameFile;

private:
	void Shuffle();

	int GetScore(CCard card[21], int n);

	void DisplayStatus(int n, bool bHide = false);

	void ShowResult(int n);

	CCard DealOneCard();

private:
	//Data members
	CCard d[52];
	int dealPos;
	CCard Cards[6][21];
	int nCard[6];
	char Name[6][NAME_MAX_LENGTH];
	//int ID[6];//ID of players 
	int nPlayer;
	string fileOfGame;
	string fileOfPlayer;
	int curPlayer;
};


//struct Player {
//	char Name[8][NAME_MAX_LENGTH];
//	int id;//ID of players 
//	CCard hand[21];
//
//	Player(int id_):id(id_) {
//		memset(&hand,0,sizeof(hand)*21);
//	}
//
//	void StorePlayer(ofstream file) {// Store the player by binary format
//		int *buf = new int[sizeof(int)];
//		buf[0] = id;
//		file.write((char*)buf,sizeof(int));
//		delete[] buf;
//		buf = NULL;
//
//		buf = new int[21*sizeof(CCard)];
//		memset(buf,0,21*sizeof(CCard));
//		for (size_t i = 0; i < 21;i++) {
//
//		}
//		file.write((char*)buf, 21 * sizeof(CCard)*sizeof(CCard));
//		delete[] buf;
//		buf = NULL;
//	}
//};
//
//class Table {
//
//};