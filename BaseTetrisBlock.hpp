#ifndef BASE_TETRIS_BLOCK_HPP
#define BASE_TETRIS_BLOCK_HPP
#include<vector>
#include"SFML/System/Vector2.hpp"
#include"Map.h"
#include"GetRandomNumber.h"
#include<functional>
#include<iostream>
#include<algorithm>
class TetrisBLock
{
protected:
	std::vector<sf::Vector2u> poses;
	virtual void update()
	{
		for (size_t i = 0; i < poses.size(); i++)
			poses[i].y += 1;
	}

	bool being_eaten = false;
	bool eatable = false;
	bool is_attached_to_snake = false;

	bool win()
	{
		for (auto& pos : poses)
			if (pos.y == 1) return true;
		return false;
	}
	bool reached_the_top = false;
private:
	State cur_state;
	State choose_type(const sf::Color& color)
	{
		if (color == sf::Color::Green)   return State::green_apple;
		else if (color == sf::Color::Yellow)  return State::yellow_apple;
		else if (color == sf::Color::Magenta) return State::magenta_apple;
		else
			return State::red_apple;
	}
public:
	TetrisBLock(const sf::Color& color_to_eat)
	{
		int rand_n = get_random_int(0, 6);
		eatable = true ? rand_n > 3: false;
		if (eatable)
		{
			cur_state = choose_type(color_to_eat);
		}
		else
		{
			//exlude color_to_eat
			vector<sf::Color> colors_to_move;
			std::copy_if(colors.begin(), colors.end(), back_inserter(colors_to_move),
				[&](sf::Color color) { return color != color_to_eat; });

			int rand_n = get_random_int(0, colors_to_move.size() - 1);
			cur_state = choose_type(colors_to_move[rand_n]);
		}

	}
	virtual ~TetrisBLock() {}


	virtual void clear(Map* map)
	{
		for (auto& pos : poses)
			map->set_element(pos.x, pos.y, GameState(State::none));
	}
	void  move(Map* map)
	{
		if (can_move(map))
		{
			clear(map);
			update();
			set(map);
		}
	}
	void move(Map* map, Direction dir)
	{
		clear(map);
		for (size_t i = 0; i < poses.size(); i++)
		{
			poses[i] = move_point(poses[i], dir);
		}
		set(map);
	}
	virtual void set(Map* map)
	{
		for (auto& pos : poses)
			map->set_element(pos.x, pos.y, GameState(cur_state));
	}
	virtual void generate() = 0;
	virtual bool can_move(Map* map) = 0;

	bool is_eaten(const sf::Vector2u& snake_head)
	{
		for (auto it = poses.begin(); it != poses.end(); ++it)
		{
			if ((*it) == snake_head)
			{
				being_eaten = true;
				it = poses.erase(it);
				return true;
			}
		}

		being_eaten = false;
		return false;
	}
	bool should_die() { return poses.empty(); }
	bool is_eatable() { return eatable; }
	State get_state() { return cur_state; }
	bool did_reach_the_top() { return reached_the_top; }

	bool does_intersect_snake(const sf::Vector2u& head_pos)
	{
		return std::find(poses.begin(), poses.end(), head_pos) != poses.end();
	}
	bool should_fall()
	{
		return !is_attached_to_snake;
	}
	void set_attaching_flag(bool flag)
	{
		is_attached_to_snake = flag;
	}

	bool is_going_out_of_map(Direction dir)
	{
		for (auto& pos : poses)
		{
			auto next = move_point(pos, dir);
			if (next.x > CELL_MAX - 1 or next.x < 0)return true;
			if (next.y > CELL_MAX - 1 or next.y < 0)return true;
		}
		return false;
	}

	bool can_move_left(Map* map)
	{
		vector<sf::Vector2u> _poses;
		std::copy(poses.begin(), poses.end(), back_inserter(_poses));
		sort(_poses.begin(), _poses.end(),
			[&](sf::Vector2u pos1, sf::Vector2u pos2)
		{
			return pos1.x < pos2.x;
		});


		//first sorted element is always the leftest
		// and only the second one can be the letest too
		//since all blocks have only 2 cells with the same x position
		//if you add another specific block this solution might not work correctly
		//in some cases
		vector<sf::Vector2u> leftest;
		leftest.push_back(_poses[0]);
		if (_poses[1].x == _poses[0].x)leftest.push_back(_poses[1]);

		for (size_t i = 0; i < leftest.size(); i++)
		{
			leftest[i] = move_point(leftest[i], Direction::Left);
			bool check = std::find(poses.begin(), poses.end(), leftest[i]) != poses.end();

			if (!map->is_empty(leftest[i].x, leftest[i].y) and !check)
				return false;
		}
		return true;
	}
	bool can_move_right(Map* map)
	{
		vector<sf::Vector2u> _poses;
		std::copy(poses.begin(), poses.end(), back_inserter(_poses));
		sort(_poses.begin(), _poses.end(),
			[&](sf::Vector2u pos1, sf::Vector2u pos2)
		{
			return pos1.x > pos2.x;
		});

		//same case like in previous function
		vector<sf::Vector2u> rightest;
		rightest.push_back(_poses[0]);
		if (_poses[1].x == _poses[0].x)rightest.push_back(_poses[1]);

		for (size_t i = 0; i < rightest.size(); i++)
		{
			rightest[i] = move_point(rightest[i], Direction::Right);

			bool check = std::find(poses.begin(), poses.end(), rightest[i]) != poses.end();
			if (!map->is_empty(rightest[i].x, rightest[i].y) and !check)
				return false;
		}
		return true;
	}
	bool can_move_with_dir(Map* map, Direction dir)
	{
		if (dir == Direction::Down) return can_move(map);
		else if (dir == Direction::Left)return can_move_left(map);
		else if (dir == Direction::Right) return can_move_right(map);
		else return false;
	}

	void erase(vector<sf::Vector2u>& poses_to_erase, Map* map)
	{
		for (auto it = poses_to_erase.begin(); it != poses_to_erase.end(); ++it)
		{
			auto pit = poses.begin();
			while (pit != poses.end())
			{
				if (*pit == *it)
				{
					map->set_element((*pit).x, (*pit).y, GameState(State::none));
					pit = poses.erase(pit);
				}
				else ++pit;
			}

		}
		set(map);
	}
};


#endif