
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
enum CreatIdInvalid { InvalidID = uint64_t(-1) };

struct Card {
    creat_id_t id;
    CardType type;
    CardRarity rarity;
    // TODO: In the future, things like the name and description could be
    // stored in a hash table based on the card id. Same goes for attack lists
    // and things like that.
    Info info;
    std::vector<Attack> moves;

    explicit Card() {
        id = InvalidID;
    }

    Card(creat_id_t _id, CardType _type, CardRarity _rarity, std::string name, std::string description) {
        id = _id;
        type = _type;
        rarity = _rarity;
        info.name = name;
        info.description = description;
    }
};

struct IdAndCount { creat_id_t id; uint64_t quantity; };
struct NameAndCount { uint64_t quantity; std::string name; };

struct Recipe {
    creat_id_t id;
    std::vector<NameAndCount> output;
    std::vector<NameAndCount> input;

    explicit Recipe() { id = InvalidID; }

    Recipe(creat_id_t _id, std::vector<NameAndCount> _output,
           std::vector<NameAndCount> _input) {
        id = _id;
        output = _output;
        input = _input;
    }
};

struct Reality {
    // Card data.
    std::vector<Card> cards;

    // Recipe data.
    std::vector<Recipe> recipes;

    // The cards obtained.
    std::vector<IdAndCount> obtained;
};

Reality creat_init_reality();
// TODO: This should not be here.
void creat_save_obtained(const Reality& r);

std::vector<Card> creat_discover();

bool creat_craft(Reality &r, creat_id_t recipe_id);
creat_id_t
recipe_id_by_single_output_name(const Reality &r, std::string output_name);
