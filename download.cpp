#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <fcntl.h>
using namespace std;

#define BUFSIZE 2048
#define PORT_N 80

class download {
public:
	download(){};
	virtual ~download(){};
	virtual bool download_file() = 0;
	int create_socket(); 
	int parse_url(string url, string* hostname, string* filepath, string* filename);
	int download::get_code(string head)

private:
	string filepath;
    string host;
    string filename;

   	string response;
	struct hostent *server;

	int client_socket;

	struct sockaddr_in server_address;

	ssize_t bytesrx = 1;
	ssize_t bytestx;

	// navratovy kod head requestu
	int return_code = 0;

	// cislo portu
	int port;

	// kontrolna premenna pre redirecty
	int redirects = 0;

	string final_filename;

};

int download::create_socket() {
	int client_socket;

	// Vytvorenie socketu
	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
        perror("ERROR in socket");
        exit(EXIT_FAILURE);
	}
	return client_socket;
}

int download::parse_url(string url, string* hostname, string* filepath, string* filename) {

	string url_filepath;
	string url_hostname;
	string server_hostname;

	int port = 80;

	string medzera = "%20";
	string vlnovka = "%7E";

	filename->clear();
	*filename = url;
	while(filename->find('/') != url.npos) {
		string tmp_string = filename->substr(filename->find('/') + 1);
		*filename = tmp_string.c_str();
	}


	// Nahradenie medzier %20 a ~ $7E
	for (unsigned i = 0; i < url.size(); i++) {

		if (url.at(i) == ' ') {

			url = url.insert(i, medzera.c_str());
			i+=3;
		}

	}

	string tmp_string = url;
	url = "";
	for (unsigned i = 0; i < tmp_string.size(); i++) {

		if ((tmp_string.at(i) != ' ')) {

			url += tmp_string.at(i);
		}
	}

	// Skontrolujem, ci ma url validny format
	if(!regex_match(url, regex("http:\\/\\/.*"))) {
		if(!regex_match(url, regex("https:\\/\\/.*"))) {
			perror("invalid url");
			exit(EXIT_FAILURE);
		}
	}
	url = url.substr(url.find("//")+2);


	if(regex_match(url, regex("www\\..*"))) {
		url_hostname.append("www.");
		url = url.substr(4);
	}


	if (url.at(strlen(url.c_str())-1) == '/')
		*filename = "index.html";

	if (url.find('/') != url.npos) {
		url_filepath = url.substr(url.find('/'));
		server_hostname = url.substr(0, url.find('/'));

	}
	else {

		filename->clear();
		url_filepath = "/";
		server_hostname = url;
		*filename = "index.html";
	}

	// Cislo portu v adrese
	if (server_hostname.find(':') != server_hostname.npos) {
		tmp_string = server_hostname.substr(server_hostname.find(':') + 1);
		server_hostname = server_hostname.erase(server_hostname.find(':'));
		port = atoi(tmp_string.c_str());
	}

	if ((port > 70000) || (port < 0)) {
		perror("invalid port");
		exit(EXIT_FAILURE);
	}

	url_hostname.append(server_hostname);

	*hostname = url_hostname;
	*filepath = url_filepath;

	return port;
}
int download::get_code(string head) {

	unsigned i =0;
	for(i = 0; i < head.size(); i++) {
		if ( head.at(i) == ' ') {
			break;
		}
	}
	string code = head.substr(i+1, 3);
	#ifdef DEBUG
		uts(code.c_str());
	#endif
	return atoi(code.c_str());
}

