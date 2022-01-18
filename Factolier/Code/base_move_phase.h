#pragma once

#include <DirectXMath.h>
#include <functional>
#include <map>
#include <memory>

#include "base_move.h"



template<typename Enemy>
class Move_Phase final
{
private:
    using Move_State        = Base_Move<Enemy>;
    using ptr_Move_State    = Move_State*;

    using transition_func   = std::function<bool(float)>;
    using initialize_func   = std::function<void()>;
    using finalize_func     = initialize_func;

public:
    struct Transition final
    {
        ptr_Move_State next_move    = {};               // 遷移先
        transition_func Is          = {};               // 遷移条件
    };

    struct MoveData final
    {
        size_t unique_id = {};                          // 識別番号

        std::unique_ptr<Move_State> move = {};           // 更新する内容

        initialize_func initialize = {};                // 初期化処理
        finalize_func   finalize = {};                  // 終了処理

        std::vector<Transition> transitions = {};       // 次の遷移先とその条件
    };

public:
    Move_Phase() = default;
    ~Move_Phase() = default;

public:
    void update(float elapsed_time_, Enemy& type_)
    {
        for (auto& transition : moves[current_move_index].transitions)
        {
            // 遷移条件がそろっていたら遷移
            if (transition.Is(elapsed_time_) == false) continue;

            // 現在の行動の終了処理
            if (moves[current_move_index].finalize) moves[current_move_index].finalize();
            // 更新する行動の番号に変更
            current_move_index = find_move_index(transition.next_move);
            // 変更した行動の初期化処理
            if (moves[current_move_index].initialize) moves[current_move_index].initialize();

            break;
        }

        // 現在の行動で更新する
        moves[current_move_index].move->update(elapsed_time_, type_);
    }

public:
    Move_Phase& set(ptr_Move_State move_)
    {
        const int index = static_cast<int>(find_move_index(move_));
        if (index != -1)
        {
            current_move_index = static_cast<size_t>(index);
            return *this;
        }

        try_add_move(move_);
        current_move_index = moves.size() - 1;

        return *this;
    }

    Move_Phase& add_transition(ptr_Move_State from, ptr_Move_State to, const transition_func& Is_)
    {

        try_add_move(to);
        try_add_move(from).transitions.emplace_back(Transition{ to,Is_ });
        return *this;
    }

    Move_Phase& set_initialize(ptr_Move_State move_, const initialize_func& initialize)
    {
        try_add_move(move_).initialize = initialize;
        return *this;
    }

    Move_Phase& set_finalize(ptr_Move_State move_, const finalize_func& finalize)
    {
        try_add_move(move_).finalize = finalize;
        return *this;
    }

    size_t& get_now_move() { return moves[current_move_index].unique_id; }

private:
    MoveData& try_add_move(ptr_Move_State move_)
    {
        const size_t index = find_move_index(move_);
        if(index != -1)
        {
            return moves.at(index);
        }

        std::unique_ptr<Move_State> move;
        move.reset(move_);
        moves.emplace_back(MoveData{ moves_size++,std::move(move) });
        return moves.back();
    }

    size_t find_move_index(const ptr_Move_State move)
    {
        const auto it = std::find_if(moves.begin(), moves.end(), [&move](const MoveData& data) {return move == data.move.get(); });
        if (it == moves.end()) return -1;

        return it - moves.begin();
    }

private:
    size_t moves_size;
    size_t current_move_index;
    std::vector<MoveData> moves;
};