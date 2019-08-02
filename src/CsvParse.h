#pragma once
#include <istream>
#include <memory>
#include <vector>

class CsvParse {
public:
    enum Status {
        RowRead = 0,
        FieldRead,
        NoMoreData,
        StreamError,
        NoLFAfterCR,
        QuoteInUnquoted,
        UnterminatedQuote,
        CharacterAfterQuote,
    };

    CsvParse(std::shared_ptr<std::istream> stream);

    CsvParse(const CsvParse&) = delete;
    void operator= (CsvParse) = delete;

    Status readField(std::string& field);
    Status readRow(std::vector<std::string>& row);
private:
    std::shared_ptr<std::istream> m_stream;
};