#include "ApiCall.hxx"

namespace bolt {
    ApiCall::ApiCall() {
        backend = curl_easy_init();
    }

    ApiCall::~ApiCall() {
        curl_easy_cleanup(backend);
        curl_slist_free_all(headers);
    }

    ApiCall& ApiCall::append(const std::string &head) {
        headers = curl_slist_append(headers, head.c_str());
        return *this;
    }

    nlohmann::json ApiCall::call(std::string const &url, int request, const nlohmann::json &query) {
        CURLcode ret;
        std::string response;

        curl_easy_setopt(backend, CURLOPT_URL, url.c_str());
        curl_easy_setopt(backend, CURLOPT_WRITEFUNCTION, [](void* buf, size_t size, size_t nmemb, void* userp) -> size_t {
            std::string* s = reinterpret_cast<std::string *>(userp);
            s->append((char *)buf, (char *)buf + size * nmemb);
            return size * nmemb;
        });
        curl_easy_setopt(backend, CURLOPT_WRITEDATA, (void*) &response);
        curl_easy_setopt(backend, CURLOPT_USERAGENT, "rlxos-bolt/1.0");
        curl_easy_setopt(backend, CURLOPT_TIMEOUT, 15);
        curl_easy_setopt(backend, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(backend, CURLOPT_MAXREDIRS, 10);
        curl_easy_setopt(backend, CURLOPT_CUSTOMREQUEST, request == POST ? "POST" : "GET");
        curl_easy_setopt(backend, CURLOPT_HTTPHEADER, headers);
        if (request == POST) {
            std::stringstream ss;
            ss << query;
            curl_easy_setopt(backend, CURLOPT_POSTFIELDS, ss.str().c_str());
        }

        ret = curl_easy_perform(backend);
        auto res = nlohmann::json();
        if (ret != CURLE_OK) {
            res["data"] = curl_easy_strerror(ret);
            return res;
        }
        std::stringstream ss(response);
        ss >> res;

        return res;
    }
} // bolt