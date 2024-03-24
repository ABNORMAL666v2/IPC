#include <iostream>
#include <string>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

size_t write_callback(void* buffer, size_t size, size_t nmemb, std::string* userdata) {
	size_t totalSize = size * nmemb;
	userdata->append(static_cast<char*>(buffer), totalSize);
	return totalSize;
}

void print_pretty_json(const std::string& json) {
	rapidjson::Document d;
	d.Parse(json.c_str());
	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
	d.Accept(writer);
	std::cout << sb.GetString() << std::endl;
}

int main(int argc, char* argv[]) {
	const char* url = "https://if-config.co";
	const char* ip_api_url = "http://ip-api.com/json";
	std::string response;
	long http_code = 0;

	bool pretty_flag = false, json_flag = false;
	for (int i = 1; i < argc; ++i) {
		if (std::string(argv[i]) == "-pretty") pretty_flag = true;
		else if (std::string(argv[i]) == "-json") json_flag = true;
	}

	if (pretty_flag) {
		CURL* curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, ip_api_url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
		if (http_code == 200)
			print_pretty_json(response);
		else
			std::cerr << "HTTP Error: " << http_code << std::endl;
		curl_easy_cleanup(curl);
	}
	else if (json_flag) {
		CURL* curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, ip_api_url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
		if (http_code == 200)
			std::cout << response << std::endl;
		else
			std::cerr << "HTTP Error: " << http_code << std::endl;
		curl_easy_cleanup(curl);
	}
	else {
		CURL* curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
		if (http_code == 200)
			std::cout << "IP: " << response << std::endl;
		else
			std::cerr << "HTTP Error: " << http_code << std::endl;
		curl_easy_cleanup(curl);
	}

	return 0;
}
