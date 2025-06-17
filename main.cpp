#include <iostream>

#include "vodozemac.h"


void test() {
    const auto key_base64 = "AgAAAADjMeY85jzlVdbwXCKCk126hPGnoA9KNDedFdaat6i4nqtsCVN7nVPHuZuw931XV0NNipfrvCnyAU8yTe/9prdJteH0/FKak/XWyfJlfcisBCltKJvmj/3sKq+P/R5BuSmIgfNInLzBTfuzZvWkLkAPJYF46ysBx5o6jpK9cl9iHstlv9z+pII93sRUdpN74zW0JduDnBalAog6y2TKip9cIbPNjUxYkSGaCYV7vsRBq/JBjr825cZtZe+9O7GBVdzM18fuTJgFjM0eg7b2vjM6Df6106sc3MhDlmuTG+86DQ";
    const auto message_base64 = "AwgBEpABAMicubEndBOrixcOh1jC8w72L/tEnZjQnvQu7H30x8KfCeeIUG8djywJjFpVOH32mIxBtotcgdElsQEuyROUoSMHNLMkcAGgT72Zc2xiYHlXE0k3GsfIU5Xhq8UdFnT75MeXH2sFjdbMW6Ntyt7LnBIIf22jEMGlMMbk0kZqOs4xuHc/d1LynAZrRxinmQtTiZkxddn7+QgxYzANHhg9lK2u2oTyj+/EjnnSU+6G185Itpq8LxhbZEkynM58nAaRTiQhi+kRkSGqWCjcURRyB/vY5lCMjAsK";

    auto message = vodozemac::megolm::megolm_message_from_base64(message_base64);
    auto key = vodozemac::megolm::session_key_from_base64(key_base64);
    auto session = vodozemac::megolm::new_inbound_group_session(*key);

    auto result = session->decrypt(*message);
    auto plaintext = result.plaintext;

    std::string text(plaintext.begin(), plaintext.end());

    std::cout << text;
    1 + 2;
}

int main() {
    test();
}
