#pragma once

#include <cstdint>

#include "vodozemac.h"

namespace bridge {
    struct DecryptionSession {
        rust::Box<vodozemac::megolm::SessionKey> key;
        rust::Box<vodozemac::megolm::InboundGroupSession> session;

        DecryptionSession(const char* key_b64);
    };

    struct MegolmMsg {
        rust::Box<vodozemac::megolm::MegolmMessage> message;

        MegolmMsg(const char* message_b64);
    };
}