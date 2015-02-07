#ifndef NORMALIZEOUTBUFFER_H
#define NORMALIZEOUTBUFFER_H

template<typename T>
class NormalizeOutBuffer {
private
	T *buffer;
	int normal;
	int threshold;
	int waterline;
public:
	NormalizeOutBuffer(int);
	NormalizeOutBuffer(int, int);

	void setNormalOut(int);
	int getNormalOut();

	void setThreshold(int);
	int getThreshold();

	void getWaterline();

	bool readyWrite();
	bool readyRead();
}
#endif
