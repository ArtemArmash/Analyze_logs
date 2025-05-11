#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <regex>
#include <unordered_map>
using namespace std;

vector<string>http_methods{ "GET","POST","PUT","PATCH","DELETE","HEAD","OPTIONS" };
map<string, string> HTTPStatusDescriptions = {
	{"200", "OK"},
	{"201", "Created"},
	{"202", "Accepted"},
	{"204", "No Content"},
	{"301", "Moved Permanently"},
	{"302", "Found"},
	{"400", "Bad Request"},
	{"401", "Unauthorized"},
	{"403", "Forbidden"},
	{"404", "Not Found"},
	{"500", "Internal Server Error"},
	{"502", "Bad Gateway"},
	{"503", "Service Unavailable"}
};



unordered_map<string, int>IPaddresses;
map<string, int>HTTPstatuses;
map<string, int>HTTPmethods;
unordered_map<string, int>URLrequests;

int countRequests = 0;
int unique_IP_address = 0;


void extractionForFile() 
{
	const char* FILENAME = "server.log";

	fstream fin(FILENAME);
	string str;

	regex ip_address_pattern("((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\\.){3}(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])");
	smatch match;

	regex status_code_pattern(R"( - (\d{3})$)");
	smatch status_match;

	regex http_method_pattern(R"(\b(GET|POST|PUT|PATCH|DELETE|HEAD|OPTIONS)\b)");
	smatch method_match;

	regex rgx(R"(\s([A-Z]+)\s+(\S+))");
	smatch rgx_match;


	
	if (!fin.is_open()) 
	{
		return;
	}
	else 
	{
		while (getline(fin,str)) {
			countRequests++;

			if (regex_search(str, match, ip_address_pattern)) {
				string ip = match[0].str();
				IPaddresses[ip]++;
			}

			if (regex_search(str, status_match, status_code_pattern)) {
				string statusCode = status_match[1];
				HTTPstatuses[statusCode]++;
			}


		
			if (regex_search(str, method_match, http_method_pattern)) {
				string methodHTTP = method_match[1];
				HTTPmethods[methodHTTP]++;
					
			}
			

			if (regex_search(str, rgx_match, rgx)) {
				string url_requests = rgx_match[2];
				URLrequests[url_requests]++;
			}
			
		}
	}
	
	fin.close();
}

void show() {
	cout << "Log File Analysis Report for: server.log " << endl;
	cout << "============================================\n" << endl;


	cout << "Total Requests: " << countRequests << endl << endl;
	cout << "Unique IP Addresses: " << IPaddresses.size() << endl << endl;

	cout << "IP Address Statistics: " << endl;
	cout << "---------------------------------------------" << endl;
	int count_ip_address = 1;
	for (const auto& current_ip_address : IPaddresses) {
		cout << count_ip_address << ". " << current_ip_address.first << ": " << current_ip_address.second << " request(s)\n";
		count_ip_address++;
	}
	cout << "\nHTTP Status Code Statistics:" << endl;
	cout << "---------------------------------------------" << endl;
	for (const auto& current_http_status : HTTPstatuses) {
		string description = HTTPStatusDescriptions.count(current_http_status.first) ? HTTPStatusDescriptions[current_http_status.first] : "Unknown";
		cout << current_http_status.first << " (" << description << "): " << current_http_status.second << " request(s)\n";
	}

	cout << "\nHTTP Method Statistics:" << endl;
	cout << "---------------------------------------------" << endl;
	for (const auto& current_http_method : HTTPmethods) {
		cout << current_http_method.first << ": " << current_http_method.second << " request(s)\n";
	}


	cout << "\nRequested URLs:" << endl;
	cout << "---------------------------------------------" << endl;
	int count_url_request = 1;
	for (const auto& current_url_request : URLrequests) {
		cout << count_url_request << ". " << current_url_request.first << ": " << current_url_request.second << " request(s)\n";
		count_url_request++;
	} 
}

