#ifndef _MODEL_LIST_H_
#define _MODEL_LIST_H_

#include <vector>
#include "Model.h";

#pragma once
class ModelList
{
public:
	ModelList();
	~ModelList();

	void Initialize();

	bool Render();
	bool Update();

private:
	std::vector<Model>	m_Models;
};

#endif