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
            "What do you get when you cross a snowman and a shark? Frostbite.",
            "Why don't scientists trust atoms? Because they make up everything!",
            "Why couldn't the bicycle stand up by itself? Because it was two-tired!",
            "Why don't seagulls fly over the bay? Because then they would be called bagels!",
            "What did one ocean say to the other ocean? Nothing, they just waved!",
            "What did the astronaut use to keep his pants up? An asteroid belt!",
            "Why did the tomato turn red? Because it saw the salad dressing!",
            "How do you catch a squirrel? Climb a tree and act like a nut!",
            "What's a skeleton's least favorite room in the house? The living room!",
            "Why don't skeletons fight each other? They don't have the guts!",
            "Why did the scarecrow win an award? Because he was outstanding in his field!",
            "Why don't eggs tell jokes? Because they might crack up!",
            "What do you call fake spaghetti? An impasta!",
            "Why was the math book sad? Because it had too many problems!",
            "How does a penguin build its house? Igloos it together!",
            "Why did the tomato turn green? Because it saw the salad dressing!",
            "What did the left eye say to the right eye? Between us, something smells!",
            "Why did the scarecrow get promoted? Because he was outstanding in his field!",
            "Why don't cows have any money? Because farmers milk them dry!",
            "Why don't scientists trust atoms anymore? Because they found out they make up everything!",
            "What's a ninja's favorite type of shoes? Sneakers!",
            "How does a penguin catch a fish? With its beak and waddle!",
            "Why do fish never do well in school? Because they swim below \"sea\" level!",
            "What's the difference between a snowman and a snowwoman? Snowballs!",
            "Why did the tomato turn yellow? Because it saw the banana split!",
            "Why couldn't the bicycle stand up by itself? It was two-tired!",
            "Why did the scarecrow win an award? Because he was outstanding in his scare!",
            "What's the best way to catch a squirrel? Climb a tree and act like a nut!",
            "Why don't hummingbirds hum? Because they don't know the words!",
            "What do you call a dinosaur with an extensive vocabulary? A thesaurus!",
            "Why do hummingbirds hum? Because they don't know the words!",
            "What's orange and sounds like a parrot? A carrot!",
            "Why was the belt arrested? It was holding up a pair of pants!",
            "What's a skeleton's favorite musical instrument? A trombone!",
            "Why did the tomato turn blue? It saw the salad dressing!",
            "How do you organize a space party? You planet!",
            "Why don't skeletons fight each other? They don't have the guts!",
            "What did the buffalo say to his son when he left for college? Bison!",
            "Why are ghosts bad at lying? Because they are too easily spotted!",
            "What's a vampire's favorite fruit? A neck-tarine!",
            "Why was the math book sad? Because it had too many problems!",
            "Why did the computer go to art school? To improve its byte!",
            "Why did the tomato turn red? Because it saw the salad dressing!",
            "What do you call a dinosaur with an extensive vocabulary? A thesaurus!",
            "Why don't dogs make good dancers? Because they have two left feet!",
            "What's the difference between a snowman and a snowwoman? Snowballs!",
            "Why was the math test sad? It had too many problems!",
            "Why did the tomato turn green? Because it saw the salad dressing!",
            "What do you call a pile of cats? A meowtain!",
            "Why do scientists call helium, curium, and barium the medical elements? Because if you can't helium or curium, you barium!",
            "Why was the math book sad? It had too many problems!"
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