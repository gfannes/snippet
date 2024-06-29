#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

using Id = unsigned int;
using Ix = std::size_t;
using Count = unsigned int;
using Slot = unsigned int;
using Hour = unsigned int;

const Count slot_count = 32;
const Count max_occupation = 25;

struct Course
{
    using Id = Id;

    Id id;
    std::string name;
    Count hours;

    static Course make(std::string name, Count hours)
    {
        static Id g_id = 0;
        return Course{.id = g_id++, .name = name, .hours = hours};
    }

    bool operator==(const Course &rhs) const { return id == rhs.id; }
};
std::ostream &operator<<(std::ostream &os, const Course &course)
{
    os << "[Course](id:" << course.id << ")(name:" << course.name << ")(hours:" << course.hours << ")";
    return os;
}

const Course aarderijkskunde_1 = Course::make("ard", 1);
const Course aarderijkskunde_2 = Course::make("ard", 2);
const Course biologie_1 = Course::make("bio", 1);
const Course biologie_2 = Course::make("bio", 2);
const Course chemie_1 = Course::make("che", 1);
const Course chemie_2 = Course::make("che", 2);
const Course duits = Course::make("dui", 3);
const Course economie = Course::make("eco", 5);
const Course engels_2 = Course::make("eng", 2);
const Course engels_3 = Course::make("eng", 3);
const Course esthetica_1 = Course::make("est", 1);
const Course esthetica_2 = Course::make("est", 2);
const Course filosofie = Course::make("fil", 2);
const Course frans_3 = Course::make("fra", 3);
const Course frans_4 = Course::make("fra", 4);
const Course frans_5 = Course::make("fra", 5);
const Course fysica_2 = Course::make("fys", 2);
const Course fysica_3 = Course::make("fys", 3);
const Course geschiedenis = Course::make("ges", 2);
const Course godsdienst = Course::make("god", 2);
const Course grieks = Course::make("gri", 4);
const Course kunstbeschouwing = Course::make("kun", 2);
const Course latijn = Course::make("lat", 4);
const Course lo = Course::make("lop", 2);
const Course natuurweteschappen = Course::make("nat", 2);
const Course nederlands = Course::make("ned", 4);
const Course sociale = Course::make("soc", 5);
const Course spaans = Course::make("spa", 3);
const Course statistiek = Course::make("sta", 1);
const Course methodiek = Course::make("met", 2);
const Course wiskunde_3 = Course::make("wis", 3);
const Course wiskunde_4 = Course::make("wis", 4);
const Course wiskunde_6 = Course::make("wis", 6);
const Course wiskunde_8 = Course::make("wis", 8);

struct Lesson
{
    static std::vector<Lesson> all;

    using Ix = Ix;

    Course course;
    Hour hour;

    bool operator==(const Lesson &rhs) const { return course == rhs.course && hour == rhs.hour; }

    std::ostream &write(std::ostream &os, std::optional<Ix> ix = {}) const
    {
        os << "[Lesson]";
        if (ix)
            os << "(ix:" << *ix << ")";
        os << "(course:" << course << ")(hour:" << hour << ")";
        return os;
    }
};
std::vector<Lesson> Lesson::all;
std::ostream &operator<<(std::ostream &os, const Lesson &lesson)
{
    return lesson.write(os);
}

std::vector<Lesson::Ix> make_lessons(std::initializer_list<Course> courses)
{
    std::vector<Lesson::Ix> ret;
    for (auto course : courses)
    {
        for (Hour hour = 0u; hour < course.hours; ++hour)
        {
            const Lesson lesson{.course = course, .hour = hour};
            auto it = std::find(Lesson::all.begin(), Lesson::all.end(), lesson);
            if (it == Lesson::all.end())
            {
                Lesson::all.push_back(lesson);
                it = Lesson::all.end() - 1;
            }
            ret.push_back(it - Lesson::all.begin());
        }
    }
    auto cmp = [](const auto a, const auto b) {
        const Lesson &aa = Lesson::all[a];
        const Lesson &bb = Lesson::all[b];
        return aa.course.hours > bb.course.hours;
    };
    std::sort(ret.begin(), ret.end(), cmp);
    return ret;
}

struct Class
{
    std::string name;
    Count students;
    std::vector<Lesson::Ix> lesson_ixs;

    std::ostream &write(std::ostream &os) const
    {
        os << "[Class](name:" << name << ")(students:" << students << ")(lessons:" << lesson_ixs.size() << "){" << std::endl;
        for (auto ix : lesson_ixs)
        {
            const Lesson &lesson = Lesson::all[ix];
            os << "\t";
            lesson.write(os, ix) << std::endl;
        }
        os << "}";
        return os;
    }
};
std::ostream &operator<<(std::ostream &os, const Class &cl)
{
    return cl.write(os);
}

struct Room
{
    using Ix = Ix;

    struct Info
    {
        std::optional<Lesson::Ix> lesson_ix;
        Count occupation{};
    };
    std::vector<Info> slot__info = std::vector<Info>(slot_count);
};

struct Schedule
{
    std::vector<Room> rooms;
    std::vector<Class> classes;
    using Slot__Room = std::vector<std::optional<Room::Ix>>;
    std::vector<Slot__Room> class__slot__room;

    bool init(const std::vector<Class> &classes)
    {
        for (const auto &cl : classes)
            if (cl.lesson_ixs.size() != slot_count)
                return (std::cout << "Unexpected slot count for class " << cl << std::endl, false);

        this->classes = classes;
        class__slot__room.clear();
        class__slot__room.resize(this->classes.size());
        for (auto &slot__room : class__slot__room)
            slot__room.resize(slot_count);

        return true;
    }

    bool naive()
    {
        using ClassLesson = std::pair<Ix, Ix>;
        std::vector<ClassLesson> class_lessons;
        for (Ix clix{}; clix < classes.size(); ++clix)
        {
            const Class &cl = classes[clix];
            for (auto &lesson_ix : cl.lesson_ixs)
            {
                class_lessons.push_back({clix, lesson_ix});
            }
        }

        auto cmp = [](const auto &a, const auto &b) {
            const Lesson &aa = Lesson::all[a.second];
            const Lesson &bb = Lesson::all[b.second];
            return std::make_pair(bb.course.hours, bb.hour) < std::make_pair(aa.course.hours, aa.hour);
        };
        std::sort(class_lessons.begin(), class_lessons.end(), cmp);

        for (const auto [clix, lesson_ix] : class_lessons)
        {
            const Class &cl = classes[clix];
            auto &slot__room = class__slot__room[clix];
            const auto [slot, room_ix] = fit(lesson_ix, cl.students, slot__room);
            if (slot__room[slot])
                return (std::cout << "Slot " << slot << " is already occupied" << std::endl, false);
            slot__room[slot] = room_ix;
            if (room_ix >= rooms.size())
                rooms.resize(room_ix + 1);
            auto &info = rooms[room_ix].slot__info[slot];
            info.lesson_ix = info.lesson_ix.value_or(lesson_ix);
            if (*info.lesson_ix != lesson_ix)
                return (std::cout << "Room " << room_ix << " is already used for a different lesson" << std::endl, false);
            info.occupation += cl.students;
        }
        return true;
    }

    std::pair<Slot, Room::Ix> fit(Lesson::Ix lesson_ix, Count count, const Slot__Room &slot__room)
    {
        for (Slot slot{}; slot < slot_count; ++slot)
        {
            if (slot__room[slot])
                continue;
            while (true)
            {
                for (Ix rix{}; rix < rooms.size(); ++rix)
                {
                    const auto &info = rooms[rix].slot__info[slot];
                    if (info.lesson_ix.value_or(lesson_ix) == lesson_ix && (info.occupation + count) <= max_occupation)
                        return {slot, rix};
                }
                std::cout << "Creating new room " << rooms.size() << std::endl;
                rooms.emplace_back();
            }
        }
        return {0, 0};
    }

    std::ostream &write(std::ostream &os) const
    {
        if (!"print classes")
            for (const auto &cl : classes)
                os << cl << std::endl;

        const auto sep = '\t';
        {
            std::cout << sep;
            for (Ix clix{}; clix < classes.size(); ++clix)
                std::cout << classes[clix].name << '/' << classes[clix].students << sep;
            std::cout << std::endl;
        }
        for (Slot slot{}; slot < slot_count; ++slot)
        {
            std::cout << slot << sep;
            for (Ix clix{}; clix < classes.size(); ++clix)
            {
                auto room_ix = class__slot__room[clix][slot];
                if (room_ix)
                {
                    const auto &room = rooms[*room_ix];
                    const auto lesson_ix = room.slot__info[slot].lesson_ix;
                    if (lesson_ix)
                    {
                        const auto &course = Lesson::all[*lesson_ix].course;
                        std::cout << course.name << course.hours << '/' << *room_ix << sep;
                    }
                    else
                        std::cout << " ? " << sep;
                }
                else
                    std::cout << " - " << sep;
            }
            std::cout << std::endl;
        }

        {
            std::cout << sep;
            for (Ix rix{}; rix < rooms.size(); ++rix)
                std::cout << "Room" << rix << sep;
            std::cout << std::endl;
        }
        for (Slot slot{}; slot < slot_count; ++slot)
        {
            std::cout << slot << sep;
            for (Ix rix{}; rix < rooms.size(); ++rix)
            {
                const Room &room = rooms[rix];
                const auto &info = room.slot__info[slot];
                if (info.lesson_ix)
                    std::cout << *info.lesson_ix << '/' << info.occupation << sep;
                else
                    std::cout << " - " << sep;
            }
            std::cout << std::endl;
        }

        return os;
    }
};
std::ostream &operator<<(std::ostream &os, const Schedule &schedule)
{
    return schedule.write(os);
}

int main()
{
    const std::vector<Class> classes = {
        Class{.name = "HuWb", .students = 21, .lesson_ixs = make_lessons({aarderijkskunde_1, engels_2, filosofie, frans_4, geschiedenis, godsdienst, kunstbeschouwing, lo, natuurweteschappen, nederlands, sociale, statistiek, wiskunde_3})},
        Class{.name = "EcMt", .students = 3, .lesson_ixs = make_lessons({aarderijkskunde_1, duits, economie, engels_3, frans_4, geschiedenis, godsdienst, lo, natuurweteschappen, nederlands, wiskunde_4})},
        Class{.name = "HuWa", .students = 7, .lesson_ixs = make_lessons({aarderijkskunde_1, engels_2, filosofie, frans_4, geschiedenis, godsdienst, kunstbeschouwing, lo, natuurweteschappen, nederlands, sociale, statistiek, wiskunde_3})},
        Class{.name = "Mt", .students = 8, .lesson_ixs = make_lessons({aarderijkskunde_1, duits, engels_3, esthetica_2, frans_5, geschiedenis, godsdienst, lo, natuurweteschappen, nederlands, spaans, wiskunde_3})},
        Class{.name = "WeWi6", .students = 11, .lesson_ixs = make_lessons({aarderijkskunde_2, biologie_2, chemie_2, engels_2, frans_3, fysica_3, geschiedenis, godsdienst, lo, nederlands, methodiek, wiskunde_6})},
        Class{.name = "WeWi8", .students = 9, .lesson_ixs = make_lessons({aarderijkskunde_2, biologie_2, chemie_2, engels_2, esthetica_1, frans_3, fysica_2, geschiedenis, godsdienst, lo, nederlands, wiskunde_8})},
        Class{.name = "EcWi", .students = 7, .lesson_ixs = make_lessons({aarderijkskunde_1, biologie_1, chemie_1, economie, engels_2, esthetica_1, frans_3, fysica_2, geschiedenis, godsdienst, lo, nederlands, wiskunde_6})},
        Class{.name = "GrLa", .students = 1, .lesson_ixs = make_lessons({aarderijkskunde_1, engels_2, esthetica_2, frans_3, geschiedenis, godsdienst, grieks, latijn, lo, natuurweteschappen, nederlands, wiskunde_4})},
        Class{.name = "LaMt", .students = 2, .lesson_ixs = make_lessons({aarderijkskunde_1, duits, engels_3, esthetica_2, frans_4, geschiedenis, godsdienst, latijn, lo, natuurweteschappen, nederlands, wiskunde_3})},
        Class{.name = "LaWe", .students = 4, .lesson_ixs = make_lessons({aarderijkskunde_2, biologie_2, chemie_2, engels_2, esthetica_1, frans_3, fysica_2, geschiedenis, godsdienst, latijn, lo, nederlands, wiskunde_4})},
        Class{.name = "LaWi6", .students = 9, .lesson_ixs = make_lessons({aarderijkskunde_1, biologie_2, chemie_1, engels_2, esthetica_1, frans_3, fysica_2, geschiedenis, godsdienst, latijn, lo, nederlands, wiskunde_6})},
        Class{.name = "LaWi8", .students = 1, .lesson_ixs = make_lessons({aarderijkskunde_1, biologie_1, chemie_1, engels_2, frans_3, fysica_2, geschiedenis, godsdienst, latijn, lo, nederlands, wiskunde_8})},
    };

    Schedule schedule;
    if (!schedule.init(classes))
        return (std::cout << "Could not set classes" << std::endl, -1);

    std::cout << schedule << std::endl;
    if (!schedule.naive())
        return (std::cout << "Could not Schedule.naive()" << std::endl, -1);
    std::cout << schedule << std::endl;
    return 0;
}
