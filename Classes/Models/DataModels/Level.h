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
	std::list<int>* scores;
	bool ignoreEveryoneHardDelta;

#pragma region Get Set Methods
public:
	int level_number() const
	{
		return levelNumber;
	}

	__declspec(property(get = level_number)) int LevelNumber;

	cocos2d::__Array* boards1() const
	{
		return boards;
	}

	__declspec(property(get = boards1)) cocos2d::__Array* Boards;

	char episode1() const
	{
		return episode;
	}

	__declspec(property(get = episode1)) char Episode;

	std::string title1() const
	{
		return title;
	}

	__declspec(property(get = title1)) std::string Title;

	bool is_hard_level() const
	{
		return isHardLevel;
	}

	__declspec(property(get = is_hard_level)) bool IsHardLevel;

	std::list<std::string>* pregame_boosters() const
	{
		return pregameBoosters;
	}

	__declspec(property(get = pregame_boosters)) std::list<std::string>* PregameBoosters;

	char pregame_boosters_at_move() const
	{
		return pregameBoostersAtMove;
	}

	__declspec(property(get = pregame_boosters_at_move)) char PregameBoostersAtMove;

	char game1() const
	{
		return game;
	}

	__declspec(property(get = game1)) char Game;

	std::list<std::string>* boosters1() const
	{
		return boosters;
	}

	__declspec(property(get = boosters1)) std::list<std::string>* Boosters;

	std::list<Limit>* limit1() const
	{
		return limit;
	}

	__declspec(property(get = limit1)) std::list<Limit>* Limit1;

	std::string background1() const
	{
		return background;
	}

	__declspec(property(get = background1)) std::string Background;

	std::list<int>* scores1() const
	{
		return scores;
	}

	__declspec(property(get = scores1)) std::list<int>* Scores;

	bool ignore_everyone_hard_delta() const
	{
		return ignoreEveryoneHardDelta;
	}

	__declspec(property(get = ignore_everyone_hard_delta)) bool IgnoreEveryoneHardDelta;
#pragma endregion

};

