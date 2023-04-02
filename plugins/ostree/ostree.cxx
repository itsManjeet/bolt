#include "Bolt.hxx"

extern "C" bool BOLT_PLUGIN(std::string const &query, bolt::Bolt *bot) {
    bot->respond("Hey");
    return true;
}