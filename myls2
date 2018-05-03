#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <codecvt>
#include <iterator>
#include <numeric>
#include <functional>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>

class Adatok
{
  public:
    Adatok(std::string name, std::string path, std::string date, int points) : _name(name), _path(path), _date(date), _points(points) {}
    Adatok(const Adatok &other) = default;
    Adatok &operator=(const Adatok &other) = default;

    std::string get_name() { return _name; }
    std::string get_path() { return _path; }
    std::string get_date() { return _date; }
    int get_points() { return _points; }

  private:
    std::string _name;
    std::string _path;
    std::string _date;
    int _points;
};
class Ranglista
{
  public:
    Ranglista(std::string name, int points) : _name(name), _points(points) {}
    Ranglista(const Ranglista &other) = default;
    Ranglista &operator=(const Ranglista &other) = default;

    std::string get_name() { return _name; }
    int get_points() { return _points; }
    bool operator<(const Ranglista &other)
    {
        return _points > other._points;
    }

  private:
    std::string _name;
    int _points;
};
class Vedes
{
  public:
    Vedes(std::string name, int point) : _name(name), _point(point) { _count++; }
    Vedes(const Vedes &other) = default;
    Vedes &operator=(const Vedes &other) = default;
    std::string get_name() { return _name; }
    int get_point() { return _point; }
    int get_count() { return _count; }
    void set_stuff(int &points)
    {
        _point += points;
        _count++;
    }
    bool operator<(const Vedes &other)
    {
        return _count > other._count;
    }

  private:
    std::string _name;
    int _point;
    int _count = 0;
};

typedef std::map<std::string, int> path_value; //minden path + az erteke
typedef std::vector<Adatok> name_path_date;    //bejegyzesek
typedef std::vector<Ranglista> ranglista;      //ranglista
typedef std::vector<Vedes> vedes_vec;          //vedesek

void create_pv(path_value &, boost::filesystem::path);                                   //feltolti a path+ertek map-ot
void create_npd(name_path_date &, path_value &, std::string input);                      //feltolti a bejegyzesek vektorat + mindenki pontszama map
void create_np(name_path_date &, path_value &);                                          // name + path map
void list_np(path_value &name_point);                                                    // nam + path kiiratas
void list_bejegyzesek(name_path_date &bejegyzesek);                                      // bejegyzesek vektora kiiratas
bool check_bejegyzesek(name_path_date &bejegyzesek, std::string name, std::string path); //van-e mar ilyen bejegyzes
void create_rl(ranglista &rl_vec, path_value &name_point);                               //ranglista feltoltes
void list_rl(ranglista &rl_vec);                                                         //ranglista kiiratas
void vedes_atlag(name_path_date &bejegyzesek, vedes_vec &v_vec);                         //vedes atlag map
void list_atlag(vedes_vec &v_vec);                                                       //vedes atlag kiiratas
bool check_vedes(vedes_vec &v_vec, std::string name);
void vedes_elem(vedes_vec &v_vec, std::string name, int &&points); //
//void accumulate_pv(path_value&);

int main(int argc, char **argv)
{
    std::vector<std::string> roots = {"City/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/", "City/Debrecen/Oktatás/Informatika/Programozás/DEIK/"};
    std::string input_file_name = "db-2018-04-14.csv";

    /* OPTIONS */
    boost::program_options::options_description desc("ALLOWED OPTIONS");
    desc.add_options()("help", "help msg")("root,r", boost::program_options::value<std::vector<std::string>>())("csv", boost::program_options::value<std::string>(), "comma separated values")("rank", "rang lista")("vedes", "labor vedesek");
    boost::program_options::positional_options_description pdesc;
    pdesc.add("root", -1);
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).positional(pdesc).run(), vm);
    boost::program_options::notify(vm);

    int sum = 0;
    path_value pv_map;
    if (vm.count("help") || argc == 1)
    {
        std::cout << desc << std::endl;
        return 1;
    }
    if (vm.count("root"))
    {
        roots = vm["root"].as<std::vector<std::string>>();
        for (auto &i : roots)
        {
            boost::filesystem::path path(i);
            create_pv(pv_map, path);
        }
        for (path_value::iterator it{pv_map.begin()}; it != pv_map.end(); it++)
            sum += it->second;
        //std::cout << sum << std::endl;
        std::cout << std::accumulate(pv_map.begin(), pv_map.end(), 0, [](int value, const std::map<std::string, int>::value_type &p) { return value + p.second; });
        std::cout << std::endl;
    }

    if (vm.count("csv"))
    {
        //input_file_name = vm["csv"].as<std::string>();
        std::ifstream input_file{vm["csv"].as<std::string>()};
        name_path_date bejegyzesek;
        std::string temp;
        path_value name_point;

        while (getline(input_file, temp))
            create_npd(bejegyzesek, pv_map, temp);
        create_np(bejegyzesek, name_point);
        //list_bejegyzesek(bejegyzesek);
        //list_np(name_point);
        if (vm.count("rank"))
        {
            ranglista rl_vec;
            create_rl(rl_vec, name_point);
            list_rl(rl_vec);
        }
        if (vm.count("vedes"))
        {
            vedes_vec v_vec;
            vedes_atlag(bejegyzesek, v_vec);
            list_atlag(v_vec);
        }
        return 0;
    }
    return 0;
}

void create_pv(path_value &pv_map, boost::filesystem::path path)
{
    boost::filesystem::directory_iterator it{path}, eod;
    BOOST_FOREACH (boost::filesystem::path const &p, std::make_pair(it, eod))
    {
        if (boost::filesystem::is_regular_file(p))
        {
            boost::filesystem::ifstream regular_file{p};
            std::string temp;
            int sum = 0; //aktualis .props erteke
            while (getline(regular_file, temp))
            {
                temp.erase(0, temp.find_last_of('/'));
                temp.erase(0, temp.find_first_of(' '));
                sum += std::atoi((temp.substr(temp.find_first_of("0123456789"), temp.find_last_of("0123456789"))).c_str());
            }
            std::string result = p.string();
            std::string result_path = result.substr(0, result.find_last_of('/'));
            //pv_map.insert(std::make_pair(result, sum));
            pv_map[result_path] = sum;
        }
        else
            create_pv(pv_map, p);
    }
}
//void accumulate_pv(path_value& pv_map)
//{
//   std::cout<<std::accumulate(pv_map.begin(),pv_map.end(),0,[](int value,const path_value::int& p){return value+p.second;});
//}
void create_npd(name_path_date &bejegyzesek, path_value &pv_map, std::string input)
{
    boost::tokenizer<boost::escaped_list_separator<char>> tokenizer{input};
    boost::tokenizer<boost::escaped_list_separator<char>>::iterator it{tokenizer.begin()};
    std::string name = *it;
    std::string path = *(++it);
    std::string date = *(++it);
    path = path.substr(2);
    if (!check_bejegyzesek(bejegyzesek, name, path))
        bejegyzesek.push_back(Adatok(name, path, date, pv_map[path]));
}
bool check_bejegyzesek(name_path_date &bejegyzesek, std::string name, std::string path)
{
    bool ok = false;
    for (name_path_date::iterator it{bejegyzesek.begin()}; it != bejegyzesek.end(); it++)
    {
        if ((it->get_name() == name) && (it->get_path() == path))
            ok = true;
    }
    return ok;
}
bool check_vedes(vedes_vec &v_vec, std::string name)
{
    bool ok = false;
    for (auto &it : v_vec)
        if ((it.get_name() == name))
            ok = true;
    return ok;
}
void vedes_elem(vedes_vec &v_vec, std::string name, int &&points)
{
    for (auto &it : v_vec)
        if (it.get_name() == name)
            it.set_stuff(points);
}
void create_np(name_path_date &bejegyzesek, path_value &name_point)
{
    for (name_path_date::iterator it{bejegyzesek.begin()}; it != bejegyzesek.end(); it++)
        if (name_point.count(it->get_name()) == 0)
            name_point.insert(std::make_pair(it->get_name(), it->get_points()));
        else
            name_point[it->get_name()] += it->get_points();
}
void list_np(path_value &name_point)
{
    for (path_value::iterator it{name_point.begin()}; it != name_point.end(); it++)
    {
        if (it->second)
            std::cout << it->first << " " << it->second << std::endl;
    }
}
void list_bejegyzesek(name_path_date &bejegyzesek)
{
    for (name_path_date::iterator it{bejegyzesek.begin()}; it != bejegyzesek.end(); it++)
        if (it->get_name() == "Varga Erik")
            std::cout << it->get_name() << " " << it->get_path() << " " << it->get_points() << std::endl;
}
void create_rl(ranglista &rl_vec, path_value &name_point)
{
    for (auto &it : name_point)
    {
        if (it.second > 0)
            rl_vec.push_back(Ranglista(it.first, it.second));
    }
    std::sort(rl_vec.begin(), rl_vec.end());
}
void list_rl(ranglista &rl_vec)
{
    for (auto &it : rl_vec)
        std::cout << it.get_name() << " " << it.get_points() << std::endl;
}
void vedes_atlag(name_path_date &bejegyzesek, vedes_vec &v_vec)
{
    std::string key = "City/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/Labor/Védés/";
    for (auto &it : bejegyzesek)
    {
        if ((it.get_path().find("City/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/Labor/Védés/") != std::string::npos) && (it.get_points()) && (!check_vedes(v_vec, it.get_name())))
            v_vec.push_back(Vedes(it.get_name(), it.get_points()));
        else if ((check_vedes(v_vec, it.get_name())) && (it.get_path().find("City/Debrecen/Oktatás/Informatika/Programozás/DEIK/Prog1/Labor/Védés/") != std::string::npos) && (it.get_points()))
            vedes_elem(v_vec, it.get_name(), it.get_points());
    }
}
void list_atlag(vedes_vec &v_vec)
{
    std::sort(v_vec.begin(), v_vec.end());
    int sum = 0;
    int csum = 0;
    for (auto &it : v_vec)
    {
        //std::cout << it.get_name() << " " << (it.get_point() - it.get_count()) / it.get_count() << std::endl;
        //sum += it.get_point();
        //csum += it.get_count();
        sum = std::accumulate(v_vec.begin(), v_vec.end(), 0, [](int i, Vedes &o) { return i + o.get_point(); });
        csum = std::accumulate(v_vec.begin(), v_vec.end(), 0, [](int i, Vedes &o) { return i + o.get_count(); });
    }
    std::cout << (sum - csum) / csum << std::endl;
}
