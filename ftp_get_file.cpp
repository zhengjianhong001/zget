#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h> 
#include <curl/curl.h>
#include <stdio.h>
using namespace std;

struct FtpFile 
{
	const char *filename;
	FILE *stream;
};

static size_t FetchFiles(void *buffer, size_t size, size_t nmemb, void *stream)
{
	struct FtpFile *out = (struct FtpFile *)stream;
	if (out && !out->stream) 
	{
		// open file for writing 
		out->stream = fopen(out->filename, "wb");
		if (!out->stream)
			return -1; // failure, can't open file to write
	}
	return fwrite(buffer, size, nmemb, out->stream);
}

int DownloadFtpFile()
{
	CURL *curl;
	CURLcode res;
	struct FtpFile ftpfile = {
		"m.jpg", // name to store the file as if succesful//
		NULL
	};

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	if (curl) 
	{
		curl_easy_setopt(curl, CURLOPT_URL,"ftp://112.74.57.28/m.jpg");
		curl_easy_setopt(curl, CURLOPT_USERPWD, "root:201995zjh");
		// Define our callback to get called when there's data to be written //
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FetchFiles);
		// Set a pointer to our struct to pass to the callback //
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

		// Switch on full protocol/debug output //
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		res = curl_easy_perform(curl);

		// always cleanup 
		curl_easy_cleanup(curl);

		if (CURLE_OK != res) 
		{
			//we failed 
			fprintf(stderr, "curl told us %d\n", res);
		}
	}

	if (ftpfile.stream)
		fclose(ftpfile.stream); // close the local file 

	curl_global_cleanup();

	getchar();

	return 0;
}

int main(void)
{
	DownloadFtpFile();
	return 0;
}