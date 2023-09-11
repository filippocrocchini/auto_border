#pragma once

#include "str.h"

struct str8_path
{
	str8 full_path;

	str8 directory; // includes the "/"
	str8 name;
	str8 extension; // includes the "."
	str8 name_with_extension;
};

constexpr inline str8_path split_path(str8 file_path)
{
	str8_path result = {};
	result.full_path = file_path;
	
	auto last_slash  = find_last_of(file_path, "\\/");
	auto last_dot    = find_last_of(file_path, ".");

	result.directory           = make_str8_substring(file_path,              0, last_slash + 1);
	result.name                = make_str8_substring(file_path, last_slash + 1, last_dot);
	result.extension           = make_str8_substring(file_path,       last_dot, file_path.count);
	result.name_with_extension = make_str8_substring(file_path, last_slash + 1, file_path.count);

	return result;
}