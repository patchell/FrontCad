#pragma once

constexpr auto MAX_EXCEPTION_STRING_LEN = 256;

enum class ExceptionType {
	LOAD_FILE_SYNTAX_ERROR
};

struct SException {
	ExceptionType Type;
	int badToken;
	CString ErrorString;
	int MaxStringLen = MAX_EXCEPTION_STRING_LEN;
};

extern SException Exception;
