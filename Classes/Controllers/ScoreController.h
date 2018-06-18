#pragma once
#include <list>
#include "Models/BoardModels/Score.h"
#include "2d/CCNode.h"


namespace cocos2d {
	class Vec2;
}

class ScoreController
{
public:
	ScoreController();
	~ScoreController();
	static ScoreController* getInstance();

	void addScore(Score& score) { scoreQueue.push_back(score); }

	void showScoreAction(Score score);

	void processScoreQueue();

	void setScoresContainerNode(cocos2d::Node* node) { scoresContainerNode = node; }
	void setTotalScoreNode(cocos2d::Node* node) { totalScoreNode = node; }
	void setStarScores(int star1, int star2, int star3);
	char getStar();
	int getTotalScore();

private:
	void addTotalScore(int scoreVal);

	int totalScore = 0;

	int star1Score = 0;
	int star2Score = 0;
	int star3Score = 0;

	cocos2d::Node* scoresContainerNode = nullptr;
	cocos2d::Node* totalScoreNode = nullptr;

	static ScoreController* instance;

	std::list<Score> scoreQueue;
};



