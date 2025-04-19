// CLICreatures by Rylan Kellogg

#include <format>

#include <creat.h>

[[nodiscard]]
std::string display_card_basic(const Card &card) {
    std::string out;

    switch (card.rarity) {
    case CardRarity::Common: break;
    case CardRarity::Exciting:
        out += "!! ";
        break;
    case CardRarity::UltraRare:
        out += "!RARE! ";
        break;
    case CardRarity::Legendary:
        out += "!LEGENDARY! ";
        break;
    }

    switch (card.type) {
    case CardType::Creature:
        out += "Creature: ";
        break;
    case CardType::Material:
        out += "Material: ";
        break;
    }

    out += card.info.name;
    out += ": ";
    out += card.info.description;

    out += '\n';
    return out;
}

enum class ProgramOperation {
    Discover,
    List,
    Burn,
    Craft,
};

void discover(Reality& r) {
    auto obtained_cards = creat_discover();
    for (const auto &c : obtained_cards) {
        printf("%s", display_card_basic(c).data());
        // Find id in r.obtained, add to quantity, OR, if not in r.obtained,
        // add new entry with quantity of 1 to r.obtained. Smells like a set.
        bool found{false};
        for (auto& x : r.obtained) {
          if (x.id == c.id) {
              found = true;
              ++x.quantity;
          }
        }
        if (not found) {
            r.obtained.emplace_back(IdAndCount{c.id, 1});
        };
    }
}

void list(const Reality &r) {
    for (auto x : r.obtained) {
        // Skip empty entries.
        if (x.quantity == 0) continue;

        // Find card info by ID.
        for (const auto& c : r.cards) {
            if (c.id == x.id) {
                printf("%6zux  %s", x.quantity, display_card_basic(c).data());
                break;
            }
        }
    }
}

void burn(Reality& r) {
    printf("TODO: burn cards\n");
}

void craft(Reality& r, std::string output_name) {
    auto recipe_id = recipe_id_by_single_output_name(r, output_name);
    if (recipe_id == InvalidID) {
        printf("Could not find recipe with output named \"%s\"\n", output_name.data());
        return;
    }
    auto success = creat_craft(r, recipe_id);
    if (not success) {
        printf("Could not craft!\n");
        return;
    }
    printf("Successfully crafted \"%s\" (recipe id %u)\n", output_name.data(), (unsigned)recipe_id);
}

int main(int argc, char** argv) {
    ProgramOperation program_op{ProgramOperation::Discover};

    if (argc > 1) {
        std::string operative_word = argv[1];
        if (operative_word == "burn") {
            program_op = ProgramOperation::Burn;
        }
        else if (operative_word == "craft") {
            program_op = ProgramOperation::Craft;
        }
        else if (operative_word == "list") {
            program_op = ProgramOperation::List;
        } else if (operative_word == "discover") {
            program_op = ProgramOperation::Discover;
        } else {
            printf("ERROR: Invalid argument in operative word position: \"%s\"\n", operative_word.data());
            return 1;
        }
    }

    std::vector<std::string> args;
    for (int arg_counter = 2; argc > arg_counter; ++arg_counter) {
        std::string arg = argv[arg_counter];
        args.emplace_back(arg);
    }

    auto r = creat_init_reality();

    switch (program_op) {
    case ProgramOperation::Discover:
        if (not args.empty())
            printf("ERROR: Ignoring arguments given, as discover operative word takes no arguments.\n");
        discover(r);
        creat_save_obtained(r);
        break;

    case ProgramOperation::List:
        if (not args.empty())
            printf("ERROR: Ignoring arguments given, as list operative word takes no arguments.\n");
        list(r);
        break;

    case ProgramOperation::Burn:
        burn(r);
        creat_save_obtained(r);
        break;

    case ProgramOperation::Craft:
        if (args.empty()) {
            printf("TODO: Display recipes\n");
            break;
        }
        if (args.size() > 1) {
            printf("ERROR: Ignoring additional arguments given, as craft operative word takes one argument.\nYou may want to put quotes around them, if you meant to type one name with multiple words.\n");
        }
        craft(r, args.at(0));
        creat_save_obtained(r);
        break;
    }
    return 0;
}
