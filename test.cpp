#include <iostream>
#include <unistd.h>
#include "curl/curl.h"

using namespace std;

class download {
public:
   download(char *download_url, char *local_url);
   ~download();
   void download_file();
   static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);
   void print();
   struct FtpFile {
    const char *filename;
    FILE *stream;
  };

private:
  char *local_url;
  char *download_url;
  struct FtpFile ftpfile;

};

void download::print() {
  cout << download_url << endl;
  cout << local_url << endl;
}
download::download(char *download_url, char *local_url) {
  this->download_url = download_url;
  string path(download_url);
  int pos = path.find_last_of('/');
  string k(path.substr(pos) );
  char* s = strdup(k.c_str());
  //char* s = "/m.jpg";
  strcat(local_url, s);
  this->local_url = local_url;

}

download::~download() {
}

void download::download_file() {
    CURL *curl;
    CURLcode res;

    struct FtpFile ftpfile = {
         local_url, //定义下载到本地的文件位置和路径
         NULL
     };

     curl_global_init(CURL_GLOBAL_DEFAULT);
     curl = curl_easy_init();  //初始化一个curl指针
     if(curl) { //curl对象存在的情况下执行的操作

         //设置远端地址
         curl_easy_setopt(curl, CURLOPT_URL, download_url);

         //执行写入文件流操作

         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);//当有数据被写入，回调函数被调用，

         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile); //设置结构体的指针传递给回调函数

         //启用时会汇报所有的信息，存放在STDERR或指定的CURLOPT_STDERR中

         curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
         curl_easy_setopt(curl, CURLOPT_USERPWD, "SUREN:SUREN");

         //写入文件

         res = curl_easy_perform(curl);

         //释放curl对象

         curl_easy_cleanup(curl);

 

         if(res != CURLE_OK) {
              cout << stderr << res << endl;
         }

     }

     if(ftpfile.stream) {
         //关闭文件流
         fclose(ftpfile.stream);
     }
      //释放全局curl对象
     curl_global_cleanup();
}

int ParseSubStrByDelimiter(string strLine, char cDelimiter, string* strArray) {
    int nSeparator = -1;
    int nNextSeparator = -1;
    int nStrLen = strLine.length();
    int nStrArraySize = 0;
    int i = 0;
     
    //strLine replace '\t' to ' '
  int nIndex = strLine.find('\t', 0);
  while(nIndex >= 0)
  {
  strLine.replace(nIndex, 1, " ");
  nIndex = strLine.find('\t', 0);
  }
   
  int nCommaPos = strLine.find(',');
      int nSemicolonPos = strLine.find(';');
      if (nCommaPos < 0 && nSemicolonPos < 0) {
          cDelimiter = '/';
      }
    
      switch (cDelimiter)
      {
      case '/':
          for (i = 0; i < nStrLen;)
          {
              if (strLine.at(i) != cDelimiter)
              {
                  nSeparator = i;
                  nNextSeparator = strLine.find(cDelimiter, nSeparator + 1);
                  if (nNextSeparator < 0)
                  {
                      strArray[nStrArraySize++] = strLine.substr(nSeparator, nStrLen - 1);
                      break;
                  }
                  strArray[nStrArraySize++] = strLine.substr(nSeparator, nNextSeparator-nSeparator);
                   
                  i = nNextSeparator;
                  if (i >= nStrLen)
                  {
                      break;
                  }
              }
              else
              {
                  i++;
              }
          }
          break;
           
      case ',':
      case ';':
          for (i = 0; i < nStrLen;)
          {
              if (i == 0)
              {
                  if (strLine.at(i) == cDelimiter)
                  {
                      strArray[nStrArraySize++] = "";
                      nSeparator = i;
                  }
                  else
                  {
                      nSeparator = strLine.find(cDelimiter, nSeparator + 1);
                      if (nSeparator < 0)
                      {
                          break;
                      }
                      strArray[nStrArraySize++] = strLine.substr(0, nSeparator);
                  }
                   
                  i = nSeparator;
              }
              else
              {
                  nSeparator = i;
                  nNextSeparator = strLine.find(cDelimiter, nSeparator + 1);
                  if (nNextSeparator < 0)
                  {
                      strArray[nStrArraySize++] = strLine.substr(nSeparator + 1, nStrLen - nSeparator - 1);
                      break;
                  }
                   
                  if (nNextSeparator == nSeparator + 1)
                  {
                      strArray[nStrArraySize++] = "";
                  }
                  else
                  {
                      strArray[nStrArraySize++] = strLine.substr(nSeparator + 1, nNextSeparator - nSeparator - 1);
                  }
                   
                  i = nNextSeparator;
              }
          }
          break;
      default:
          break;
      }
    return nStrArraySize;
}

size_t download::my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream) {

  struct FtpFile *out=(struct FtpFile *)stream;

  if(out && !out->stream) {

    out->stream=fopen(out->filename, "wb");//打开文件进行写入

    if(!out->stream)

      return -1;

  }

  return fwrite(buffer, size, nmemb, out->stream);

}

int main(int argc, char **argv) {
  char local_url[80]; 
  char url[80]; 
  char *ptr;
  struct hostent *hptr;  
  string strArray[15];

  ptr = argv[1];
  strcpy(url, ptr);
  getcwd(local_url,sizeof(local_url)); 
  int Arraysize= ParseSubStrByDelimiter(url,'/',strArray);
  download d(url, local_url);
  d.download_file();
  d.print();

  return 0;
}

























