/* 2023-02-04 */

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/program_options.hpp>
#include <ios>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;

namespace po = boost::program_options;

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

string cypher_shift(string message, int shift) {

  std::vector<char> converted_chars{};

  for (auto c : message) {

    converted_chars.push_back(shift_letter(c, shift));
  }

  return string{converted_chars.begin(), converted_chars.end()};
}

int main(int argc, char *argv[]) {

  po::options_description description{
      "cypher [options] message"};

  description.add_options()("rotation,r", po::value<int>(),
                            "Amount to shift");

  /*
  string message = "What sphinx of cement and aluminum bashed open their "
                   "skulls and ate up their brains and imaginations?";


  string encoded = cypher_shift(message, 13);

  string decoded = cypher_shift(encoded, 13);

  cout << encoded << '\n';
  cout << decoded << '\n';
  */
}
