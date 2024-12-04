#pragma once

#include <string>
#include <map>

namespace Messages {
    namespace toReplaceText {
        const std::string commands = "commands";
        const std::string command = "command";
        const std::string operatorStr = "operator";
        const std::string position = "position";
        const std::string charStr = "char";
        const std::string pos = "pos";
        const std::string formula = "formula";
        const std::string expected = "expected";
        const std::string provided = "provided";
    };

    const std::string COMMAND_PROMPT = "Wprowadz polecenie ({commands}):";
    const std::string UNKNOWN_COMMAND = "Nieznane polecenie: '{command}'.";
    const std::string ERROR_MISSING_ARGS = "Brakuje argumentu dla operatora '{operator}' na pozycji {position}. Naprawiono wstawiajac domyslny argument.";
    const std::string ERROR_INVALID_CHAR = "W nazwie zmniennej niedozwolony znak '{char}' na pozycji {pos}. Zostanie zignorowany.";
    const std::string ERROR_MISSING_VAR_LETTER = "Nazwa zmiennej na pozycji {pos} nie ma zadnej litery. Zmienna zostanie zignorowana.";
    const std::string ERROR_EXTRA_ARGS = "Nadmiarowe argumenty w formule od pozycji {pos}. Nie zostaly wczytane.";
    const std::string FINAL_FORMULA = "Przetwarzana bedzie formula: {formula}";
    const std::string ERROR_COMP_VALUES = "Blad: Oczekiwano {expected} wartosci, a podano {provided}.";
    const std::string PROGRAM_EXIT = "Koniec programu.";
    const std::string EMPTY_MSG = "";
    const std::string SPACE_MSG = " ";
    const std::string COLON_MSG = ":";


    static std::string formatMessage(const std::string& templateMessage, const std::map<std::string, std::string> values) {
        std::string result = templateMessage;
        
        for (const auto& pair : values) {
            std::string placeholder = "{" + pair.first + "}";
            size_t pos = 0;
        
            while ((pos = result.find(placeholder, pos)) != std::string::npos) {
                result.replace(pos, placeholder.size(), pair.second);
                pos += pair.second.size();
            }
        }

        return result;
    }
}