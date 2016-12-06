#pragma once

class Ticker
{
public:
	Ticker(int length);
	Ticker() = default;

	bool triggered() const;
	void forceTrigger();

	void tick();

	void reset();

	bool getState(int periodLength) const;

	void setState(bool state, int periodLength);

	void setLength(int length);
	int getTickCount();

private:
	int tickLength;
	int tickCount = 0;
	int periodCount = 0;
};