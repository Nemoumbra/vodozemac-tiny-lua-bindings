
#include "bridge.hh"
#include "c_bindings.h"

namespace bridge {
    DecryptionSession::DecryptionSession(const char* key_b64):
        key(vodozemac::megolm::session_key_from_base64(key_b64)),
        session(vodozemac::megolm::new_inbound_group_session(*key)) {}

    MegolmMsg::MegolmMsg(const char *message_b64):
        message(vodozemac::megolm::megolm_message_from_base64(message_b64)) {}

} // namespace bridge


// Pure C bindings
extern "C" {
HMegolmMsg *build_megolm_message(const char *message_b64) {
    auto *message = new bridge::MegolmMsg(message_b64);
    return reinterpret_cast<HMegolmMsg *>(message);
}
void destroy_megolm_message(HMegolmMsg *message) {
    delete reinterpret_cast<bridge::MegolmMsg *>(message);
}

HDecryptionSession *build_decryption_session(const char *key_b64) {
    auto *session = new bridge::DecryptionSession(key_b64);
    return reinterpret_cast<HDecryptionSession *>(session);
}
void destroy_decryption_session(HDecryptionSession *session) {
    delete reinterpret_cast<bridge::DecryptionSession *>(session);
}
}