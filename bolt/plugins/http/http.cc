#include "http.hh"
using namespace bolt::plugins;

#include <curl/curl.h>

std::string Http::request(std::string url, std::string data) {
  std::string output;

  CURL* curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(
      curl, CURLOPT_WRITEFUNCTION,
      +[](void* ptr, size_t size, size_t nmemb, std::string* s) -> size_t {
        s->append((char*)ptr, size * nmemb);
        return size * nmemb;
      });
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)(&output));
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    return curl_easy_strerror(res);
  }
  curl_easy_cleanup(curl);
  return output;
}

std::string Http::respond(std::string intension, std::string mesg) {
  return request("https://rlxos.dev", "");
}