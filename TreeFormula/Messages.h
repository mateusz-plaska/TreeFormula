#pragma once

#include <string>
#include <map>

namespace Messages {
    namespace toReplaceText {
        const std::string commands = "commands";
        const std::string command = "command";
        const std::string operatorStr = "operator";
        const std::string valueStr = "value";
        const std::string position = "position";
        const std::string charStr = "char";
        const std::string pos = "pos";
        const std::string formula = "formula";
        const std::string expected = "expected";
        const std::string provided = "provided";
        const std::string name = "name";
    };

    const std::string COMMAND_PROMPT = "Wprowadz polecenie ({commands}):";
    const std::string UNKNOWN_COMMAND = "Nieznane polecenie: '{command}'.";
    const std::string ERROR_MISSING_ARGS = "Brakuje argumentu dla operatora '{operator}' na pozycji {position}. Naprawiono wstawiajac domyslny argument.";
    const std::string ERROR_INVALID_CHAR = "W nazwie zmniennej niedozwolony znak '{char}' na pozycji {pos}. Zostanie zignorowany.";
    const std::string ERROR_MISSING_VAR_LETTER = "Nazwa zmiennej na pozycji {pos} nie ma zadnej litery. Zmienna zostanie zignorowana.";
    const std::string ERROR_EXTRA_ARGS = "Nadmiarowe argumenty w formule od pozycji {pos}. Nie zostaly wczytane.";
    const std::string FINAL_FORMULA = "Przetwarzana bedzie formula: {formula}";
    const std::string ERROR_COMP_VALUES = "Blad: Oczekiwano {expected} wartosci, a podano {provided}.";
    const std::string ERROR_MISSING_VALUE_FOR_VAR = "Blad: Brakuje wartosci dla zmiennej '{name}'.";
    const std::string ERROR_INVALID_VALUE = "Blad: Wartosc {value} jest niedozwolona. Niemozliwa konwersja na typ liczbowy";
    const std::string ERROR_OVERFLOW = "Blad: Wykryto przepelnienie arytmetyczne! Nastapilo przekroczenie zakresu wartosci dla operatora '{operator}' !!!";
    const std::string ERROR_DIVISION_BY_0 = "Blad: Dzielenie przez 0";
    const std::string ERROR_DIVISION_BY_NEGATIVE_NUMBER = "Blad: Dzielenie przez ujemna liczbe (niedozwolone)";
    const std::string ERROR_FILE_OPEN = "Blad podczas otwarcia pliku.";
    const std::string ERROR_FILE_WRITE = "Blad podczas zapisu do pliku.";
    const std::string PROGRAM_EXIT = "Koniec programu.";
    const std::string EMPTY_MSG = "";
    const std::string SPACE_MSG = " ";
    const std::string COLON_MSG = ":";
    const std::string EQUAL_SIGN = "=";


    namespace Logs {
        const std::string CREATE_TREE = "Tworzone drzewo z formuly: ";
        const std::string COMPUTE = "Obliczanie wartosci dla drzewa: ";
        const std::string VARS = "Zmienne: ";
        const std::string JOIN = "Join do drzewa: ";
    }


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