#pragma once
#include <string>
#include "json/document.h"
#include "General/TypeDefs.h"
using namespace rapidjson;

class Level
{
public:
	Level();
	~Level();

	void load(int levelNumber);
	void initWithJson(rapidjson::Value& json);
	static char GetEpisodeNumber(const int levelNumber);
	static std::string GetEpisodeFileName(char episodeNumber);

	rapidjson::Value* BoardsJson;

	int getMoveCount() const { return limit->front().Value; }

private:
	int levelNumber;
	char episode;
	cocos2d::__Array* boards;
	std::string title;
	bool isHardLevel;
	std::list<std::string>* pregameBoosters;
	char pregameBoostersAtMove;
	char game;
	std::list<std::string>* boosters;
	std::list<Limit>* limit;
	std::string background;
	std::vector<int>* scores = nullptr;
	bool ignoreEveryoneHardDelta;


public:
	int getLevelNumber() const
	{
		return levelNumber;
	}

	void setLevelNumber(int level_number)
	{
		levelNumber = level_number;
	}

	char getEpisode() const
	{
		return episode;
	}

	void setEpisode(char episode)
	{
		this->episode = episode;
	}

	cocos2d::__Array* getBoards() const
	{
		return boards;
	}

	void setBoards(cocos2d::__Array* boards)
	{
		this->boards = boards;
	}

	std::string getTitle() const
	{
		return title;
	}

	void setTitle(const std::string& title)
	{
		this->title = title;
	}

	bool getIsHardLevel() const
	{
		return isHardLevel;
	}

	void setIsHardLevel(bool is_hard_level)
	{
		isHardLevel = is_hard_level;
	}

	std::list<std::string>* getPregameBoosters() const
	{
		return pregameBoosters;
	}

	void setPregameBoosters(std::list<std::string>* basic_strings)
	{
		pregameBoosters = basic_strings;
	}

	char getPregameBoostersAtMove() const
	{
		return pregameBoostersAtMove;
	}

	void setPregameBoostersAtMove(char pregame_boosters_at_move)
	{
		pregameBoostersAtMove = pregame_boosters_at_move;
	}

	char getGame() const
	{
		return game;
	}

	void setGame(char game)
	{
		this->game = game;
	}

	std::list<std::string>* getBoosters() const
	{
		return boosters;
	}

	void setBoosters(std::list<std::string>* basic_strings)
	{
		boosters = basic_strings;
	}

	std::list<Limit>* getLimit() const
	{
		return limit;
	}

	void setLimit(std::list<Limit>* limits)
	{
		limit = limits;
	}

	std::string getBackground() const
	{
		return background;
	}

	void setBackground(const std::string& background)
	{
		this->background = background;
	}

	std::vector<int>* getScores() const
	{
		return scores;
	}

	void setScores(std::vector<int>* scores)
	{
		this->scores = scores;
	}

	bool getIgnoreEveryoneHardDelta() const
	{
		return ignoreEveryoneHardDelta;
	}

	void setIgnoreEveryoneHardDelta(bool ignore_everyone_hard_delta)
	{
		ignoreEveryoneHardDelta = ignore_everyone_hard_delta;
	}
};

