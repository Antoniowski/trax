#include <iostream>
#include <fstream>

struct OutputSuppressor {
    std::streambuf* oldCout;
    std::streambuf* oldCerr;
    std::ofstream null;

    public:
        OutputSuppressor() : null("/dev/null") {
            oldCout = std::cout.rdbuf(null.rdbuf());
            oldCerr = std::cerr.rdbuf(null.rdbuf());
        }
        ~OutputSuppressor() {
            std::cout.rdbuf(oldCout);
            std::cerr.rdbuf(oldCerr);
        }
};