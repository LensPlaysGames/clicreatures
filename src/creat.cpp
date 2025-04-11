#include <vector>
#include <random>
#include <printf.h>

#include <creat.h>

Reality creat_init_reality() {
    // TODO: Persistent cards
    return {};
}

std::vector<Card> creat_list(const Reality& r) {
    return r.cards;
}

void creat_add(Reality& r, const Card &card) {
    r.cards.push_back(card);
}

std::vector<Card> get_all_cards() {
    std::vector<Card> out{
#include <all_cards.txt>
    };
    return out;
}

constexpr unsigned long convert_rarity_to_weight(CardRarity rarity) {
    switch (rarity) {
    case CardRarity::Common: return 900;
    case CardRarity::Exciting: return 90;
    case CardRarity::UltraRare: return 9;
    case CardRarity::Legendary: return 1;
    }
    return 0;
}

std::vector<Card> creat_discover() {
    auto all_cards = get_all_cards();

    // Confidence Check
    if (all_cards.empty()) {
        fprintf(stderr, "ERROR: All cards vector is empty... how can we discover cards if there are none to discover?\n");
        return {};
    }

    // Randomly select cards from pool of available cards.

    // TODO: A rope data structure would mean we don't have to recalc weights
    // and stuff. Perfect for walking, too.
    unsigned long total_weight{0};
    for (const Card &c : all_cards) {
        total_weight += convert_rarity_to_weight(c.rarity);
    }

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, total_weight);

    std::vector<Card> out;

    constexpr int discover_amount = 4;
    for (int i = 0; i < discover_amount; ++i) {
        // Generate a random weight.
        int random_number = dist(rng);
        // Iterate cards until we get to a card that exhausts the randomized weight.
        for (const Card &c : all_cards) {
            random_number -= convert_rarity_to_weight(c.rarity);
            if (random_number <= 0) {
                out.push_back(c);
                break;
            }
        }
    }

    return out;
}
