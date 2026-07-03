#pragma once
#include <QString>

enum class Role { User, Assistant, System };

struct Message {
    Role    role;
    QString content;
    qint64  timestamp;
};