#include <string_view>

enum class [[nodiscard]] ErrorCode {
    OK,
    Fatal,
    System,
    FileIssue
};

ErrorCode OpenFile(std::string_view fileName) 
{ 
    return ErrorCode::OK; 
}

ErrorCode SendEmail(std::string_view sendto, std::string_view text) 
{ 
    return ErrorCode::OK; 
}

ErrorCode SystemCall(std::string_view text) {
	return ErrorCode::OK; 
}

int main()
{
    if (OpenFile("test.txt") != ErrorCode::OK)
        return 1;
        
    if (SendEmail("joe", "hello") != ErrorCode::OK)
        return 1;   
        
    SystemCall("cd ..");
}