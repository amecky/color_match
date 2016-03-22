#include "SimpleHighscoreService.h"
#include <stdio.h>
#include <fstream>
#include "utils\Log.h"

void HighscoreService::setScore(int level,int points,int seconds,int minutes) {
	bool found = false;
	for ( size_t i = 0; i < m_Scores.size(); ++i ) {
		Highscore* s = &m_Scores[i];
		if ( s->level == level ) {
			found = true;
			if ( points > s->points ) {
				s->points = points;
				s->minutes = minutes;
				s->seconds = seconds;
				save();
			}
		}
	}
	if ( !found ) {
		Highscore hs;
		hs.points = points;
		hs.level = level;
		hs.minutes = minutes;
		hs.seconds = seconds;
		m_Scores.push_back(hs);
		save();
	}
}

bool HighscoreService::get(int level,Highscore& highscore) {	
	bool found = false;
	for ( size_t i = 0; i < m_Scores.size(); ++i ) {
		Highscore* s = &m_Scores[i];
		if ( s->level == level ) {
			found = true;
			highscore.points = s->points;
			highscore.level = s->level;
			highscore.minutes = s->minutes;
			highscore.seconds = s->seconds;
		}
	}
	return found;
}

void HighscoreService::load() {
	FILE* f = fopen("score.his","rb");
	if ( f ) {
		int total;
		fread(&total,sizeof(int),1,f);
		for ( int i = 0; i < total; ++i ) {
			Highscore hs;
			fread(&hs,sizeof(Highscore),1,f);
			m_Scores.push_back(hs);
		}
		fclose(f);
	}	
	else {
		LOGE << "score.his not found";
	}
}

void HighscoreService::save() {
	FILE* f;
	f = fopen("score.his","wb");
	if ( f ) {
		int total = (int)m_Scores.size();
		fwrite(&total,sizeof(int),1,f);
		for ( size_t i = 0; i < m_Scores.size();++i ) {
			Highscore* hs = &m_Scores[i];
			fwrite(hs,sizeof(Highscore),1,f);
		}
	}
	fclose(f);
}

