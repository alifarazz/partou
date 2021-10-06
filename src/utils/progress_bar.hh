#pragma once

#include <iostream>
#include <string_view>
namespace partou
{
namespace cli
{
class ProgressBar
{
  const std::size_t total;
  std::string msg_view;
  std::ostream& out;
  std::size_t idx;

public:
  ProgressBar(std::size_t total,
              std::string_view msg_view = std::string_view {""},
              std::ostream& out = std::cerr,
              std::size_t idx = 0)
      : total {total}
      , msg_view {msg_view}
      , out {out}
      , idx {idx}
  {
  }

  void tick()
  {
    const auto percent = 100.f * (static_cast<float>(idx++) / total);
    out << '\r' << msg_view << ": " << static_cast<int>(percent) << "%"
        << std::flush;
  }
};
}  // namespace cli
}  // namespace partou
