
#include "bridge.hh"
#include "c_bindings.h"

#include <cstring>

namespace bridge {
    DecryptionSession::DecryptionSession(const char* key_b64):
        key(vodozemac::megolm::session_key_from_base64(key_b64)),
        session(vodozemac::megolm::new_inbound_group_session(*key)) {}

    MegolmMessage::MegolmMessage(const char *message_b64):
        message(vodozemac::megolm::megolm_message_from_base64(message_b64)) {}

} // namespace bridge


// Pure C bindings
extern "C" {
HMegolmMessage *build_megolm_message(const char *message_b64) {
    try {
        auto *message = new bridge::MegolmMessage(message_b64);
        return reinterpret_cast<HMegolmMessage *>(message);
    }
    catch (...) {
        return nullptr;
    }
}
void destroy_megolm_message(HMegolmMessage *message) {
    delete reinterpret_cast<bridge::MegolmMessage *>(message);
}

HDecryptionSession *build_decryption_session(const char *key_b64) {
    try {
        auto *session = new bridge::DecryptionSession(key_b64);
        return reinterpret_cast<HDecryptionSession *>(session);
    }
    catch (...) {
        return nullptr;
    }
}
void destroy_decryption_session(HDecryptionSession *session) {
    delete reinterpret_cast<bridge::DecryptionSession *>(session);
}

DecryptedMessage* decrypt(HDecryptionSession* session, HMegolmMessage* message) {
    // Assuming the args are valid

    auto& session_box = reinterpret_cast<bridge::DecryptionSession*>(session)->session;
    const auto& message_box = reinterpret_cast<bridge::MegolmMessage*>(message)->message;

    try {
        auto decrypted = session_box->decrypt(*message_box);
        auto& raw = decrypted.plaintext;
        const auto len = raw.size();

        auto* result = new DecryptedMessage();

        // Fill in the blanks
        try {
            result->data = new char[len];
            result->len = len;
            result->message_index = decrypted.message_index;
            std::memcpy(result->data, raw.data(), len);
            return result;
        }
        catch (std::bad_alloc&) {
            delete result;
            return nullptr;
        }
    }
    catch (...) {
        return nullptr;
    }
}
void destroy_decrypted_message(DecryptedMessage* message) {
    delete[] message->data;
    delete message;
}

}