#pragma once
#include "dxstdafx.h"
#include <vector>
#include "..\Constants.h"

struct Highscore {
	int level;
	int points;
	int seconds;
	int minutes;
};

class HighscoreService {

typedef std::vector<Highscore> Highscores;

public:
	HighscoreService() : m_TotalScores(0) {}
	~HighscoreService() {}
	void load();
	void save();
	bool get(int level,Highscore& highscore);
	void setScore(int level,int points,int seconds,int minutes);	
private:
	int m_TotalScores;
	Highscores m_Scores;
};

