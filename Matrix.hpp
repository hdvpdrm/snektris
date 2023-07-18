#ifndef MATRIX_H
#define MATRIX_H
#include<variant>

using namespace std;
template<class T>
class Matrix
{
private:
	T** matrix;
	size_t x, y;
public:
	Matrix(size_t x, size_t y)
	{
		this->x = x;
		this->y = y;

		//create matrix
		matrix = new T* [y];
		for (size_t i = 0; i < y; ++i) 
		{
			matrix[i] = new T[x];
		}

		//init matrix with default values
		for (size_t y = 0; y < this->y; ++y)
			for (size_t x = 0; x < this->x; ++x)
				matrix[y][x] = T();
	}
	~Matrix()
	{
		for (size_t i = 0; i < y; ++i) 
		{
			delete[] matrix[i];
		}
		delete[] matrix;
	}

	std::variant<T, void*> get_elem(size_t x, size_t y)
	{
		bool out = x > this->x or y > this->y;
		if (out) return (void*)0;

		return matrix[y][x];
	}
	void set_element(const T& val, size_t x, size_t y)
	{
		if (x > this->x or y > this->y) return;

		matrix[y][x] = val;
	}
	std::variant<Matrix<T>*, void*> get_row(size_t y)
	{
		bool out = y > this->y;
		if (out) return (void*)0;

		Matrix<T>* row = new Matrix<T>(x, 1);
		for (int i = 0; i < x; i++)
		{
			row->set_element(get<0>(get_elem(i,y)), i, 0);
		}
		return row;
	}
	std::variant<Matrix<T>*, void*> get_col(size_t x)
	{
		bool out = x > this->x;
		if (out) return (void*)0;

		Matrix<T>* col = new Matrix<T>(1, y);
		for (int i = 0; i < y; i++)
		{
			col->set_element(get<0>(get_elem(x, i)), 0, i);
		}

		return col;
	}
	size_t get_width() { return x; }
	size_t get_height() { return y; }
};


#endif //MATRIX_H