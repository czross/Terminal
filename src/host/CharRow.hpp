/*++
Copyright (c) Microsoft Corporation

Module Name:
- CharRow.hpp

Abstract:
- contains data structure for character data of a row

Author(s):
- Michael Niksa (miniksa) 10-Apr-2014
- Paul Campbell (paulcam) 10-Apr-2014

Revision History:
- From components of output.h/.c
  by Therese Stowell (ThereseS) 1990-1991
- Pulled into its own file from textBuffer.hpp/cpp (AustDi, 2017)
--*/

#pragma once

#include <memory>
#include <vector>

#include "DbcsAttribute.hpp"

// Characters used for padding out the buffer with invalid/empty space
#define PADDING_CHAR UNICODE_SPACE



// the characters of one row of screen buffer
// we keep the following values so that we don't write
// more pixels to the screen than we have to:
// left is initialized to screenbuffer width.  right is
// initialized to zero.
//
//      [     foo.bar    12-12-61                       ]
//       ^    ^                  ^                     ^
//       |    |                  |                     |
//     Chars Left               Right                end of Chars buffer
class CHAR_ROW final
{
public:
    static const SHORT INVALID_OLD_LENGTH = -1;

    CHAR_ROW(short rowWidth);
    CHAR_ROW(const CHAR_ROW& a);
    CHAR_ROW& operator=(const CHAR_ROW& a);
    CHAR_ROW(CHAR_ROW&& a) noexcept;
    ~CHAR_ROW();

    void swap(CHAR_ROW& other) noexcept;

    const DbcsAttribute& GetAttribute(_In_ const size_t column) const;
    DbcsAttribute& GetAttribute(_In_ const size_t column);

    std::vector<DbcsAttribute>::iterator GetAttributeIterator(_In_ const size_t column);
    std::vector<DbcsAttribute>::const_iterator GetAttributeIterator(_In_ const size_t column) const;

    std::vector<DbcsAttribute>::const_iterator GetAttributeIteratorEnd() const noexcept;

    std::vector<wchar_t>::iterator GetTextIterator(_In_ const size_t column);
    std::vector<wchar_t>::const_iterator GetTextIterator(_In_ const size_t column) const;

    std::vector<wchar_t>::const_iterator GetTextIteratorEnd() const noexcept;

    std::wstring GetText() const;

    void ClearGlyph(const size_t column);

    const wchar_t& GetGlyphAt(const size_t column) const;
    wchar_t& GetGlyphAt(const size_t column);

    void Reset(_In_ short const sRowWidth);

    HRESULT Resize(_In_ size_t const newSize);

    void SetWrapStatus(_In_ bool const fWrapWasForced);
    bool WasWrapForced() const;

    void SetDoubleBytePadded(_In_ bool const fDoubleBytePadded);
    bool WasDoubleBytePadded() const;

    enum class RowFlags
    {
        NoFlags = 0x0,
        WrapForced = 0x1, // Occurs when the user runs out of text in a given row and we're forced to wrap the cursor to the next line
        DoubleBytePadded = 0x2, // Occurs when the user runs out of text to support a double byte character and we're forced to the next line
    };

    bool ContainsText() const;

    size_t GetWidth() const;
    short MeasureLeft() const;
    short MeasureRight() const;

    friend constexpr bool operator==(const CHAR_ROW& a, const CHAR_ROW& b) noexcept;

private:
    RowFlags bRowFlags;
    size_t _rowWidth;
    std::vector<DbcsAttribute> _attributes; // all DBCS lead & trail bit in row
    std::vector<wchar_t> _chars;

#ifdef UNIT_TESTING
    friend class CharRowTests;
#endif

};

DEFINE_ENUM_FLAG_OPERATORS(CHAR_ROW::RowFlags);
void swap(CHAR_ROW& a, CHAR_ROW& b) noexcept;
constexpr bool operator==(const CHAR_ROW& a, const CHAR_ROW& b) noexcept
{
    return (a.bRowFlags == b.bRowFlags &&
            a._rowWidth == b._rowWidth &&
            a._chars == b._chars &&
            a._attributes == b._attributes);
}

// this sticks specialization of swap() into the std::namespace for CHAR_ROW, so that callers that use
// std::swap explicitly over calling the global swap can still get the performance benefit.
namespace std
{
    template<>
    inline void swap<CHAR_ROW>(CHAR_ROW& a, CHAR_ROW& b) noexcept
    {
        a.swap(b);
    }
}
