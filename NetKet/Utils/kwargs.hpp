// Copyright 2018 Tom Westerhout
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef NETKET_KWARGS_HPP
#define NETKET_KWARGS_HPP

#include <sstream>
#include "exceptions.hpp"

//
// A more of less transparent handling of pybind11::kwargs and nlohmann::json
// types as configuration parameters.
//
// Synopsis:
//
// namespace netket {
//     // Returns whether parameters have the specified field.
//     template <class Parameters>
//     bool FieldExists(Parameters const& parameters, const std::string& field);
//
//     // Verifies that `field` exists in `parameters` and throws an
//     // `InvalidInputError` otherwise.
//     //
//     // @param context is used in the error message to help users locate the
//     // location of the error.
//     //
//     // Example usage: `CheckFieldExists(pars["Key"], "SubKey", "Key");`
//     // If `SubKey` does not exists, this will throw and error with message
//     // "Field 'SubKey' (below 'Key') is not defined in the input".
//     template <class Parameters>
//     void CheckFieldExists(const Parameters& parameters,
//                           const std::string& field,
//                           const std::string& context = ...);
//
//     // If `Value` template parameter is specified, safely returns
//     // `parameters[field]` converted to type `Value`. Otherwise,
//     // `parameters[field]` is seen as a subgroup of the parameters.
//     template <class Value = ..., class Parameters>
//     auto FieldVal(const Parameters& parameters, const std::string& field,
//                   const std::string& context = ...);
// } // namespace netket
//

namespace netket {
namespace detail {

template <class Parameters>
struct ParameterTraits; /*{
  Exposition only!

  // Returns whether `parameters` have a field `field`.
  static bool FieldExists(Parameters const& parameters,
                          std::string const& field);

  // Returns the field `field` of `parameters` converted to type `T`.
  //
  // @precondition: `FieldExists(parameters, field) == true`.
  template <class T>
  static T GetField(Parameters const& parameters, std::string const& field);

  // Returns the field `field` of `parameters` seen as parameters (i.e. as a
  // dictionary or json object).
  //
  // @precondition: `FieldExists(parameters, field) == true`.
  template <class T>
  static ? GetPart(Parameters const& parameters, std::string const& field);
};*/

// A dummy type that lives only during compilation.
struct _NoneType {
  _NoneType() = delete;
  _NoneType(_NoneType const&) = delete;
  _NoneType(_NoneType&&) = delete;
  _NoneType& operator=(_NoneType const&) = delete;
  _NoneType& operator=(_NoneType&&) = delete;
};

template <class Parameters, class = int>
struct HasParameterTraits : std::false_type {};

template <class Parameters>
struct HasParameterTraits<
    Parameters, decltype(static_cast<void>(ParameterTraits<Parameters>{}), 0)>
    : std::true_type {};

}  // namespace detail

template <class Parameters>
bool FieldExists(Parameters const& parameters, const std::string& field) {
  static_assert(
      detail::HasParameterTraits<Parameters>::value,
      "There is no `detail::ParameterTraits` specialisation for `Parameters`. "
      "You most likely forgot to include the corresponding header file "
      "(\"Utils/json_helper.hpp\" or \"Utils/python_helper.hpp\").");
  return detail::ParameterTraits<Parameters>::FieldExists(parameters, field);
}

template <class Parameters>
void CheckFieldExists(const Parameters& parameters, const std::string& field,
                      const std::string& context = "") {
  static_assert(
      detail::HasParameterTraits<Parameters>::value,
      "There is no `detail::ParameterTraits` specialisation for `Parameters`. "
      "You most likely forgot to include the corresponding header file "
      "(\"Utils/json_helper.hpp\" or \"Utils/python_helper.hpp\").");
  if (!FieldExists(parameters, field)) {
    std::ostringstream s;
    s << "Field '" << field << "' ";
    if (!context.empty()) {
      s << "(below '" << context << "') ";
    }
    s << "is not defined in the input";
    throw InvalidInputError(s.str());
  }
}

namespace detail {
template <class Value, class Parameters, class TrueOrFalse, class _Dummy = void>
void FieldVal(const Parameters&, const std::string&, TrueOrFalse) {
  static_assert(!detail::HasParameterTraits<Parameters>::value, "Bug!");
  static_assert(
      detail::HasParameterTraits<Parameters>::value,
      "There is no `detail::ParameterTraits` specialisation for `Parameters`. "
      "You most likely forgot to include the corresponding header file "
      "(\"Utils/json_helper.hpp\" or \"Utils/python_helper.hpp\").");
}

template <class _Dummy, class Parameters>
auto FieldValImpl(Parameters const& parameters, std::string const& field,
                  std::true_type /*Value == NoneType*/)
    -> decltype(detail::ParameterTraits<Parameters>::GetPart(parameters,
                                                             field)) {
  return detail::ParameterTraits<Parameters>::GetPart(parameters, field);
}

template <class Value, class Parameters>
auto FieldValImpl(Parameters const& parameters, std::string const& field,
                  std::false_type /*Value == NoneType*/)
    -> decltype(detail::ParameterTraits<Parameters>::template GetField<Value>(
        parameters, field)) {
  return detail::ParameterTraits<Parameters>::template GetField<Value>(
      parameters, field);
}
}  // namespace detail

template <class Value = detail::_NoneType, class Parameters>
auto FieldVal(const Parameters& parameters, const std::string& field,
              const std::string& context = "")
    -> decltype(detail::FieldValImpl<Value, Parameters>(
        parameters, field, std::is_same<Value, detail::_NoneType>{})) {
  CheckFieldExists(parameters, field, context);
  return detail::FieldValImpl<Value, Parameters>(
      parameters, field, std::is_same<Value, detail::_NoneType>{});
}

template <class Value, class Parameters>
Value FieldOrDefaultVal(const Parameters& parameters, const std::string& field,
                        Value defval) {
  static_assert(
      detail::HasParameterTraits<Parameters>::value,
      "There is no `detail::ParameterTraits` specialisation for `Parameters`. "
      "You most likely forgot to include the corresponding header file "
      "(\"Utils/json_helper.hpp\" or \"Utils/python_helper.hpp\").");
  if (FieldExists(parameters, field)) {
    return detail::FieldValImpl<Value, Parameters>(parameters, field,
                                                   std::false_type{});
  } else {
    return defval;
  }
}

}  // namespace netket

#endif  // NETKET_KWARGS_HPP
