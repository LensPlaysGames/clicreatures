#include <vector>
#include <random>
#include <fstream>
#include <printf.h>

#include <creat.h>

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

Reality creat_init_reality() {
    // Persistent cards (populate .obtained).
    std::vector<IdAndCount> obtained{};
    std::ifstream infile;
    constexpr auto obtained_cards_path= "obtainedcards.dat";
    infile.open(obtained_cards_path, std::ios::binary | std::ios::in);
    if (infile.is_open()) {
        uint64_t count{0};
        infile.read((char*)&count, sizeof(decltype(count)));
        IdAndCount data{};
        for (decltype(count) i = 0; i < count; ++i) {
            infile.read((char*)&data, sizeof(IdAndCount));
            obtained.emplace_back(data);
        }
        infile.close();
    } else {
        printf("Could not open obtained cards data at \"%s\"\n", obtained_cards_path);
    }

    return {get_all_cards(), obtained};
}

void creat_save_obtained(const Reality& r) {
    std::ofstream outfile;
    constexpr auto obtained_cards_path= "obtainedcards.dat";
    outfile.open(obtained_cards_path, std::ios::binary | std::ios::out);
    if (outfile.is_open()) {
        printf("r.obtained.size(): %zu\n", r.obtained.size());
        uint64_t count{r.obtained.size()};
        outfile.write((char*)&count, sizeof(decltype(count)));
        for (auto data : r.obtained)
            outfile.write((char*)&data, sizeof(IdAndCount));

        outfile.close();
    } else {
        printf("Could not save obtained cards data at \"%s\"\n", obtained_cards_path);
    }
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
                out.emplace_back(c);
                break;
            }
        }
    }

    return out;
}
