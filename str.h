#pragma once

#include <cstdint>
#include <cctype> // tolower()


enum CaseSensitivity
{
	CASE_SENSITIVE = 0,
	IGNORE_CASE,
};

constexpr inline int64_t c_string_length(const char* c_string)
{
	int64_t count = 0;
    for (auto c = c_string; *c; c++) count++;
    return count;
}

struct str8
{
	uint8_t* data  = nullptr;
	int64_t count = 0;

	constexpr inline uint8_t& operator[](int64_t index)
    {
        return data[index];
    }

	constexpr inline uint8_t operator[](int64_t index) const
    {
        return data[index];
    }
    
	constexpr inline operator bool() const
	{
		return count && data;
	}

    constexpr str8() = default;

	constexpr inline str8(uint8_t* c_string, int64_t count)
    :
        data{c_string},
        count{count}
    {}

	constexpr inline str8(uint8_t* c_string)
    :
        str8(c_string, c_string_length((const char*) c_string))
    {}

    constexpr inline str8(const char* c_string)
    :
        str8((uint8_t*) c_string, c_string_length(c_string))
    {}
};

constexpr inline str8 make_str8(uint8_t* c_string, int64_t count) {
    str8 result = {};
    result.count = count;
    result.data = c_string;
    return result;
}

constexpr inline str8 make_str8(uint8_t* c_string) {
	int64_t count = 0;
    for (auto c = c_string; *c; c++) count++;
    return make_str8(c_string, count);
}

constexpr inline str8 make_str8(const char* c_string, int64_t count) { return make_str8((uint8_t*) c_string, count); }
constexpr inline str8 make_str8(const char* c_string)            { return make_str8((uint8_t*) c_string); }

// [start; end[
constexpr inline str8 make_str8_substring(str8 source, int64_t start, int64_t end)
{
	if (start >= end || end > source.count)
		return {};
    
	return make_str8(source.data + start, end - start);
}

constexpr inline bool starts_with(str8 a, str8 prefix, CaseSensitivity sens = CASE_SENSITIVE)
{
    bool success = true;
	if(sens == CASE_SENSITIVE)
	{
		for (int64_t i = 0; success && i < prefix.count; i++)
		{
			success = a[i] == prefix[i];
		}
	}
	else 
	{
		for (int64_t i = 0; success && i < prefix.count; i++)
		{
			success = tolower(a[i]) == tolower(prefix[i]);
		}
	}
    return success;
}

constexpr inline bool operator==(str8 a, str8 b)
{
    bool are_equal = false;
    if (a.count == b.count) {
        if (a.data == b.data) {
            are_equal = true;
        } else {
            are_equal = starts_with(a, b);
        }
    }
    return are_equal;
}

constexpr inline bool operator==(str8 a, const char* b)
{
    return a == make_str8(b);
}

constexpr inline bool operator==(const char* a, str8 b)
{
    return make_str8(a) == b;
}

constexpr inline int64_t compare(str8 a, str8 b, CaseSensitivity sens = CASE_SENSITIVE)
{
	if(sens == CASE_SENSITIVE)
	{
		int64_t i = 0;
		for(; i < a.count && i < b.count; i++)
		{
			if(a.data[i] != b.data[i]) break;
		}

		int64_t last_a = i == a.count ? 0 : (int64_t)a.data[i];
		int64_t last_b = i == b.count ? 0 : (int64_t)b.data[i];

		return last_a - last_b;
	}
	else 
	{
		int64_t i = 0;
		for(; i < a.count && i < b.count; i++)
		{
			if(tolower(a.data[i]) != tolower(b.data[i])) break;
		}

		int64_t last_a = i == a.count ? 0 : (int64_t) tolower(a.data[i]);
		int64_t last_b = i == b.count ? 0 : (int64_t) tolower(b.data[i]);

		return last_a - last_b;
	}
}

constexpr inline bool operator!=(str8 a, str8 b)        { return !(a == b); }
constexpr inline bool operator!=(str8 a, const char* b) { return !(a == b); }
constexpr inline bool operator!=(const char* a, str8 b) { return !(a == b); }

constexpr inline int64_t find_first_of(str8 s, const char* char_set)
{
	int64_t result = 0;
	
	for (; result < s.count; result++)
	{
		for (const char* c = char_set; *c; c++)
		{
			if (*c == s[result]) return result;
		}
	}

	return result;
}

constexpr inline int64_t find_last_of(str8 s, const char* char_set)
{
	int64_t result = s.count - 1;
	
	for (; result != -1; result--)
	{
		for (const char* c = char_set; *c; c++)
		{
			if (*c == s[result]) return result;
		}
	}

	return result;
}

constexpr inline bool contains(str8 s, uint8_t c)
{
	bool found = false;
	for (int64_t i = 0; !found && i < s.count; i++)
	{
		found = s[i] == c;
	}
	return found;
}