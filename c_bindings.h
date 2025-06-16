#pragma once

struct HMegolmMsg;
struct HDecryptionSession;

typedef struct HMegolmMsg HMegolmMsg;
typedef struct HDecryptionSession HDecryptionSession;

#ifdef __cplusplus
extern "C" {
#endif
HMegolmMsg *build_megolm_message(const char *message_b64);
void destroy_megolm_message(HMegolmMsg *message);

HDecryptionSession *build_decryption_session(const char *key_b64);
void destroy_decryption_session(HDecryptionSession *session);
#ifdef __cplusplus
}
#endif