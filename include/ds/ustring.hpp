#ifndef _DS_USTRING_HPP____by_Duzy_Chan__
#define _DS_USTRING_HPP____by_Duzy_Chan__ 1
#       include <unicode/umachine.h> // for UChar
#       include <string>

namespace ds
{
  /**
   *  @brief The Unicode character type.
   */
  typedef UChar uchar;

  /**
   *  @brief The Unicode string.
   */
  typedef std::basic_string<uchar> ustring;
}

#endif//_DS_USTRING_HPP____by_Duzy_Chan__
