#pragma once
#define WARNING 1
#define ERROR 2
#define SUCCESS 3
#define INFO 4


namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}
Color::Modifier red(Color::FG_RED);
Color::Modifier green(Color::FG_GREEN);
Color::Modifier blue(Color::FG_BLUE);
Color::Modifier def(Color::FG_DEFAULT);
Color::Modifier green_BG(Color::BG_GREEN);

void status_msg(int status, std::string msg)
{
    switch(status)
    {
        case WARNING:
            std::cout <<  "[" << blue << "*" << def << "] " << blue << "WARNING" << def << ": " << msg << std::endl; break;
        case ERROR:
            std::cout << "[" << red << "!" << def << "] " << red << "ERROR" << def << ": " << msg << std::endl; break;
        case SUCCESS:
            std::cout << "[" << green << "+" << def << "] " << green << "SUCCESS" << def << ": " << msg << std::endl; break;
        case INFO:
            std::cout << "[" << green << "+" << def << "] " << green << "INFO" << def << ": " << msg << std::endl; break;
    }
}