#ifndef HIGH_SCORE_MANAGER_H
#define HIGH_SCORE_MANAGER_H
#include<fstream>
#include<unordered_map>
#include<string>

class HighScoreManager
{
private:
	std::unordered_map<std::string, int> data;

	void save();
	void load();
public:
	HighScoreManager();
	~HighScoreManager();

	void add(const std::string& name, int score);
};


#endif