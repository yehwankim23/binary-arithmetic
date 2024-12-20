#include <string>
#include <cctype>
#include <sstream>
#include <iostream>

struct Binary32 {
public:
    std::string sign;
    std::string exponent;
    std::string error;
    std::string mantissa;

    Binary32(const std::string& sign, const std::string& exponent, const std::string& mantissa) {
        this->sign = sign;
        this->exponent = exponent;
        this->mantissa = mantissa;
    }

    Binary32(const std::string& error) {
        this->error = error;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Binary32& binary32) {
        std::string error = binary32.error;

        if (error.empty()) {
            stream << binary32.sign << " " << binary32.exponent + " " << binary32.mantissa;
        } else {
            stream << error;
        }

        return stream;
    }
};

Binary32 toBinary32(std::string number_string) {
    {
        bool is_number = true;

        for (char character : number_string) {
            if (!std::isdigit(character) && character != '-' && character != '.') {
                is_number = false;
                break;
            }
        }

        if (!is_number) {
            return Binary32("'" + number_string + "' is not a number");
        }

        std::istringstream stream(number_string);
        double test;
        stream >> test;

        if (stream.fail()) {
            return Binary32("'" + number_string + "' is not a number");
        }
    }

    int left_index = 0;
    std::string sign_string = "0";

    if (number_string[left_index] == '-') {
        left_index++;
        sign_string = "1";
    }

    int number_length = number_string.size();
    int integer = 0;

    for (; left_index < number_length; left_index++) {
        char character = number_string[left_index];

        if (character == '.') {
            break;
        }

        integer = integer * 10 + (character - '0');
    }

    int exponent = 127;
    std::string mantissa_string = "";

    while (integer > 1) {
        exponent++;
        mantissa_string = (integer % 2 == 0 ? "0" : "1") + mantissa_string;
        integer /= 2;
    }

    if (exponent > 256) {
        return Binary32("'" + number_string + "' is too large");
    }

    double decimal = 0;

    for (int right_index = number_length - 1; right_index > left_index; right_index--) {
        decimal = (decimal + (number_string[right_index] - '0')) / 10;
    }

    while (mantissa_string.size() < 22) {
        decimal *= 2;

        if (decimal >= 1) {
            decimal -= 1;
            mantissa_string += "1";
        } else {
            mantissa_string += "0";
        }

        if (mantissa_string == "0") {
            mantissa_string = "";
            exponent--;
        }
    }

    mantissa_string += (decimal == 0) ? "0" : "1";

    if (exponent < 0) {
        return Binary32("'" + number_string + "' is too small");
    }

    std::string exponent_string = "";

    while (exponent > 0) {
        exponent_string = (exponent % 2 == 0 ? "0" : "1") + exponent_string;
        exponent /= 2;
    }

    while (exponent_string.size() < 8) {
        exponent_string = "0" + exponent_string;
    }

    std::cout << std::endl
        << "sign     : " << sign_string << std::endl
        << "exponent : " << exponent_string << std::endl
        << "mantissa : " << mantissa_string << std::endl
        << std::endl;

    return Binary32(sign_string, exponent_string, mantissa_string);
}

int main() {
    std::cout << "enter a number : ";

    std::string input;
    std::getline(std::cin, input);

    std::cout << toBinary32(input) << std::endl;
    return 0;
}
