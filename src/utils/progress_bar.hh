#pragma once

#include <iostream>
#include <string_view>
namespace partou::cli
{
class ProgressBar
{
  const std::size_t total;
  std::string_view m_msg_view;
  const std::size_t m_tick_duration;
  std::ostream& m_out;
  std::size_t m_idx;
  std::size_t m_tick_duration_counter {0};

public:
  explicit ProgressBar(std::size_t total,
                       std::string_view msg_view = std::string_view {""},
                       std::size_t tick_duration_shift = 0,
                       std::ostream& out = std::cout,
                       std::size_t idx = 0)
      : total {total >> tick_duration_shift}
      , m_msg_view {msg_view}
      , m_tick_duration {std::size_t(tick_duration_shift > 0 ? 1 : 0) << tick_duration_shift}
      , m_out {out}
      , m_idx {idx}
  {
  }

  void tick()
  {
    if (m_tick_duration_counter == 0) {
      const auto percent = 100.F * (static_cast<float>(m_idx++) / total);
      m_out << '\r' << m_msg_view << ": " << static_cast<int>(percent) << "%" << std::flush;
      m_tick_duration_counter = m_tick_duration;
    } else {
      m_tick_duration_counter--;
    }
  }
};
}  // namespace partou::cli
