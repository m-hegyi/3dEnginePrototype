#ifndef _ABSTRACT_STATE_H_
#define _ABSTRACT_STATE_H_

#include <vector>
#include "AbstractScreen.h"

#pragma once
class AbstractState
{
public:

	virtual bool Update(DX::StepTimer const& timer) = 0;
	virtual bool Render() = 0;

	/*virtual void LoadScreen(int screenIndex = -1) = 0;
	virtual void UnLoadScreen(int screenIndex = -1) = 0;

	virtual void ChangeScreen(int screenIndex) = 0;

	virtual void Start() = 0;

	int GetActiveScreenIndex() const { return m_activeScreenIndex; }*/

protected:
	int m_activeScreenIndex;

	std::vector<std::shared_ptr<AbstractScreen>> m_screenList;
};

#endif // !_ABSTRACT_STATE_H_
