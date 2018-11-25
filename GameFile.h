#pragma once
#include "game.h"
#include "File.h"
#include <fstream>
#include "card.h"
#include "Error.h"
//extern class Game;
//template<class Game>

class GameFile: public File<Game> {
public:
	GameFile(Game *pGame){
		table.flag = 0;
		table.dealPos = pGame->dealPos;
		//table.d = pGame->d;
		for (size_t i = 0; i < 52; i++) {
			table.d[i] = pGame->d[i];
		}
		table.curPlayer = pGame->curPlayer;
		table.nPlayer = pGame->nPlayer;
		for (size_t i = 0; i < 6;i++) {
			for (size_t j = 0; j < 21;j++) {
				table.Cards[i][j] = pGame->Cards[i][j];
			}
		}
		for (size_t i = 0; i < 6; i++) {
			table.nCard[i] = pGame->nCard[i];
		}
		for (size_t i = 0; i < 6; i++) {
			for (size_t j = 0; j < NAME_MAX_LENGTH; j++) {
				Name[i][j] = pGame->Name[i][j];
			}
		}
		fileOfGame = pGame->fileOfGame;
		fileOfPlayer = pGame->fileOfPlayer;
	}
	~GameFile(){}

	bool ReadCache(string fGame, string fPlayer) {
		fileOfGame = fGame;
		fileOfPlayer = fPlayer;
		ifstream infile1, infile2;
		infile1.open(fGame, std::ios::binary);
		infile2.open(fPlayer);

		if (!infile1 || !infile2) {
			infile1.close();
			infile2.close();
			return false;
		}
		//Table table;
		int len = sizeof(Table);
		int *buf = new int[len];
		infile1.read((char*)buf, len);
		infile1.close();

		if (buf[0] == 0) {
			return false;
		}

		table.dealPos = buf[1];
		int pos = 1;
		for (int i = 0; i < 52; i++) {
			pos++;
			table.d[i].suit = (SuitType)buf[pos];
			pos++;
			table.d[i].rank = (RankType)buf[pos];
		}
		++pos;
		table.curPlayer = buf[pos];
		++pos;
		table.nPlayer = buf[pos];
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 21; j++) {
				++pos;
				table.Cards[i][j].suit = (SuitType)buf[pos];
				++pos;
				table.Cards[i][j].rank = (RankType)buf[pos];
			}
		}
		for (int i = 0; i < 6; i++) {
			++pos;
			table.nCard[i] = buf[pos];
		}

		delete[] buf;
		buf = nullptr;

		string str;
		getline(infile2, str);
		if (str.empty() || str.size() > NAME_MAX_LENGTH) {
			infile2.close();
			return false;
		}
		int j = 0;
		for (; j < str.size(); j++) {
			Name[0][j] = str[j];
		}
		Name[0][j] = '\0';

		for (int i = 1; i <= table.nPlayer; i++) {
			getline(infile2, str);
			if (str.size() > NAME_MAX_LENGTH) {
				infile2.close();
				return false;
			}
			if (str.empty()) break;
			j = 0;
			for (; j < str.size(); j++) {
				Name[i][j] = str[j];
			}
			Name[i][j] = '\0';
		}
		infile2.close();
		return true;
	}
	void ClearCache() {
		ofstream out(fileOfGame, std::ios::binary);
		int *buf = new int;
		buf[0] = 0;
		out.write((char*)buf, sizeof(int));
		out.close();

		delete buf;
		buf = nullptr;

		out.open(fileOfPlayer);
		out.close();
		//out.open();
	}

private:

	Table table;
	char Name[6][NAME_MAX_LENGTH];
	string fileOfGame;
	string fileOfPlayer;
};
