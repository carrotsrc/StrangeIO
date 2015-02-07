#ifndef NORMALIZEOUTBUFFER_H
#define NORMALIZEOUTBUFFER_H

template<typename T>
class NormalizeOutBuffer {
private:

	T *buffer, *remainder, *whead, *rhead;

	int capacity;
	int normal;
	int threshold;
	int loadline;
	int dtsize;

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

	T *read();
	void write(T*,int);
};
#endif
