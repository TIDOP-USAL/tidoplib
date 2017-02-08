#include "exception.h"

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

Exception::Exception(const char *error)
  : mError(error), mFile(""), mLine(-1), mFunction("")
{
  messagef();
}

Exception::Exception(const char *error, const char *file, int line, const char *function )
  : mError(error), mFile(file), mLine(line), mFunction(function)
{
  messagef();
}

const char *Exception::what() const throw ()
{
  return mMessage.c_str();
}

void Exception::messagef()
{
  char buf[1000];
  if (mLine == -1) {
    mMessage = mError;
  } else {
#if defined _MSC_VER
    sprintf_s(buf, 1000, "%s (%s:%u, %s)", mError.c_str(), mFile.c_str(), mLine, mFunction.c_str());
#else
    snprintf(buf, 1000, "%s (%s:%u, %s)", mError.c_str(), mFile.c_str(), mLine, mFunction.c_str());
#endif
    mMessage = std::string(buf);
  }
}

/* ---------------------------------------------------------------------------------- */

void throw_exception(const char *error, const char *file, int line, const char *function)
{
  throw Exception(error, file, line, function);
}

Exception error(const char *error, const char *file, int line, const char *function)
{
  return Exception(error, file, line, function);
}

/* ---------------------------------------------------------------------------------- */

} // End namespace I3D
