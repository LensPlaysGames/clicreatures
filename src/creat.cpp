#include <vector>
#include <random>
#include <fstream>
#include <printf.h>

#include <creat.h>

// Check return value against the invalid ID.
creat_id_t card_id_by_name(const Reality& r, std::string_view name) {
    // TODO: We could probably implement a cache lookup here to immensely
    // speed up this process (if it's even slow in the first place).

    for (auto c : r.cards) {
        if (c.info.name == name) {
          return c.id;
        }
    }

    return InvalidID;
}

// Check returned recipe's ID against the invalid ID.
Recipe recipe_by_id(const Reality& r, creat_id_t id) {
    for (auto r : r.recipes) {
        if (r.id == id) {
          return r;
        }
    }

    return Recipe{};
}

// Check returned card's ID against the invalid ID.
Card card_by_id(const Reality& r, creat_id_t id) {
    for (auto c : r.cards) {
        if (c.id == id) {
          return c;
        }
    }

    return Card{};
}

uint64_t in_obtained(const Reality &r, creat_id_t id) {
    for (auto x : r.obtained) {
        if (x.id == id) return x.quantity;
    }
    return 0;
}

std::vector<Card> get_all_cards() {
    uint32_t next_id{0};
    auto id = [&]() { return ++next_id; };
    std::vector<Card> out{
#include <all_cards.txt>
    };
     return out;
}

std::vector<Recipe> get_all_recipes() {
    uint32_t next_id{0};
    auto id = [&]() { return ++next_id; };
    std::vector<Recipe> out{
#include <recipes.txt>
    };
     return out;
}

creat_id_t
recipe_id_by_single_output_name(const Reality &r, std::string output_name) {
    creat_id_t out{InvalidID};
    bool ambiguous{false};
    for (auto recipe : r.recipes) {
        if (recipe.output.size() == 1
            and recipe.output.at(0).name == output_name) {
            if (out != InvalidID) {
                printf("Ambiguous recipe selection: multiple recipes with single output named \"%s\"\n", output_name.data());
                return InvalidID;
            }
            out = recipe.id;
        }
     }
    if (ambiguous) return InvalidID;
    return out;
}

bool creat_craft(Reality &r, creat_id_t recipe_id) {
    auto recipe_data = recipe_by_id(r, recipe_id);
    std::vector<IdAndCount> processed_inputs{};
    processed_inputs.reserve(recipe_data.input.capacity());
    for (auto input : recipe_data.input) {
        auto input_id = card_id_by_name(r, input.name);

        // If input quantity is greater than the amount we have stored in
        // obtained, then we fail to craft.
        if (input.quantity > in_obtained(r, input_id)) {
            // TODO: Encode this failure data into return value
            printf("Failed to craft due to lack of input \"%s\"\n", input.name.data());
            return false;
        }

        processed_inputs.emplace_back(IdAndCount{input_id, input.quantity});
    }

    // TODO: We could do both at once without looping twice if we just keep
    // track of increment vs decrement and how much for each input or output
    // card id.

    // Reduce quantities of recipe inputs.
    for (auto input : processed_inputs) {
        for (auto& x : r.obtained) {
            if (x.id == input.id)
                x.quantity -= input.quantity;

            // TODO: Remove entry from obtained if quantity is zero. Or just make
            // obtained always have every id stored, even zero quantities.
        }
    }

    for (auto output : recipe_data.output) {
        auto output_id = card_id_by_name(r, output.name);

        // Find id in r.obtained, add to quantity, OR, if not in r.obtained,
        // add new entry to r.obtained.
        bool found{false};
        for (auto& x : r.obtained) {
          if (x.id == output_id) {
              found = true;
              x.quantity += output.quantity;
          }
        }
        if (not found) {
            r.obtained.emplace_back(IdAndCount{output_id, output.quantity});
        };
    }

    return true;
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

    return {get_all_cards(), get_all_recipes(), obtained};
}

void creat_save_obtained(const Reality& r) {
    std::ofstream outfile;
    constexpr auto obtained_cards_path= "obtainedcards.dat";
    outfile.open(obtained_cards_path, std::ios::binary | std::ios::out);
    if (outfile.is_open()) {
        //printf("r.obtained.size(): %zu\n", r.obtained.size());
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
