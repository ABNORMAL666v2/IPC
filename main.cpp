#include <iostream>
#include <string>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

// Callback function to receive HTTP response
size_t write_callback(void* buffer, size_t size, size_t nmemb, std::string* userdata) {
    size_t totalSize = size * nmemb;
    userdata->append(static_cast<char*>(buffer), totalSize);
    return totalSize;
}

void print_json(const rapidjson::Value& value, const std::string& prefix = "") {
    if (value.IsObject()) {
        for (auto it = value.MemberBegin(); it != value.MemberEnd(); ++it) {
            std::cout << prefix << it->name.GetString() << ": ";
            print_json(it->value, prefix + "    ");
        }
    }
    else if (value.IsString()) {
        std::cout << value.GetString() << std::endl;
    }
    else if (value.IsNumber()) {
        if (value.IsDouble())
            std::cout << value.GetDouble() << std::endl;
        else if (value.IsInt())
            std::cout << value.GetInt() << std::endl;
        else if (value.IsUint())
            std::cout << value.GetUint() << std::endl;
        else if (value.IsInt64())
            std::cout << value.GetInt64() << std::endl;
        else if (value.IsUint64())
            std::cout << value.GetUint64() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    const char* url = "https://checkip.amazonaws.com/";

    // Initialize libcurl for each thread
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize libcurl." << std::endl;
        return 1;
    }

    std::string response;

    // If there are no flags provided, get the IP only.
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return 1;
    }

    // Parse JSON response.
    rapidjson::Document document;
    document.Parse(response.c_str());

    if (document.HasParseError()) {
        std::cerr << "JSON parse error: " << rapidjson::GetParseError_En(document.GetParseError()) << std::endl;
        curl_easy_cleanup(curl);
        return 1;
    }

    std::cout << "Received data from IP-API:" << std::endl;
    print_json(document);

    // Cleanup libcurl for each thread
    curl_easy_cleanup(curl);

    return 0;
}
