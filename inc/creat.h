
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

typedef uint64_t creat_id_t;

struct Card {
    creat_id_t id;
    CardType type;
    CardRarity rarity;
    // TODO: In the future, things like the name and description could be
    // stored in a hash table based on the card id. Same goes for attack lists
    // and things like that.
    Info info;
    std::vector<Attack> moves;

    Card(creat_id_t _id, CardType _type, CardRarity _rarity, std::string name, std::string description) {
        id = _id;
        type = _type;
        rarity = _rarity;
        info.name = name;
        info.description = description;
    }
};

struct IdAndCount { creat_id_t id; uint64_t quantity; };

struct Reality {
    // Card data.
    // TODO: Vector/Hash table by id, depending on number of elements.
    std::vector<Card> cards;
    // The cards obtained.
    std::vector<IdAndCount> obtained;
};

Reality creat_init_reality();
// TODO: This should not be here.
void creat_save_obtained(const Reality& r);

std::vector<Card> creat_list(const Reality&);
void creat_add(Reality&, const Card&);
// void creat_remove(const Card& card);

std::vector<Card> creat_discover();
