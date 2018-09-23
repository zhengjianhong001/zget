// zget实现单个文件的下载，默认将文件存储在当前路径下，程序支持 HTTP、HTTPS、FTP 下载

g++ zget.cpp -o zget -lcurl

./zget http://img.1ppt.com/uploads/allimg/1605/4_160504095943_1.jpg

./zget https://curl.haxx.se/libcurl/c/example.html

./zget ftp://user1:passwd1@10.10.0.182/baculum/README   // 此 FTP 服务器已停用
