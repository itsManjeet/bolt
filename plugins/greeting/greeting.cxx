#include "Bolt.hxx"

using namespace bolt;

#define LAST_RESPONSE_ID "greeting.last-response"
PLUGIN(greeting) {
    auto last_response = bolt->recall<std::string>(LAST_RESPONSE_ID, "");

    const std::string first_response = "Hey human, My name is Bolt and I am here to help you, as I am a completely personal virtual assistant that respect your privacy"
                                       " you are free to ask me anything."
                                       "\n"
                                       "Instead of just responding some of your queries I also can perform actions to help you automate your tasks.\n"
                                       "However i come with a very less knowledge, hence you need to train me to make get more from me.\n"
                                       "Please visit https://rlxos.dev/wiki/bolt/ to know more about me.";
    if (last_response.empty()) {
        bolt->say(first_response);
        bolt->remember(LAST_RESPONSE_ID, first_response);
        return true;
    }
    // TODO: handle last response
    bolt->say({
                      "hey",
                      "hello",
                      "how are you",
              });
    return true;
}