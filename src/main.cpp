// CLICreatures by Rylan Kellogg

#include <format>

#include <creat.h>

std::string display_card_basic(const Card &card) {
    std::string out;

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

int main(int argc, char** argv) {
    auto cards = creat_discover();
    for (const auto &c : cards) {
        printf("%s", display_card_basic(c).data());
    }
}
