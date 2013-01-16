/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <windows.h>
#include "FCGIRecord.h"

FCGIRecord::FCGIRecord()
  : good_(false),
    contentData_(0),
    plainTextBuf_(0)
{ }

FCGIRecord::~FCGIRecord()
{
  clear();
}

void FCGIRecord::clear()
{
  if (contentData_)
    delete []contentData_;
  if (plainTextBuf_)
    free(plainTextBuf_);

  contentData_ = 0;
  plainTextBufLength_ = 0;
  plainTextBuf_ = 0;
  plainTextLength_ = 0;
}

int FCGIRecord::getChar(int fd, bool waitForIt)
{
  unsigned char buf[1];

  int result;
  result = ::read(fd, buf, 1);
  if (result == -1) {
    perror("read");
    exit(1);
  } else
    if (result == 0) {
      if (waitForIt) {
	while (result == 0) {
	  //Sleep(100);
	  result = ::read(fd, buf, 1);
	  if (result == -1) {
	    perror("read");
	    exit(1);
	  }
	}
      } else
	return -1;
    }
     
  if (plainTextLength_ >= plainTextBufLength_) {
    plainTextBufLength_ += 100;
    plainTextBuf_ = (unsigned char *)
      realloc(plainTextBuf_, plainTextBufLength_);
  }

  plainTextBuf_[plainTextLength_++] = buf[0];

  return buf[0];
}

bool FCGIRecord::getAndAssign(int fd, unsigned char& result,
			      bool waitForIt)
{
  int c = getChar(fd, waitForIt);
  if (c != -1) {
    result = c;
    return true;
  } else {
    return false;
  }
}

bool FCGIRecord::getBuffer(int fd, unsigned char *buf,
			   int length)
{
  int count = 0;

  while (count < length) {
	  int result = ::read(fd, buf + count, length - count);
    if (result == -1) {
      perror("read");
      exit(1);
    } else {
      count += result;
    }
  }

  if (plainTextLength_ + length > plainTextBufLength_) {
    plainTextBufLength_ += length;
    plainTextBuf_ = (unsigned char *)
      realloc(plainTextBuf_, plainTextBufLength_);
  }

  memcpy(plainTextBuf_ + plainTextLength_, buf, length);
  plainTextLength_ += length;

  return true;
}

void FCGIRecord::read(int fd)
{
  clear();

  if (!getAndAssign(fd, version_, false))
    return;

  if (!getAndAssign(fd, type_, true))
    return;

  unsigned char IdB1;
  unsigned char IdB0;

  if (!getAndAssign(fd, IdB1, true))
    return;
  if (!getAndAssign(fd, IdB0, true))
    return;

  requestId_ = (IdB1 << 8) | IdB0;

  unsigned char contentLengthB1;
  unsigned char contentLengthB0;

  if (!getAndAssign(fd, contentLengthB1, true))
    return;
  if (!getAndAssign(fd, contentLengthB0, true))
    return;

  contentLength_ = (contentLengthB1 << 8) | contentLengthB0;

  if (!getAndAssign(fd, paddingLength_, true))
    return;

  if (!getAndAssign(fd, reserved_, true))
    return;

  contentData_ = new unsigned char[contentLength_];
  if (!getBuffer(fd, contentData_, contentLength_))
    return;

  unsigned char c;
  for (unsigned i = 0; i < paddingLength_; ++i)
    if (!getAndAssign(fd, c, true))
      return;

  good_ = true;
}

std::ostream& operator<< (std::ostream& o, const FCGIRecord& r)
{
  o << "version = " << (int)r.version()
    << " type = " << (int)r.type()
    << " requestId = " << (int)r.requestId()
    << " contentLength = " << r.contentLength();

  if (r.type() == 5) {
    o << " content = ";
    for (unsigned i = 0; i < r.contentLength(); ++i)
      o << r.contentData()[i];
  }

  return o;
}

void FCGIRecord::getParamPair(std::string& name,
			      std::string& value) const
{
  unsigned int nameLen;
  unsigned i = 0;

  if ((contentData_[i] >> 7) == 0) {
    nameLen = contentData_[i];
    i += 1;
  } else {
    nameLen =
      ((unsigned)(contentData_[i] & 0x7F) << 24)
      | ((unsigned)contentData_[i+1] << 16)
      | ((unsigned)contentData_[i+2] << 8)
      | ((unsigned)contentData_[i+3]);
    i += 4;
  }

  unsigned int valueLen;
  
  if ((contentData_[i] >> 7) == 0) {
    valueLen = contentData_[i];
    i += 1;
  } else {
    valueLen =
      (((unsigned)contentData_[i] & 0x7F) << 24)
      | ((unsigned)contentData_[i+1] << 16)
      | ((unsigned)contentData_[i+2] << 8)
      | ((unsigned)contentData_[i+3]);
    i += 4;
  }

  name = std::string((char *)contentData_ + i, nameLen);
  value = std::string((char *)contentData_ + i + nameLen, valueLen);
}
