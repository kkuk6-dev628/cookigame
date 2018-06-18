#include "ScoreController.h"
#include "cocos2d.h"
#include "PoolController.h"
#include "ActionController.h"
#include "ui/UIText.h"


ScoreController::ScoreController()
{
}


ScoreController::~ScoreController()
{
}

ScoreController* ScoreController::instance = nullptr;

ScoreController* ScoreController::getInstance()
{
	if (instance == nullptr)
	{
		instance = new ScoreController();
	}
	return instance;
}

void ScoreController::showScoreAction(Score score)
{
	auto scoreShow = PoolController::getInstance()->getScoreShow();
	
	scoreShow->setString(StringUtils::toString(score.getScore()));
	scoreShow->setOpacity(255);
	scoreShow->setBMFontSize(60);

	scoreShow->setPosition(score.getPosition());
	
	if(scoresContainerNode != nullptr)
	{
		scoresContainerNode->addChild(scoreShow);
		CKAction action;
		action.node = scoreShow;
		action.action = ActionController::getInstance()->createScoreShowAction([=] {
			PoolController::getInstance()->recycleScoreShow(scoreShow);
		}, scoreShow);
		ActionController::getInstance()->pushAction(action, false);
	}
}

void ScoreController::processScoreQueue()
{
	char loopCount = 3;
	while(loopCount > 0)
	{
		if(scoreQueue.empty())
		{
			return;
		}
		auto score = scoreQueue.front();

		addTotalScore(score.getScore());

		showScoreAction(score);
		scoreQueue.pop_front();
		loopCount--;
	}
}

void ScoreController::addTotalScore(int scoreVal)
{
	totalScore += scoreVal;
	if(totalScoreNode == nullptr)
	{
		return;
	}
	auto scoreNode = static_cast<ui::Text*>(totalScoreNode->getChildByName("score"));
	scoreNode->setString(StringUtils::toString(totalScore));

	auto star1Node = totalScoreNode->getChildByName("top_star_background_1");
	auto star1Child = star1Node->getChildByName("top_start_1");
	auto star2Node = totalScoreNode->getChildByName("top_star_background_2");
	auto star2Child = star2Node->getChildByName("top_start_2");
	auto star3Node = totalScoreNode->getChildByName("top_star_background_3");
	auto star3Child = star3Node->getChildByName("top_start_3");

	auto progressBackgroundNode = totalScoreNode->getChildByName("statebelt_background");
	auto progressNode = progressBackgroundNode->getChildByName("statebelt");
	if(totalScore > star3Score)
	{
		progressNode->setContentSize(progressBackgroundNode->getContentSize());
	}
	else
	{
		auto progressWidth = progressBackgroundNode->getContentSize().width * totalScore / star3Score;
		progressNode->setContentSize(Size(progressWidth, 30));
	}

	if(totalScore > star3Score)
	{
		star1Child->setVisible(true);
		star2Child->setVisible(true);
		star3Child->setVisible(true);
	}
	else if (totalScore > star2Score)
	{
		star1Child->setVisible(true);
		star2Child->setVisible(true);
		star3Child->setVisible(false);
	}
	else if (totalScore > star1Score)
	{
		star1Child->setVisible(true);
		star2Child->setVisible(false);
		star3Child->setVisible(false);
	}
	else
	{
		star1Child->setVisible(false);
		star2Child->setVisible(false);
		star3Child->setVisible(false);
	}
}

void ScoreController::setStarScores(int star1, int star2, int star3)
{
	star1Score = star1;
	star2Score = star2;
	star3Score = star3;
	totalScore = 0;
	addTotalScore(0);
}

char ScoreController::getStar()
{
	if (totalScore > star3Score)
	{
		return 3;
	}
	else if (totalScore > star2Score)
	{
		return 2;
	}
	else if (totalScore > star1Score)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int ScoreController::getTotalScore()
{
	processScoreQueue();
	return totalScore;
}
