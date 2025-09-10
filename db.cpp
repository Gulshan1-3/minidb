#include <iostream>
#include <string>
#include <cstdint>

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)



using namespace std;
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
class Row {
    public:
    u_int32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
};


const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);

const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

const uint32_t PAGE_SIZE = 4096;
#define TABLE_MAX_PAGES 100
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

class Table{
    public:
    uint32_t num_rows;
  void* pages[TABLE_MAX_PAGES];
};

void print_row(Row* row) {
std::cout << "(" << row->id << ", " 
          << row->username << ", " 
          << row->email << ")" << std::endl;
}

class InputBuffer {
public:
    std::string buffer;
    size_t buffer_length;
    size_t input_length;

    InputBuffer() : buffer(""), buffer_length(0), input_length(0) {}

    bool readInput() {
        if (!std::getline(std::cin, buffer)) {
           
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
    UnrecognizedStatement,
    SyntaxError
};
enum class StatementType {
 StatementInsert,
 StatementSelect
};
class Statement {
  public:
  StatementType type;
  Row row_to_insert;
};


PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (input_buffer->buffer.rfind("insert", 6) == 0) {
        statement->type = StatementType::StatementInsert;
        int args_assigned = sscanf(
            input_buffer->buffer.c_str(),
            "insert %d %s %s",
            &statement->row_to_insert.id,
            statement->row_to_insert.username,
            statement->row_to_insert.email
        );

        if (args_assigned < 3) {
            return PrepareResult::SyntaxError;
        }
        return PrepareResult::Success;
    }

    if (input_buffer->buffer == "select") {
        statement->type = StatementType::StatementSelect;
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
      switch (prepare_statement(&inputBuffer, &statement)) {
    case PrepareResult::Success:
        break;
         case (PrepareResult::SyntaxError):
	std::cout<<("Syntax error. Could not parse statement.\n");
	continue;
    case PrepareResult::UnrecognizedStatement:
        std::cout << "Unrecognized keyword at start of '" 
                  << inputBuffer.buffer << "'.\n";
        continue;
}

execute_statement(statement);
std::cout << "Executed.\n";
    }
}
