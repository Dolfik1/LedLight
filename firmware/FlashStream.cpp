#include "FlashStream.h"

FlashStream::FlashStream(const char *data, const String& fileName)
{
  _data = data;
  _currentPosition = 0;
  _size = strlen_P(_data);
  _fileName = fileName;
}

size_t FlashStream::size()
{
  return _size;
}

size_t FlashStream::available()
{
  return _size - _currentPosition;
}

size_t FlashStream::read(void *buffer, size_t count)
{
  count = (count > _size - _currentPosition) ? _size - _currentPosition : count;
  memcpy_P(buffer, _data + _currentPosition, count);
  _currentPosition += count;
  return count;
}

String FlashStream::name()
{
  return _fileName;
}