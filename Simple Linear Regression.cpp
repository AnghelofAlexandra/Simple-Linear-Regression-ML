#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

bool readCSVdata(const string& fileName, vector<double>& X, vector<double>& Y)
{
	ifstream file(fileName);
	if (!file.is_open())
	{
		cout << "\nNO SUCH FILE!";
	}

	string line;

	while (getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}

		stringstream ss(line);
		string cell;

		if (getline(ss, cell, ','))
		{
			try
			{
				X.push_back(stod(cell));
			}
			catch (const invalid_argument& e)
			{
				continue;
			}
		}

		if (getline(ss, cell, ','))
		{
			try
			{
				Y.push_back(stod(cell));
			}
			catch (const invalid_argument& e)
			{
				if (!X.empty())
				{
					X.pop_back();
				}
				continue;
			}
		}
	}

	file.close();
	return 1;
}

class SimpleLinearRegression
{
	double intercept = 0;
	double slope = 0;
	double learningRate = 0.01;

public:

	double predict(double _x)
	{
		return  this->intercept + this->slope * _x;
	}

	// eroare patratica
	double computeCost(vector<double> _X, vector<double> _Y)
	{
		int n = _X.size();
		double cost = 0;

		for (int i = 0; i < n; i++)
		{
			cost += pow(predict(_X[i]) - _Y[i], 2);
		}

		return cost / (2 * n);
	}

	void gradientDescent(const vector<double>& _X, const vector<double>& _Y)
	{
		double gradientIntercept = 0;
		double gradientSlope = 0;

		for (int i = 0; i < _X.size(); i++)
		{
			gradientIntercept += predict(_X[i]) - _Y[i];
			gradientSlope += (predict(_X[i]) - _Y[i]) * _X[i];
		}

		gradientIntercept /= _X.size();
		gradientSlope /= _X.size();

		this->intercept -= this->learningRate*gradientIntercept;
		this->slope -=this->learningRate*gradientSlope;
	}

	void fit(vector<double> _X, vector<double> _Y)
	{
		double oldCost = computeCost(_X, _Y);
		double threshold = 0.000001;
		int numOfIteratons = 0;

		for (int i = 0; i < 50000; i++)
		{
			gradientDescent(_X, _Y);
			double curentCost = computeCost(_X, _Y);

			if (abs(curentCost - oldCost) < threshold)
			{
				numOfIteratons = i + 1;
				break;
			}

			oldCost = curentCost;
		}

		cout << "\n------------ Results After Training ------------";
		cout <<"\nFinal cost: " << computeCost(_X, _Y);
		cout << "\nSlope: " << this->slope;
		cout << "\nIntercept: " << this->intercept;
		cout << "\nNumber of Iterations: " << numOfIteratons;
	}
};

double minimum(const vector<double>& V)
{
	if (V.size() > 0)
	{
		double min = V[0];

		for (int i = 0; i < V.size(); i++)
		{
			if (V[i] < min)
			{
				min = V[i];
			}
		}

		return min;
	}
	else return 0;
}

double maximum(const vector<double>& V)
{
	if (V.size() > 0)
	{
		double max = V[0];

		for (int i = 0; i < V.size(); i++)
		{
			if (V[i] > max)
			{
				max = V[i];
			}
		}

		return max;
	}
	else return 0;
}

void scaleMinMax(vector<double>& V, double min, double max)
{
	if (V.size() > 0)
	{
		if (max - min > 0)
		{
			for (int i = 0; i < V.size(); i++)
			{
				V[i] = (V[i] - min) / (max - min);
			}
		}
	}
}

double rescale(double val, double min, double max)
{
	return val * (max - min) + min;
}

int main()
{
	vector<double> X;
	vector<double> Y;

	if (!readCSVdata("dataSimpleLinearRegression.csv", X, Y))
	{
		return 1;
	}

	double minX = minimum(X);
	double minY = minimum(Y);
	double maxX = maximum(X);
	double maxY = maximum(Y);

	scaleMinMax(X, minX, maxX);
	scaleMinMax(Y, minY, maxY);

	SimpleLinearRegression model;
	model.fit(X, Y);

	for (int i = 0; i < X.size(); i++)
	{
		cout << "\n For " << rescale(X[i], minX, maxX) << " the prediction is " << rescale(model.predict(X[i]), minY, maxY);
	}

	return 0;
}