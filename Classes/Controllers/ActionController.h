#pragma once
class ActionController
{
public:
	~ActionController();
	static ActionController* getInstance();
private:
	ActionController();
	static ActionController* instance;
};

