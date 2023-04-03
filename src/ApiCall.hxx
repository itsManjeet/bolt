#ifndef BOLT_APICALL_HXX
#define BOLT_APICALL_HXX

#include <curl/curl.h>
#include <string>
#include "json.hpp"

namespace bolt {

    class ApiCall {
    private:
        CURL *backend{nullptr};
        curl_slist *headers = nullptr;

    public:
        enum {
            POST, GET
        };

        ApiCall();

        ApiCall& append(std::string const &head);

        nlohmann::json call(std::string const& url, int request = POST, nlohmann::json const& query = {});

        virtual ~ApiCall();
    };

} // bolt

#endif //BOLT_APICALL_HXX
