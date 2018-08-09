/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: restInterface.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include "aREST.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/schema.h"
#include <curl/curl.h>
#include <functional>
#include <iostream>

using namespace std;
using namespace rapidjson;
using namespace RestItem;

/// @brief Generate a RestInterface object of the type specified by the given JSON.
unique_ptr<RestInterface> RestInterface::restInterfaceFactory (rapidjson::Value &d) {
    if (!d.Accept(Configuration::instance()->getInterfaceSchemaValidator())) {
        throw std::invalid_argument("Invalid interface JSON");
    }
    cerr << "Creating new interface" << endl;
    if (!(d.IsObject() && d.HasMember("interfaceType") && d["interfaceType"].IsString())) {
        throw std::invalid_argument("Invalid interface JSON");
    }
    std::string ifacetype(d["interfaceType"].GetString(), d["interfaceType"].GetStringLength());

    if (ifacetype == "network") {
        cerr << "Interface type is network" << endl;
        //auto n = new RestNetwork(d, ifacetype);
        return unique_ptr<RestInterface>(new RestNetwork(d, ifacetype));
    }
    return unique_ptr<RestInterface>(nullptr);
}

/// @brief Create RestNetwork object from the given configuration JSON
RestNetwork::RestNetwork (rapidjson::Value &d, std::string type) {
    interfaceType = type;
    cerr << "Creating new network interface" << endl;
    if (d.HasMember("url")) {
        url = d["url"].GetString();
    } else {
        throw std::invalid_argument("Invalid interface JSON");
    }
}

///@brief Make an aREST request to the configured target
unique_ptr<rapidjson::Document> RestNetwork::makeRequest(std::string request) {
    CURLcode ret;
    CURL *hnd;
    string response;
    char errbuf[CURL_ERROR_SIZE];

    request = url + request;
    cerr << "Making a request with URL " << request << endl;

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_URL, request.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_SSH_KNOWNHOSTS, "/home/debian/.ssh/known_hosts");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(hnd, CURLOPT_ERRORBUFFER, errbuf);
    curl_easy_setopt(hnd, CURLOPT_FAILONERROR, 1);			// trigger a failure on a 400-series return code.
    curl_easy_setopt(hnd, CURLOPT_TIMEOUT_MS, 250);			// set the timeout to 250 ms

    // set up data writer callback
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void *)&response);

    // make request
    ret = curl_easy_perform(hnd);

    // clean up request
    curl_easy_cleanup(hnd);
    hnd = NULL;

    // parse the incoming JSON
    auto *d = new Document();
    if (!d->Parse(response.c_str()).HasParseError()) {
        return unique_ptr<Document>(d);
    } else {
        return unique_ptr<Document>(nullptr);
    }

}

/// @ Data writer callback for libcurl. userp must be of type std::string, otherwise it's gonna be weird.
size_t RestNetwork::writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	string *target = (string *)userp;
	char *ptr = (char *)contents;

	//target->clear();
	size *= nmemb;
	target->reserve(size);
	for (size_t i = 0; i < size; i++) {
		*target += ptr[i];
	}
	return size;
}
