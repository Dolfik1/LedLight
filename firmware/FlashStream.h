#ifndef FlashStream_h
#define FlashStream_h
#include <pgmspace.h>
#include <Arduino.h>

class FlashStream
{
  public:
    FlashStream(const char *data);

    size_t size();
    size_t available();
    size_t read(void *buffer, size_t count);

  private:
    const char* _data;
    size_t _currentPosition;
    size_t _size;
};
#endif