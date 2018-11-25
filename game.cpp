#include "game.h"
#include <vector>
#include <memory.h>
#include <fstream>
#include <string>
#include <time.h>
//#include <conio.h>

#include "Error.h"
#include "card.h"

ostream &operator<<(ostream &o, const CCard &c)
{
	if (c.suit == CLUBS)
		o << "Club";
	//o << char(8710);
	else if (c.suit == DIAMONDS)
		o << "Diamond";
	//o << char(8710);
	else if (c.suit == HEARTS)
		o << "Heart";
	//o << char(8710);
	else if (c.suit == SPADES)
		o << "KING";

	if (c.rank == ACE)
		o << "A";
	else if (c.rank == JACK)
		o << "J";
	else if (c.rank == QUEEN)
		o << "Q";
	else if (c.rank == KING)
		o << "K";
	else cout << (int)c.rank;

	return o;
}


void Game::Initial() {
	int curPos = 0;
	dealPos = 0;
	int i = 0;

	for (int suitpos = 0; suitpos < 4; suitpos++)
	{
		for (int rankPos = 1; rankPos <= 13; rankPos++)
		{
			d[curPos].suit = (SuitType)suitpos;
			d[curPos].rank = (RankType)rankPos;
			curPos++;
		}
	}

	cout << "How many people will join in the game? <1~5>: ";
	//cin >> nPlayer;
	string str;
	cin >> str;
	nPlayer = atoi(str.c_str());
	while (nPlayer < 1 || nPlayer>5)
	{
		cout << "The capacity of players is limited to 1~5 people.\nPlease enter the number of players again <1~5>: ";
		//cin >> nPlayer;//FIXME: If input a wrong char, the program will loop forever!
		cin >> str;
		nPlayer = atoi(str.c_str());
	}

	for (i = 0; i <= nPlayer; i++)
		nCard[i] = 0;

	strcpy(Name[0], "Banker");
	for (i = 1; i <= nPlayer; i++)
	{
		cout << "The Name of Player " << i << ": ";
		cin >> Name[i];
	}

	cout << "\nThe game is starting..." << endl;
	cout << "You can press P anytime to hang up the game.\n\n" << endl;

	Shuffle();

	//Issue two cards for Banker
	for (size_t i = 1; i <= 2; i++)
	{
		Cards[0][nCard[0]] = DealOneCard();
		nCard[0]++;
	}
	// Display the first two cards of Banker
	DisplayStatus(0, true);

	for (size_t i = 1; i <= nPlayer; i++)
		for (size_t j = 0; j < 2; j++)
		{
			Cards[i][nCard[i]] = DealOneCard();
			nCard[i]++;
		}

	for (size_t i = 1; i <= nPlayer; i++)
		DisplayStatus(i);

	cout << endl;

}
void Game::Continue() {//Prepare for a new game
	cout << "Your game is not over yet. Do you want to continue<y, n>? ";
	char ch;
	cin >> ch;

	while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N')
	{
		cout << "you have an error input, please input again. <y, n>? ";
		cin >> ch;
	}

	if (ch == 'n' || ch == 'N') {
		Initial();
		return;
	}
	cout << endl << endl;

	DisplayStatus(0, true);
	for (int i = 1; i <= nPlayer; i++) {
		DisplayStatus(i);
	}

	//memset(Name, 0, sizeof(Name));

}
void Game::Play() {
	curPlayer = 1;
	//GameFile cacheFile(this);
	try {
		ReadCache("lastGame", "nameOfPlayer.txt");
		Continue();
	}
	catch (const Exception& exception) {
		Initial();
	}
	//if (ReadCache("lastGame","nameOfPlayer.txt")) {
	//	Continue();
	//}

	//else {
	//	Initial();
	//}

	for (size_t i = curPlayer; i <= nPlayer; i++)
	{
		char ch;
		cout << endl << Name[i] << ", do you want a card<y, n>? ";
		cin >> ch;

		while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N' && ch != 'p' && ch != 'P')
		{
			cout << Name[i] << ", you have an error input, please input again. <y, n>? ";
			cin >> ch;
		}

		//bool flag = false;
		while (GetScore(Cards[i], nCard[i]) <= 21 && (ch == 'y' || ch == 'Y' || ch == 'p' || ch == 'P'))
		{
			if (ch == 'p' || ch == 'P') {
				curPlayer = i;
				StoreGame();
				//flag = true;
				//system("pause");
				cout << "\nThe game has been hang up, press Enter to continue...";
				cin.get();//Accept the last Enter
				cin.get();
				//getchar();
				//getch();
				cout << "\n\nThe game is restarting...\nYou can input P anytime to hang up game.\n\n\n";
				cout << Name[i] << ", do you want one card again<y, n>? ";
				cin >> ch;
				continue;
			}
			//if (flag) {
			//	i=Initial();
			//}
			Cards[i][nCard[i]] = DealOneCard();
			nCard[i]++;
			DisplayStatus(i);

			if (GetScore(Cards[i], nCard[i]) > 21)
				break;
			cout << Name[i] << ", do you want one card again<y, n>? ";
			cin >> ch;
			while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N' && ch != 'p' && ch != 'P')
			{
				cout << Name[i] << ", you have an error input, please input again. <y, n>? ";
				cin >> ch;
			}
		}
	}

	cout << endl;
	DisplayStatus(0);
	//ShowResult(1);
	//int cnt = 1;
	while (GetScore(Cards[0], nCard[0]) <= 16)
	{
		//cnt++;
		Cards[0][nCard[0]] = DealOneCard();
		nCard[0]++;
		DisplayStatus(0);
		//ShowResult(cnt);
	}
	cout << endl;

	if (GetScore(Cards[0], nCard[0]) > 21)
	{
		for (size_t i = 1; i <= nPlayer; i++)
		{
			if (GetScore(Cards[i], nCard[i]) <= 21)
				cout << Name[i] << ", congratulations! You win! " << endl;
			else cout << Name[i] << ", you draw a tie. " << endl;
		}
	}
	else
	{
		for (size_t i = 1; i <= nPlayer; i++)
		{
			if (GetScore(Cards[i], nCard[i]) <= 21 && GetScore(Cards[i], nCard[i]) > GetScore(Cards[0], nCard[0]))
				cout << Name[i] << ", congratulations! You win! " << endl;
			else if (GetScore(Cards[i], nCard[i]) == GetScore(Cards[0], nCard[0]))
				cout << Name[i] << ", you draw a tie. " << endl;
			else
				cout << Name[i] << ", sorry, you lose!" << endl;
		}
	}
	ClearCache();
}

bool Game::ReadCache(string fGame, string fPlayer) {
	fileOfGame = fGame;
	fileOfPlayer = fPlayer;
	std::ifstream infile1(fGame, std::ios::binary), infile2(fPlayer);
	//infile1.open(fGame, std::ios::binary);
	//infile2.open(fPlayer);

	//if (!infile1 || !infile2) {
	//	infile1.close();
	//	infile2.close();

	//	throw Exception(ER_ERROR_NO_CACHE);
	//	return false;
	//}
	//Table table;
	if (infile1 && infile2) {
		int len = sizeof(Table);
		int *buf = new int[len];
		infile1.read((char*)buf, len);
		infile1.close();

		if (buf[0] == 0) {
			throw Exception(ER_ERROR_NO_CACHE);
			return false;
		}

		dealPos = buf[1];
		int pos = 1;
		for (int i = 0; i < 52; i++) {
			pos++;
			d[i].suit = (SuitType)buf[pos];
			pos++;
			d[i].rank = (RankType)buf[pos];
		}
		++pos;
		curPlayer = buf[pos];
		++pos;
		nPlayer = buf[pos];
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 21; j++) {
				++pos;
				Cards[i][j].suit = (SuitType)buf[pos];
				++pos;
				Cards[i][j].rank = (RankType)buf[pos];
			}
		}
		for (int i = 0; i < 6; i++) {
			++pos;
			nCard[i] = buf[pos];
		}

		delete[] buf;
		buf = nullptr;

		std::string str;
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

		for (int i = 1; i <= nPlayer; i++) {
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
	else {
		infile1.close();
		infile2.close();

		throw Exception(ER_ERROR_NO_CACHE);
		return false;

	}

}
void Game::StoreGame() {
	if (!StoreGame(fileOfGame, fileOfPlayer)) {
		StoreGame("lastGame", "nameOfPlayer.txt");
	}
}
bool Game::StoreGame(string fGame, string fPlayer) {//TODO
	ofstream out1(fGame, std::ios::binary), out2(fPlayer);
	if (!out1 || !out2) return false;

	int *buf = new int[sizeof(Table)];
	buf[0] = 1;
	buf[1] = dealPos;
	int pos = 1;
	for (int i = 0; i < 52; i++) {
		pos++;
		buf[pos] = d[i].suit;
		pos++;
		buf[pos] = d[i].rank;
	}
	++pos;
	buf[pos] = curPlayer;
	++pos;
	buf[pos] = nPlayer;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 21; j++) {
			++pos;
			buf[pos] = Cards[i][j].suit;
			++pos;
			buf[pos] = Cards[i][j].rank;
		}
	}
	for (int i = 0; i < 6; i++) {
		++pos;
		buf[pos] = nCard[i];
	}

	out1.write((char*)buf, sizeof(Table));
	out1.close();

	delete[] buf;
	buf = nullptr;

	for (int i = 0; i <= nPlayer; i++) {
		for (int j = 0; j < NAME_MAX_LENGTH; j++) {
			if (Name[i][j] == '\0')
				break;
			out2 << Name[i][j];
		}
		out2 << endl;
	}
	out2.close();
	return true;
}
void Game::ClearCache() {
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

void Game::Help() {
	cout << "/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*";
	cout << endl;
	cout << endl;

	puts("The origin of games:");
	puts("Blackjack, also known as twenty-one, ");
	puts("is a very interesting card game.");
	puts("It first appeared in the sixteenth century, ");
	puts("originated in France, French name: vingt-et-un. ");
	puts("Blackjack is a comparing card game between usually several players and a banker, ");
	puts("where each player in turn competes against the banker,");
	puts("but players do not play against each other.");
	puts("It is played with one or more decks of 52 cards,");
	puts("and is the most widely played casino banking game in the world.");
	puts("If players get Black Heart A and Black Heart J, they will be rewarded extra, ");
	puts("in English they are Blackjack.");
	cout << endl;
	cout << endl;

	puts("Rules of the game: ");
	puts("The objective of the game is to beat the banker in one of the following ways:");
	puts("In Blackjack game, 1-8 decks are generally used.");
	puts("The banker sends two clear cards to each player, and the cards face up.");
	puts("The banker gives himself two cards, one facing up (called a clear card), ");
	puts("and one facing down (called a dark card).");
	puts("The calculation of the number of poker points in your hands is:");
	puts("A, K, Q, J, and 10 cards count as 10 points.");
	puts("Players are free to decide how many cards to ask for, ");
	puts("The purpose is to try to go to 21 o'clock,");
	puts("the closer the better, the best is 21 o'clock of cource.");
	puts("In the process of asking for cards, ");
	puts(" if all the cards add up to 21 points, the player loses - called \"Bust\",");
	puts("then player loses.");
	puts("Generally no longer take cards at 17 or 17 points, ");
	puts("but it is also possible to take cards at 15 to 16 or even 12 to 13 points.");
	puts("Once all the players have completed their hands, it is the dealer��s turn. ");
	puts("The dealer hand will not be completed if all players have either busted ");
	puts("or received blackjacks. ");
	puts("Players win by not busting and having a total higher than the dealer, ");
	puts("or not busting and having the dealer bust, ");
	puts("or getting a blackjack without the dealer getting a blackjack.");
	puts("If the player and dealer have the same total (not counting blackjacks), ");
	puts("If the player and dealer have the same total (not counting blackjacks), ");
	puts("this is called a \"push\", ");
	puts("and the player typically does not win or lose money on that hand. ");
	puts("Otherwise, the dealer wins.");
	cout << "/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*";
	cout << endl;
	cout << endl;
}



void Game::Shuffle()
{
	srand(time(NULL));

	for (int i = 51; i > 0; i--)
	{
		int pos = rand() % (i + 1);
		CCard t;
		t = d[pos];
		d[pos] = d[i];
		d[i] = t;
	}
}

int Game::GetScore(CCard card[21], int n)
{
	int p;
	int score = 0;

	for (p = 0; p < n; p++)
	{
		if (card[p].rank == ACE)
			score += 1;
		else if (card[p].rank > TEN)
			score += 10;
		else
			score += (int)card[p].rank;
	}

	for (p = 0; p < n && score>21; p++)
		if (card[p].rank == ACE)
			score -= 10;

	return score;
}

void Game::DisplayStatus(int n, bool bHide)
{
	cout << Name[n] << ":";
	if (bHide)
		cout << " <Hide>";
	else
	{
		if (Cards[n][0].suit == CLUBS)
			cout << " [Club]";
		//cout << char(8710);
		else if (Cards[n][0].suit == DIAMONDS)
			cout << " [Diamond]";
		//cout << char(8710);
		else if (Cards[n][0].suit == HEARTS)
			cout << " [Heart]";
		//cout << char(8710);
		else if (Cards[n][0].suit == SPADES)
			cout << " [Spade]";
		//cout << char(8710);

		if (Cards[n][0].rank == ACE)
			cout << "A";
		else if (Cards[n][0].rank == JACK)
			cout << "J";
		else if (Cards[n][0].rank == QUEEN)
			cout << "Q";
		else if (Cards[n][0].rank == KING)
			cout << "K";
		else
			cout << (int)Cards[n][0].rank;
	}


	for (int i = 1; i < nCard[n]; i++)
	{
		if (Cards[n][i].suit == CLUBS)
			cout << " [Club]";
		else if (Cards[n][i].suit == DIAMONDS)
			cout << " [Diamond]";
		else if (Cards[n][i].suit == HEARTS)
			cout << " [Heart]";
		else if (Cards[n][i].suit == SPADES)
			cout << " [Spade]";

		if (Cards[n][i].rank == ACE)
			cout << "A";
		else if (Cards[n][i].rank == JACK)
			cout << "J";
		else if (Cards[n][i].rank == QUEEN)
			cout << "Q";
		else if (Cards[n][0].rank == KING)
			cout << "K";
		else
			cout << (int)Cards[n][i].rank;
	}

	if (!bHide)
		cout << "   Score(" << GetScore(Cards[n], nCard[n]) << ")";
	cout << endl;

	if (GetScore(Cards[n], nCard[n]) > 21)
		cout << Name[n] << " blows up!" << endl;
}

void Game::ShowResult(int n) {
	cout << Name[0] << " VS " << Name[n] << ":";
	//DisplayStatus(0);
	for (int i = 0; i < nCard[0]; i++)
	{
		if (Cards[0][i].suit == CLUBS)
			cout << " [Club]";
		else if (Cards[0][i].suit == DIAMONDS)
			cout << " [Diamond]";
		else if (Cards[0][i].suit == HEARTS)
			cout << " [Heart]";
		else if (Cards[0][i].suit == SPADES)
			cout << " [Spade]";

		if (Cards[0][i].rank == ACE)
			cout << "A";
		else if (Cards[0][i].rank == JACK)
			cout << "J";
		else if (Cards[0][i].rank == QUEEN)
			cout << "Q";
		else if (Cards[0][0].rank == KING)
			cout << "K";
		else
			cout << (int)Cards[0][i].rank;
	}
}

CCard Game::DealOneCard()
{
	return d[dealPos++];
}



struct Player {
	char Name[8][NAME_MAX_LENGTH];
	int id;//ID of players 
	CCard hand[21];

	Player(int id_) :id(id_) {
		memset(&hand, 0, sizeof(hand) * 21);
	}

	void StorePlayer(ofstream file) {// Store the player by binary format
		int *buf = new int[sizeof(int)];
		buf[0] = id;
		file.write((char*)buf, sizeof(int));
		delete[] buf;
		buf = NULL;

		buf = new int[21 * sizeof(CCard)];
		memset(buf, 0, 21 * sizeof(CCard));
		for (size_t i = 0; i < 21; i++) {

		}
		file.write((char*)buf, 21 * sizeof(CCard) * sizeof(CCard));
		delete[] buf;
		buf = NULL;
	}
};
//
//class Table {
//
//};

class Players {
public:
	Player& operator[](size_t n) {
		return m_players[n];
	}
private:
	vector<Player> m_players;
};