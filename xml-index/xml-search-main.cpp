
#include "indexing_xml.h"

int main(int argc, char* argv[]) {

    const char *small_data = "./data/Hansard-Example.xml";
    const char *big_data = "./data/SenateHansard1979vol2.xml";

    Debates query(big_data);
    query.DisplayDebates();

return 0;
}