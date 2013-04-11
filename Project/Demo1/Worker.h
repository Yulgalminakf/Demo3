#pragma once
class Worker
{
public:
	Worker(void);
	~Worker(void);

	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
};