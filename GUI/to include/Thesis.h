#include "curl/curl.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#ifndef THESIS_H
#define THESIS_H

using namespace rapidjson;

struct data {
	char *string;
	size_t len;
};

extern CURL *curl;
extern CURLcode res;
extern Document response;
extern struct data s;
extern char url[];
extern char errorMsg[];

int thesis_request(const char *);

#endif