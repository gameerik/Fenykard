#include <iostream>
#include <vector>
#include <iterator>
#include <cctype>
#include <string>
#include <sstream>
#include <boost/filesystem.hpp>

typedef std::vector<boost::filesystem::path> vec_type;
int last;

void acheck(int&);
void allpoints(boost::filesystem::path&,vec_type,int&);

int main(int argc,char* argv[])
{
    int sum = 0;
    vec_type myvec;
    acheck(argc); //argument vizsgalat
    boost::filesystem::path mypath = argv[1];
    allpoints(mypath,myvec,sum); //bejaras rekurzivan + pontszamolas
    std::cout<<sum<<std::endl; //vegso eredmeny
   


    return 0;
}

void acheck(int& argc)
{
    if (argc < 2) { std::cout<<"tul keves argument !\nusage : ./valami /directory/amit/akarok/bejarni \n"; exit(-1);}

}
void allpoints(boost::filesystem::path& mypath,vec_type myvec,int& sum)
{    
    if (boost::filesystem::is_regular_file(mypath))
        {
            std::string temp ="";
            std::string temp2 = "";
            int temp_sum = 0;
            boost::filesystem::ifstream myif{mypath};
            while (std::getline(myif,temp))
                {
                    if ((temp.find_last_of("0123456789"))!= temp.npos)
                    {
                    temp2 = temp.substr(temp.find_last_not_of("0123456789")+1,temp.find_last_of("0123456789"));
                    std::stringstream ph(temp2);
                    ph >>temp_sum;
                    if (temp_sum == 0)  //ha a legutolso szam utan van meg valami
                    {
                    temp2 = temp.substr(temp.find_last_of("/")+1,temp.find_last_of("0123456789"));
                    temp2 = temp2.substr(temp2.find_first_of(" "),temp2.find_last_of("0123456789"));
                    temp2 = temp2.substr(temp2.find_first_of("0123456789"),temp2.find_last_of("0123456789"));
                    std::stringstream ph2(temp2);
                    ph2>>temp_sum;
                    }
                    std::cout<<temp_sum<<" ";
                    sum+=temp_sum; 
                    }
                }
                std::cout<<std::endl;
                 

        }
    else if (boost::filesystem::is_directory(mypath))
        {
            int last = myvec.size();
            std::copy(boost::filesystem::directory_iterator(mypath),boost::filesystem::directory_iterator(),back_inserter(myvec));
            for (vec_type::iterator it(myvec.begin()+last);it != myvec.end();it++)
                {
                    allpoints(*it,myvec,sum);
                    std::cout<<mypath<<std::endl;
                }
        }
    
}