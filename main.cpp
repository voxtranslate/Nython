#include <Nython.hpp>


using Repl = replxx::Replxx;
using namespace replxx::color;

using namespace std;
using namespace nython;
using namespace nython::io;
using namespace nython::vm;
using namespace nython::node;
using namespace nython::lexer;
using namespace nython::kernel;
using namespace nython::parser;
using namespace nython::reader;
using namespace nython::exception;


/**
* *range  → term ('..' term)?
* term    → factor (('+' | '-') factor)*
* factor  → unary (('*' | '/' | '//' | '%') unary)*
*/

// Print the header
void print_header(int argc, const char* argv[])
{
    /// fg(fmt::color::crimson) | fmt::emphasis::bold,
    /// fg(fmt::color::floral_white) | bg(fmt::color::slate_gray) | fmt::emphasis::underline,
    /// fg(fmt::color::steel_blue) | fmt::emphasis::italic | | fmt::emphasis::bold,
    fmt::print(fmt::fg(fmt::color::steel_blue) | fmt::emphasis::bold, "{}\n", R"(                                                                              )");
    fmt::print(fmt::fg(fmt::color::steel_blue) | fmt::emphasis::bold, "{}\n", R"( %****&    #**%               *###,    %**#                                   )");
    fmt::print(fmt::fg(fmt::color::steel_blue) | fmt::emphasis::bold, "{}\n", R"( %******#  #**% *(((,    ((((/#***#//, %**# /@@@       *@@&@(     (((( /@@@   )");
    fmt::print(fmt::fg(fmt::color::steel_blue) | fmt::emphasis::bold, "{}\n", R"( %****/**/ #**%  #***,  %**% @@***@@@( %***(&&***/  .***#*,&***(  #***(&&***( )");
    fmt::print(fmt::fg(fmt::color::steel_blue) | fmt::emphasis::bold, "{}\n", R"( %**** #***#**%   #***.@**%   ****,    %**#   &***  %**/    %***  #**#   &**/ )");
    fmt::print(fmt::fg(fmt::color::steel_blue) | fmt::emphasis::bold, "{}\n", R"( %****   #****%    #*****%    ****/    %**#   &***  #***#  .***@  #**#   %**/ )");
    fmt::print(fmt::fg(fmt::color::steel_blue) | fmt::emphasis::bold, "{}\n", R"( %****    ****%     #***&     *****    %**#   &***    %/*****&    #**#   %**/ )");
    fmt::print(fmt::fg(fmt::color::steel_blue) | fmt::emphasis::bold, "{}\n", R"(                   ,***@      *****    %**#   &***                            )");
    fmt::print(fmt::fg(fmt::color::steel_blue) | fmt::emphasis::bold, "{}\n", R"(                                                                              )");

    fmt::print(fmt::emphasis::bold | bg(fmt::color::slate_gray) | fmt::emphasis::underline, "Nython {} (commit at branch {})\n", NYTHON_VERSION, "master");
    fmt::print("Interpreter and compiler for the Nython programming language\n");
    fmt::print("Documentation and source available at https://nython.dev/\n\n");
}


int main(int argc,char** argv,char** env) {
    int return_value = 0;

    std::thread mthread = std::thread([&return_value, &argc, &argv, &env]() {
        try {
            ConsoleManager.setupConsole();
            Regex regex_pattern("/((?:[^\\\\/]|\\\\.)*)/[A-Za-z]*");
            print_header(argc, (const char**)argv);
            std::ofstream out("examples/if.log");
            // display initial welcome message
            fmt::print("Welcome to Nython\n");
            fmt::print(fg(fmt::color::floral_white) | bg(fmt::color::slate_gray) | fmt::emphasis::underline | fmt::emphasis::italic | fmt::emphasis::bold , "Warning: {}\n", regex_pattern.match("hello world", 0).success());
            std::string filename = "examples/if.ny";
            auto source = SourceCode(filename);
            // Create the error reporter
            auto reporter = std::make_shared<Reporter>(source);
            /// Create lexer for lexing task
            auto lexer = std::make_shared<Lexer>(source);
            /// Create the virtual machine
            auto vm = std::make_shared<VirtualMachine>(reporter.get());
            /// Create the parser
            auto parser = std::make_shared<Parser>(reporter.get(), (Runnable*) vm.get(), lexer.get());
            auto tokens = lexer->tokenize();
            out << lexer->toString();
            out.close();
            lexer->statistics();
            auto nodes = parser->parse();
            fmt::print("Leaving Nython Interpreter\n");
            ConsoleManager.restoreConsole();
        } catch(kernel::RegexException& e) {
            std::cerr << e.what() << std::endl;
        } catch(exception::UnexpectedCharError& e) {
            std::cerr << e.what() << std::endl;
        } catch(std::bad_alloc& e) {
            std::cerr << "Allocation failure: can not allocate more memory: " << e.what() << std::endl;
        } catch(std::overflow_error& e) {
            std::cerr << "OverFlow Error: can not continue any more." << std::endl;
        } catch(std::system_error& e) {
            std::cerr << std::string("A System error occurred with value: ").append(e.what()) << std::endl;
        } catch(std::runtime_error& e) {
            std::cerr << "During runtime this error occur : " << e.what() << std::endl;
        } catch(std::exception& e) {
            std::cerr << "Undefined Exception occur with Value: " << e.what()  << std::endl;
        }
    });
    mthread.join();
    return return_value;
}
