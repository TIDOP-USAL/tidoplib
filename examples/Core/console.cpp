#include <tidop/core/app/app.h>
#include <tidop/core/app/message.h>

int main(int argc, char **argv) 
{
    // Initialising the application
    App app(argc, argv);

    // Configuring the console
    Console &console = App::console();
    console.setTitle("My Console Application");
    console.setMessageLevel(MessageLevel::all);
    Message::addMessageHandler(&console);
	
    // Define command line arguments
    Command cmd("my_console_app", "Example of a console application using TidopLib");
    cmd.addArgument<std::string>("file", 'f', "Example of a mandatory parameter of type string");
    cmd.addArgument<int>("int", 'i', "Required integer value");
    cmd.addArgument<bool>("bool", 'b', "Boolean flag", false);
    cmd.addArgument<double>("double", "Optional double parameter. If omitted, the default value is taken", 0.5);

    // Parse arguments
    Command::Status status = cmd.parse(argc, argv);
    if(status == Command::Status::parse_error) {
        return 1;
    } else if(status == Command::Status::show_help) {
        return 0;
    } else if(status == Command::Status::show_licence) {
        return 0;
    } else if(status == Command::Status::show_version) {
        return 0;
    }

    // Get argument values
    auto file = cmd.value<std::string>("file");
    auto value_int = cmd.value<int>("int");
    auto option = cmd.value<bool>("bool");
    auto value_double = cmd.value<double>("double");

    // Display the values in the console
    Message::info("Application started successfully.");
    Message::info("File: {}", file);
    Message::info("Integer Value: {}", value_int);
    Message::info("Boolean Flag: {}", option ? "true" : "false");
    Message::info("Double Value: {}", value_double);

    return 0;
}
