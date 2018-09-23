#include <iostream>
#include <unistd.h>
#include <string.h>
#include "curl/curl.h"
using namespace std;

class Download {
	protected:
        char *local_url;
        char *download_url;
		char *file_name;
	public:
		Download(char* download_url, char* local_url) {
			this->download_url = download_url;
			this->local_url = local_url;

			string str_url;
			str_url = download_url;
			int pos = str_url.find_last_of('/');  
			string s(str_url.substr(pos + 1));
			s = "/" + s;
			// 获取并设置文件名
			this->file_name = (char*)s.c_str();
			// 将文件名添加到保存路径中
			strcat(local_url, this->file_name);
			this->local_url = local_url;
		}

		virtual void download() = 0; 
};
