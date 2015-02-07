#ifndef NORMALIZEOUTBUFFER_H
#define NORMALIZEOUTBUFFER_H

template<typename T>
class NormalizeOutBuffer {
private:
	T *buffer;
	int capacity;
	int normal;
	int threshold;
	int loadline;

public:
	NormalizeOutBuffer(int);
	NormalizeOutBuffer(int, int);

	void setNormalOut(int);
	int getNormalOut();

	void setThreshold(int);
	int getThreshold();

	int getLoadline();
	int getCapacity();

	bool readReady();
	bool writeReady(int);
};
#endif
