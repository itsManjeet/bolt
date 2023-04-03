#include "Bolt.hxx"
#include "nlp.hxx"

using namespace bolt;

PLUGIN(ostree) {
    auto tokens = nlp::tokenize(sentence);
    auto remote_name = bolt->recall<std::string>("ostree.remote", "rlxos");
    if (tokens.size() == 2 && tokens[0] == "switch") {
        auto branch = tokens[1];
        bolt->say("switch branch to " + branch);
        if (WEXITSTATUS(system(("pkexec /bin/ostree pull " + remote_name + " " + branch).c_str())) != 0) {
            bolt->say("failed to pull ref:" + branch + " from remote '" + remote_name + "', please verify the branch name");
            return true;
        }

        if (WEXITSTATUS(system(("pkexec /bin/ostree admin deploy " + remote_name + ":" + branch).c_str())) != 0) {
            bolt->say("failed to deploy ref:" + branch);
            return true;
        }
        bolt->say("branch switch successfully, please reboot your system to apply changes");
        return true;
    } else if (nlp::contains_any(sentence, {"update", "upgrade"})) {
        // ostree update
        bolt->say("upgrading system");
        if (WEXITSTATUS(system(("pkexec /bin/ostree admin upgrade"))) != 0) {
            bolt->say("failed to get system updates");
            return false;
        }
        return true;
    }
    return false;
}