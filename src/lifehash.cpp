#include <math.h>

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <random>
#include <iterator>
#include <sstream>

#include <lifehash/lifehash.hpp>
#include "png-writer.hpp"

using namespace std;

vector<string> args;

string appending_path_component(const string& path, const string& component) {
    if(path.empty()) { return component; }
    if(path.back() == '/') {
        return path + component;
    }
    return path + "/" + component;
}

template <typename T>
string join(const T& v, const string& delim) {
    ostringstream s;
    for (const auto& i : v) {
        if (&i != &v[0]) {
            s << delim;
        }
        s << i;
    }
    return s.str();
}

template<typename Iter, typename RandomGenerator>
Iter random_element(Iter start, Iter end, RandomGenerator& g) {
    uniform_int_distribution<> dis(0, distance(start, end) - 1);
    advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter random_element(Iter start, Iter end) {
    static random_device rd;
    static mt19937 gen(rd());
    return random_element(start, end, gen);
}

string make_random_input() {
    string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    auto letter = [&]() -> char { return *random_element(letters.begin(), letters.end()); };
    auto cluster = [&]() -> string { return string({ letter(), letter(), letter() }); };
    return join(vector<string>({ cluster(), cluster() }), "-");
}

string replace_first(
    string& s,
    const string& from,
    const string& to)
{
    size_t pos = s.find(from);
    if (pos == string::npos) return s;
    return s.replace(pos, from.length(), to);
}

void print_usage() {
    string s = R"USAGE(        
Usage: {0} [OPTION...] [INPUT]

    -v VERSION      [version1|version2|detailed|fiducial|grayscaleFiducial] (default: version2)
    -m MODULE_SIZE  Size of each module ("pixel") (default: 1)
    -p OUTPUT_PATH  (default: current directory)

    -h --help       Print this message.

* The output PNG file will have the same name as the INPUT string.

* If no INPUT string is provided, a random one will be generated of the form "XXX-XXX" where X is a random letter.
)USAGE";
    cout << replace_first(s, "{0}", args[0]) << endl;
}

struct Parameters {
    string input;
    LifeHash::Version version;
    size_t module_size;
    string output_file;

    Parameters(const vector<string>& arguments) {
        auto remaining = arguments;
        remaining.erase(remaining.begin());

        auto has_next = [&]() -> bool {
            return !remaining.empty();
        };

        auto look_next = [&]() -> string {
            return remaining.front();
        };

        auto next = [&]() -> string {
            if(!has_next()) {
                throw invalid_argument("Insufficient arguments.");
            }

            auto n = remaining.front();
            remaining.erase(remaining.begin());
            return n;
        };

        auto next_int = [&]() -> size_t {
            auto i = stoi(next());
            if(i < 0) {
                throw invalid_argument("Illegal value.");
            }
            return i;
        };

        version = LifeHash::Version::version2;
        module_size = 1;
        string output_path;
        while(has_next()) {
            auto s = look_next();
            if(s == "-h" || s == "--help") {
                print_usage();
                exit(0);
            } else if(s == "-v") {
                next();
                auto n = next();
                if(n == "version1") {
                    version = LifeHash::Version::version1;
                } else if(n == "version2") {
                    version = LifeHash::Version::version2;
                } else if(n == "detailed") {
                    version = LifeHash::Version::detailed;
                } else if(n == "fiducial") {
                    version = LifeHash::Version::fiducial;
                } else if(n == "grayscaleFiducial") {
                    version = LifeHash::Version::grayscale_fiducial;
                } else {
                    throw invalid_argument("Invalid version.");
                }
            } else if(s == "-m") {
                next();
                module_size = next_int();
            } else if(s == "-p") {
                next();
                output_path = next();
            } else {
                break;
            }
        }

        if(has_next()) {
            input = next();
        }

        if(has_next()) {
            throw invalid_argument("Extra unknown arguments found.");
        }

        if(input.empty()) {
            input = make_random_input();
        }

        auto output_filename = input + ".png";
        output_file = appending_path_component(output_path, output_filename);
    }
};

void write_image(const LifeHash::Image& image, const string& filename) {
    PNGWriter png(image.width, image.height);
    for(size_t y = 0; y < image.height; y++) {
        for(size_t x = 0; x < image.width; x++) {
            auto offset = (y * image.width + x) * 3;
            auto r = image.colors[offset];
            auto g = image.colors[offset + 1];
            auto b = image.colors[offset + 2];
            png.set(x, y, r, g, b);
        }
    }
    png.write(filename);
}

void run(const vector<string>& arguments = args) {
    auto parms = Parameters(arguments);
    auto image = LifeHash::make_from_utf8(parms.input, parms.version, parms.module_size);
    write_image(image, parms.output_file);
}

int main( int argc, char *argv[] ) {
    for(auto i = 0; i < argc; i++) {
        args.push_back(string(argv[i]));
    }
    try {
        run(args);
    } catch(invalid_argument e) {
        cout << "🤨 " << e.what() << endl;
        cout << endl;
        print_usage();
        exit(1);
    }
    return 0;
}
