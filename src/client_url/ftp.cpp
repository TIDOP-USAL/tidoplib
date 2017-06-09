#include "ftp.h"

#include <curl/curl.h>

#include <stdio.h>

namespace I3D
{

struct FtpFile {
  const char *filename;
  FILE *stream;
};
 
static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct FtpFile *out=(struct FtpFile *)stream;
  if(out && !out->stream) {
    /* open file for writing */ 
    out->stream=fopen(out->filename, "wb");
    if(!out->stream)
      return -1; /* failure, can't open file to write */ 
  }
  return fwrite(buffer, size, nmemb, out->stream);
}


Ftp::Ftp() 
  : mUrl(""),
    mUsername(""),
    mPassword(""),
    mCURL(0),
    mHandler(0)
{
  curl_global_init(CURL_GLOBAL_ALL);
  mCURL = curl_easy_init(); //TODO: Aqui???
}

Ftp::~Ftp()
{
  curl_easy_cleanup(mCURL); //TODO: Aqui???
  curl_global_cleanup();
}

bool Ftp::connect(const char *url, const char *username, const char *password)
{
  mUrl = url;
  mUsername = username;
  mPassword = password;
}


void Ftp::download(const char *remoteFile, const char *localFile)
{
  CURLcode res;
  struct FtpFile ftpfile = {
    localFile, /* name to store the file as if successful */ 
    NULL
  };

  if (mCURL) {
    curl_easy_setopt(mCURL, CURLOPT_URL, remoteFile);

    /* Define our callback to get called when there's data to be written */ 
    curl_easy_setopt(mCURL, CURLOPT_WRITEFUNCTION, my_fwrite);
    /* Set a pointer to our struct to pass to the callback */ 
    curl_easy_setopt(mCURL, CURLOPT_WRITEDATA, &ftpfile);
 
    /* Switch on full protocol/debug output */ 
    curl_easy_setopt(mCURL, CURLOPT_VERBOSE, 1L);
 
    res = curl_easy_perform(mCURL);
 
    /* always cleanup */ 
    curl_easy_cleanup(mCURL);
 
    if(CURLE_OK != res) {
      /* we failed */ 
      fprintf(stderr, "curl told us %d\n", res);
    }
  }

  if(ftpfile.stream)
    fclose(ftpfile.stream); /* close the local file */ 
}

void Ftp::upload(const char *localFile, const char *remoteFile)
{
  mHandler = std::fopen(localFile, "rb");
  if(!mHandler) {
    return;
  }

  if (mCURL) {
    //headerlist = curl_slist_append(headerlist, buf_1);
    //headerlist = curl_slist_append(headerlist, buf_2);
  
    /* we want to use our own read function */ 
    //curl_easy_setopt(mCURL, CURLOPT_READFUNCTION, read_callback);
    /* enable uploading */ 
    curl_easy_setopt(mCURL, CURLOPT_UPLOAD, 1L);
    /* specify target */ 
    curl_easy_setopt(mCURL, CURLOPT_URL, mUrl.c_str());
    /* pass in that last of FTP commands to run after the transfer */ 
    curl_easy_setopt(mCURL, CURLOPT_POSTQUOTE, headerlist);
    /* now specify which file to upload */ 
    curl_easy_setopt(mCURL, CURLOPT_READDATA, mHandler);
    /* Set the size of the file to upload (optional).  If you give a *_LARGE
       option you MUST make sure that the type of the passed-in argument is a
       curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
       make sure that to pass in a type 'long' argument. */ 
    //curl_easy_setopt(mCURL, CURLOPT_INFILESIZE_LARGE,
    //                 (curl_off_t)fsize);

    CURLcode res;
    /* Now run off and do what you've been told! */ 
    res = curl_easy_perform(mCURL);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    /* clean up the FTP commands list */ 
    curl_slist_free_all(headerlist);
  }

  fclose(mHandler);
}


} // End namespace I3D