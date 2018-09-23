#include "Download.cpp"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "curl/curl.h"
using namespace std;

class HttpFtpDownload : public Download {
	public:
        HttpFtpDownload(char *download_url, char *local_url) : Download(download_url, local_url) {
        }

        void download() {
            // cout << local_url << endl << download_url << endl << file_name << endl;
                CURL *curl;
                CURLcode res;
                struct FtpFile ftpfile = {
                     get_save_location(), //定义下载到本地的文件位置和路径
                     NULL
                 };

                 curl_global_init(CURL_GLOBAL_DEFAULT);
                 curl = curl_easy_init();  //初始化一个curl指针
                 if(curl) { 
                     //设置远端地址
                     curl_easy_setopt(curl, CURLOPT_URL, get_download_url());
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
                          cout << "Download Error!" << endl;
                     }

                 }

                 if(ftpfile.stream) {
                     //关闭文件流
                     fclose(ftpfile.stream);
                 }
                  //释放全局curl对象
                 curl_global_cleanup();
        };

        static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);
        struct FtpFile {
            const char *filename;
            FILE *stream;
        };
        char* get_save_location() {
			return this->local_url;
		}
         char* get_download_url() {
            return this->download_url;
		}

    private:
        char *file_name;
        struct FtpFile ftpfile;
};

size_t HttpFtpDownload::my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream) {
    struct FtpFile *out=(struct FtpFile *)stream;
    if(out && !out->stream) {
    out->stream=fopen(out->filename, "wb");//打开文件进行写入
    if(!out->stream)
        return -1;
    }
    return fwrite(buffer, size, nmemb, out->stream);
}
