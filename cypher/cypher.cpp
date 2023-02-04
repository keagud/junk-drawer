/* 2023-02-04 */

/* Simple Caeser cypher implementation */

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/any.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

namespace po = boost::program_options;

// shift a single letter by an int value
// maintains case, ignores non-alphabetic characters
char shift_letter(char original, int shift) {

  static const auto alpha_classifier = boost::algorithm::is_alpha();
  static const auto upper_classifer = boost::algorithm::is_upper();

  if (!alpha_classifier(original))
    return original;

  const auto shift_char = upper_classifer(original) ? 'A' : 'a';

  auto shifted_value = (original - shift_char) + shift;
  shifted_value %= 26;

  return (char)(shifted_value + shift_char);
}

// apply shift_letter to an entire string
string cypher_shift(string message, int shift) {

  vector<char> converted_chars{};

  for (auto c : message) {

    converted_chars.push_back(shift_letter(c, shift));
  }

  return string{converted_chars.begin(), converted_chars.end()};
}

int main(int argc, char *argv[]) {

  int rotation{};

  po::options_description description{"cypher [-r rotation] message"};

  description.add_options()("rotation,r",
                            po::value<int>(&rotation)->default_value(13),
                            "Amount to shift")(
      "message", po::value<vector<string>>()->multitoken(), "input message");

  po::positional_options_description positional{};
  positional.add("message", -1);

  po::command_line_parser parser{argc, argv};

  parser.positional(positional);

  parser.options(description);
  auto parsed_input = parser.run();

  po::variables_map vars{};

  po::store(parsed_input, vars);
  po::notify(vars);

  if (vars["message"].empty()) {
    std::cerr << "No message given\n";
    std::exit(EXIT_FAILURE);
  }

  std::stringstream message_input{};
  std::stringstream program_output{};

  auto message_tokens_vec = vars["message"].as<vector<string>>();

  std::copy(message_tokens_vec.begin(), message_tokens_vec.end() - 1,
            std::ostream_iterator<string>(message_input, " "));

  message_input << message_tokens_vec.back();

  program_output << cypher_shift(message_input.str(), rotation);

  cout << program_output.str() << std::endl;
}
