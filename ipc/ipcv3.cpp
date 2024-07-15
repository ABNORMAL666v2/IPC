// ***********************************************************************
// Assembly         : 
// Author           : Abnormal666
// Created          : 03-12-2024
//
// Last Modified By : Abnormal666
// Last Modified On : 07-15-2024
// ***********************************************************************
// <copyright file="ipcv3.cpp" company="Abnormal666">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#pragma comment(lib, "Ws2_32.Lib")
#pragma comment(lib, "Wldap32.Lib")
#pragma comment(lib, "Crypt32.Lib")

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

void ShowIp(const std::string& response)
{
	rapidjson::Document d;
	d.Parse(response.c_str());
	std::cout << "IP Address: " << d["query"].GetString() << std::endl;
}

int main(int argc, char* argv[]) {
	const char* ip_api_url = "http://ip-api.com/json";
	std::string response;
	long http_code = 0;
	bool pretty_flag = false;
	bool json_flag = false;
	for (int i = 1; i < argc; ++i) {
		if (std::string(argv[i]) == "-pretty") pretty_flag = true;
		else if (std::string(argv[i]) == "-json") json_flag = true;
	}
	CURL* curl = curl_easy_init();
	if (!curl) {
		std::cerr << "Failed to initialize CURL" << std::endl;
		return 1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, ip_api_url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
	curl_easy_perform(curl);
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

	if (http_code == 200) {
		if (pretty_flag) {
			print_pretty_json(response);
			curl_easy_cleanup(curl);
			return 0;
		}
		if (json_flag) {
			std::cout << response << std::endl;
			curl_easy_cleanup(curl);
			return 0;
		}
		//if (json_flag == false && pretty_flag == false) {
		//	std::cout << "Please provide a flag: -pretty or -json\n" << std::endl;
		//	curl_easy_cleanup(curl);
		//	return 1;
		//}
		else {
			ShowIp(response);
			curl_easy_cleanup(curl);
			return 0;
		}
	}
	else {
		std::cerr << "HTTP Error: " << http_code << std::endl;
		return 1;
	}

	curl_easy_cleanup(curl);
	return 0;
}