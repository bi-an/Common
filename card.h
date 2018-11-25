#define _CRT_SECURE_NO_DEPRECATE

#pragma once

using namespace std;

#define NAME_MAX_LENGTH 21

typedef enum
{
	//Pips of cards: ACE(A),TWO(2)~TEN(10),JACK(J),QUEEN(Q),KING(K)
	ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING
}RankType;

typedef enum
{
	//Suits of cards: CAUBS(Club),DIAMONDS(Diamond),HEARTS(Heart),SPADES(Spade)
	CLUBS, DIAMONDS, HEARTS, SPADES
}SuitType;

struct CCard
{
	RankType rank;
	SuitType suit;
};

struct Table {
	int flag;//0: Have no cache;	1: Have a cache.
	int dealPos;//The position in d
	CCard d[52];//The shuffled cards
	int curPlayer; //The turn of player now
	int nPlayer;//The number of players
	CCard Cards[6][21];
	int nCard[6];//The number of cards each player

	//char Name[6][NAME_MAX_LENGTH];
	//int ID[6];//ID of players 
	//string fileOfGame;
};