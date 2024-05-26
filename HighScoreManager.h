#ifndef HIGH_SCORE_MANAGER_H
#define HIGH_SCORE_MANAGER_H
#include<fstream>
#include<unordered_map>
#include<string>
#include<filesystem>
#include<stdio.h>

namespace fs=std::filesystem;

class HighScoreManager
{
private:
  std::unordered_map<std::string, int> data;

  void save();
  void load();

  inline bool does_hs_file_exist();
  inline void create_hs_file();

  std::string prepare_data();
  void read_hs_file();
  bool split(const std::string& line,std::string& name, std::string& score);
  std::string substr(const std::string& str, int a, int b);
  bool is_number(const std::string& str);
public:
  HighScoreManager();
  ~HighScoreManager();

  void add(const std::string& name, int score);
};


#endif
