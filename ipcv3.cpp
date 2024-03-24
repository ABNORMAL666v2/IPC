#include <iostream>
#include <string>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

// http callback
size_t write_callback(void* buffer, size_t size, size_t nmemb, std::string* userdata) {
	size_t totalSize = size * nmemb;
	userdata->append(static_cast<char*>(buffer), totalSize);
	return totalSize;
}

int main(int argc, char* argv[]) {
	const char* aws_url = "https://checkip.amazonaws.com/";
	const char* ip_api_url = "http://ip-api.com/json/";
	std::string response;

	// int aws
	CURL* aws_curl = curl_easy_init();
	if (!aws_curl) {
		std::cerr << "Failed to initialize libcurl for AWS." << std::endl;
		return 1;
	}

	// Fetch IP address from AWS endpoint
	curl_easy_setopt(aws_curl, CURLOPT_URL, aws_url);
	curl_easy_setopt(aws_curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(aws_curl, CURLOPT_WRITEDATA, &response);

	CURLcode aws_res = curl_easy_perform(aws_curl);
	if (aws_res != CURLE_OK) {
		std::cerr << "curl_easy_perform() for AWS failed: " << curl_easy_strerror(aws_res) << std::endl;
		curl_easy_cleanup(aws_curl);
		return 1;
	}

	std::string aws_ip = response;

	curl_easy_cleanup(aws_curl);

	// Check for command-line flags
	bool geo_flag = false;
	bool json_flag = false;
	if (argc > 1) {
		for (int i = 1; i < argc; ++i) {
			std::string flag = argv[i];
			if (flag == "-geo") {
				geo_flag = true;
			}
			else if (flag == "-json") {
				json_flag = true;
			}
		}
	}

	if (geo_flag) {
		CURL* ip_api_curl = curl_easy_init();
		if (!ip_api_curl) {
			std::cerr << "Failed to initialize libcurl for IP-API." << std::endl;
			return 1;
		}

		curl_easy_setopt(ip_api_curl, CURLOPT_URL, ip_api_url);
		response.clear();
		curl_easy_setopt(ip_api_curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(ip_api_curl, CURLOPT_WRITEDATA, &response);

		CURLcode ip_api_res = curl_easy_perform(ip_api_curl);
		if (ip_api_res != CURLE_OK) {
			std::cerr << "curl_easy_perform() for IP-API failed: " << curl_easy_strerror(ip_api_res) << std::endl;
			curl_easy_cleanup(ip_api_curl);
			return 1;
		}

		std::cout << response << std::endl;

		curl_easy_cleanup(ip_api_curl);
	}
	else if (json_flag) {
		CURL* ip_api_curl = curl_easy_init();
		if (!ip_api_curl) {
			std::cerr << "Failed to initialize libcurl for IP-API." << std::endl;
			return 1;
		}

		curl_easy_setopt(ip_api_curl, CURLOPT_URL, ip_api_url);
		response.clear();
		curl_easy_setopt(ip_api_curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(ip_api_curl, CURLOPT_WRITEDATA, &response);

		CURLcode ip_api_res = curl_easy_perform(ip_api_curl);
		if (ip_api_res != CURLE_OK) {
			std::cerr << "curl_easy_perform() for IP-API failed: " << curl_easy_strerror(ip_api_res) << std::endl;
			curl_easy_cleanup(ip_api_curl);
			return 1;
		}

		std::cout << response << std::endl;

		curl_easy_cleanup(ip_api_curl);
	}
	else {
		std::cout << "IP: " << aws_ip << std::endl;
	}
	return 0;
}
