#include "CsvParse.h"

using namespace std;

CsvParse::CsvParse(shared_ptr<istream> stream)
    :m_stream(stream)
{
}

static CsvParse::Status doEof(shared_ptr<istream>& stream)
{
    if (stream->bad()) {
        return CsvParse::StreamError;
    }
    stream.reset();
    return CsvParse::RowRead;
}

static CsvParse::Status doLF(shared_ptr<istream>& stream)
{
    if (stream->peek() == -1) {
        return doEof(stream);
    }
    return CsvParse::RowRead;
}

static CsvParse::Status doCR(shared_ptr<istream>& stream)
{
    auto c = stream->get();
    if (c == '\n') {
        return doLF(stream);
    }
    return CsvParse::NoLFAfterCR;
}

static CsvParse::Status doQuotedField(shared_ptr<istream>& stream, string& field)
{
    for (;;) {
        auto c = stream->get();
        if (c == -1) {
            if (stream->bad()) {
                return CsvParse::StreamError;
            }
            stream.reset();
            return CsvParse::UnterminatedQuote;
        }

        if (c == '"') {
            c = stream->get();
            switch (c) {
                case '"': break;
                case ',': return CsvParse::FieldRead;
                case '\r': return doCR(stream);
                case '\n': return doLF(stream);
                case -1: return doEof(stream);
                default: return CsvParse::CharacterAfterQuote;
            }
        }

        field.push_back(c);
    }
}

CsvParse::Status CsvParse::readField(string& field)
{
    field.clear();

    if (!m_stream) {
        return NoMoreData;
    }

    for (;;) {
        auto c = m_stream->get();
        switch (c) {
            case ',': return CsvParse::FieldRead;
            case '"':
                if (field.empty()) {
                    return doQuotedField(m_stream, field);
                }
                return CsvParse::QuoteInUnquoted;
            case '\r': return doCR(m_stream);
            case '\n': return doLF(m_stream);
            case -1: return doEof(m_stream);
        }

        field.push_back(c);
    }
}

CsvParse::Status CsvParse::readRow(vector<string>& row)
{
    row.clear();
    for (;;) {
        string field;
        auto status = readField(field);
        row.push_back(field);
        if (status != FieldRead) {
            return status;
        }
    }
}