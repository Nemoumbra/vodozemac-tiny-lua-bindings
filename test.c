
#include "c_bindings.h"
#include <stdio.h>

int main() {
    const char* key_base64 = "AgAAAADjMeY85jzlVdbwXCKCk126hPGnoA9KNDedFdaat6i4nqtsCVN7nVPHuZuw931XV0NNipfrvCnyAU8yTe/9prdJteH0/FKak/XWyfJlfcisBCltKJvmj/3sKq+P/R5BuSmIgfNInLzBTfuzZvWkLkAPJYF46ysBx5o6jpK9cl9iHstlv9z+pII93sRUdpN74zW0JduDnBalAog6y2TKip9cIbPNjUxYkSGaCYV7vsRBq/JBjr825cZtZe+9O7GBVdzM18fuTJgFjM0eg7b2vjM6Df6106sc3MhDlmuTG+86DQ";
    const char* message_base64 = "AwgBEpABAMicubEndBOrixcOh1jC8w72L/tEnZjQnvQu7H30x8KfCeeIUG8djywJjFpVOH32mIxBtotcgdElsQEuyROUoSMHNLMkcAGgT72Zc2xiYHlXE0k3GsfIU5Xhq8UdFnT75MeXH2sFjdbMW6Ntyt7LnBIIf22jEMGlMMbk0kZqOs4xuHc/d1LynAZrRxinmQtTiZkxddn7+QgxYzANHhg9lK2u2oTyj+/EjnnSU+6G185Itpq8LxhbZEkynM58nAaRTiQhi+kRkSGqWCjcURRyB/vY5lCMjAsK";

    HMegolmMessage* message = build_megolm_message(message_base64);
    if (!message) {
        return -1;
    }

    HDecryptionSession* session = build_decryption_session(key_base64);
    if (!session) {
        destroy_megolm_message(message);
        return -1;
    }

    DecryptedMessage* result = decrypt(session, message);
    if (!result) {
        destroy_megolm_message(message);
        destroy_decryption_session(session);
        return -1;
    }

    printf("%.*s", result->len, result->data);

    destroy_megolm_message(message);
    destroy_decryption_session(session);
    destroy_decrypted_message(result);
}