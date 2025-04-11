// CLICreatures by Rylan Kellogg

#include <format>

#include <creat.h>

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
    Burn,
};

void discover(Reality& r) {
    auto cards = creat_discover();
    r.cards.insert(r.cards.end(), cards.begin(), cards.end());

    for (const auto &c : r.cards) {
        printf("%s", display_card_basic(c).data());
    }
}

void burn(Reality& r) {
    printf("TODO: burn cards");
}

int main(int argc, char** argv) {
    ProgramOperation program_op{ProgramOperation::Discover};

    if (argc > 1) {
        std::string operative_word = argv[1];
        if (operative_word == "burn") {
            program_op = ProgramOperation::Burn;
        }
        else if (operative_word == "discover") {
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
        break;

    case ProgramOperation::Burn:
        burn(r);
        break;
    }
    return 0;
}
