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

int main(int argc, char** argv) {
    auto r = creat_init_reality();

    auto cards = creat_discover();
    r.cards.insert(r.cards.end(), cards.begin(), cards.end());

    for (const auto &c : r.cards) {
        printf("%s", display_card_basic(c).data());
    }

    return 0;
}
