#include "HttpFtpDownload.cpp"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "curl/curl.h"
using namespace std;

int main(int argc, char **argv) {
    char local_url[80]; 
    char url[80]; 
    string str_url;
    char *ptr;
    struct hostent *hptr;  
    ptr = argv[1];
    strcpy(url, ptr);
    str_url = url;
    // 将当前路径作为下载文件保存位置
    getcwd(local_url, sizeof(local_url));

    // 使用多态
    Download *download_file;
    HttpFtpDownload http_ftp_download(url, local_url);
    download_file = &http_ftp_download;

    download_file->download();

    return 0;
}
