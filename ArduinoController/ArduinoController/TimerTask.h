#pragma once

class TimerTask
{
private:
	unsigned long currentTime;
	unsigned long intervalMillis;
	void(*task)();
public:
	TimerTask(unsigned long intervalMillis = 0, void(*task)() = nullptr);
	void reset();
	void run();
	void setTask(void(*task)());
	void setIntervalMillis(unsigned long intervalMillis);
};

TimerTask::TimerTask(unsigned long intervalMillis = 0, void(*task)() = nullptr)
{
	setIntervalMillis(intervalMillis);
	setTask(task);
	reset();
}

void TimerTask::reset()
{
	currentTime = millis();
}

void TimerTask::run()
{
	if (millis() - currentTime > intervalMillis)
	{
		if (task != nullptr)
			task();
		reset();
	}
}

void TimerTask::setTask(void(*task)())
{
	this->task = task;
}

void TimerTask::setIntervalMillis(unsigned long intervalMillis)
{
	this->intervalMillis = intervalMillis;
}