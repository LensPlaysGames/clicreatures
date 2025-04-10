#include <array>
#include <vector>

#include <creat.h>

std::vector<Card> creat_list(const Reality& r) {
    return r.cards;
}

void creat_add(Reality& r, const Card &card) {
    r.cards.push_back(card);
}

std::vector<Card> creat_discover() {

    // TODO: We need a better way to obtain the list of all possible cards here.
    const Card material_wood {
        CardType::Material,
        "Lempur Wood",
        "Wood from a Lempur tree, found in the Drestra region."
    };

    const Card material_grass {
        CardType::Material,
        "Sinch Grass",
        "Especially long and strong grass, found in the Drestra region. Great source of fiber."
    };

    const Card material_scara {
        CardType::Material,
        "Scara Beatle",
        "Large beatles found in the Drestra region. Sharp pincers and sticky claws make these beatles useful to inventors and tamers alike. Great source of protein."
    };

    const Card creature_caerbannog {
        CardType::Creature,
        "Caerbannog",
        "Innocent mouse-like bunny creature. Fits in the hand. Peaceful unless provoked. Extremely fatal, though often it's an eye for an eye."
    };

    const Card creature_soyboy {
        CardType::Creature,
        "Soyboy",
        "Anomaly with young soybean plants sometimes \"learning\" to walk. Territorial. Eats Scara Beatles, Sinch Grass, and Glow Ferns."
    };

    std::vector<Card> out;

    // TODO: Randomly select cards from pool of available cards.
    out.push_back(material_wood);
    out.push_back(material_grass);

    out.push_back(creature_caerbannog);
    out.push_back(creature_soyboy);

    return out;
}
