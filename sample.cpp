#include "src/CsvParse.h"

#include <iostream>
#include <sstream>

using namespace std;

static void dump(CsvParse& csvParse)
{
  for (;;) {
    vector<string> bf;
    auto status = csvParse.readRow(bf);

    if (status)
      break;

    cout << "Row:\n";
    for (const auto& i : bf) {
      cout << " - '" << i << "'\n";
    }
  }

}

static void string_test()
{
  const char* textCsv = "x,y,\n1,\"abc \"\" def\",2\n,3,4";
  CsvParse c(make_shared<istringstream>(textCsv));

  dump(c);
}

int main()
{
  string_test();

  return 0;
}