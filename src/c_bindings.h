#pragma once

#include <stddef.h>

struct HMegolmMessage;
struct HDecryptionSession;

typedef struct HMegolmMessage HMegolmMessage;
typedef struct HDecryptionSession HDecryptionSession;

#ifdef __cplusplus
extern "C" {
#endif
HMegolmMessage *build_megolm_message(const char *message_b64);
void destroy_megolm_message(HMegolmMessage *message);

HDecryptionSession *build_decryption_session(const char *key_b64);
void destroy_decryption_session(HDecryptionSession *session);

struct DecryptedMessage {
    char* data;
    size_t len;
    size_t message_index;
};
typedef struct DecryptedMessage DecryptedMessage;

DecryptedMessage* decrypt(HDecryptionSession* session, HMegolmMessage* message);
void destroy_decrypted_message(DecryptedMessage* message);
#ifdef __cplusplus
}
#endif