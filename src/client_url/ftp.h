#ifndef I3D_FTP_H
#define I3D_FTP_H

#include <string>

#include "core/defs.h"

namespace I3D
{

// Clase estatica para inicializar curl


class I3D_EXPORT Ftp
{
protected:

  std::string mUrl;
  std::string mUsername;
  std::string mPassword;

  CURL *mCURL;

  FILE *mHandler;

  struct curl_slist *headerlist;

public:

  Ftp();
  ~Ftp();

  /*!
   * \brief Conexión con la ftp
   */
  bool connect(const char *url, const char *username, const char *password);

  /*!
   * \brief Descarga un fichero de la ftp
   */
  void download(const char *remoteFile, const char *localFile);

  /*!
   * \brief Sube un fichero a la ftp
   */
  void upload(const char *localFile, const char *remoteFile);

private:

};

} // End namespace I3D

#endif // I3D_FTP_H
