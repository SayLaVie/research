#include <array>
#include <algorithm>
#include <cinttypes>
#include <memory>
#include <iostream>
#include <iomanip>
#include <utility>
#include <random>
#include <cstring>
#include <cstdio>
#include <chrono>
#include <sstream>


template<int Size>
using PlayerState = std::array<uint16_t, Size>;

template<int Size>
bool __attribute__((noinline)) _isEndGame(const PlayerState<Size>& player_state, uint32_t player) noexcept {
  enum State {
    LinkDown,
    LinkUp,
    Capture
  };

  PlayerState<Size> board(player_state);
  PlayerState<Size> shadow_board;
  shadow_board.fill(0);

  uint32_t shadow_row = board[0];
  uint32_t row = 0;

  shadow_board[0] = board[0];
  board[0] = 0;

  State state = State::LinkDown;
  uint32_t row_num = 1;

  uint32_t connections = 0;


/********************************************************************
It looks like the author is checking for a connection that spans from
side to side. They do this by AND'ing each successive row together along
with the same row shifted once to the side, presumably to take into
account the hexagonal shape of the tiles.

Still trying to decipher what LinkDown, LinkUp, and Capture are meant
to represent.
Maybe Capture represents that two neighboring rows are connected?
********************************************************************/

  while (true) {
    switch (state) {
    case State::LinkDown: {
      row = board[row_num];
      connections = (row & shadow_row) | ((shadow_row >> 1) & row);

      if (connections) {
        if (row_num == Size - 1)
          return true;

        state = State::Capture;
      } else {
        shadow_row = shadow_board[row_num];
        if (shadow_row) {
          row_num++;
        } else {
          return false;
        }
      }
      break;
    }
    case State::LinkUp: {
      row = board[row_num];
      connections = (row & shadow_row) | ((shadow_row << 1) & row);
      if (connections) {
        state = State::Capture;
      } else {
        shadow_row = shadow_board[row_num];
        row_num++;
        state = State::LinkDown;
      }
      break;
    }
    case State::Capture: {
      uint32_t capture = 0;
      capture = connections;

      board[row_num] ^= capture;
      shadow_board[row_num] |= capture;

      // capture right bits
      do {
        capture >>= 1;
        capture &= board[row_num];
        board[row_num] ^= capture;
        shadow_board[row_num] |= capture;
      } while(capture);

      // capture left bits
      capture = connections;
      do {
        capture <<= 1;
        capture &= board[row_num];
        board[row_num] ^= capture;
        shadow_board[row_num] |= capture;
      } while(capture);

      shadow_row = shadow_board[row_num];

      if (board[row_num-1]) {
        row_num--;
        state = State::LinkUp;
      } else {
        row_num++;
        state = State::LinkDown;
      }
      break;
    }
    }
  }
}


template<int Size>
class AIBitBoard {
public:
  static_assert(Size >= 3 && Size <= 16, "Size should be in the range [3, 16]");

  static const uint32_t size = Size;

  void setState(const PlayerState<size>& s) noexcept {
    _state = s;
  }

  void toggle(uint32_t row, uint32_t col) noexcept {
    _state[row] |= 1 << col;
  }

  bool isEndGame(uint32_t player) const noexcept {
    return _isEndGame<size>(_state, player);
  }

private:
  PlayerState<size> _state;
};


template<int Size>
class BitBoard {
public:
  static_assert(Size >= 3 && Size <= 16, "Size should be in the range [3, 16]");

  static const uint32_t size = Size;

  BitBoard() {
    reset();
  }

  void reset() noexcept {
    _players[0].fill(0);
    _players[1].fill(0);
  }

  void toggle(uint32_t id, uint32_t player) noexcept {
    uint32_t row, col;
    if (player) {
      row = id % size;
      col = id / size;
    } else {
      row = id / size;
      col = id % size;
    }
    _players[player][row] |= 1 << col;
  }

  bool isToggled(uint32_t id) const noexcept {
    uint32_t row = id / size;
    uint32_t col = id % size;

    return (((_players[0][row] >> col) & 1) |
            ((_players[1][col] >> row) & 1));
  }

  bool isEndGame(uint32_t player) const noexcept {
    return _isEndGame<11>(_players[player], player);
  }

  const PlayerState<size>& getPlayerState(uint32_t player) const noexcept {
    return _players[player];
  }

  template<int S>
  friend std::ostream& operator<<(std::ostream& o, const BitBoard<S>& b) noexcept;

private:
  PlayerState<size> _players[2];
};

/*
  Print board to stream
 */
template<int Size>
std::ostream& operator<<(std::ostream& o, const BitBoard<Size>& b) noexcept {
  o << ' ';
  for (uint32_t i = 0; i < Size; ++i) {
    o << ' ' << static_cast<char>('A' + i) << ' ';
  }
  o << std::endl;

  for (uint32_t i = 0; i < Size; ++i) {
    // indentation
    for (int s = 0; s < i; ++s) {
      o << "  ";
    }
    o << std::setw(2) << i+1;

    for (int j = 0; j < Size; ++j) {
      o << ' ';
      if (b._players[0][i] >> j & 1) {
        o << 'X';
      } else if (b._players[1][j] >> i & 1) {
        o << 'O';
      } else {
        o << '.';
      }
      o << ' ';
    }

    o << std::setw(2) << i+1 << std::endl;
  }

  // indentation
  o << ' ';
  for (int s = 0; s < Size; ++s) {
    o << "  ";
  }
  for (int i = 0; i < Size; ++i) {
    o << ' ' << static_cast<char>('A' + i) << ' ';
  }
  o << std::endl;

  return o;
}

template<int Size>
class GameBoard {
public:
  static const uint32_t size = Size;

  GameBoard() : _board() {
    reset();
  }

  void reset() noexcept {
    std::iota(std::begin(_freeNodes), std::end(_freeNodes), 0);
    _freeNodesIndex = _freeNodes;
  }

  bool toggle(uint32_t id, uint32_t player) noexcept {
    if (_board.isToggled(id)) {
      return false;
    }

    _board.toggle(id, player);
    id = _freeNodesIndex[id];
    std::swap(_freeNodes[id], _freeNodes[--_freeNodesCount]);
    std::swap(_freeNodesIndex[id], _freeNodesIndex[_freeNodesCount]);

    return true;
  }

  bool toggle(uint32_t row, uint32_t col, uint32_t player) noexcept {
    return toggle(row * size + col);
  }

  bool isToggled(uint32_t id) const noexcept {
    return _board.isToggled(id);
  }

  bool isEndGame(uint32_t player) {
    return _board.isEndGame(player);
  }

  const BitBoard<size>& getBoard() const noexcept {
    return _board;
  }

  const std::array<uint16_t, size * size>& getFreeNodes() const noexcept {
    return _freeNodes;
  }

  uint32_t getFreeNodesCount() const noexcept {
    return _freeNodesCount;
  }

  template<int S>
  friend std::ostream& operator<<(std::ostream& os, const GameBoard<S>& b) noexcept;

private:
  BitBoard<size> _board;
  std::array<uint16_t, size * size> _freeNodes;
  std::array<uint16_t, size * size> _freeNodesIndex;
  uint32_t _freeNodesCount = size * size;
};

template<int Size>
std::ostream& operator<<(std::ostream& os, const GameBoard<Size>& b) noexcept {
  os << b._board;
  os << std::endl;
  os << std::endl;

  return os;
}

template<class B>
class MonteCarloAI {
private:
  struct Position {
    union {
      struct {
        uint8_t row;
        uint8_t col;
      };
      uint16_t v;
    };

    bool operator==(const Position& o) const noexcept {
      return v == o.v;
    }
  };

public:
  using gameBoardType = B;
  static const uint32_t boardSize = B::size;

  MonteCarloAI(uint32_t seed) : _rng(seed) {}

  uint32_t getNextMove(gameBoardType& board, uint32_t player, uint32_t iterations) {
    auto start = std::chrono::steady_clock::now();

    uint32_t free_nodes_count = board.getFreeNodesCount();
    Position free_nodes[free_nodes_count];
    Position free_nodes_copy[free_nodes_count];

    auto& nodes = board.getFreeNodes();

    for (int i = 0; i < free_nodes_count; ++i) {
      uint32_t id = nodes[i];
      uint8_t row, col;
      if (player) {
        row = id % boardSize;
        col = id / boardSize;
      } else {
        row = id / boardSize;
        col = id % boardSize;
      }
      free_nodes[i].row = row;
      free_nodes[i].col = col;
    }

    std::memcpy(&free_nodes_copy[0],
                &free_nodes[0],
                free_nodes_count * sizeof(Position));

    AIBitBoard<boardSize> b;
    uint32_t max_wins = 0;
    Position win_pos = free_nodes_copy[0];

    typedef std::uniform_int_distribution<uint16_t> distr_type;
    typedef distr_type::param_type distr_param;

    distr_type distr;
    uint32_t moves = (free_nodes_count - 2) / 2 + player;

    for (uint32_t p = 0; p < free_nodes_count; ++p) {
      uint32_t wins = 0;
      uint32_t possible_wins = iterations;
      Position pos = free_nodes_copy[p];

      for (uint32_t j = 0; j < iterations; ++j) {
        b.setState(board.getBoard().getPlayerState(player));
        b.toggle(pos.row, pos.col);

        for (uint32_t k = 0; k < moves; ++k) {
          using std::swap;

          uint32_t kpos = distr(_rng, distr_param(0, (free_nodes_count - 1) - k));
          Position id = free_nodes[kpos];
          swap(free_nodes[kpos], free_nodes[(free_nodes_count - 1) - k]);

          if (pos == id) {
            k++;
            continue;
          }

          b.toggle(id.row, id.col);

        }
        if (b.isEndGame(player)) {
          wins++;
        } else {
          possible_wins--;
        }

        if (possible_wins < max_wins) {
          goto end_loop;
        }
      }

      if (wins > max_wins) {
        win_pos = pos;
        max_wins = wins;
      }

    end_loop: {
      }
    }

    auto end = std::chrono::steady_clock::now();

    auto diff = end - start;
    std::cout << "time:" << std::chrono::duration<double, std::milli>(diff).count() << std::endl;

    if (player)
      return win_pos.col * boardSize + win_pos.row;
    else
      return win_pos.row * boardSize + win_pos.col;
  }

private:
  std::mt19937 _rng;
};


bool _askYesOrNo(std::string msg) {
  std::string line;

  while (true) {
    std::cout << msg;
    std::cout << " [y/n] ";
    std::getline(std::cin, line);
    std::istringstream iss(line);
    char answer;
    iss >> answer;

    answer |= 0x20;
    if (answer == 'y') {
      return true;
    } else if (answer == 'n') {
      return false;
    }
  }
}

uint32_t _askBotDifficulty() {
  std::cout << "Bot Difficulty:\n";
  std::cout << " 1. Easy\n";
  std::cout << " 2. Normal\n";
  std::cout << " 3. Hard\n";
  std::cout << " 4. Insane\n";

  std::string line;
  while (true) {
    std::cout << "[1/2/3/4]: ";
    std::getline(std::cin, line);
    std::istringstream iss(line);
    int mode;
    iss >> mode;
    if (mode > 0 && mode < 5)
      return mode;
  }
}

/*
  Human Player
 */
template<class Board>
class Player {
public:
  explicit Player(uint32_t player_id, Board& b) : _id(player_id), _board(b) {}

  virtual uint32_t askMove() {
    char colc;
    int32_t rowc;

    std::string line;
    while (true) {
      std::getline(std::cin, line);
      std::istringstream iss(line);
      iss >> colc >> rowc;
      int32_t row, col;
      col = colc - 'A';
      row = rowc - 1;

      if (col >= 0 && col < Board::size && row >= 0 && row < Board::size)
        return (row * Board::size + col);
      else
        std::cout << "Invalid position\n";
    }
  }

protected:
  uint32_t _id;
  Board& _board;
};

/*
  Hex game Bot
 */
template<class Board>
class HexBot : public Player<Board> {
public:
  HexBot(uint32_t player_id, Board& b, uint32_t difficulty, uint32_t seed)
      : Player<Board>(player_id, b), _ai(seed) {
    switch (difficulty) {
    case 1:
      _trials = 100;
      break;
    case 2:
      _trials = 1000;
      break;
    case 3:
      _trials = 5000;
      break;
    case 4:
      _trials = 40000;
      break;
    }
  }

  virtual uint32_t askMove() {
    return _ai.getNextMove(Player<Board>::_board, Player<Board>::_id, _trials);
  }

protected:
  MonteCarloAI<Board> _ai;
  uint32_t _trials;
};

/*
  Main Game Object
 */
template<int Size>
class Game {
public:
  typedef GameBoard<Size> gameBoardType;
  typedef MonteCarloAI<gameBoardType> aiType;
  typedef Player<gameBoardType> playerType;
  typedef HexBot<gameBoardType> botType;

  /*
    Game state
   */
  enum class State : uint8_t {
    Menu,
    Game,
    EndGame,
    Shutdown
  };

  /*
    Game Constructor
   */
  Game(uint32_t seed) {}

  /*
    Run Game
   */
  void run() {
    while (_state != State::Shutdown) {
      // clear screen
      //std::cout << "\x1B[2J\x1B[H";

      switch (_state) {
      case State::Menu: {
        std::cout << "Welcome to the HEX game\n";

        uint32_t second = _askYesOrNo("Do you want to play first?");
        uint32_t first = second ^ 1;
        _players[first].reset(new playerType(first, _board));
        uint32_t difficulty = _askBotDifficulty();
        _players[second].reset(new botType(second, _board, difficulty, 0));
        _state = State::Game;

        break;
      }
      case State::Game: {
        _printInfo();
        std::cout << _board;

        std::cout << std::endl;

        std::cout << "Player [" << _currentPlayer + 1 << "] turn\n";
        uint32_t pos;
        while (true) {
          pos = _players[_currentPlayer]->askMove();

          if (_board.isToggled(pos)) {
            std::cout << "This position is already occupied\n";
          } else {
            break;
          }
        }
        _board.toggle(pos, _currentPlayer);
        if (_board.isEndGame(_currentPlayer)) {
          _state = State::EndGame;
        } else {
          _currentPlayer ^= 1;
          _turn += 1;
        }
        break;
      }
      case State::EndGame: {
        _printInfo();
        std::cout << _board;
        std::cout << std::endl;
        std::cout << "Player [" << _currentPlayer + 1 << "] wins\n";
        bool yes = _askYesOrNo("Restart");
        if (yes) {
          _state = State::Menu;
          _reset();
        } else {
          _state = State::Shutdown;
        }
        break;
      }
      case State::Shutdown:
        break;
      }
    }
  }

  State getState() const noexcept { return _state; }

private:
  void _reset() {
    _board.reset();
    _currentPlayer = 0;
    _turn = 0;
  }

  void _printInfo() {
    std::cout << "Current turn: " << _turn << std::endl;
    std::cout << "Player 1: X (top-down)\n";
    std::cout << "Player 2: O (left-right)\n";
    std::cout << std::endl;
  }

  gameBoardType _board;
  State _state = State::Menu;
  uint32_t _turn = 0;
  uint32_t _currentPlayer = 0;
  std::unique_ptr<playerType> _players[2];
};

int main(int argc, char* argv[]) {
  std::random_device rd;

  auto g = Game<11>(rd());
  g.run();

  return 0;
}