#include <cassert>
#include <string>
#include <vector>

#include "ArgParse.hpp"

namespace {

std::vector<char*> make_argv(const std::vector<std::string>& args) {
    std::vector<char*> argv;
    argv.reserve(args.size());
    for (const auto& arg : args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    return argv;
}

void test_default_options() {
    std::vector<std::string> args = {"msf_simulation"};
    auto argv = make_argv(args);

    ArgParse::Options options;
    std::string error;

    const bool ok = ArgParse::parse(static_cast<int>(argv.size()), argv.data(), options, error);
    assert(ok);
    assert(error.empty());
    assert(!options.show_help);
    assert(options.scenario_path == "Test/scenario/basic.xml");
}

void test_help_flag() {
    std::vector<std::string> args = {"msf_simulation", "--help"};
    auto argv = make_argv(args);

    ArgParse::Options options;
    std::string error;

    const bool ok = ArgParse::parse(static_cast<int>(argv.size()), argv.data(), options, error);
    assert(ok);
    assert(options.show_help);
}

void test_short_scenario_flag() {
    std::vector<std::string> args = {"msf_simulation", "-s", "Test/scenario/basic.xml"};
    auto argv = make_argv(args);

    ArgParse::Options options;
    std::string error;

    const bool ok = ArgParse::parse(static_cast<int>(argv.size()), argv.data(), options, error);
    assert(ok);
    assert(options.scenario_path == "Test/scenario/basic.xml");
}

void test_long_scenario_equals_flag() {
    std::vector<std::string> args = {"msf_simulation", "--scenario=Test/scenario/basic.xml"};
    auto argv = make_argv(args);

    ArgParse::Options options;
    std::string error;

    const bool ok = ArgParse::parse(static_cast<int>(argv.size()), argv.data(), options, error);
    assert(ok);
    assert(options.scenario_path == "Test/scenario/basic.xml");
}

void test_missing_scenario_value() {
    std::vector<std::string> args = {"msf_simulation", "--scenario"};
    auto argv = make_argv(args);

    ArgParse::Options options;
    std::string error;

    const bool ok = ArgParse::parse(static_cast<int>(argv.size()), argv.data(), options, error);
    assert(!ok);
    assert(error.find("Missing value") != std::string::npos);
}

void test_unknown_argument() {
    std::vector<std::string> args = {"msf_simulation", "--bogus"};
    auto argv = make_argv(args);

    ArgParse::Options options;
    std::string error;

    const bool ok = ArgParse::parse(static_cast<int>(argv.size()), argv.data(), options, error);
    assert(!ok);
    assert(error.find("Unknown argument") != std::string::npos);
}

} // namespace

int main() {
    test_default_options();
    test_help_flag();
    test_short_scenario_flag();
    test_long_scenario_equals_flag();
    test_missing_scenario_value();
    test_unknown_argument();
    return 0;
}
