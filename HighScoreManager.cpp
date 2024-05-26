#include"HighScoreManager.h"


void HighScoreManager::save()
{
  std::ofstream ofs(".highscore",std::ios::trunc);
  ofs<<prepare_data();
  ofs.close();
}
void HighScoreManager::load()
{
  if(!does_hs_file_exist())
    {
      create_hs_file();
    }
  else
    {
      read_hs_file();
    }
}
bool HighScoreManager::does_hs_file_exist()
{
  return fs::exists(fs::path(".highscore"));
}
void HighScoreManager::create_hs_file()
{
  std::ofstream ofs(".highscore");
  ofs << ""; 
  ofs.close();
}
void HighScoreManager::read_hs_file()
{
  bool ok = true;
  std::ifstream ifs(".highscore");
  for(std::string line;getline(ifs,line);)
    {
      std::string name, score;
      ok = split(line,name,score);
      if(!ok)
	{
	  printf("something went wrong while loading .highscore file!\n");
	  data.clear();
	  return;
	}
      else
	{
	  data[name] = atoi(score.c_str());
	}
    }
}
bool HighScoreManager::split(const std::string& line,std::string& name, std::string& score)
{
  int delim_pos = line.find('#');
  if(delim_pos == std::string::npos) return false;
  name  = substr(line,0,delim_pos);
  score = substr(line,delim_pos+1,line.size());
  if(!is_number(score)) return false;

  return true;
}
bool HighScoreManager::is_number(const std::string& str)
{
  for(auto& ch:str) if(!isdigit(ch)) return false;
  return true;
}
std::string HighScoreManager::substr(const std::string& str, int a, int b)
{
  std::string result;
  for(int i = a;i<b;++i)
    {
      result+=str[i];
    }
  return result;
}
std::string HighScoreManager::prepare_data()
{
  std::string result;
  for(auto& el: data)
    {
      result+=el.first+"#"+std::to_string(el.second)+"\n";
    }
  return result;
}
HighScoreManager::HighScoreManager()
{
  load();
}
HighScoreManager::~HighScoreManager()
{
  save();
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
