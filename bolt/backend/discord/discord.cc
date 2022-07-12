#include "discord.hh"
using namespace bolt::backend;

#include <algorithm>
#include <regex>

bool Discord::start(Bolt* bolt) {
  if (!mNode["token"]) {
    mError = "no token specified";
    return false;
  }

  std::string token = mNode["token"].as<std::string>();
  mBackend = std::make_shared<dpp::cluster>(token);

  if (mNode["logging"] && mNode["logging"].as<bool>()) {
    mBackend->on_log(dpp::utility::cout_logger());
  }

  long author_id = -1;
  if (mNode["author-id"]) {
    author_id = mNode["author-id"].as<long>();
  }

  mBackend->on_message_create([&](const dpp::message_create_t& event) {
    auto iter = std::find_if(
        event.msg.mentions.begin(), event.msg.mentions.end(),
        [&](std::pair<dpp::user, dpp::guild_member> const& d) -> bool {
          return d.first.id == mBackend->me.id;
        });

    if (iter == event.msg.mentions.end()) {
      return;
    }

    std::string sentence = event.msg.content;
    std::regex rgx("<@[^>]*>");

    sentence = std::regex_replace(sentence, rgx, "");
    auto [score, prediction] = bolt->predict(sentence);
    mBackend->message_create(dpp::message(event.msg.channel_id, prediction));
  });

  mBackend->on_ready([&](const dpp::ready_t& event) {

  });

  mBackend->start(false);
  return true;
}