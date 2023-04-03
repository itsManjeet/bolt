#include <Bolt.hxx>

using namespace bolt;

#include "ApiCall.hxx"

PLUGIN(joke) {
    std::vector<std::string> jokes = {
            "Why did the tomato turn red? Because it saw the salad dressing!",
            "Why don't scientists trust atoms? Because they make up everything.",
            "Why don't oysters share their pearls? Because they're shellfish.",
            "What did the grape say when it got stepped on? Nothing, it just let out a little wine.",
            "Why did the coffee file a police report? It got mugged.",
            "Why was the math book sad? Because it had too many problems.",
            "What do you call a fake noodle? An impasta.",
            "Why don't skeletons fight each other? They don't have the guts.",
            "What do you call a bear with no teeth? A gummy bear.",
            "Why did the banana go to the doctor? Because it wasn't peeling well.",
            "Why did the chicken cross the playground? To get to the other slide.",
            "What did the janitor say when he jumped out of the closet? \"Supplies!\"",
            "Why did the bicycle fall over? Because it was two-tired.",
            "What did one hat say to the other? \"You stay here, I'll go on ahead.\"",
            "Why did the scarecrow win an award? Because he was outstanding in his field.",
            "How do you organize a space party? You planet.",
            "Why did the cookie go to the doctor? Because it was feeling crummy.",
            "What did one wall say to the other? \"I'll meet you at the corner.\"",
            "Why don't scientists trust atoms? Because they make up everything.",
            "What do you get when you cross a snowman and a shark? Frostbite."
    };
    if (ApiCall().call(
            bolt->recall<std::string>("rlxos.url", "https://api.rlxos.dev/v1/check"))
        ["data"].get<std::string>() == "online") {
        auto api = bolt->recall<std::string>("joke.api", "https://api.rlxos.dev/v1/joke");
        auto request = ApiCall()
                .append("Accept: application/json")
                .append("Content-Type: application/json")
                .call(api, ApiCall::GET);
        if (request.contains("data")) {
            jokes.clear();
            for(auto const& i : request["data"].items()) {
                jokes.push_back(i.value().get<std::string>());
            }
        }
    }

    bolt->say(jokes);
    return true;
}