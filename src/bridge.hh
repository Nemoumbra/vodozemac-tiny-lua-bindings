#pragma once

#include <cstdint>

#include "vodozemac.h"

namespace bridge {
    struct DecryptionSession {
        rust::Box<vodozemac::megolm::SessionKey> key;
        rust::Box<vodozemac::megolm::InboundGroupSession> session;

        explicit DecryptionSession(const char* key_b64);
    };

    struct MegolmMessage {
        rust::Box<vodozemac::megolm::MegolmMessage> message;

        explicit MegolmMessage(const char* message_b64);
    };


}