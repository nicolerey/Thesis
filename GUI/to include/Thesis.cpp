/*
	Date: November 17, 2015
	Description: This program contains necessary functions for the whole project,
				 such as perform GET request using CURL, parsing JSON data to CPP object.
*/

#include "stdafx.h"
#include "Thesis.h"
#include "string.h"
#include <string>
#include <iostream>

CURL *curl;
CURLcode res;
Document response;
struct data s;
char url[] = "http://localhost/thesisv2/index.php?";
char errorMsg[] = "Server is not available!";

void init_string(struct data *s) {
	s->len = 0;
	s->string = (char *)malloc(s->len + 1);
	if (s->string == NULL) {
		fprintf(stderr, "malloc() failed\n");
		exit(EXIT_FAILURE);
	}
	s->string[0] = '\0';
}

size_t writefunc(void *string, size_t size, size_t nmemb, struct data *s)
{
	size_t new_len = s->len + size*nmemb;
	s->string = (char *)realloc(s->string, new_len + 1);
	if (s->string == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
	}
	memcpy(s->string + s->len, string, size*nmemb);
	s->string[new_len] = '\0';
	s->len = new_len;

	return size*nmemb;
}

void thesis_start_request() {
	curl = curl_easy_init();
	init_string(&s);
}

void thesis_end_request() {
	curl_easy_cleanup(curl);
	free(s.string);
}

int thesis_request(const char *function) {
	int toReturn;

	thesis_start_request();

	curl_easy_setopt(curl, CURLOPT_URL, function);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		toReturn = 0;
	}
	else {
		response.Parse(s.string);
		toReturn = 1;
	}

	thesis_end_request();

	return toReturn;
}