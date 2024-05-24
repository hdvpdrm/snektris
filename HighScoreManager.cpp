#include"HighScoreManager.h"


void HighScoreManager::save()
{

}
void HighScoreManager::load()
{

}

HighScoreManager::HighScoreManager()
{
}
HighScoreManager::~HighScoreManager()
{
}


void HighScoreManager::add(const std::string& name, int score)
{
	if (data.find(name) == data.end())
	{
		data[name] = score;
	}
	else
	{
		if (data[name] < score)
		{
			data[name] = score;
		}
	}
}