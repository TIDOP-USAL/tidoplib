#include "exception.h"

#if defined WIN32
#include <atlstr.h>
#endif

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

//void throw_exception(const char *error, const char *file, int line, const char *function)
//{
//  throw Exception(error, file, line, function);
//}

Exception make_exception(const char *error, const char *file, int line, const char *function)
{
  return Exception(error, file, line, function);
}

/* ---------------------------------------------------------------------------------- */


#ifdef WIN32
std::string formatWindowsErrorMsg(DWORD errorCode)
{
  DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM
    | FORMAT_MESSAGE_IGNORE_INSERTS
    | FORMAT_MESSAGE_MAX_WIDTH_MASK;
  
  TCHAR errorMessage[1024] = TEXT("");

  FormatMessage(flags,
                NULL,
                errorCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                errorMessage,
                sizeof(errorMessage)/sizeof(TCHAR),
                NULL);

  std::string strError = CW2A(errorMessage);
  return strError;
}
#endif


} // End namespace I3D
