#include <fstream>
#include <html/html_doc.hpp>
#include <iostream>
#include <sstream>
#include "curl/curl.h"

using namespace std;
using namespace html;

/**
 * curl返回数据的回调函数。可以直接返回每次的数据量，不处理数据。使用string接收和处理。
 * 一旦收到需要保存的数据，libcurl 就会立即调用此回调函数。对于大多数传输，此回调将被调用多次，每次调用都会传递另一块数据。
 * ptr 指向传递的数据，该数据的大小为 nmemb；size大小始终为 1。
 * 关于该函数的使用说明可以参考 CURLOPT_WRITEFUNCTION explained 和 getinmemory.c
 *
 * @param ptr 指向传递的数据的指针，保存所有的数据。
 * @param size 每次都是1，size * nmemb 表示返回的数据长度
 * @param nmemb 每次返回的内容的长度
 * @param stream 数据流，本次收到的数据
 * @return
 */
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    auto *str = dynamic_cast<std::string *>((std::string *)stream); // 强制转换
    str->append((char*) ptr, size * nmemb); // size * mem 表示接受数据的多少
//    std::cout << *str << std::endl;
    return size * nmemb;
}

int main()
{
    curl_global_init(CURL_GLOBAL_ALL);
//    auto curl = curl_easy_init();
    CURL* curl = curl_easy_init(); // 获得curl的句柄
    curl_easy_setopt(curl, CURLOPT_URL, "https://www.baidu.com");
    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
//    auto ipInfo = curl_easy_getinfo(curl, CURLINFO_LOCAL_IP, 0);
    auto result = curl_easy_perform(curl);
    curl_easy_cleanup(curl); // 清理，释放资源
    if (result == CURLcode::CURLE_OK) {
//        std::cout << response << std::endl;
        std::stringstream ss(response);
//        istringstream stream("<!doctype html><html><HEAD><title>Title<!--</title>--></head></title><body><h1 align=\"center\">Hello</H1><P>Hello <p>world!<br/></body></html>");
        html_doc doc;
        ss >> doc;
//        stream >> doc;
//        cout << doc << endl;

//        for (auto beg = doc.node()["body"].front()["div"].begin(); beg != doc.node()["body"].front()["div"].end(); ++beg) {
//            cout << beg->front() << std::endl;
//        }

        for (auto& p : doc.node()["body"].front()["script"])
        {
            cout << p.front().text() << std::endl;
        }
    }

}
