#include <iostream>
#include <string>
using namespace std;


class InputBuffer {
public:
    std::string buffer;
    size_t buffer_length;
    size_t input_length;

    InputBuffer() : buffer(""), buffer_length(0), input_length(0) {}

    bool readInput() {
        if (!std::getline(std::cin, buffer)) {
            // If we hit EOF (Ctrl+D / Ctrl+Z), exit gracefully
            return false;
        }
        input_length = buffer.size();
        buffer_length = buffer.capacity();
        return true;
    }

    void close() {
        buffer.clear();
        buffer_length = 0;
        input_length = 0;
    }
};

enum class MetaCommandResult {
    CommandSuccess,
    UnrecognizedCommand
};

enum class PrepareResult {
    Success,
    UnrecognizedStatement
};
enum class StatementType {
 StatementInsert,
 StatementSelect
};
class Statement {
  public:
  StatementType type;
};


PrepareResult prepare_statement(InputBuffer& inputBuffer,Statement& statement){
    if(inputBuffer.buffer.rfind("insert",0)==0) {
     statement.type = StatementType::StatementInsert;
     return PrepareResult::Success;
    }
    if(inputBuffer.buffer == "select") {
     statement.type = StatementType::StatementSelect;
          return PrepareResult::Success;
    }
       return PrepareResult::UnrecognizedStatement;
}

void execute_statement(Statement& statement) {
    switch(statement.type) {
        case StatementType::StatementInsert:
        std::cout << "This is where u should insert\n";
        break;

            case StatementType::StatementSelect:
            std::cout << "This is where we would do a select.\n";
            break;  
    }
}

void printPrompt() {
    cout << "db > " << std::flush;
}
MetaCommandResult do_meta_command (InputBuffer& inputBuffer){
      if (inputBuffer.buffer == ".exit") {
        inputBuffer.close();
        exit(EXIT_SUCCESS);
    }
    return MetaCommandResult::UnrecognizedCommand;
}
int main() {
    InputBuffer inputBuffer;

    while (true) {
        printPrompt();
        Statement statement;

        if (!inputBuffer.readInput()) {
            cout << "\nExiting (EOF detected)\n";
            break;
        }
       if (!inputBuffer.buffer.empty() && inputBuffer.buffer[0] == '.') {
            switch (do_meta_command(inputBuffer)) {
            case MetaCommandResult::CommandSuccess:
                continue;

            case MetaCommandResult::UnrecognizedCommand:
                cout << "Unrecognized command: " << inputBuffer.buffer << "\n";
                continue;
        }
          
      }
      switch (prepare_statement(inputBuffer, statement)) {
    case PrepareResult::Success:
        break;
    case PrepareResult::UnrecognizedStatement:
        std::cout << "Unrecognized keyword at start of '" 
                  << inputBuffer.buffer << "'.\n";
        continue;
}

execute_statement(statement);
std::cout << "Executed.\n";
    }
}
