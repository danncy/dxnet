#ifndef FRAMEWORK_SPLIT_H_
#define FRAMEWORK_SPLIT_H_

#include <vector>
#include <string>

namespace framework {

/**
 * @brief split a string by a character
 *
 * @param s
 * @param delimiter
 * @return std::vector<std::string>
 */
std::vector<std::string> split(const std::string& s, char delimiter);

/**
 * @brief split a string by a string
 *
 * @param s
 * @param delimiter
 * @return std::vector<std::string>
 */
std::vector<std::string> split(const std::string& s, const std::string& delimiter);

} // namespace framework

#endif