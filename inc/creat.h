
#include <vector>
#include <string>
#include <cstdint>

struct Info {
    std::string name;
    std::string description;
};

// I think the idea with attacks is that it will add events to the event
// queue. Some attacks will add just one (the damaging effect of the
// attack), while others will have multiple events added (poison for three
// turns, rebound damage that bounces back next turn to the attacker, etc).
struct Attack {
    Info info;
    uint16_t damage;
    uint8_t cost;
};

enum class CardType : uint8_t {
    Creature,
    Material,
};

enum class CardRarity : uint8_t {
    Common,
    Exciting,
    UltraRare,
    Legendary,
};

struct Card {
    uint64_t id;
    CardType type;
    CardRarity rarity;
    // TODO: In the future, things like the name and description could be
    // stored in a hash table based on the card id. Same goes for attack lists
    // and things like that.
    Info info;
    std::vector<Attack> moves;

    Card(CardType _type, CardRarity _rarity, std::string name, std::string description) {
        type = _type;
        rarity = _rarity;
        info.name = name;
        info.description = description;
    }
};

struct Reality {
    std::vector<Card> cards;
};

Reality creat_init_reality();

std::vector<Card> creat_list(const Reality&);
void creat_add(Reality&, const Card&);
// void creat_remove(const Card& card);

std::vector<Card> creat_discover();
