#pragma once
//stored file successful
//failed

#if defined(_WIN32)
#	ifndef UNICODE
#		define UNICODE
#	endif
#endif

#include <iostream>
#include <assert.h>

#if defined(_WIN32)
#	include <WinSock2.h>
#	include <Windows.h>
#	include <Shlwapi.h>
#else
#	include <dlfcn.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#endif

#if defined ( _DEBUG ) || defined( DEBUG ) || defined (__DEBUG__)
#	ifndef DEBUG
#       define DEBUG
#   endif
#endif

#if defined(_MSC_VER)
#	pragma warning(disable : 4068)
#	pragma warning(disable : 4251)
#	pragma warning(disable : 4275)
#	pragma warning(disable : 4290)
#	pragma warning(disable : 4819)
#	pragma warning(disable : 4996)
#endif

#if ! defined(NULL)
#if defined(__cplusplus) 
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#if ! defined(SAFE_DELETE)
#	define SAFE_DELETE(p) do{if(p!=NULL){delete p;p=NULL;}}while(0)
#endif

#if ! defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY(p) do{if(p!=NULL){delete[] p;p=NULL;}}while(0)
#endif

#define endll std::endl << std::endl // double end line definition

//debug 
#ifdef DEBUG
#ifdef _MSC_VER
#define YT_DEBUG_PRINTF(_arg_,...) printf_s((std::string("[%s:%d]")+_arg_).c_str(),__FILE__, __LINE__ , __VA_ARGS__)
#else
#define YT_DEBUG_PRINTF(_arg_,...) printf((std::string("[%s:%d]")+_arg_).c_str(),__FILE__, __LINE__ , __VA_ARGS__)
#endif
#else  
#define YT_DEBUG_PRINTF(_arg_,...)
#endif // DEBUG



enum Error
{
	ER_SUCCESS = 0,
	ER_NO_ERROR = 0,
	ER_ERROR_UNKNOWN_EXCEPTION,

	//serialize
	ER_ERROR_SERIALIZE_ERROR,
	ER_ERROR_DESERIALIZE_ERROR,

	//file manage
	ER_ERROR_FILE_NOT_EXIST,
	ER_ERROR_INVALID_SAVE,
	ER_ERROR_INVALID_FILENAME,
	ER_ERROR_INVALID_FILE,
	ER_ERROR_NEW_FILE_FAILED,
	ER_ERROR_PARSE_FILE_FAILED,
	ER_ERROR_SAVE_FILE_FAILED,

	//check input
	ER_ERROR_INVAILD_INPUT,

	//cache
	ER_ERROR_NO_CACHE,
	ER_ERROR_SAVE_CACHE_FAILED,

	ER_ERROR_COUNT
};
static const char* gErrorMessages[ER_ERROR_COUNT] =
{
	"successful.",
	"unknown error.",
	"serialize error.",
	"deserialize error.",
	"file not exist.",
	"invalid save.",
	"invalid file name.",
	"invalid file.",
	"initialize file failed.",
	"parse file failed.",
	"save file failed.",
	"invalid input.",
	"no cache exist.",
	"save cache failed."
};

static const char* GetErrorMessage(Error err0) {
	uint32_t err = static_cast<uint32_t>(err0);
	assert(err >= 0 && err < ER_ERROR_COUNT);
	const char* msg = gErrorMessages[err];
	assert(msg && msg[0]);
	return msg;
}


class Exception : public std::exception {
public:

	Exception(Error err) : m_message(), m_errorcode(err) {

	}
	Exception(const std::string& msg) : m_message(msg), m_errorcode(ER_ERROR_UNKNOWN_EXCEPTION) {

	}
	virtual ~Exception(void) throw() {}
	virtual const char* what() const throw() {
		if (m_message.empty()) {
			return GetErrorMessage(m_errorcode);
		}
		else {
			return m_message.c_str();
		}
	}
protected:
	Error m_errorcode;
	std::string m_message;
};
